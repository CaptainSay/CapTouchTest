#include "msp430g2553.h"
#include "CapTouchBoard.h"
#include "Actuator_Waveforms.h"
#include "SimonGame.h"
#include "BinaryModes.h"

//------------------------------------------------------------------------------
// Hardware-related definitions
//------------------------------------------------------------------------------
#define UART_TXD   0x02                     // TXD on P1.1 (Timer0_A.OUT0)
#define UART_RXD   0x04                     // RXD on P1.2 (Timer0_A.CCI1A)

//------------------------------------------------------------------------------
// Conditions for 9600 Baud SW UART, SMCLK = 1MHz
//------------------------------------------------------------------------------
#define UART_TBIT_DIV_2     (8000000 / (9600 * 2))
#define UART_TBIT           (8000000 / 9600)

//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------
unsigned int txData;                        // UART internal variable for TX
unsigned char rxBuffer;                     // Received UART character
#define SCROLL 250

extern struct Element* modePtr;			// Pointer to current mode button pressed
extern struct Element* buttonPtr;		// Pointer to current effect button pressed
struct Element* lastButtonPtr;			// Pointer to last effect button pressed

const uint8_t buttonLeds[NUMBER_BUTTONS] =
{
  BIT0,
  BIT1,
  BIT2,
  BIT3
};

const struct Element* buttonList[NUMBER_BUTTONS] = 	// Array of CTS elements (buttons)
{
  &button1,
  &button2,
  &button3,
  &button4
};

unsigned int i;

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------
void TimerA_UART_init(void);
void TimerA_UART_tx(unsigned char byte);
void TimerA_UART_print(char *string);

//------------------------------------------------------------------------------
// main()
//------------------------------------------------------------------------------
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;               // Stop watchdog timer
    if (CALBC1_1MHZ==0xFF)					// If calibration constant erased
    {
      while(1);                               // do not load, trap CPU!!
    }

    DCOCTL = 0;                             // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_8MHZ;                  // Set DCOCLK to 8MHz
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 |= DIVA_0;                    // ACLK/(0:1,1:2,2:4,3:8)
    //UNCOMMENTING BCSCTL2 MAKES MANY ERRORS
    //BCSCTL2 |= DIVS_3;                    // SMCLK/(0:1,1:2,2:4,3:8)
    BCSCTL3 |= LFXT1S_2;                  // LFXT1 = VLO (select SMCLK source)

    // 8000000 Hz  9600 bps
    UCA0BR0 = 0x41;
    UCA0BR1 = 0x03;
    UCA0MCTL = UCBRS_3 + UCBRF_0;


    P1OUT = 0x00;                           // Initialize all GPIO
    P1SEL = UART_TXD + UART_RXD;            // Timer function for TXD/RXD pins
    P1DIR = (0xFF & ~UART_RXD) ;               // Set all pins but RXD to output

    /*****I Assumed that I would not use the buttons on the CapTouch board, so I commented them out*****/
    //P1DIR |= (BUTTON1+BUTTON2+BUTTON3+BUTTON4);
    //P2OUT = 0x00;
    P2OUT = 0x40;
    //P2SEL = 0x00;
    P2SEL &= 0xBF;
    P2SEL2 &= 0xBF;						// P2.0-P2.5 = cap sense, P2.6 = I/O
    //P2DIR = 0xFF;
    P2DIR = 0x40;
    P3OUT = 0x00;
    P3SEL |= 0x04;
    P3DIR |= (MODE0+MODE1+MODE2+MODE3+MODE4+BIT2+BIT1+BIT0);  // Set P3.0-P3.1, P3.3-P3.7 as outputs = LRA/^ERM, EN, M0-M4

    // TimerA Setup
      // Set PWM Frequency,  TimerA gives 255 counts at SMCLK frequency
      TA1CCR0 = 0x00FF;

    __enable_interrupt();

    CapTouch_Init();				    // Initialize the cap touch board setting and statuses
    Haptics_Init();						// Initialize Haptics settings

    TimerA_UART_init();                     // Start Timer_A UART
    TimerA_UART_print("G2xx2 TimerA UART\r\n");
    TimerA_UART_print("READY.\r\n");

    for (;;)
    {
        // Wait for incoming character
        __bis_SR_register(LPM0_bits);

        // Echo received character
        TimerA_UART_tx(rxBuffer);
    }

}
//------------------------------------------------------------------------------
// Function configures Timer_A for full-duplex UART operation
//------------------------------------------------------------------------------
void TimerA_UART_init(void)
{
    TACCTL0 = OUT;                          // Set TXD Idle as Mark = '1'
    TACCTL1 = SCS + CM1 + CAP + CCIE;       // Sync, Neg Edge, Capture, Int
    TACTL = TASSEL_2 + MC_2;                // SMCLK, start in continuous mode
}
//------------------------------------------------------------------------------
// Outputs one byte using the Timer_A UART
//------------------------------------------------------------------------------
void TimerA_UART_tx(unsigned char byte)
{
    while (TACCTL0 & CCIE);                 // Ensure last char got TX'd
    TACCR0 = TAR;                           // Current state of TA counter
    TACCR0 += UART_TBIT;                    // One bit time till first bit
    TACCTL0 = OUTMOD0 + CCIE;               // Set TXD on EQU0, Int
    txData = byte;                          // Load global variable
    txData |= 0x100;                        // Add mark stop bit to TXData
    txData <<= 1;                           // Add space start bit
}

