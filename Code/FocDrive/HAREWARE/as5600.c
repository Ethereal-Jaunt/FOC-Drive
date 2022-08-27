#include "as5600.h"
#include "Drv_time.h"
#include "Drv_i2c.h"
#include "usart.h"

uint16_t _rawStartAngle=0;
uint16_t _zPosition=0;
uint16_t _rawEndAngle=0;
uint16_t _mPosition=0;
uint16_t _maxAngle=0;	


/************************************************************************************************************/
/************************************************************************************************************/
/*******************************************************************************/


uint8_t highByte(uint16_t value)
{
	uint8_t ret;
	value = value>>8;
	ret = (uint8_t)value;
  return ret;
}

uint8_t lowByte(uint16_t value)
{
	uint8_t ret;
	value = value&0x00ff;
	ret = (uint8_t)value;
  return ret;
}
/*******************************************************
 Method: readOneByte
 In: register to read
 Out: data read from i2c
 Description: reads one byte register from i2c
******************************************************/
uint8_t readOneByte(uint8_t in_adr)
{
  uint8_t retVal = -1;
	u8 NumByte=1;
	I2C_DMA_Read( &retVal, _ams5600_Address, in_adr, &NumByte );

  return retVal;
}

/*******************************************************
 Method: readOnTwoByte
 In: two registers to read
 Out: data read from i2c as a int16_t
 Description: reads two bytes register from i2c
******************************************************/
uint16_t readTwoBytes(uint8_t in_adr_hi, uint8_t in_adr_lo)
{
  uint16_t retVal = -1;
	uint8_t retValue[2];
  u8 NumByte=2;
	I2C_DMA_Read( retValue, _ams5600_Address, in_adr_hi, &NumByte );
	retVal=retValue[0]<<8 |retValue[1];
  return retVal;
}


/*******************************************************
 Method: writeOneByte
 In: address and data to write
 Out: none
 Description: writes one byte to a i2c register
******************************************************/
void writeOneByte(uint8_t adr_in, uint8_t dat_in)
{
	uint8_t dat = dat_in;
	u8 NumByte=1;
//  Sim_I2C1_Write8(_ams5600_Address,adr_in,1,&dat);
	I2C_DMA_Write( &dat_in, _ams5600_Address,adr_in, &NumByte );
}

/****************************************************
 Method: AMS_5600
 In: none
 Out: i2c address of AMS 5600
 Description: returns i2c address of AMS 5600
   **************************************************/
int16_t getAddress()
{
  return _ams5600_Address; 
}

/*******************************************************
 Method: getMaxAngle
 In: none
 Out: value of max angle register
 Description: gets value of maximum angle register.
******************************************************/
int16_t getMaxAngle(void)
{
  return readTwoBytes(_mang_hi, _mang_lo);
}

/*******************************************************
 Method: getRawAngle
 In: none
 Out: value of raw angle register
 Description: gets raw value of magnet position.
 start, end, and max angle settings do not apply
******************************************************/
int16_t getRawAngle(void)
{
  return readTwoBytes(_raw_ang_hi, _raw_ang_lo);
}

/*******************************************************
 Method: getStartPosition
 In: none
 Out: value of start position register
 Description: gets value of start position register.
******************************************************/
int16_t getStartPosition(void)
{
  return readTwoBytes(_zpos_hi, _zpos_lo);
}  

/*******************************************************
 Method: getEndPosition
 In: none
 Out: value of end position register
 Description: gets value of end position register.
******************************************************/
int16_t getEndPosition(void)
{
  int16_t retVal = readTwoBytes(_mpos_hi, _mpos_lo);
  return retVal;
}

/*******************************************************
 Method: getScaledAngle
 In: none
 Out: value of scaled angle register
 Description: gets scaled value of magnet position.
 start, end, or max angle settings are used to 
 determine value
******************************************************/
int16_t getScaledAngle(void)
{
  return readTwoBytes(_ang_hi, _ang_lo);
}

/*******************************************************
 Method: get Agc
 In: none
 Out: value of AGC register
 Description: gets value of AGC register.
******************************************************/
int16_t getAgc()
{
  return readOneByte(_agc);
}

/*******************************************************
 Method: getMagnitude
 In: none
 Out: value of magnitude register
 Description: gets value of magnitude register.
******************************************************/
int16_t getMagnitude()
{
  return readTwoBytes(_mag_hi, _mag_lo);  
}

/*******************************************************
 Method: getBurnCount
 In: none
 Out: value of zmco register
 Description: determines how many times chip has been
 permanently written to. 
******************************************************/
int16_t getBurnCount()
{
  return readOneByte(_zmco);
}
/*******************************************************
 Method: getRawAngle
 In: none
 Out: value of raw angle register
 Description: gets raw value of magnet position.
 start, end, and max angle settings do not apply
******************************************************/
int16_t AgetRawAngle(void)
{
  return readTwoBytes(_raw_ang_hi, _raw_ang_lo);
}
/*******************************************************
 Method: setEndtPosition
 In: new end angle position
 Out: value of end position register
 Description: sets a value in end position register.
 If no value is provided, method will read position of
 magnet.  
******************************************************/
int16_t setEndPosition(int16_t endAngle)
{
  if(endAngle == -1)
    _rawEndAngle = getRawAngle();
  else
    _rawEndAngle = endAngle;
 
  writeOneByte(_mpos_hi, highByte(_rawEndAngle));
  Delay_ms(2); 
  writeOneByte(_mpos_lo, lowByte(_rawEndAngle)); 
  Delay_ms(2);               
  _mPosition = readTwoBytes(_mpos_hi, _mpos_lo);
  
  return(_mPosition);
}



