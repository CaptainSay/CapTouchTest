/******************************************************************************
 * Actuator_Waveforms.h
 *
 * Created on: Dec 16, 2011
 * Board: DRV2603EVM-CT RevD
 * Author: a0866685
 *
 * Desc: This file contains the waveform data arrays.  See instructions below
 * 			to create new waveforms.
 *
 * Modified: Feb 15, 2011 - Updated for DRV2603EVM-CT RevC
 *
 ******************************************************************************
 * INSTRUCTIONS FOR CREATING A HAPTIC WAVEFORM
 * ****************************************************************************
 * 1. Create an array of data point pairs and label it
 * 		"effect_data". Where "effect" is the name of the
 * 		waveform. The first value in the pair is the
 * 		amplitude and the second value is the time. Each
 * 		unit of time is ~5ms.
 * 		waveform[] = {
 * 			amplitude,time,
 * 			amplitude,time,
 * 			...
 * 		};
 *
 * 2. Create a Waveform struct below and call it "effect".
 *
 * 3. The Waveform structure should be set to:
 * 		Waveform effect = {
 * 			mode, 	// LRA_AUTOON, LRA_AUTOOFF, or ERM
 * 			length, // size in bytes of "effect_data"
 * 			data	// name of the data array "effect_data"
 * 		};
 *
 *	  LRA_AUTOON - LRA with auto-resonance ON
 *	  LRA_AUTOFF - LRA waveform with auto-resonance OFF
 *	  ERM - ERM waveform
 ******************************************************************************/

#include "Actuator_Waveforms.h"

//--------------------------------------------------------//
//LRA Standard Effects
//--------------------------------------------------------//
const unsigned char lra_click_data[] = {
		LRA_AUTOON_MAX, 0x05,
		0x00, 0x07};
const Waveform lra_click = {LRA_AUTOON,4,lra_click_data};

const unsigned char lra_click_nobrake_data[] = {
		LRA_AUTOON_MAX, 0x0A,
		0x80, 0x02};
const Waveform lra_click_nobrake = {LRA_AUTOON,4,lra_click_nobrake_data};

const unsigned char lra_doubleclick_data[] = {
		LRA_AUTOON_MAX, 0x08,
		0x00, 0x09,
		LRA_AUTOON_MAX, 0x08,
		0x00, 0x09};
const Waveform lra_doubleclick = {LRA_AUTOON,8,lra_doubleclick_data};

const unsigned char lra_doubleclick_nobrake_data[] = {
		LRA_AUTOON_MAX, 0x0D,
		0x80, 0x09,
		LRA_AUTOON_MAX, 0x0D,
		0x80, 0x09};
const Waveform lra_doubleclick_nobrake = {LRA_AUTOON,8,lra_doubleclick_nobrake_data};

const unsigned char lra_alert_data[] = {
		LRA_AUTOON_MAX, 0x85};
const Waveform lra_alert = {LRA_AUTOON,2,lra_alert_data};

const unsigned char lra_rampup_data[] = {
		0xFF, 0x02,
		0x90, 0x15,
		0xA0, 0x15,
		0xB0, 0x10,
		0xD0, 0x09,
		LRA_AUTOON_MAX, 0x05,
		0x00, 0x03
};
const Waveform lra_rampup = {LRA_AUTOON,14,lra_rampup_data};

const unsigned char lra_rampdown_data[] = {
		LRA_AUTOON_MAX, 0x06,
		0xD0, 0x09,
		0xB0, 0x10,
		0xA0, 0x15,
		0x90, 0x15,
		0x00, 0x08
};
const Waveform lra_rampdown = {LRA_AUTOON,12,lra_rampdown_data};

//--------------------------------------------------------//
//LRA Standard Effects in Dumb Mode
//Note: Using ERM mode to emulate dumb drive, so it must be scaled for 2Vrms
//--------------------------------------------------------//
const unsigned char lra_click_dumb_data[] = {
		LRA_AUTOOFF_MAX, 0x0C,
		0x1A, 0x09};
const Waveform lra_click_dumb = {LRA_AUTOOFF, 4,lra_click_dumb_data};

const unsigned char lra_click_nobrake_dumb_data[] = {
		LRA_AUTOOFF_MAX, 0x0D,
		0x80, 0x09};
const Waveform lra_click_nobrake_dumb = {LRA_AUTOOFF,4,lra_click_nobrake_dumb_data};

const unsigned char lra_doubleclick_dumb_data[] = {
		LRA_AUTOOFF_MAX, 0x0C,
		0x1A, 0x09,
		LRA_AUTOOFF_MAX, 0x0C,
		0x1A, 0x09};
