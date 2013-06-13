/******************************************************************************
 * SimonGame.c
 *
 * Created on: Dec 16, 2012
 * Board: DRV2603EVM-CT RevD
 * Author: a0866685
 *
 * Desc: This file contains functions to operate and play the game of "Concentration".
 *
 * To Play:
 * 		1) Press any button (B1-B4) to start the game
 * 		2) The game counts down and then shows a pattern of signals
 * 		3) Once you have seen the pattern, repeat the pattern by touching
 * 			the buttons in the same order
 * 		4) If you repeat the entire pattern correctly, the board will repeat
 * 			the pattern plus one additional signal.  Continue playing until you
 * 			repeat the sequence incorrectly.
 * 		5) If you repeat the pattern incorrectly, the board will buzz and the
 * 			game is over.
 *
 * Modified:
 * Jan 30, 2012 - Updated functions for DRV2603EVM-CT.
 *
 ******************************************************************************/

#include "SimonGame.h"
#include <stdlib.h>

// private variables
static uint8_t Simon_isPlaying = 0;					// status if the user is playing a Simon game
static uint8_t Simon_isPatternDisplayed = 0;		// status if the pattern has been shown to the user
static uint8_t pattern[MAXPATTERNLENGTH];			// stores the current pattern
static uint8_t patternLength = MINPATTERNLENGTH;	// current pattern length

void (*buttonEffects[NUMBUTTONS])() = {				// array of function pointers for button effects
		Simon_EffectButton1,
		Simon_EffectButton2,
		Simon_EffectButton3,
		Simon_EffectButton4

};

/**
 * Simon_Init - Initialize Simon status and variables
 */
void Simon_Init(void)
{
	Simon_isPlaying = 1;
	Simon_isPatternDisplayed = 0;
	patternLength = MINPATTERNLENGTH;
}

/**
 * Simon_Play - call to begin Simon game
 * @param uint8_t button - the current button being pressed
 */
void Simon_Play(uint8_t button)
{
	Simon_PlayButton(button);	// play the haptic effect for the button pressed

	if(!Simon_isPlaying)		// if not currently in a Simon game
	{
		Simon_Init();					// Initialize the Simon game settings
		Simon_IntroSequence();			// Intro Haptics/LED sequence
		sleep(2500);
		Simon_GeneratePattern();		// Generate Simon pattern
		Simon_ShowPattern();			// Display the pattern to the user
	}
	else if(Simon_isPlaying && Simon_isPatternDisplayed)		// if playing Simon and the pattern has been displayed
	{
		Simon_VerifyPattern(button);	// verify the current button is correct
	}
	else if(Simon_isPlaying)	// else if playing Simon and the pattern has NOT been displayed
	{
		CapTouch_ButtonLEDsOff();
		Simon_ShowPattern();
	}
}

/**
 * Simon_Reset - call to cancel current Simon game, call when mode changes
 */
void Simon_Reset(void)
{
	Simon_isPlaying = 0; 	// cancel current game
}

/**
 * Simon_IntroSequence - Simon game intro sequence
 */
void Simon_IntroSequence(void)
{
	CapTouch_FlashButtonLEDs(3);
	CapTouch_ButtonLEDsOn();
	sleep(LEDBLINKDELAY);
	CapTouch_ButtonLEDOffSequence();
}

/**
 * Simon_CountDownSequence - count down sequence shown on Mode LEDs
 */
void Simon_CountDownSequence(void)
{
	int delayOn = 5500;

	sleep(delayOn);

	CapTouch_ModeLEDsOn();						// Flash mode LEDs
	Haptics_SendWaveform(COUNTDOWNEFFECT);			// Play Haptics Effect
	sleep(PATTERNPAUSE);						// Wait while LEDs are on
	CapTouch_ModeLEDsOff();						// Turn LEDs off
	sleep(LEDOFFDELAY);							// Wait while LEDs are off

	P3OUT |= MODE3+MODE2+MODE1+MODE0;
	Haptics_SendWaveform(COUNTDOWNEFFECT);
	sleep(PATTERNPAUSE);
	CapTouch_ModeLEDsOff();
	sleep(LEDOFFDELAY);

	P3OUT |= MODE2+MODE1+MODE0;
	Haptics_SendWaveform(COUNTDOWNEFFECT);
	sleep(PATTERNPAUSE);
	CapTouch_ModeLEDsOff();
	sleep(LEDOFFDELAY);

	P3OUT |= MODE1+MODE0;
	Haptics_SendWaveform(COUNTDOWNEFFECT);
	sleep(PATTERNPAUSE);
	CapTouch_ModeLEDsOff();
	sleep(LEDOFFDELAY);

	P3OUT |= MODE0;
	Haptics_SendWaveform(COUNTDOWNEFFECT);
	sleep(PATTERNPAUSE);

	CapTouch_ModeLEDsOff();
	CapTouch_ButtonLEDsOn();
	Haptics_SendWaveform(ERROREFFECT);
	sleep(5000);
	CapTouch_ButtonLEDsOff();
}

