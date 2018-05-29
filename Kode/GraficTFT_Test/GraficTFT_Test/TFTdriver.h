/*
 * TFTdriver.h
 *
 * Created: 22-03-2018 09:39:44
 *  Author: Liver
 */ 

void DisplayInit();
void DisplayOff();
void DisplayOn();
void SleepOut();
void MemoryAccessControl(unsigned char parameter);
void InterfacePixelFormat(unsigned char parameter);
void WritePixel(unsigned char Red, unsigned char Blue, unsigned char Green);
void SetColomnAddress(unsigned int Start,unsigned int End);
void SetPageAddress(unsigned int Start,unsigned int End);
void MemoryWrite();
void FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned int Height, unsigned char Red, unsigned char Blue, unsigned char Green);

void getSymbolParameters(int symbol_length, int startX, int startY);
void drawSymbol(const uint8_t bitmap[],int length,int count,int startx,int starty, int letter);
void drawXletters(const uint8_t bitmap[],int size, int x, int y);
void writeString(char str[],int startx, int starty);
void writePixel2(int16_t x, int16_t y, uint16_t color);
void drawPixel(int16_t x, int16_t y, uint16_t color);
int lengthOfFixedLetter(int length_of_letter);
void writeInt(long int num, int startX, int startY);

void drawRed(int count, int height, int realHeight);
void drawGreen(int count, int height, int realHeight);
void drawBlue(int count, int height, int realHeight);

void drawTotal(float Red, float Green, float Blue);