//------------------------------------------------------------------------------
// Prints a string over using the Timer_A UART
//------------------------------------------------------------------------------
void TimerA_UART_print(char *string)
{
    while (*string) {
        TimerA_UART_tx(*string++);
    }
}
//------------------------------------------------------------------------------
// Timer_A UART - Transmit Interrupt Handler
//------------------------------------------------------------------------------
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0_ISR(void)
{
    static unsigned char txBitCnt = 10;

    TACCR0 += UART_TBIT;                    // Add Offset to CCRx
    if (txBitCnt == 0) {                    // All bits TXed?
        TACCTL0 &= ~CCIE;                   // All bits TXed, disable interrupt
        txBitCnt = 10;                      // Re-load bit counter
    }
    else {
        if (txData & 0x01) {
          TACCTL0 &= ~OUTMOD2;              // TX Mark '1'
        }
        else {
          TACCTL0 |= OUTMOD2;               // TX Space '0'
        }
        txData >>= 1;
        txBitCnt--;
    }
}
//------------------------------------------------------------------------------
// Timer_A UART - Receive Interrupt Handler
//------------------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer_A1_ISR(void)
{
    static unsigned char rxBitCnt = 8;
    static unsigned char rxData = 0;

    switch (__even_in_range(TA0IV, TA0IV_TAIFG)) { // Use calculated branching
        case TA0IV_TACCR1:                        // TACCR1 CCIFG - UART RX
            TACCR1 += UART_TBIT;                 // Add Offset to CCRx
            if (TACCTL1 & CAP) {                 // Capture mode = start bit edge
                TACCTL1 &= ~CAP;                 // Switch capture to compare mode
                TACCR1 += UART_TBIT_DIV_2;       // Point CCRx to middle of D0
            }
            else {
                rxData >>= 1;
                if (TACCTL1 & SCCI) {            // Get bit waiting in receive latch
                    rxData |= 0x80;
                }
                rxBitCnt--;
                if (rxBitCnt == 0) {             // All bits RXed?
                    rxBuffer = rxData;           // Store in global variable
                    rxBitCnt = 8;                // Re-load bit counter
                    TACCTL1 |= CAP;              // Switch compare to capture mode
                    __bic_SR_register_on_exit(LPM0_bits);  // Clear LPM0 bits from 0(SR)
                }
            }
            break;
    }
}
//------------------------------------------------------------------------------
