/******************************************************************************
 * CapTouchBoard.c
 *
 * Created on: Dec 16, 2011
 * Board: DRV2603EVM-CT RevD
 * Author: a0866685
 *
 * Desc: This file contains common functions for operating the capacitive
 * 		touch EVM.
 *
 ******************************************************************************/
#include "CapTouchBoard.h"
#include "Actuator_Waveforms.h"

// public variables

uint8_t CapTouch_mode = MODEOFF;			// Current mode, show on mode LEDs
uint8_t CapTouch_isBinaryModeCounter = 0; 	// If true, count the mode LEDs in binary, otherwise only six modes
uint8_t CapTouch_isModeBtnReleased = 1;		// CapTouch board mode buttons are released/not pressed
uint8_t CapTouch_isEffectBtnReleased = 1;	// CapTouch board effect (B1-B4) buttons are released/not pressed
uint8_t CapTouch_isLEDFrozen = 0; 			// CapTouch board colored LEDs lock current state
uint8_t CapTouch_RandomNumber = 1;			// Random number generated from interrupt

// private variables
static uint8_t 	repeatEffectEnabled = 0;	// Allow the effect to repeat, used in RepeatOnHoldWait()
static uint16_t buttonPressCounter = 0;		// Counts the number of button samples
static uint16_t repeatModeEnabled = 0;		// Allow the mode button to be held down
static uint16_t	modePressCounter = 0;		// Counts the number of mode button samples
static uint8_t 	modeIncrementOk = 1; 	    // Flag if the mode can be incremented

/**
 * CapTouch_Init - Initialization settings for captouch evaluation board
 */
void CapTouch_Init(void)
{
	CapTouch_mode = MODEOFF;					// Set current mode to mode with all LEDs off
	CapTouch_isModeBtnReleased = 1;			// Mode buttons status = mode buttons are released (not pressed)
	CapTouch_isEffectBtnReleased = 1;			// Effect buttons Status (B1-B4) = effect buttons are released (not pressed)

	// Establish Capacitive Touch Baseline
	TI_CAPT_Init_Baseline(&mode_change);		// Measure initial capacitance of mode buttons
	TI_CAPT_Init_Baseline(&buttons);			// Measure initial capacitance of effect buttons (B1-B4)
	TI_CAPT_Update_Baseline(&mode_change,5);	// Measure mode buttons capacitance 5x times and average, this tracks changing capacitance
	TI_CAPT_Update_Baseline(&buttons,5);		// Measure effect buttons capacitance 5x times and average, this tracks changing capacitance
}

/**
 * CapTouch_PowerUpSequence - visual startup sequence for the evaluation board
 */
void CapTouch_PowerUpSequence(void)
{
	CapTouch_ButtonLEDOnSequence();

	CapTouch_FlashModeLEDs(3);

	// Vibrate LRA
	Haptics_SendWaveform(lra_rampup);

	// Rotate ERM
	Haptics_SendWaveform(erm_rampup);

	CapTouch_ButtonLEDOffSequence();
}

/**
 * CapTouch_IncrementMode - increase the board mode (binary or carousel)
 */
void CapTouch_IncrementMode(void)
{
	if(modeIncrementOk)
	{
		if(CapTouch_isBinaryModeCounter)
			CapTouch_IncrementModeBinary();
		else
			CapTouch_IncrementModeCarousel();

		// Vibrate LRA when mode button is pressed
		Haptics_SendWaveform(lra_tick);
	}
}

/**
 * CapTouch_DecrementMode - decrease the board mode (binary or carousel)
 */
void CapTouch_DecrementMode(void)
{
	if(modeIncrementOk)
	{
		if(CapTouch_isBinaryModeCounter)
			CapTouch_DecrementModeBinary();
		else
			CapTouch_DecrementModeCarousel();

		// Vibrate LRA when mode button is pressed
		Haptics_SendWaveform(lra_tick);
	}
}

/**
 * CapTouch_SetBinaryMode - set the mode counter to binary format
 */
void CapTouch_SetBinaryMode()
{
	CapTouch_isBinaryModeCounter = 1;
	CapTouch_mode = 0;
	Haptics_OutputEnableSet(1);
	Haptics_SendWaveform(lra_click);
	Haptics_SendWaveform(lra_click);
	Haptics_SendWaveform(lra_click);
	Haptics_OutputEnableSet(0);
}

/**
 * CapTouch_SetBinaryMode - set the mode counter to carousel format
 */
void CapTouch_UnsetBinaryMode()
{
	CapTouch_isBinaryModeCounter = 0;
	CapTouch_mode = 0;
	Haptics_OutputEnableSet(1);
	Haptics_SendWaveform(lra_click);
	Haptics_SendWaveform(lra_click);
	Haptics_SendWaveform(lra_click);
	Haptics_OutputEnableSet(0);
}

