/******************************************************************************
 * CapTouchBoard.h
 *
 * Created on: Dec 16, 2011
 * Board: DRV2603EVM-CT RevD
 * Author: a0866685
 *
 * Desc: This file contains common functions for operating the capacitive
 * 		touch EVM.
 *
 ******************************************************************************/

#ifndef CAPTOUCHBOARD_H_
#define CAPTOUCHBOARD_H_

#include "CTS/CTS_Layer.h"
#include "Haptics.h"
#include "msp430.h"
#include "CTS/structure.h"
#include "Timer.h"

// Button Definitions
#define BUTTON_MASK   (BUTTON1+BUTTON2+BUTTON3+BUTTON4)		// Button Mask
#define BUTTON1   BIT0										// B1
#define BUTTON2   BIT1										// B2
#define BUTTON3   BIT2										// B3
#define BUTTON4   BIT3										// B4
#define NUMBER_BUTTONS  4									// Number of Buttons

// Mode Definitions
#define MODE_OFFSET BIT3
#define MODE_MASK   (MODE0+MODE1+MODE2+MODE3+MODE4)			// Mode LEDs
#define MODEOFF 	0										// No mode LEDs on
#define MODE0       BIT3									// M0 LED
#define MODE1       BIT4									// M1 LED
#define MODE2       BIT5									// M2 LED
#define MODE3       BIT6									// M3 LED
#define MODE4       BIT7									// M4 LED
#define BUTTONMINUS BIT4									// Mode Button -
#define BUTTONPLUS  BIT5									// Mode Button +

#define LEDBLINKDELAY 1500							// LED blink rate

// Status variables
extern uint8_t 	CapTouch_mode; 						// Current mode, show on mode LEDs
extern uint8_t  CapTouch_isBinaryModeCounter; 		// If true, count the mode LEDs in binary, otherwise only six modes
extern uint8_t 	CapTouch_isModeBtnReleased;	   	 	// CapTouch board mode buttons are released/not pressed
extern uint8_t 	CapTouch_isEffectBtnReleased;		// CapTouch board effect (B1-B4) buttons are released/not pressed
extern uint8_t  CapTouch_isLEDFrozen;				// CapTouch board colored LEDs lock current state
extern uint8_t 	CapTouch_RandomNumber; 				// Random number generated from interrupt

/**
 * CapTouch_Init - Initialization settings for captouch evaluation board
 */
void CapTouch_Init(void);

/**
 * CapTouch_PowerUpSequence - visual startup sequence for the evaluation board
 * @TODO - Update this function when changing actuators types (ERM, LRA, Piezo)
 */
void CapTouch_PowerUpSequence(void);

/**
 * CapTouch_IncrementMode - increase the board mode (binary or carousel)
 */
void CapTouch_IncrementMode(void);

/**
 * CapTouch_DecrementMode - decrease the board mode (binary or carousel)
 */
void CapTouch_DecrementMode(void);

/**
 * CapTouch_SetBinaryMode - set the mode counter to binary format
 */
void CapTouch_SetBinaryMode();

/**
 * CapTouch_SetBinaryMode - set the mode counter to carousel format
 */
void CapTouch_UnsetBinaryMode();

/**
 * CapTouch_IncrementModeCarousel - increase the board mode (6 modes)
 */
void CapTouch_IncrementModeCarousel(void);

/**
 * CapTouch_DecrementModeCarousel - decrease the board mode (6 modes)
 */
void CapTouch_DecrementModeCarousel(void);

/**
 * CapTouch_IncrementModeBode - increase the board mode (32 modes)
 */
void CapTouch_IncrementModeBinary(void);

/**
 * CapTouch_DecrementModeBinary - decrease the board mode (32 modes)
 */
void CapTouch_DecrementModeBinary(void);

/**
 * CapTouch_ModeLEDBinary - display binary on the mode LEDs
 * @param unsigned char number - number to be displayed
 */
void CapTouch_ModeLEDBinary(uint8_t number);

/**
 * CapTouch_ModeLEDsOn - turn all mode LEDs on
 */
void CapTouch_ModeLEDsOn(void);

/**
 * CapTouch_ModeLEDsOff - turn all mode LEDs off
 */
void CapTouch_ModeLEDsOff(void);

/**
 * CapTouch_ButtonLEDsOn - turn all button LEDs on
 */
void CapTouch_ButtonLEDsOn(void);

/**
 * CapTouch_ButtonLEDsOff - turn all button LEDs off
 */
void CapTouch_ButtonLEDsOff(void);

/**
 * CapTouch_FlashButtonLEDs - flash all button LEDs x times
 * @param unsigned int numberOfBlinks - number of flashes
 */
void CapTouch_FlashButtonLEDs(unsigned int numberOfBlinks);

/**
 * CapTouch_FlashModeLEDs - flash all mode LEDs x times
 * @param unsigned int numberOfBlinks - number of flashes
 */
void CapTouch_FlashModeLEDs(unsigned int numberOfBlinks);

/**
 * CapTouch_ButtonLEDOnSequence - turn on LEDS from B1->B2->B3->B4
 */
void CapTouch_ButtonLEDOnSequence(void);

/**
 * CapTouch_ButtonLEDOffSequence - turn on LEDS from B1<-B2<-B3<-B4
 */
void CapTouch_ButtonLEDOffSequence(void);

/**
 * CapTouch_ModeLEDsScroll - scroll the mode LEDs to the right
 * 		(does not affect the current mode)
 * @param unsigned int numberOfScolls - number of shifts from left to right
 */
void CapTouch_ModeLEDsScroll(unsigned int numberOfScrolls);

/**
 * CapTouch_RepeatOnHold - repeat the effect when button is held
 */
void CapTouch_RepeatOnHold(void);

/**
 * CapTouch_RepeatOnHoldWait - hold button and wait X ms before playing repeat waveform
 */
void CapTouch_RepeatOnHoldWait(void);

/**
 * CapTouch_EnableOnHold - hold a button to call a function
 * @param action - pointer to function to be called after hold
 */
void CapTouch_EnableOnHold(void (*action)());

/**
 * CapTouch_FreezeLEDs - freeze the current state of the LEDs
 */
void CapTouch_FreezeLEDs(void);

/**
 * CapTouch_UnfreezeLEDs - unfreeze the current state of the LEDs
 */
void CapTouch_UnfreezeLEDs(void);

/**
 * CapTouch_RepeatReset - to be called in main loop when button is released
 */
void CapTouch_RepeatReset(void);

/**
 * CapTouch_ModeRepeatReset - to be called in main loop when a mode button is released
 */
void CapTouch_ModeRepeatReset(void);

#endif /* CAPTOUCHBOARD_H_ */
