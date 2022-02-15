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

//////////// I2C_lib.h is the driver to bit-bang the I2C protocol in this project ////////////

#ifndef _I2C_C
#define _I2C_C

#ifndef _I2C_PINS_
#define _I2C_PINS_

#byte TRISC = 0xF94
#bit SCL1_T = TRISC.3
#bit SDA1_T = TRISC.4
#bit test_T = TRISC.7
#byte PORTC = 0xF82
#bit SCL1 = PORTC.3
#bit SDA1 = PORTC.4

#endif

unsigned int16 I2C__PULSE;

void I2C__INIT(void){                              //I2C bus idle state
   SCL1_T = 1;            
   SDA1_T = 1;
}

void I2C_freq_kHz(unsigned int16 freq_I2C){   //I2C frequency calculator
   I2C__PULSE = (unsigned int16)(500/freq_I2C);           //pulse = 1/2 freq ; pulse in us = (1000/freq_I2C in kHz) /2
}

void I2C__start(void){                        //I2C master start
   SDA1_T = 0;
   SDA1 = 0;
   delay_us(I2C__PULSE);
}

void I2C__stop(void){                         //I2C master stop
   SDA1_T = 0;
   SDA1 = 0;
   SCL1_T = 1;
   SDA1_T = 1;
}

unsigned int1 I2C__ACK(void){                 //I2C master ACK/NACK detection
   unsigned int1 ACK;
   if(SDA1==0){
      ACK=0;                                  //ACK
   }
   else{
      ACK=1;                                  //NACK
   }
   return(ACK);
}

void I2C__rstart(void){                       //I2C master repeated start
   SDA1_T=0;
   SDA1=0;   
      SCL1_T=0;
      SCL1=0;
      SDA1_T=1;
   SCL1_T=1;
   SDA1_T=0;
   SDA1=0;
}

unsigned int8 I2C__read(unsigned int1 ACK){
   unsigned int8 data;
   unsigned int i;
   SDA1_T=1;
   delay_us(I2C__PULSE);
   for(i=8 ; i>0 ; i--){                      //reading data from slave
      SCL1_T=0;
      SCL1=0;
      if(SDA1==1){                            //data is set on falling edge of CLK
         bit_set(data,(i-1));
      }
      else{
         bit_clear(data,(i-1));
      }
      delay_us(I2C__PULSE);
      SCL1_T=1;
      delay_us(I2C__PULSE);
   }
   SCL1_T=0;
   SCL1=0;
   // 8-bits were read
   
   //LOW period of 8th clock cycle
   
   if(ACK==1){                                //NACK
      SDA1_T=1;
   }
   else if(ACK==0){                           //ACK --> Master ACKs the receiver
      SDA1_T=0;
      SDA1=0;
   }
   delay_us(I2C__PULSE);
   SCL1_T=1;                                  //HIGH period of 9th clock cycle
   delay_us(I2C__PULSE);
   SCL1_T=0;
   SCL1=0;
   delay_us(I2C__PULSE);                      //LOW period of 9th clock cycle
   
   return(data);
}

unsigned int1 I2C__write(unsigned int8 data){
   unsigned int i;
   for(i=0 ; i<8 ; i++){                      //send data
      SCL1_T=0;
      SCL1=0;
      if( (data & 0x80) == 0x80 ){
         SDA1_T = 1;
      }
      else{
         SDA1 = 0;
         SDA1_T = 0;
      }
      delay_us(I2C__PULSE);
      SCL1_T=1;
      delay_us(I2C__PULSE);
      data <<= 1;
   }
   
   SCL1_T=0;
   SCL1=0;
   delay_us(I2C__PULSE);                      //LOW period of 8th clock cycle
   SCL1_T=1;                                  //HIGH period of 9th clock cycle
   SDA1_T=1;
   while(!SCL1);                              //clock stretching detection
   unsigned int1 ACK;
   ACK=I2C__ACK();                            //ACK detection
   
   delay_us(I2C__PULSE);
   SCL1_T=0;
   SCL1=0;
   delay_us(I2C__PULSE);                      //LOW period of 9th clock cycle
   return(ACK);
}

#endif



