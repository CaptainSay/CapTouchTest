//******************************************************************************
//  RO_PINOSC_TA_WDTp example with the MSP430G2452
//  threshold and maxResponse values must be updated for electrode design,
//  system clock settings, selection of gate measurement source, and 
//  accumulation cycles
//******************************************************************************

#include "structure.h"
#include "../CapTouchBoard.h"

//PinOsc Volume down P2.0
const struct Element button1 = {
	
			  .inputPxselRegister = (unsigned char *)&P2SEL,
              .inputPxsel2Register = (unsigned char *)&P2SEL2,  
              .inputBits = BIT0,
              // measured for a 1Mhz SMCLK
              .maxResponse = 0+655, // actual measure was 980
              .threshold = 130,
              .referenceNumber = BUTTON1,
              .sequenceNumber = 0
};      

const struct Element button2 = {

              .inputPxselRegister = (unsigned char *)&P2SEL,  
              .inputPxsel2Register = (unsigned char *)&P2SEL2,  
              .inputBits = BIT1,
              // measured for a 1Mhz SMCLK
              .maxResponse = 150+655, // actual measure was 980
              .threshold = 150,
              .referenceNumber = BUTTON2,
              .sequenceNumber = 1
};  
const struct Element button3 = {

              .inputPxselRegister = (unsigned char *)&P2SEL,  
              .inputPxsel2Register = (unsigned char *)&P2SEL2,  
              .inputBits = BIT2,
              // measured for a 1Mhz SMCLK
              .maxResponse = 130+655, // actual measure was 980
              .threshold = 130,
              .referenceNumber = BUTTON3,
              .sequenceNumber = 2
}; 
const struct Element button4 = {

              .inputPxselRegister = (unsigned char *)&P2SEL,  
              .inputPxsel2Register = (unsigned char *)&P2SEL2,  
              .inputBits = BIT3,
              // measured for a 1Mhz SMCLK
              .maxResponse = 120+655, // actual measure was 980
              .threshold = 120,
              .referenceNumber = BUTTON4,
              .sequenceNumber = 3
};           
const struct Element mode_down = {

              .inputPxselRegister = (unsigned char *)&P2SEL,  
              .inputPxsel2Register = (unsigned char *)&P2SEL2,  
              .inputBits = BIT4,
              // measured for a 1Mhz SMCLK
              .maxResponse = 130+655, // actual measure was 980
              .threshold = 130,
              .referenceNumber = BUTTONMINUS,
              .sequenceNumber = 4
};      
const struct Element mode_up = {

              .inputPxselRegister = (unsigned char *)&P2SEL,  
              .inputPxsel2Register = (unsigned char *)&P2SEL2,  
              .inputBits = BIT5,
              // measured for a 1Mhz SMCLK
              .maxResponse = 150+655, // actual measure was 980
              .threshold = 150,
              .referenceNumber = BUTTONPLUS,
              .sequenceNumber = 5
};              

//*** Sensor   *******************************************************/
// This defines the grouping of sensors, the method to measure change in
// capacitance, and the function of the group

const struct Sensor buttons =
               { 
                  .halDefinition = RO_PINOSC_TA0_WDTp,
                  .numElements = 4,
                  .baseOffset = 0,
                  // Pointer to elements
                  .arrayPtr[0] = &button1,  // point to first element
                  .arrayPtr[1] = &button2,  // 
                  .arrayPtr[2] = &button3,  // 
                  .arrayPtr[3] = &button4,  //
                  // Timer Information
                  .measGateSource= GATE_WDT_SMCLK,     //  0->SMCLK, 1-> ACLK
                  //.accumulationCycles= WDTp_GATE_32768             //32768
                  //.accumulationCycles= WDTp_GATE_8192               // 8192
                  .accumulationCycles= WDTp_GATE_512             //512
                  //.accumulationCycles= WDTp_GATE_64             //64                  
               };

const struct Sensor mode_change =
               { 
                  .halDefinition = RO_PINOSC_TA0_WDTp,
                  .numElements = 2,
                  .baseOffset = 4,
                  // Pointer to elements
                  .arrayPtr[0] = &mode_down,  // point to first element
                  .arrayPtr[1] = &mode_up,  // point to first element
                  // Timer Information
                  .measGateSource= GATE_WDT_SMCLK,     //  0->SMCLK, 1-> ACLK
                  //.accumulationCycles= WDTp_GATE_32768             //32768
                  //.accumulationCycles= WDTp_GATE_8192               // 8192
                  .accumulationCycles= WDTp_GATE_512             //512
                  //.accumulationCycles= WDTp_GATE_64             //64                  
               };
