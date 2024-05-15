
#include "hw262.h"


Mc74hc595a Display::ic(LCHCLK_PIN, SFTCLK_PIN, SDI_PIN);

const byte* Display::numbers[10]
{
  DISPLAY_0,
  DISPLAY_1,
  DISPLAY_2,
  DISPLAY_3,
  DISPLAY_4,
  DISPLAY_5,
  DISPLAY_6,
  DISPLAY_7,
  DISPLAY_8,
  DISPLAY_9,
};


/**
 * @brief 
 * 
 * @param c 
 * @return byte 
 */
byte Display::convertCharToSegments(char c)
{
  char myChar = tolower(c);

  if(myChar =='a')return DISPLAY_A;
  else if(myChar =='b')return DISPLAY_B;
  else if(myChar =='c')return DISPLAY_C;
  else if(myChar =='d')return DISPLAY_D;
  else if(myChar =='e')return DISPLAY_E;
  else if(myChar =='f')return DISPLAY_F;
  else if(myChar =='g')return DISPLAY_G;
  else if(myChar =='h')return DISPLAY_H;
  else if(myChar =='i')return DISPLAY_I;
  else if(myChar =='j')return DISPLAY_J;
  else if(myChar =='l')return DISPLAY_L;
  else if(myChar =='n') return DISPLAY_N;
  else if(myChar =='ñ') return DISPLAY__N;
  else if(myChar =='o') return DISPLAY_O;
  else if(myChar =='p') return DISPLAY_P;
  else if(myChar =='r') return DISPLAY_R;
  else if(myChar =='s') return DISPLAY_S;
  else if(myChar =='t') return DISPLAY_T;
  else if(myChar =='u') return DISPLAY_U;
  else if(myChar =='y') return DISPLAY_Y;
  else if(myChar ==' ') return DISPLAY_BLANK;
  else return DISPLAY_BLANK;
}


/**
* @brief
*
*/
void Display::clear()
{
  uint8_t position;

  for(int i=0; i<NUMBER_DIGITS_ON_DISPLAY;i++)
  {
    position = (MAX_DISPLAY_POSITION-i);
    ic.SendByte(DISPLAY_BLANK);  
    ic.SendByte(0x10 << position );
    ic.End();
  }  
}


/**
 * @brief 
 * 
 * @param number 
 * @param left_zeros 
 * @return char* 
 */
char* Display::convertIntToBytes(int16_t number, bool left_zeros)
{
  uint32_t factor;
  uint8_t digit;
  bool startNumber = false;
  char* charNumber = new char[NUMBER_DIGITS_ON_DISPLAY];
  uint8_t position;
  byte segmentDigit;
  uint16_t absNumber = abs(number);
  
  for(int8_t i=3; i>-1; i--)
  {
    position = (MAX_DISPLAY_POSITION-i);
    factor = (i>1) ? (pow(10, i)+1) : pow(10, i); // correction for pow method 
    digit = absNumber / factor;  
    absNumber -= digit*factor;
    if(digit != 0 && startNumber == false)
    {
      if(number < 0)  charNumber[position-1]=DISPLAY_MINUS; 
      startNumber = true;
    }
    segmentDigit = numbers[digit];
    charNumber[position] = (startNumber == true || (digit == 0 && i == 0) || left_zeros == true) ?  segmentDigit : DISPLAY_BLANK;
  }
  return charNumber;
}


/**
 * @brief 
 * 
 * @param text 
 */
void Display::write(const char* text)
{
  uint8_t position;
  byte c;
  uint8_t len = strlen(text);

  if(len > NUMBER_DIGITS_ON_DISPLAY)
  {
    Serial.println(MESSAGE_INVALID_LENGHT_TEXT);
    return;
  }
  
  for(int i=0; i<len;i++)
  {
    position = (MAX_DISPLAY_POSITION-i);
    c = convertCharToSegments(text[i]);
    ic.SendByte(c);  
    ic.SendByte(0x10 << position );
    ic.End();
  }
}


/**
 * @brief 
 * 
 * @param number 
 * @param left_zeros 
 * @param offset 
 */
void Display::writeInteger(int16_t number, bool left_zeros, uint8_t offset)
{
  char* n = new char[NUMBER_DIGITS_ON_DISPLAY];
  uint8_t position;

  if(offset>MAX_DISPLAY_POSITION)
  {
    Serial.println(MESSAGE_INVALID_OFFSET);
    return;
  }

  if(IS_VALID_NUMBER(number))
  {
    Serial.println(MESSAGE_INVALID_NUMBER);
    delete[] n;
    return;
  }

  n = convertIntToBytes(number, left_zeros);    
  for(int i=0; i<NUMBER_DIGITS_ON_DISPLAY;i++)
  {
    position = (MAX_DISPLAY_POSITION-i)+offset;
    ic.SendByte(n[i]);  
    ic.SendByte(0x10 << position );
    ic.End();
  }
  delete[] n;
}


/**
 * @brief 
 * 
 * @param number 
 * @param left_zeros 
 * @param offset 
 */
void Display::write(int16_t number, bool left_zeros, uint8_t offset)
{
  writeInteger(number, left_zeros, offset);
}



uint8_t Display::getDotPosition(float number)
{
  if(number < 10) return MAX_DISPLAY_POSITION;
  else if(number < 100) return 2;
  else if(number < 1000) return 1;
  else return 0;
}


/**
 * @brief 
 * 
 * @param number 
 */
void Display::write(float number)
{
  char* numberDigits = new char[NUMBER_DIGITS_ON_DISPLAY];
  char byteDigit;
  uint8_t digitPosition;
  uint8_t dotPosition;
  int16_t numberMils;

  if(IS_VALID_NUMBER(number))
  {
    Serial.println(MESSAGE_INVALID_NUMBER);
    delete[] numberDigits;
    return;
  }

  dotPosition = getDotPosition(number);
  numberMils = (int16_t) number*round(pow(10, dotPosition));

  strcpy(numberDigits, convertIntToBytes(numberMils));

  for(uint8_t i=0; i<NUMBER_DIGITS_ON_DISPLAY;i++)
  {
    digitPosition = MAX_DISPLAY_POSITION-i;
    byteDigit = (i != dotPosition) ? numberDigits[i] : (numberDigits[i] | DISPLAY_DOT);

    Serial.println(byteDigit, HEX);
    ic.SendByte(byteDigit);  
    ic.SendByte(0x10 << digitPosition );
    ic.End();
  }
  delete[] numberDigits;
}


/**
 * @brief 
 * 
 * @param symbol 
 * @param position 
 */
void Display::write(byte symbol, uint8_t position)
{
  ic.SendByte(symbol);  
  ic.SendByte(0x10 << position);
  ic.End();
}

