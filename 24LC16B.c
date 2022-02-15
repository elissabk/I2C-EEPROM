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

//// 24LC16B.c is the driver needed to communicate with the EEPROM being used in this project ////

#include <I2C_lib.h>

#ifndef _24LC16B_
#define _24LC16B_

#define EE_WP PIN_C5
#bit    EE_WP_T =   TRISC.5

unsigned int1 EEPROM_BUSY();

void EEPROM_init(){
   EE_WP_T = 0;
   output_low(EE_WP);
}

unsigned int8 EEPROM__READ(unsigned int8 block, unsigned int8 address){
   unsigned int8 data;
   unsigned int8 control_byte;
   control_byte = 0xA0 | (block<<1);
   I2C__START();
   if(I2C__WRITE(control_byte)==1){           //if NACK
      I2C__STOP();
   }
   else{
      if(I2C__WRITE(address)==1){             //if NACK
         I2C__STOP();
      }
      else{
         I2C__RSTART();
         if(I2C__WRITE(control_byte | 0x01)==1){
            I2C__STOP();
         }
         else{
            data = I2C__READ(1);              //master reads and NACKs
            I2C__STOP();
         }
      }
   }
   return(data);
}

void EEPROM_S_READ(unsigned int8 block, unsigned int8 address, unsigned int8 length, unsigned int8 *data){
   if(length>254){
      length = 254;
   }
   unsigned int8 control_byte;
   unsigned int8 i;
   control_byte = 0xA0 | (block<<1);
   I2C__START();
   if(I2C__WRITE(control_byte)==1){           //if NACK
      I2C__STOP();
   }
   else{
      if(I2C__WRITE(address)==1){             //if NACK
         I2C__STOP();
      }
      else{
         I2C__RSTART();
         if(I2C__WRITE(control_byte | 0x01)==1){  //if NACK
            I2C__STOP();
         }
         else{
            for(i=address; i<(address+(length)); i++){
               *data++=I2C__READ(0);
            }
            
         *data = I2C__READ(1);                //master reads and NACKs
         I2C__STOP();
         }
      }
   }
}

void EEPROM__WRITE(unsigned int8 block, unsigned int8 address, unsigned int8 data){
   unsigned int8 control_byte;
   control_byte = 0xA0 | (block<<1);
   I2C__START();
   if(I2C__WRITE(control_byte)==1){           //if NACK
      I2C__STOP();
   }
   else{
      if(I2C__WRITE(address)==1){             //if NACK
         I2C__STOP();
      }
      else{
         I2C__WRITE(data);
         I2C__STOP();
         delay_ms(10);
      }
   }
}

void EEPROM_S_WRITE(unsigned int8 block, unsigned int8 address, unsigned int8 length, unsigned int8 *data){
   unsigned int8 control_byte;
   unsigned int8 i;
   control_byte = 0xA0 | (block<<1);
   I2C__START();
   if(I2C__WRITE(control_byte)==1){           //if NACK
      I2C__STOP();
   }
   else{
      if(I2C__WRITE(address)==1){             //if NACK
         I2C__STOP();
      }
      else{
         for(i=address ; i<(address+length) ; i++){
            if(I2C__WRITE(*data)==1){         //if NACK
               I2C__STOP();
            }
            else{
               *data++;
            }
         }
         I2C__STOP();
         delay_ms(10);
      }
   }
}

void EEPROM_P_WRITE(unsigned int8 block, unsigned int8 address, unsigned int8 length, unsigned int8 *data){
   unsigned int8 count=1;
   unsigned int8 last_limit=0;
   if(length>16){
      count = length/16;
      last_limit = length - (count*16);
   }
   unsigned int8 i;
   for(i=0; i<count ;i++){
      EEPROM_S_WRITE(block,((address+16)*i),16,&data[(i*16)]);
      delay_ms(2);
   }
   if(last_limit!=0){
      EEPROM_S_WRITE(block, ((address+16)*count), (last_limit), &data[(count*16)]);
   }
   I2C__STOP();
   delay_ms(10);
}

unsigned int1 EEPROM_BUSY(){            // ? should i use this as a way to wait for an ACK from the eeprom ? if so, how and when ?
   unsigned int1 ACK;
   unsigned int8 control_byte = 0xA0;
   I2C__START();
   if(I2C__WRITE(control_byte)==1){           //if NACK
      ACK=1;
      unsigned int8 counter = 0;
      for(counter=0; counter<100; counter++){
         I2C__START();
         if(I2C__WRITE(control_byte)==0){     //if ACK
            ACK=0;
            break;
         }
      }
   }
   else{                                      //ACK
      ACK=0;
   }
   return(ACK);
}

#endif

