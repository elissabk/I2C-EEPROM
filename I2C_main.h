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

//////////// I2C_main.h has all the setup needed to run the I2C_main.c program ////////////

#include <18F25K22.h>
#device ADC=8
#use delay(internal=64000000)

#ifndef _I2C_PINS_

#byte TRISC = 0xF94
#bit SCL1_T = TRISC.3
#bit SDA1_T = TRISC.4
#bit test_T = TRISC.7
#byte PORTC = 0xF82
#bit SCL1 = PORTC.3
#bit SDA1 = PORTC.4

#define _I2C_PINS_

#endif

#include <I2C_lib.h>
#include <24LC16B.c>



