#include "display4Digits.h"

/**
 * @brief Construct a new display4 Digits::display4 Digits object
 * 
 */
display4Digits::display4Digits()
{
  pinMode(sftclk, OUTPUT);
  pinMode(lchclk, OUTPUT);
  pinMode(sdi, OUTPUT);
}

/**
 * @brief 
 * 
 */
void display4Digits::Clk()
{
  digitalWrite(sftclk,LOW);
  digitalWrite(sftclk,HIGH);
}

/**
 * @brief 
 * 
 */
void display4Digits::Latch()
{
  digitalWrite(lchclk,LOW);
  digitalWrite(lchclk,HIGH);
}

/**
 * @brief 
 * 
 */
void display4Digits::clearDisplay()
{ 
  for(int i=0;i<NUMBER_OUTPUTS_BOTH_MC74HC595A;i++)
  {
    digitalWrite(sdi,HIGH);
    Clk();
  }       
  Latch();
}

/**
 * @brief 
 * 
 * @param dot 
 */
void display4Digits::sendBitToDotting(bool dot)
{
  digitalWrite(sdi,!dot);
  Clk();
}

/**
 * @brief 
 * 
 * @param UserNumber 
 */
void display4Digits::sendBitsOfDigit(uint16_t UserNumber)
{
  for(uint8_t i=0;i<7;i++)
  {
    digitalWrite(sdi,NUMBERS[UserNumber][i]);
    Clk();
  }
}

/**
 * @brief 
 * 
 */
void display4Digits::sendShifterBits()
{
  //Shift from QE to QH bits (see schematic)
  for(uint8_t i=0;i<4;i++)
  {
    digitalWrite(sdi,HIGH);
    Clk();
  }
}

/**
 * @brief 
 * 
 * @param positions 
 */
void display4Digits::displayDigit(bool positions[])
{
  for(uint8_t i=0;i<NUMBER_DIGITS;i++)
  {
    digitalWrite(sdi,positions[i]);
    Clk();
  }
}

/***************************************************************
@Name:   writeDigit
@Brief:  Write one digit on 4-digit-7-segments display
@Params: int UserNumber ->  Set number-to-write
         boolean a      ->  Write on 1st digit
         boolean b      ->  Write on 2nd  digit
         boolean c      ->  Write on 3rd digit
         boolean d      ->  Write on 4rd digit
         boolean dot    ->  Write righted-dot
@Return: N/A
@Status: RELEASED
@Author: A. Siordia
 ***************************************************************/
void display4Digits::writeDigit(uint16_t UserNumber, bool a, bool b, bool c, bool d, bool dot)
{  
  bool positions[4]={d,c,b,a};  
  
  sendBitToDotting(dot);
  sendBitsOfDigit(UserNumber);
  sendShifterBits();    
  displayDigit(positions);         
  Latch(); 
}

/**
 * @brief 
 * 
 * @param UserNumber 
 */
void display4Digits::writeUnits(uint16_t UserNumber)
{
  uint32_t units;
  
  if(UserNumber < 10)
  {    
    units = UserNumber;  
    writeDigit(units,0,0,0,1,NO_DOT);  
  }
}

/**
 * @brief 
 * 
 * @param UserNumber 
 */
void display4Digits::writeTens(uint16_t UserNumber)
{
  uint32_t units, tens;
  
  if(UserNumber >= 10  && UserNumber < 100)
  {
    units = UserNumber%10;
    tens  = UserNumber/10;
    writeDigit(units,0,0,0,1,NO_DOT);
    writeDigit(tens,0,0,1,0,NO_DOT);
    
  }
}

/**
 * @brief 
 * 
 * @param NumerToWrite 
 */
void display4Digits::writeHundreds(uint16_t NumerToWrite)
{
  uint32_t units, tens, hundreds;

  if(NumerToWrite >= 100  && NumerToWrite < 1000)
  {
    units = NumerToWrite%10;
    tens  = (NumerToWrite%100)/10;
    hundreds  = NumerToWrite/100;
    writeDigit(hundreds,0,1,0,0,NO_DOT);
    writeDigit(tens,0,0,1,0,NO_DOT);
    writeDigit(units,0,0,0,1,NO_DOT);
  }  
}

/**
 * @brief 
 * 
 * @param NumerToWrite 
 */
void display4Digits::writeThousands(uint16_t NumerToWrite)
{
  uint32_t units, tens, hundreds, thousands;

  if(NumerToWrite >= 1000  && NumerToWrite < 10000)
  {            
    units = NumerToWrite%10;
    tens  = (NumerToWrite%100)/10;
    hundreds  = (NumerToWrite%1000)/100;
    thousands = NumerToWrite/1000;
    writeDigit(thousands,1,0,0,0,NO_DOT);
    writeDigit(hundreds,0,1,0,0,NO_DOT);
    writeDigit(tens,0,0,1,0,NO_DOT);
    writeDigit(units,0,0,0,1,NO_DOT);
  } 
}

/***************************************************************
@Name:   Write_Number
@Brief:  Write Integer Number on Display.            
@Params: int UserNumber ->  Set number-to-write (0-9999)
@Return: N/A
@Status: RELEASED
@Author: A. Siordia
 ***************************************************************/
void display4Digits::writeNumber(uint16_t NumerToWrite)
{
  clearDisplay();
  writeUnits(NumerToWrite);
  writeTens(NumerToWrite);
  writeHundreds(NumerToWrite);
  writeThousands(NumerToWrite);
}

