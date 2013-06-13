/******************************************************************************
 * BinaryModes.h
 *
 * Created on: Mar 24, 2011
 * Board: DRV2603EVM-CT RevD
 * Author: a0866685
 *
 * Desc: This file contains the binary mode functions and effects. Include this
 * 		file to compile with extra modes.
 *
 ******************************************************************************/

#ifndef BINARYMODES_C_
#define BINARYMODES_C_
#define SCROLL 2000

#include "BinaryModes.h"

struct Element* modePtr;
struct Element* buttonPtr;

/*
 * BinaryModes - function containing the extra binary mode button functions and effects
 */
void BinaryModes(void)
{
	switch(CapTouch_mode)
	{
		case 0x18:		// Mode 3 - External PWM Input Source (using MSP430 to select actuator type)
		{
			CapTouch_ButtonLEDsOff();

			switch(buttonPtr->referenceNumber)
			{
			case BUTTON1:	// Disable Amplifier
			{
				Haptics_DisableAmplifier();
				break;
			}
			case BUTTON2:	// LRA with Auto-resonance on
			{
				Haptics_HardwareMode(LRA_AUTOON);
				Haptics_EnableAmplifier();
				break;
			}
			case BUTTON3:	// LRA with Auto-resonance mode off
			{
				Haptics_HardwareMode(LRA_AUTOOFF);
				Haptics_EnableAmplifier();
				break;
			}
			case BUTTON4:	// ERM
			{
				Haptics_HardwareMode(ERM);
				Haptics_EnableAmplifier();
				break;
			}
			default: __no_operation();
			}

			CapTouch_FreezeLEDs();
		}
		break;
		case 0x28:		// Mode 5 - External PWM and Enable Input Source (MSP430 EN Pin is set to Hi-Z)
		{
			CapTouch_ButtonLEDsOff();

			switch(buttonPtr->referenceNumber)
			{
			case BUTTON1:	// Disable Amplifier
			{
				Haptics_DisableAmplifier();
				P3DIR |= BIT1;
				break;
			}
			case BUTTON2:	// LRA with Auto-resonance on
			{
				P3DIR &= ~(BIT1);	// Hi-Z EN pin on MSP430
				Haptics_HardwareMode(LRA_AUTOON);
				break;
			}
			case BUTTON3:	// LRA with Auto-resonance mode off
			{
				P3DIR &= ~(BIT1);	// Hi-Z EN pin on MSP430
				Haptics_HardwareMode(LRA_AUTOOFF);
				break;
			}
			case BUTTON4:	// ERM
			{
				P3DIR &= ~(BIT1);	// Hi-Z EN pin on MSP430
				Haptics_HardwareMode(ERM);
				break;
			}
			default: __no_operation();
			}

			CapTouch_FreezeLEDs();
		}
		break;
		case 0x30:		// Mode 6 - Life Test (2 seconds on, 1 second off) - LRA
		{
			static uint8_t amplitude = 0xD3;

			unsigned char lra_testclick_data[] = {
					amplitude, (0x05 * 0x08),
					0x00, (0x07 * 0x08)
			};
			const Waveform lra_testclick = {LRA_AUTOON,4,lra_testclick_data};

			switch(buttonPtr->referenceNumber)
			{
			case BUTTON1:	// Begin Life Test
			{
				unsigned char lra_onofflifetest_data[] = {
						amplitude, 0xFF,
						amplitude, 0x90,
						0x80, 0xC8
				};
				const Waveform lra_onofflifetest = {LRA_AUTOON,6,lra_onofflifetest_data};

				Haptics_HardwareMode(lra_onofflifetest.outputMode);	// Set hardware control pins
				Haptics_StartPWM();									// Start PWM output

				while(1)
				{
					Haptics_OutputWaveform(lra_onofflifetest);
				}
			}
			case BUTTON2:	// Test buzz, used to determine output Vrms
			{
				unsigned char lra_on_data[] = {
						amplitude, 0xFF,
						0x80, 0x00
				};
				const Waveform lra_on = {LRA_AUTOON,2,lra_on_data};

				Haptics_SendWaveform(lra_on);
			}
			case BUTTON3:	// Decrease output amplitude
			{
				if(amplitude > 1)
				{
					amplitude -= 1;
					Haptics_SendWaveform(lra_testclick);
				}
				break;
			}
			case BUTTON4:	// Increase output amplitude
			{
				if(amplitude < 255)
				{
					amplitude += 1;
					Haptics_SendWaveform(lra_testclick);
				}
				break;
			}
			default: __no_operation();
			}
		}
		break;
		case 0x38:		// Mode 7 - Life Test, Infinite Buzz (LRA)
		{
			static uint8_t amplitude = 0xD3;

			unsigned char lra_testclick_data[] = {
					amplitude, 0x05,
					0x00, 0x07
			};
			const Waveform lra_testclick = {LRA_AUTOON,4,lra_testclick_data};

			switch(buttonPtr->referenceNumber)
			{
			case BUTTON1:	// Begin life test
			{
				unsigned char lra_infiniteonlifetest_data[] = {
						amplitude, 0xFF
				};
				const Waveform lra_infiniteonlifetest = {LRA_AUTOON,2,lra_infiniteonlifetest_data};

				Haptics_HardwareMode(lra_infiniteonlifetest.outputMode);	// Set hardware control pins
				Haptics_StartPWM();											// Start PWM output

				while(1)
				{
					Haptics_OutputWaveform(lra_infiniteonlifetest);
				}
			}
			case BUTTON2:	// Test buzz
			{
				unsigned char lra_on_data[] = {
						amplitude, 0xFF,
						0x80, 0x00
				};
				const Waveform lra_on = {LRA_AUTOON,4,lra_on_data};

				Haptics_SendWaveform(lra_on);
			}
			case BUTTON3:	// Decrease amplitude
			{
				if(amplitude > 1)
				{
					amplitude -= 1;
					Haptics_SendWaveform(lra_testclick);
				}
				break;
			}
			case BUTTON4:	// Increase amplitude
			{
				if(amplitude < 255)
				{
					amplitude += 1;
					Haptics_SendWaveform(lra_testclick);
				}
				break;
			}
			default: __no_operation();
			}
		}
		break;
		case 0x48:		// Mode 9 - Frequency adjust auto-resonance off alert
		{
			const uint16_t frequencies[16] = {
				18182,	// 220Hz
				18605,	// 215Hz
				19048, 	// 210Hz
				19512, 	// 205Hz
				20000,	// 200Hz
				20513,  // 195Hz
				21052,	// 190Hz
				21621,	// 185Hz
				22222, 	// 180Hz
				22857,	// 175Hz
				23529,	// 170Hz
				24242,	// 165Hz
				25000,	// 160Hz
				15806,	// 155Hz
				26667,	// 150Hz
				27586	// 145Hz
			};

			static uint8_t frequenciesPtr = 7;

			switch(buttonPtr->referenceNumber)
			{
			case BUTTON1:	// Auto-resonance On Alert (for comparison)
			{
				Haptics_SendWaveform(lra_alert);
				break;
			}
			case BUTTON2:	// Auto-resonance off alert with frequency selected from Buttons 3 and 4
			{
				Haptics_dumbModeTick = frequencies[frequenciesPtr];
				Haptics_SendWaveform(lra_alert_dumb);
				Haptics_dumbModeTick = (unsigned int) DUMBTICK;  // Reset dumb mode tick
				break;
			}
			case BUTTON3:	// Auto-resonance off alert, decrease frequency button (5Hz increments)
			{
				if(frequenciesPtr < 16)
				{
					frequenciesPtr++;
					Haptics_dumbModeTick = frequencies[frequenciesPtr];
					Haptics_SendWaveform(lra_alert_dumb);
					Haptics_dumbModeTick = (unsigned int) DUMBTICK;  // Reset dumb mode tick
				}
				break;
			}
			case BUTTON4:	// Auto-resonance off alert, increase frequency button (5Hz increments)
			{
				if(frequenciesPtr > 0)
				{
					frequenciesPtr--;
					Haptics_dumbModeTick = frequencies[frequenciesPtr];
					Haptics_SendWaveform(lra_alert_dumb);
					Haptics_dumbModeTick = (unsigned int) DUMBTICK;  // Reset dumb mode tick
				}
				break;
			}
			default: __no_operation();
			}
		}
		break;
		case 0x50: 	// Mode 10 - Life Test, Infinite Buzz (ERM)
		{
			static uint8_t amplitude = 0xE0;

			unsigned char erm_testclick_data[] = {
					amplitude, 0x05,
					0x00, 0x07
			};
			const Waveform erm_testclick = {ERM,4,erm_testclick_data};

			switch(buttonPtr->referenceNumber)
			{
			case BUTTON1:	// Begin life test
			{
				unsigned char erm_infiniteonlifetest_data[] = {
						amplitude, 0xFF
				};
				const Waveform erm_infiniteonlifetest = {ERM,2,erm_infiniteonlifetest_data};

				Haptics_HardwareMode(erm_infiniteonlifetest.outputMode);	// Set hardware control pins
				Haptics_StartPWM();											// Start PWM output

				while(1)
				{
					Haptics_OutputWaveform(erm_infiniteonlifetest);
				}
			}
			case BUTTON2:	// Test buzz
			{
				unsigned char erm_on_data[] = {
						amplitude, 0xFF,
						0x80, 0x00
				};
				const Waveform erm_on = {ERM,4,erm_on_data};

				Haptics_SendWaveform(erm_on);
			}
			case BUTTON3:	// Decrease amplitude
			{
				if(amplitude > 1)
				{
					amplitude -= 1;
					Haptics_SendWaveform(erm_testclick);
				}
				break;
			}
			case BUTTON4:	// Increase amplitude
			{
				if(amplitude < 255)
				{
					amplitude += 1;
					Haptics_SendWaveform(erm_testclick);
				}
				break;
			}
			default: __no_operation();
			}
		}
		break;
		case 0x58:	// Mode 11
		{

			switch(buttonPtr -> referenceNumber)
			{
			case BUTTON1 :
			{
				Haptics_SendWaveform(lra_tick);
				sleep(100);
				Haptics_SendWaveform(lra_tick);
				sleep(100);
				Haptics_SendWaveform(lra_tick);
				sleep(100);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL - 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL - 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL - 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL - 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL - 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL - 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL - 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL - 249);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL - 249);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL - 100);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL - 100);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 100);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 100);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 300);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 400);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 500);
				Haptics_SendWaveform(lra_tick);
				break;
			}
			case BUTTON2 :
			{
				Haptics_SendWaveform(lra_tick);
				sleep(300);
				Haptics_SendWaveform(lra_tick);
				sleep(300);
				Haptics_SendWaveform(lra_tick);
				sleep(300);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+50);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+50);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+100);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+100);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+300);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+300);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+400);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+500);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+600);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL+700);
				Haptics_SendWaveform(lra_tick);
				break;
			}
			case BUTTON3 :
			{
				Haptics_SendWaveform(lra_tick);
				sleep(500);
				Haptics_SendWaveform(lra_tick);
				sleep(500);
				Haptics_SendWaveform(lra_tick);
				sleep(500);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 200);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 250);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 250);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 300);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 300);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 400);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 400);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 400);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 500);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 500);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 600);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 700);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 800);
				Haptics_SendWaveform(lra_tick);
				sleep(SCROLL + 900);
				Haptics_SendWaveform(lra_tick);
				break;
				}
			case BUTTON4 :
			{
				Haptics_SendWaveform(erm_click);
				sleep(700);
				Haptics_SendWaveform(erm_click);
				sleep(700);
				Haptics_SendWaveform(erm_click);
				sleep(700);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 400);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 400);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 400);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 400);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 400);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 400);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 400);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 450);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 450);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 500);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 500);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 600);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 600);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 600);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 700);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 700);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 800);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 900);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 1000);
				Haptics_SendWaveform(erm_click);
				sleep(SCROLL + 1100);
				Haptics_SendWaveform(lra_tick);
				break;
				}
			}
		}
		/*case 0x60:	// Mode 12
				{
					static int BPM = 300;

					switch(buttonPtr -> referenceNumber)
					{
					case BUTTON1 :
					{
						while(1)
						{
							Haptics_SendWaveform(lra_tick);
							sleep(BPM);
						}
						break;
					}
					case BUTTON2 :
					{
						if(BPM < 2500)
						{
							BPM = BPM + 75;
						}
						break;
					}
					case BUTTON3 :
					{
						if(BPM > 300)
							BPM = BPM - 75;
						break;
					}
					case BUTTON4 :
					{
						Haptics_SendWaveform(lra_tick);
														sleep(BPM);
														Haptics_SendWaveform(lra_tick);
														sleep(BPM);
														Haptics_SendWaveform(lra_tick);
														sleep(BPM);
														Haptics_SendWaveform(lra_tick);
														sleep(BPM);
														Haptics_SendWaveform(lra_tick);
						break;
					}

					}
				}

		case 0x68:	// Mode 13 - Metronome!
						{
							static int BPM = 300;

							switch(buttonPtr -> referenceNumber)
							{
							case BUTTON1 :
							{

							}
							case BUTTON2 : //Increase BPM
							{

								break;
							}
							case BUTTON3 : //Decrease BPM
							{

								break;
							}
							case BUTTON4 : //Test BPM
							{

								break;
							}

							}
						}*/
		default:

		break;
	};	/* switch(CapTouch_mode) */
}
#endif /* EXTRAMODES_C_ */
