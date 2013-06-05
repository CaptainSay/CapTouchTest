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
 ******************************************************************************/

#ifndef HAPTICS_H_
#define HAPTICS_H_

#include "msp430.h"
#include "CTS/structure.h"
#include "timer.h"

#define DELAY 		250
#define DUMBTICK  	LRAFREQ_185	// Select the LRA resonant frequency for "dumb" (auto-resonance off) mode

// LRA Resonant Frequencies (see DUMBTICK above)
#define LRAFREQ_220	18182	// 220Hz
#define LRAFREQ_215	18605	// 215Hz
#define LRAFREQ_210	19048 	// 210Hz
#define LRAFREQ_205	19512 	// 205Hz
#define LRAFREQ_200 20000	// 200Hz
#define LRAFREQ_195 20513   // 195Hz
#define LRAFREQ_190 21052	// 190Hz
#define LRAFREQ_185	21621	// 185Hz
#define LRAFREQ_180	22222 	// 180Hz
#define LRAFREQ_175	22857	// 175Hz
#define LRAFREQ_170	23529	// 170Hz
#define LRAFREQ_165 24242	// 165Hz
#define LRAFREQ_160 25000	// 160Hz
#define LRAFREQ_155 15806	// 155Hz
#define LRAFREQ_150	26667	// 150Hz
#define LRAFREQ_145	27586	// 145Hz

// PWM output modes
#define LRA_AUTOON 		0		// LRA Auto-resonance on
#define LRA_AUTOOFF		1		// LRA Auto-resonance off
#define ERM 			2		// ERM Output Mode

extern uint16_t Haptics_dumbModeTick;		// Sets the LRA Auto-resonance off frequency (use DUMBTICK above to set frequency)
extern uint16_t Haptics_resonantModeTick;

// Waveform Structure Type Definition
typedef struct Haptics_Waveform {
	const unsigned char 	outputMode; 		// ERM, LRA_AUTOON, or LRA_AUTOOFF (see output modes above)
	const unsigned char		length;				// size of array in bytes
	const unsigned char* 	data;				// pointer to waveform array data (waveform array is in (amplitude, time) pairs
} Waveform;

/**
 * Haptics_Init - initialize haptics variables and settings
 */
void Haptics_Init(void);

/**
 * Haptics_SendWaveform - send haptic waveform
 * @param struct Waveform - the waveform output type, length in bytes, and data
 */
void Haptics_SendWaveform(const Waveform waveform);

/**
 * Haptics_OutputWaveform - control the PWM output pattern
 * @param struct Waveform - the waveform output type, length in bytes, and data
 * @TODO - Modify this function to change actuator types (ERM, LRA, Piezo)
 */
void Haptics_OutputWaveform(const Waveform waveform);

/**
 * Haptics_StartPWM - Initialize PWM clocks and enable the haptics driver
 */
void Haptics_StartPWM(void);

/**
 * Haptics_StartPWM - Stop PWM clocks and disable the haptics driver
 */
void Haptics_StopPWM(void);

/**
 * Haptics_HardwareMode - Set the hardware pins to the appropriate setting
 * @param unsigned char outputMode - the waveform output type
 * @TODO - Modify this function to change actuator types (ERM, LRA, Piezo)
 */
void Haptics_HardwareMode(uint8_t outputMode);

/**
 * Haptics_OutputEnableSet - enable/disable play back of all waveforms
 */
void Haptics_OutputEnableSet(uint8_t enable);

/**
 * Haptics_EnableAmplifier - set EN pin high
 */
void Haptics_EnableAmplifier(void);

/**
 * Haptics_DisableAmplifier - set EN pin low
 */
void Haptics_DisableAmplifier(void);

/**
 * Haptics_IsOutputEnabled - check if waveform play back is enabled
 */
uint8_t Haptics_IsOutputEnabled(void);

#endif /* HAPTICS_H_ */
