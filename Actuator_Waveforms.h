/******************************************************************************
 * Actuator_Waveforms.h
 *
 * Created on: Dec 16, 2011
 * Board: DRV2603EVM-CT RevD
 * Author: a0866685
 *
 * Desc: This file contains the waveform data arrays.  See instructions in
 * 			Actuator_Waveforms.c to create new waveforms.
 *
 * Modified: Feb 15, 2011 - Updated for DRV2603EVM-CT RevC
 *
 ******************************************************************************/

#include "Haptics.h"

#define LRA_AUTOON_MAX	 	0xF0 	// Set the maximum amplitude for auto-resonance ON mode
#define LRA_AUTOOFF_MAX		0xD8 	//0xE6	// Set the maximum amplitude for auto-resonance OFF mode

//--------------------------------------------------------//
//LRA Standard Effects
//--------------------------------------------------------//
extern const Waveform lra_click;
extern const Waveform lra_click_nobrake;
extern const Waveform lra_doubleclick;
extern const Waveform lra_doubleclick_nobrake;
extern const Waveform lra_alert;
extern const Waveform lra_rampup;
extern const Waveform lra_rampdown;

//--------------------------------------------------------//
//LRA Standard Effects in Dumb Mode
//Note: Using ERM mode to emulate dumb drive, so it must be scaled for 2Vrms
//--------------------------------------------------------//
extern const Waveform lra_click_dumb;
extern const Waveform lra_click_nobrake_dumb;
extern const Waveform lra_doubleclick_dumb;
extern const Waveform lra_doubleclick_nobrake_dumb;
extern const Waveform lra_alert_dumb;

//--------------------------------------------------------//
//ERM Standard Effects
//--------------------------------------------------------//
extern const Waveform erm_click;
extern const Waveform erm_bump;
extern const Waveform erm_doubleclick;
extern const Waveform erm_doublebump;
extern const Waveform erm_alert;
extern const Waveform erm_rampup;
extern const Waveform erm_rampdown;

//--------------------------------------------------------//
//Subtle LRA Effects
//--------------------------------------------------------//
extern const Waveform lra_tick;
extern const Waveform lra_softclick;
extern const Waveform lra_softbump;
extern const Waveform lra_softalert;

//--------------------------------------------------------//
//Simon LRA Effects
//--------------------------------------------------------//
extern const Waveform lra_rampupdoubleclick;
extern const Waveform lra_threeclicks;