/**
 * CapTouch_IncrementMode - increase the board mode (6 modes)
 */
void CapTouch_IncrementModeCarousel(void)
{
	if(CapTouch_mode == MODEOFF)
	  CapTouch_mode = MODE4;
	else
	  CapTouch_mode = CapTouch_mode >> 1;
}

/**
 * CapTouch_DecrementMode - decrease the board mode (6 modes)
 */
void CapTouch_DecrementModeCarousel(void)
{
	if(CapTouch_mode == MODE4)
	  CapTouch_mode = MODEOFF;
	else if(CapTouch_mode == MODEOFF)
	  CapTouch_mode = MODE0;
	else
	  CapTouch_mode = CapTouch_mode << 1;
}

/**
 * IncrementModeBode - increase the board mode (32 modes)
 */
void CapTouch_IncrementModeBinary(void)
{
	if(CapTouch_mode == MODE_MASK)
		CapTouch_mode = MODEOFF;
	else
		CapTouch_mode = CapTouch_mode + MODE_OFFSET;
}

/**
 * DecrementModeBinary - decrease the board mode (32 modes)
 */
void CapTouch_DecrementModeBinary(void)
{
	if(CapTouch_mode == MODEOFF)
		CapTouch_mode = MODE_MASK;
	else
		CapTouch_mode = CapTouch_mode - 1;
}

/**
 * CapTouch_ModeLEDBinary - display binary on the mode LEDs
 * @param unsigned char number - number to be displayed
 */
void CapTouch_ModeLEDBinary(uint8_t number)
{
	P3OUT |= (number << 3);
}

/**
 * CapTouch_ButtonLEDsOn - turn all button LEDs on
 */
void CapTouch_ButtonLEDsOn(void)
{
	P1OUT |= 0x0F;			// turn on button LEDs
}

/**
 * CapTouch_ButtonLEDsOff - turn all button LEDs off
 */
void CapTouch_ButtonLEDsOff(void)
{
	P1OUT &= 0xF0;			// turn off button LEDs
}

/**
 * CapTouch_ModeLEDsOn - turn all mode LEDs on
 */
void CapTouch_ModeLEDsOn(void)
{
	P3OUT |= (0xF8);		// turn on mode LEDs
}

/**
 * CapTouch_ModeLEDsOff - turn all mode LEDs off
 */
void CapTouch_ModeLEDsOff(void)
{
	P3OUT &= 0x07;			// turn off mode LEDs
}

/**
 * CapTouch_FlashButtonLEDs - flash all button LEDs x times
 * @param unsigned int numberOfBlinks - number of flashes
 */
void CapTouch_FlashButtonLEDs(unsigned int numberOfBlinks)
{
	unsigned int i = 0;

	P1OUT &= 0xF0;							// turn off LEDs
	sleep(LEDBLINKDELAY);

	for(i = 0; i < numberOfBlinks; i++)
	{
		// flash button LEDs
		P1OUT |= 0x0F;						// turn on LEDs
		sleep(LEDBLINKDELAY);
		P1OUT &= 0xF0;						// turn off LEDs
		sleep(LEDBLINKDELAY);
	}
}

/**
 * CapTouch_FlashModeLEDs - flash all mode LEDs x times
 * @param unsigned int numberOfBlinks - number of flashes
 */
void CapTouch_FlashModeLEDs(unsigned int numberOfBlinks)
{
	unsigned int i = 0;

	P3OUT &= ~(MODE0+MODE1+MODE2+MODE3+MODE4);
	sleep(LEDBLINKDELAY);

	for(i = 0; i < numberOfBlinks; i++)
	{
		// flash mode LEDs
		P3OUT |= (0xF8);					// turn on mode LEDs
		sleep(LEDBLINKDELAY);
		P3OUT &= 0x07;						// turn off mode LEDs
		sleep(LEDBLINKDELAY);
	}

}

/**
 * CapTouch_ButtonLEDOnSequence - turn on LEDS from B1->B2->B3->B4
 */
void CapTouch_ButtonLEDOnSequence(void)
{
	P1OUT |= (BUTTON1);								// turn on LED B1
	sleep(LEDBLINKDELAY);
	P1OUT |= (BUTTON2);								// turn on LED B2
	sleep(LEDBLINKDELAY);
	P1OUT |= (BUTTON3);								// turn on LED B3
	sleep(LEDBLINKDELAY);
	P1OUT |= (BUTTON4);								// turn on LED B4
	sleep(LEDBLINKDELAY);
}

