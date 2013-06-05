/******************************************************************************
 * Haptics.c
 *
 * Created on: Dec 16, 2011
 * Board: DRV2603EVM-CT RevD
 * Author: a0866685
 *
 * Description: This file contains the functions for sending haptics waveforms.
 * 		Edit this file when changing actuators.
 *
 * @TODO - Update this file when changing between actuators types
 * 			(ERM, LRA, Piezo)
 *
 * a type of unsigned integer of length 8 bits = unit8_t
 ******************************************************************************/
#include "Haptics.h"

// private variables
static uint8_t  j,k;
static uint8_t 	playEffect = 1;		// if 1 = play, 0 = do not play

// public variables
uint16_t Haptics_dumbModeTick = DUMBTICK;		// Sets the LRA Auto-resonance off frequency (use DUMBTICK above to set frequency)
uint16_t Haptics_resonantModeTick;

/**
 * Haptics_Init - initialize haptics variables and settings
 */
void Haptics_Init(void)
{
	Haptics_resonantModeTick = (unsigned int) Haptics_dumbModeTick * 2;
}

/**
 * Haptics_SendWaveform - setup and send haptic waveform
 * @param struct Waveform - the waveform output type, length in bytes, and data)
 */
void Haptics_SendWaveform(const Waveform waveform)
{
	if(playEffect)
	{
		Haptics_HardwareMode(waveform.outputMode);	// Set hardware control pins
		Haptics_StartPWM();							// Start PWM output

		Haptics_OutputWaveform(waveform);			// Control the PWM or I2C

		Haptics_StopPWM(); 		// Stop PWM output
	}
}

/**
 * Haptics_OutputWaveform - control the PWM output pattern
 * @param struct Waveform - the waveform output type, length in bytes, and data
 * @TODO - Modify this function to change actuator types (ERM, LRA, Piezo)
 */
void Haptics_OutputWaveform(const Waveform waveform)
{
	switch(waveform.outputMode)
	{
	case LRA_AUTOON: 	// LRA with Auto-Resonance
		for(k=0; k < waveform.length; k=k+2)
		{
			if(waveform.data[k] == 0x80)
				P3OUT &= 0xFD;               		//Disable Amplifier
			else
				P3OUT |= 0x02;          			//Enable Amplifier, Start PWM

			TA1CCR1 = waveform.data[k];  			//Waveform*1.625
			for(j=0;j<waveform.data[k+1];++j)
			{
				timerdelay(Haptics_resonantModeTick);
			}
		}
		break;
	case LRA_AUTOOFF:		// LRA without Auto-Resonance
		for(k=0; k<waveform.length; k=k+2)
		{
			if(waveform.data[k] == 0x80)
				P3OUT &= 0xFD;               		//Disable Amplifier
			else
				P3OUT |= 0x02;          			//Enable Amplifier, Start PWM

			for(j=0;j<waveform.data[k+1];++j)
			{
				TA1CCR1 = waveform.data[k];
				timerdelay(Haptics_dumbModeTick);
				TA1CCR1 = 255 - waveform.data[k];
				timerdelay(Haptics_dumbModeTick);
			}
		}
		break;
	case ERM:		// ERM
		for(k=0; k<waveform.length; k=k+2)
		{
			TA1CCR1 = waveform.data[k];  			//Waveform*1.625
			for(j=0;j<waveform.data[k+1];++j)
			{
				timerdelay(Haptics_resonantModeTick);
			}
		}
		break;
	}
}


/**
 * Haptics_HardwareMode - Set the hardware pins to the appropriate setting
 * @param unsigned char outputMode - the waveform output type
 * @TODO - Modify this function to change actuator types (ERM, LRA, Piezo)
 */
void Haptics_HardwareMode(uint8_t outputMode)
{
	switch(outputMode)
	{
	case LRA_AUTOON: 	// LRA with Auto-Resonance
		P3OUT |= 0x01;      // Select LRA Mode on DRV2603
		P2OUT |= 0x40;      // Select LRA on Load Switch
		break;
	case LRA_AUTOOFF: 	// LRA without Auto-Resonance
		P3OUT &= 0xFE;      // Select ERM Mode on DRV2603
		P2OUT |= 0x40;      // Select LRA on Load Switch
		break;
	case ERM: 			// ERM Mode
		P3OUT &= 0xFE;      // Select ERM Mode on DRV2603
		P2OUT &= 0xBF;      // Select ERM on Load Switch
		break;
	default:			// LRA with Auto-resonance
		P3OUT |= 0x01;      // Select LRA Mode on DRV2603
		P2OUT |= 0x40;      // Select LRA on Load Switch
		break;
	}
}

/**
 * Haptics_StartPWM - Initialize PWM clocks and enable the haptics driver
 */
void Haptics_StartPWM(void)
{
	if(playEffect)
	{
		P3OUT |= 0x02;                	// Enable Amplifier, Start PWM
		BCSCTL2 = DIVS_0;               // SMCLK/(0:1,1:2,2:4,3:8)
		TA1R=0;                        	// Reset PWM Count
		TA1CCTL1 |= OUTMOD_7 + CCIE;   	// PWM Set/Reset Mode
		TA1CTL = TASSEL_2 + MC_1;       // 2: TACLK = SMCLK
		//TA1CCR1 = 0xFF/2;              	// Send 50%
		//timerdelay(6400);              	// 1 ms Startup delay
	}
}

/**
 * Haptics_StartPWM - Stop PWM clocks and disable the haptics driver
 */
void Haptics_StopPWM(void)
{
	if(playEffect)
	{
		P3OUT &= 0xFD;                   // Disable Amplifier
		TA1CCR1 = 0x00;
		TA1CTL = 0x0004;                 // Stop PWM
		TA1CCTL1 &= ~(OUTMOD_7 | OUT);   // PWM output = LOW
		P3OUT &= 0xFB;
		BCSCTL2 |= DIVS_0;               // SMCLK/(0:1,1:2,2:4,3:8)
	}
}

/**
 * Haptics_EnableAmplifier - set EN pin high
 */
void Haptics_EnableAmplifier(void)
{
	P3OUT |= 0x02;
}

/**
 * Haptics_DisableAmplifier - set EN pin low
 */
void Haptics_DisableAmplifier(void)
{
	P3OUT &= ~(0x02);
}

/**
 * Haptics_OutputEnableSet - enable/disable play back of all waveforms
 */
void Haptics_OutputEnableSet(uint8_t enable)
{
	playEffect = enable;
}

/**
 * Haptics_IsOutputEnabled - check if waveform play back is enabled
 */
uint8_t Haptics_IsOutputEnabled(void)
{
	return playEffect;
}