/**
 * Simon_IncorrectSequence - sequence shown when user is incorrect
 */
void Simon_IncorrectSequence(void)
{
	// Vibrate motor and blink button LEDs
	Haptics_SendWaveform(ERROREFFECT);
	CapTouch_FlashButtonLEDs(4);
	CapTouch_ModeLEDsOff();
	sleep(5000);

	// Scroll mode LEDs
	CapTouch_ModeLEDsScroll(3);
	CapTouch_ModeLEDsOff();
	sleep(5000);

	// Display score
	if(patternLength != MINPATTERNLENGTH + 1)
		CapTouch_ModeLEDBinary(patternLength - 2);
	sleep(20000);
	CapTouch_ModeLEDsOff();
	sleep(5000);
}

/**
 * Simon_Success - sequence shown when user enters the correct pattern
 */
void Simon_Success(void)
{
	sleep(5000);
	CapTouch_ButtonLEDOnSequence();
	Haptics_SendWaveform(SUCCESSEFFECT);	    // Play haptics effect

	CapTouch_ModeLEDsScroll(5);					// Scroll mode LEDs
	sleep(5000);
	CapTouch_ButtonLEDOffSequence();
}

/**
 * Simon_GeneratePattern - call to generate a new pattern
 */
void Simon_GeneratePattern(void)
{
	unsigned int i = 0;

	srand(CapTouch_RandomNumber);	// random number generator seed

	for(i = 0; i < MAXPATTERNLENGTH; i++)
	{
		pattern[i] = (rand() % NUMBUTTONS);		// create a pattern selecting one button for each value
	}

	CapTouch_RandomNumber = 0;
}

/**
 * Simon_ShowPattern - call to show pattern to user
 */
void Simon_ShowPattern(void)
{
	unsigned int i = 0;

	if(patternLength == MINPATTERNLENGTH)
	{
		Simon_CountDownSequence();					// Count down to beginning of game
	}

	CapTouch_ButtonLEDsOff();

	sleep(10000);

	for(i = 0; i < patternLength; i++)			// go through the pattern
	{
		CapTouch_ButtonLEDsOff();
		buttonEffects[(pattern[i])]();			// call the appropriate button function
		sleep(PATTERNPAUSE);					// pause between patterns
		CapTouch_ButtonLEDsOff();
		sleep(LEDOFFDELAY);
	}

	Simon_isPatternDisplayed = 1;				// the pattern has been displayed

	patternLength++;							// make the pattern one effect longer for next time
}

/**
 * Simon_VerifyPatter - call to verify the button pressed is correct
 * @param - unsigned int buttonNumber - current button being pressed
 */
void Simon_VerifyPattern(unsigned int buttonNumber)
{
	static unsigned int patternCounter = 0; 	// current pattern array element to compare to user's input

	if(buttonNumber == pattern[patternCounter])	// if correct button is pressed
	{
		patternCounter = patternCounter + 1;	// increment the pattern counter to the next element

		if(patternCounter == (patternLength - 1))  // if reached pattern end, success
		{
			//Simon_Success();					// success effect
			patternCounter = 0;
			Simon_isPatternDisplayed = 0;		// time to show next pattern
			sleep(LEDOFFDELAY);
			Simon_ShowPattern();				// show the next pattern
		}
	}
	else										// else, incorrect button pressed
	{
		Simon_IncorrectSequence();
		Simon_isPlaying = 0;					// game over
		patternCounter = 0;						// reset pattern counter
	}

}

/**
 * Simon_PlayButton - turn on the LED and play the corresponding effect
 */
void Simon_PlayButton(unsigned int buttonNumber)
{
	buttonEffects[(buttonNumber)]();			// call the appropriate button function
}

/**
 * Simon_EffectButton1 - function when button 1 is pressed
 */
void Simon_EffectButton1(void)
{
	P1OUT |= BUTTON1;							// turn on B1 LED

	Haptics_SendWaveform(B1EFFECT);				// Play Haptic Effect
}

/**
 * Simon_EffectButton2 - function when button 2 is pressed
 */
void Simon_EffectButton2(void)
{
	P1OUT |= BUTTON2;							// turn on B2 LED

	Haptics_SendWaveform(B2EFFECT);				// Play Haptic Effect
}

/**
 * Simon_EffectButton3 - function when button 3 is pressed
 */
void Simon_EffectButton3(void)
{
	P1OUT |= BUTTON3;							// turn on B3 LED

	Haptics_SendWaveform(B3EFFECT);				// Play Haptic Effect
}

/**
 * Simon_EffectButton4 - function when button 4 is pressed
 */
void Simon_EffectButton4(void)
{
	P1OUT |= BUTTON4;							// turn on B4 LED

	Haptics_SendWaveform(B4EFFECT);				// Play Haptic Effect
}