/*******************************************************
 Method: setStartPosition
 In: new start angle position
 Out: value of start position register
 Description: sets a value in start position register.
 If no value is provided, method will read position of
 magnet.  
******************************************************/
int16_t setStartPosition(int16_t startAngle)
{
  if(startAngle == -1)
  {
    _rawStartAngle = getRawAngle();
  }
  else
    _rawStartAngle = startAngle;

  writeOneByte(_zpos_hi, highByte(_rawStartAngle));
  Delay_ms(2); 
  writeOneByte(_zpos_lo, lowByte(_rawStartAngle)); 
  Delay_ms(2);                
  _zPosition = readTwoBytes(_zpos_hi, _zpos_lo);
  
  return(_zPosition);
}

/*******************************************************
 Method: setMaxAngle
 In: new maximum angle to set OR none
 Out: value of max angle register
 Description: sets a value in maximum angle register.
 If no value is provided, method will read position of
 magnet.  Setting this register zeros out max position
 register.
******************************************************/
int16_t setMaxAngle(int16_t newMaxAngle)
{
  int32_t retVal;
  if(newMaxAngle == -1)
  {
    _maxAngle = getRawAngle();
  }
  else
    _maxAngle = newMaxAngle;

  writeOneByte(_mang_hi, highByte(_maxAngle));
  Delay_ms(2);
  writeOneByte(_mang_lo, lowByte(_maxAngle)); 
  Delay_ms(2); 

  retVal = readTwoBytes(_mang_hi, _mang_lo);
  return retVal;
}

/*******************************************************
 Method: detectMagnet
 In: none
 Out: 1 if magnet is detected, 0 if not
 Description: reads status register and examines the 
 MH bit
******************************************************/
uint8_t detectMagnet(void)
{
  uint8_t magStatus;
  uint8_t retVal = 0;
  /*0 0 MD ML MH 0 0 0*/
  /* MD high = AGC minimum overflow, Magnet to strong */
  /* ML high = AGC Maximum overflow, magnet to weak*/ 
  /* MH high = magnet detected*/ 
  magStatus = readOneByte(_stat);
  
  if(magStatus & 0x20)
    retVal = 1; 
  
  return retVal;
}

/*******************************************************
 Method: getMagnetStrength
 In: none
 Out: 0 if no magnet is detected
      1 if magnet is to weak
      2 if magnet is just right
      3 if magnet is to strong
 Description: reads status register andexamins the MH,ML,MD bits
******************************************************/
uint8_t getMagnetStrength(void)
{
  uint8_t magStatus;
  uint8_t retVal = 0;
  /*0 0 MD ML MH 0 0 0*/
  /* MD high = AGC minimum overflow, Magnet to strong */
  /* ML high = AGC Maximum overflow, magnet to weak*/ 
  /* MH high = magnet detected*/ 
  magStatus = readOneByte(_stat);
  if(detectMagnet() ==1)
  {
      retVal = 2; /*just right */
      if(magStatus & 0x10)
        retVal = 1; /*to weak */
      else if(magStatus & 0x08)
        retVal = 3; /*to strong */
  }
  
  return retVal;
}

/*******************************************************
 Method: burnAngle
 In: none
 Out: 1 success
     -1 no magnet
     -2 burn limit exceeded
     -3 start and end positions not set (useless burn)
 Description: burns start and end positions to chip.
 THIS CAN ONLY BE DONE 3 TIMES
******************************************************/
uint8_t burnAngle()
{
  uint8_t retVal = 1;
  _zPosition = getStartPosition();
  _mPosition = getEndPosition();
  _maxAngle  = getMaxAngle();
  
  if(detectMagnet() == 1)
  {
    if(getBurnCount() < 3)
    {
      if((_zPosition == 0)&&(_mPosition ==0))
        retVal = -3;
      else
        writeOneByte(_burn, 0x80);
    }
    else
      retVal = -2;
  } 
  else
    retVal = -1;
    
  return retVal;
}

/*******************************************************
 Method: burnMaxAngleAndConfig
 In: none
 Out: 1 success
     -1 burn limit exceeded
     -2 max angle is to small, must be at or above 18 degrees
 Description: burns max angle and config data to chip.
 THIS CAN ONLY BE DONE 1 TIME
******************************************************/
uint8_t burnMaxAngleAndConfig()
{
  uint8_t retVal = 1;
  _maxAngle  = getMaxAngle();
  
  if(getBurnCount() ==0)
  {
    if(_maxAngle*0.087 < 18)
      retVal = -2;
    else
      writeOneByte(_burn, 0x40);    
  }  
  else
    retVal=  -1;
    
  return retVal;
}



/*******************************************************
 Function: convertRawAngleToDegrees
 In: angle data from AMS_5600::getRawAngle
 Out: human readable degrees as float
 Description: takes the raw angle and calculates 
 float value in degrees.
******************************************************/
float convertRawAngleToDegrees(int16_t newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087 of a degree */    
  float retVal = newAngle * 0.087;
  return retVal;
}



float Get_Angle(void)
{
	uint16_t rawdata=0;
	float degress =0;
	rawdata = getRawAngle();	//read raw data from as5600
	degress = convertRawAngleToDegrees(rawdata);	//covert to angle
	//printf("rawdata:%d  degress:%f\r\n",rawdata,degress);
	return degress;
}