const Waveform lra_doubleclick_dumb = {LRA_AUTOOFF,8,lra_doubleclick_dumb_data};

const unsigned char lra_doubleclick_nobrake_dumb_data[] = {
		LRA_AUTOOFF_MAX, 0x0D,
		0x80, 0x09,
		LRA_AUTOOFF_MAX, 0x0D,
		0x80, 0x09};
const Waveform lra_doubleclick_nobrake_dumb = {LRA_AUTOOFF,8,lra_doubleclick_nobrake_dumb_data};

const unsigned char lra_alert_dumb_data[] = {
		LRA_AUTOOFF_MAX, 0x85};
const Waveform lra_alert_dumb = {LRA_AUTOOFF,2,lra_alert_dumb_data};

//--------------------------------------------------------//
//ERM Standard Effects
//--------------------------------------------------------//
const unsigned char erm_click_data[] = {
		0xFF, 0x07,
		0x00, 0x04};
const Waveform erm_click = {ERM,4,erm_click_data};

const unsigned char erm_bump_data[] = {
		0xFF, 0x05,
		0xB4, 0x07};
const Waveform erm_bump = {ERM,4,erm_bump_data};

const unsigned char erm_doubleclick_data[] = {
		0xFF, 0x06,
		0xB4, 0x03,
		0x00, 0x04,
		0x80, 0x0C,
		0xFF, 0x07,
		0xB4, 0x03,
		0x00, 0x05};
const Waveform erm_doubleclick = {ERM,14,erm_doubleclick_data};

const unsigned char erm_doublebump_data[] = {
		0xFF, 0x05,
		0xB4, 0x08,
		0x80, 0x0C,
		0xFF, 0x05,
		0xB4, 0x08};
const Waveform erm_doublebump = {ERM,10,erm_doublebump_data};

const unsigned char erm_alert_data[] = {
		0xFF, 0x03,
		0xB4, 0x60};
const Waveform erm_alert = {ERM,4,erm_alert_data};

const unsigned char erm_rampup_data[] = {
		0x90, 0x15,
		0xA0, 0x15,
		0xB0, 0x09,
		0xC0, 0x05,
		0xFF, 0x04,
		0x00, 0x04
};
const Waveform erm_rampup = {ERM,12,erm_rampup_data};

const unsigned char erm_rampdown_data[] = {
		0xFF, 0x04,
		0xC0, 0x09,
		0xB0, 0x10,
		0xA0, 0x15,
		0x90, 0x15,
		0x00, 0x02
};
const Waveform erm_rampdown = {ERM,12,erm_rampdown_data};

//--------------------------------------------------------//
//Subtle LRA Effects
//--------------------------------------------------------//
const unsigned char lra_tick_data[] = {
		0xFF, 0x02,
		0x00, 0x02};
const Waveform lra_tick = {LRA_AUTOON,4,lra_tick_data};

const unsigned char lra_softclick_data[] = {
		0xC0, 0x06,
		0x00, 0x07};
const Waveform lra_softclick = {LRA_AUTOON,4,lra_softclick_data};

const unsigned char lra_softbump_data[] = {
		0xC0, 0x05,
		0xA0, 0x07};
const Waveform lra_softbump = {LRA_AUTOON,4,lra_softbump_data};

const unsigned char lra_softalert_data[] = {
		0xB0, 0x80};
const Waveform lra_softalert = {LRA_AUTOON,2,lra_softalert_data};

//--------------------------------------------------------//
// Simon LRA Effects
//--------------------------------------------------------//
const unsigned char lra_rampupdoubleclick_data[] = {
		LRA_AUTOON_MAX, 0x02,
		0x90, 0x40,
		0xA0, 0x40,
		0xB0, 0x30,
		0xD0, 0x20,
		0x00, 0x09,
		LRA_AUTOON_MAX, 0x08,
		0x00, 0x09,
		LRA_AUTOON_MAX, 0x09,
		0x00, 0x09
};
const Waveform lra_rampupdoubleclick = {LRA_AUTOON,20,lra_rampupdoubleclick_data};

const unsigned char lra_threeclicks_data[] = {
		LRA_AUTOON_MAX, 0x02,
		0x90, 0x0C,
		0x00, 0x09,
		0xD0, 0x0C,
		0x00, 0x09,
		LRA_AUTOON_MAX, 0x0F,
		0x00, 0x09,
		LRA_AUTOON_MAX, 0x0C,
		0x00, 0x09,
		LRA_AUTOON_MAX, 0x0C,
		0x00, 0x09
};
const Waveform lra_threeclicks = {LRA_AUTOON,22,lra_threeclicks_data};
