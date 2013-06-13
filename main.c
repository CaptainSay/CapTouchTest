#include "msp430g2553.h"
#include "CapTouchBoard.h"
#include "Actuator_Waveforms.h"
#include "BinaryModes.h"
#include <string.h>
#include <math.h>

/**********CONSTANTS**********/
#define SCROLL 250
#define BUFFERSIZE 255

/**********VARIABLES**********/
int iterator = 0;
char buffer[BUFFERSIZE] = {0};
int realBufferSize = 0;
int bufferReadIndex = 0;
int bufferWriteIndex = 0;
char character;

unsigned int i;

/**********FUNCTION PROTOTYPES**********/
void printf(char * tx_data); // Function Prototype for TX
void write(char ch);
void Erm_rampup(void);
void getc(char c);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_8MHZ;                    // Set DCO
  DCOCTL = CALDCO_8MHZ;
  BCSCTL1 |= DIVA_1;                        // ACLK /(0:1,1:2,2:4,3:8)
  BCSCTL2 |= DIVS_0;                        // SMCLK/(0:1,1:2,2:4,3:8)
  BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO (select SMCLK source)

  // 8000000 Hz  9600 bps
  UCA0BR0 = 0x41;
  UCA0BR1 = 0x03;
  UCA0MCTL = UCBRS_3 + UCBRF_0;

  //16MHz 9600 bps
 /*UCA0BR0 = 0x82; // 16MHz 9600
 UCA0BR1 = 0x06; // 16MHz 9600
 UCA0MCTL = UCBRS_5 + UCBRF_0;              // Modulation UCBRSx = 3*/

  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD

  // Set GPIO functions
  P2SEL &= 0xBF;  					        // P2.0-P2.5 = cap sense, P2.6 = I/O
  P2SEL2 &= 0xBF;					        // P2.0-P2.5 = cap sense, P2.6 = I/O
  P3SEL |= 0x04;                   	        // P3.2 = PWM Output

  // Set GPIO directions
  P1DIR |= (BUTTON1+BUTTON2+BUTTON3+BUTTON4);				// Set P1.0-P1.3 to outputs for colored LEDs
  P2DIR |= 0x40;											// Set P2.6 = Output
  P3DIR |= (MODE0+MODE1+MODE2+MODE3+MODE4+BIT2+BIT1+BIT0);  // Set P3.0-P3.1, P3.3-P3.7 as outputs = LRA/^ERM, EN, M0-M4

  // Set GPIO values
  P1OUT = 0;							    // Colored LEDs off
  P2OUT |= 0x40;						    // Set P2.6 = 1, set load switch to LRA
  P3OUT = 0x00;                    	        // White LEDs off, EN = off

  // TimerA Setup
  // Set PWM Frequency,  TimerA gives 255 counts at SMCLK frequency
  TA1CCR0 = 0x00FF;

  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

  CapTouch_Init();
  Haptics_Init();
 // CapTouch_PowerUpSequence();
  Haptics_SendWaveform(erm_rampup);

 /* while(1)
  {
	  __bis_SR_register(GIE);

	  //__low_power_mode_0(); // Interrupts enabled

	  /*if(character && character != '\r')
	  {
		  //Haptics_SendWaveform(erm_rampup);
		  write(character); //echo

		  if(character == 'a')
		  {
			  Haptics_SendWaveform(erm_rampup);
		  }*/

		  //character = 0x00;   //  <---  here

		  /*if(character != '\b')
		  {
			  buffer[iterator] = character;
			  iterator++;
		  }*/
	  /*for(i = 0; i < 1; i++)
	  {
		  write(character);
	  }*/
/*	  getc(character);
	  write(character);
	 switch(character)
	 {
	 case 'a':
	 {
		 //Haptics_SendWaveform(lra_rampup);
		 printf(" YOU PRESSED 'a'!\r\n");
		 break;
	 }
	 case 'b':
	 {
		printf(" 2YOU PRESSED 'b'! \r\n");
		break;
	 }
	 }

	  character = 0x00;
  }*/

  for(;;)
  {
	  __bis_SR_register(GIE);
	  write(character);
	  if(character == 'a')
	  {
		  printf(" Successfully pressed 'a'! \r\n");
		  Haptics_SendWaveform(erm_rampup);
	  }
	  character = 0x00;
	  //Haptics_SendWaveform(erm_rampup);
  }
}

void Erm_rampup(void)
{
	Haptics_SendWaveform(erm_rampup);
}

// Output char to UART
void getc(char c)
{
	while((UCA0STAT & UCBUSY));
	{
	UCA0RXBUF = c;
	}
}
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	character = UCA0RXBUF;
}
void write(char ch)
{
	while ((UCA0STAT & UCBUSY));
	{
				UCA0TXBUF  = ch;
	}
}
void printf(char * tx_data)
{
    unsigned int i=0;
    while(tx_data[i])
    {
        while ((UCA0STAT & UCBUSY));
        UCA0TXBUF = tx_data[i];
        i++;
    }
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_Timer0_A0(void)
{
  TA0CTL &= ~(MC_1);
  TA0CCTL0 &= ~(CCIE);

  CapTouch_RandomNumber++;

  __bic_SR_register_on_exit(LPM0_bits+GIE);
}
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A1_ISR (void)
{
  TA1CCTL1 &= ~CCIFG;   //Clear Interrupt Flag
}
