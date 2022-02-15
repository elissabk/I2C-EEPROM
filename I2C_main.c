// Copyright 2021 Elissa Bou Karam
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//       http://www.apache.org/licenses/LICENSE-2.0
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

/////////////////////////////////////////////////////////////////////////////////////////////////
//  This project aims to communicate with an EEPROM by bit-banging the I2C protocol.
/////////////////////////////////////////////////////////////////////////////////////////////////


#include <I2C_main.h>

void main(void){
   
   unsigned int8 i;
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_4);
   
   I2C__INIT();
   I2C_freq_kHz((unsigned int16)10);
   EEPROM_init();
   
   delay_ms(100);
   
   for(i=0 ; i<128 ; i++){
      EEPROM__WRITE(0,i,i);
   }
      
   unsigned int8 data[17];
   for(i=0 ; i<17 ; i++){
      data[i] = 128 + i ;
   }
      
   //writing starting address 256, means writing on a new block (eeprom has 8 blocks of 256 x 8bit memory)
   EEPROM_P_WRITE(1,0,17,&data);
   
   unsigned int8 data_r1[127];
   EEPROM_S_READ(0,0,127,data_r1[0]);
   
   unsigned int8 data_r2[16];
   EEPROM_S_READ(1,0,16,data_r2[0]);
   
   //toggle this pin for test 
   output_toggle(PIN_C7);
   delay_ms(1000);
   
}



