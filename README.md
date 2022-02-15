# I2C-EEPROM
This project aims to bit-bang the I2C protocol and create a driver to communicate with an EEPROM.

1. Using CCS C complier, a bit-bang i2c master driver was developed with the following options:
  a. The I2C driver has the following main functions:
    i.  Start 
    ii. Stop
    iii.I2c write, with ability to detect ACK/NACK
    iv. I2c read, with ability to send ACK/NAKC 
    v.  Restart
  b. Clock stretching is detected.
  c. Clock frequency is dynamic and can be changed during firmware runtime; the clock line adapts accordingly.

2. The I2C bus is connected to an EEPROM 24LC016B:
  a. the EEPROM driver has the following functions:
    i.	Single byte read 
    ii.	Single byte write 
    iii.Sequential bytes read 
    iv.	Sequential bytes write
    v.	EEPROM busy, this function simply polls the EEPROM slave address to know if it responds with an ACK (ready), or NACK (busy)
  b.	the i2c sequence for the above functions is based on the EEPROM datasheet 

3. The main program consists of the following:
  a.	Using single write to modify EEPROM address between 0 and 127 with values between 0 and 127, i.e. writing 0 to address 0, 1 to 1 ... writing 127 to address 127
  b.	Using sequential write to modify EEPROM address between 256 and 272 with values between 128 and 144, i.e. writing 128 to address 256, 129 to 257 ... writing 144 to address 272
  c.	using sequential read to read EEPROM addresses between 0 and 272

In the proteus simulation, an I2C debugger is used to parse the I2C transactions made and show the start/stop/byte read/write.

To run the program, compile the code in CCS C Compiler and use the resulting .hex file as firmware for the PIC18F25K22 microcontroller in proteus.