/**
 * CapTouch_ButtonLEDOffSequence - turn on LEDS from B1<-B2<-B3<-B4
 */
void CapTouch_ButtonLEDOffSequence(void)
{
	P1OUT &= ~(BUTTON4);							// turn off LED B4
	sleep(LEDBLINKDELAY);
	P1OUT &= ~(BUTTON3);							// turn off LED B3
	sleep(LEDBLINKDELAY);
	P1OUT &= ~(BUTTON2);							// turn off LED B2
	sleep(LEDBLINKDELAY);
	P1OUT &= ~(BUTTON1);							// turn off LED B1
}

/**
 * CapTouch_ModeLEDsScroll - scroll the mode LEDs to the right
 * 		Note: does not affect the current CapTouch_mode variable
 * @param unsigned int numberOfScolls - number of shifts from left to right
 */
void CapTouch_ModeLEDsScroll(unsigned int numberOfScrolls)
{
	unsigned int i;

	CapTouch_ModeLEDsOff();

	P3OUT |= MODE4;
	sleep(LEDBLINKDELAY);
	P3OUT &= ~MODE4;
	P3OUT |= MODE3;
	sleep(LEDBLINKDELAY);
	P3OUT &= ~MODE3;
	P3OUT |= (MODE4+MODE2);
	sleep(LEDBLINKDELAY);
	P3OUT &= ~(MODE4+MODE2);
	P3OUT |= (MODE3+MODE1);
	sleep(LEDBLINKDELAY);


	for(i = 0; i < numberOfScrolls; i++)
	{
		P3OUT &= ~(MODE3+MODE1);
		P3OUT |= (MODE4+MODE2+MODE0);
		sleep(LEDBLINKDELAY);

		P3OUT &= ~(MODE4+MODE2+MODE0);
		P3OUT |= (MODE3+MODE1);
		sleep(LEDBLINKDELAY);
	}

	P3OUT &= ~(MODE3+MODE1);
	P3OUT |= (MODE2+MODE0);
	sleep(LEDBLINKDELAY);

	P3OUT &= ~(MODE2+MODE0);
	P3OUT |= MODE1;
	sleep(LEDBLINKDELAY);

	P3OUT &= ~MODE1;
	P3OUT |= MODE0;
	sleep(LEDBLINKDELAY);

	P3OUT &= ~MODE0;
}

/**
 * CapTouch_RepeatOnHold - repeat the effect when button is held
 */
void CapTouch_RepeatOnHold(void)
{
	CapTouch_isEffectBtnReleased = 1;		// indicate the button has been released
}

/**
 * CapTouch_RepeatOnHoldWait - hold button and wait X ms before playing repeat waveform
 */
void CapTouch_RepeatOnHoldWait(void)
{
	CapTouch_isEffectBtnReleased = 1;		    // allow effect repeat

	if(!repeatEffectEnabled)
	{
		Haptics_OutputEnableSet(0);				// disable haptics output
		buttonPressCounter++;

		if(buttonPressCounter > 60)				// sets the time to wait
		{
			repeatEffectEnabled = 1;
			Haptics_OutputEnableSet(1);
		}
	}
}

/**
 * CapTouch_EnableOnHold - hold a button to call a function
 * @param action - pointer to function to be called after hold
 */
void CapTouch_EnableOnHold(void (*action)())
{
	CapTouch_isModeBtnReleased = 1;		    // allow effect repeat

	if(!repeatModeEnabled)
	{
		Haptics_OutputEnableSet(0);			// disable haptics output
		modeIncrementOk = 0; 				// disable mode increment
		modePressCounter++;

		if(modePressCounter > 80)			// sets the time to wait
		{

			modePressCounter = 1;
			(*action)();
			CapTouch_FlashModeLEDs(1);
		}
	}
}

/**
 * CapTouch_FreezeLEDs - freeze the current state of the LEDs
 */
void CapTouch_FreezeLEDs(void)
{
	CapTouch_isLEDFrozen = 1;
}

/**
 * CapTouch_UnfreezeLEDs - unfreeze the current state of the LEDs
 */
void CapTouch_UnfreezeLEDs(void)
{
	CapTouch_isLEDFrozen = 0;
}

/**
 * CapTouch_RepeatReset - to be called in main loop when button is released
 */
void CapTouch_RepeatReset(void)
{
	repeatEffectEnabled = 0;
	buttonPressCounter = 0;
	Haptics_OutputEnableSet(1);
}

/**
 * CapTouch_ModeRepeatReset - to be called in main loop when a mode button is released
 */
void CapTouch_ModeRepeatReset(void)
{
	repeatModeEnabled = 0;
	modePressCounter = 0;
	modeIncrementOk = 1;
	//Haptics_OutputEnableSet(1);
}



