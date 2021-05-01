#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

#define TFT_TEAL 0x008080
#define TFT_GREY 0x5AEB
#define XA 0
#define YA 1
#define XB 2
#define YB 3
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

byte data=0;

int txt = 2;
int posx=30;
int posy=35;
int Atextx=posx;
int Atexty=posy;
int Btextx=205;
int Btexty=205;

int border[]={20,20,200,200};
int base[]={22,22,196,196};
int border_title[]={20,20,200,12};
int title[]={22,22,196,8};
int panel[]={0,0,240,20};
int fill[]={0,20,240,220};



void moveCls();
void drawScreen();
void setFont();
void renderText();
void drawPane();
void drawBackground();

void setup()
{
  tft.init();
  Serial.begin(9600);
  tft.setRotation(2);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  tft.setTextSize(txt);
  drawScreen();
  drawPane();
}

void loop()
{
    while (SerialBT.available())
    { 
      data = SerialBT.read();

      if((data >= 0x10) &&( data <=0x1F))
      {
              switch(data)
              {
                      case 0x10:
                        moveCls(-10,0);//LEFT
                      break;
                      case 0x11:
                        moveCls(10,0);//RIGHT
                      break;
                      case 0x12:
                      moveCls(0,-10);//UP
                      break;
                      case 0x13:
                      moveCls(0,10);//DOWN
                      break;
                      case 0x14:
                                setFont(true);
                      break;
                      case 0x19:
                                setFont(false);
                      break;
                       case 0x15:                      
                                resizeCls(-10,0);//RESIZE-X
                      break;
                       case 0x16:
                                resizeCls(10,0);//RESIZE+X
                      break;
                       case 0x17:
                                resizeCls(0,10);//RESIZE+Y
                      break;
                       case 0x18:
                                resizeCls(0,-10);//RESIZE-Y
                      break;        
                      case 0x1B:
                                moveCls(0,0);//CLEAR
                      break;  
              }
      }
      else
      {
       renderText();
      }
    SerialBT.println("posx:"+String(posx)+" posy:"+String(posy)+" Atextx:"+String(Atextx)+" Atexty:"+String(Atexty)+" Btextx:"+String(Btextx)+" Btexty:" +String(Btexty));  
    }
}


void moveCls(int x, int y)
{
///////////////MOVE/////////////////////////
  
    border[0]+=x;  
    base[0]+=x; 
    border_title[0]+=x; 
    title[0]+=x; 
  
    border[1]+=y;  
    base[1]+=y; 
    border_title[1]+=y; 
    title[1]+=y; 
//////////////////////////////////////////// 
  
  Atextx+=x;
  Atexty+=y;
  Btextx+=x;
  Btexty+=y;
  posx=Atextx;
  posy=Atexty;
  drawScreen();
}

void resizeCls(int rx,int ry)
{
//////////////////RESIZE/////////////////////
if(!(Btextx<tft.fontHeight()*3 && (rx<0)))
{
    border[2]+=rx;  
    base[2]+=rx; 
    border_title[2]+=rx; 
    title[2]+=rx;
    Btextx+=rx;
}

if(!(Btexty<tft.fontHeight()*3 && (ry<0)))
{
    border[3]+=ry;  
    base[3]+=ry;
    Btexty+=ry;
} 
//////////////////////////////////////////// 
  
  drawScreen();
}

void drawScreen()
{
  drawBackground();
  drawWindow();
  drawPane();
}

void setFont(bool font)
{
    if(font)
    {
      tft.setTextSize(txt++);
    }
    else
    {
      tft.setTextSize(txt--);    
    }
    moveCls(0,0);
}

void renderText()
{
  tft.setCursor(posx, posy);
  tft.setTextColor(TFT_BLACK,TFT_GREY);
            if(data!='\n')
          {
              posx += tft.drawChar(data,posx,posy);
              if(posx>=Btextx-(tft.fontHeight()/3))
              {
                 posx=Atextx;
                 posy=posy+tft.fontHeight();
              }
              if(posy>=Btexty-(tft.fontHeight()/2))
              {
                  moveCls(0,0);
              }
          }
}

void drawPane()
{
   tft.fillRect(panel[XA],panel[YA],panel[XB],panel[YB],TFT_BLACK);
   tft.setTextColor(TFT_BLUE,TFT_BLACK);
   tft.drawCentreString("Hello",tft.width()/2, 0, 1);
}

void drawWindow()
{
  tft.fillRect(border[XA],border[YA],border[XB],border[YB],TFT_BLACK);
  tft.fillRect(base[XA],base[YA],base[XB],base[YB],TFT_GREY);
  tft.fillRect(border_title[XA],border_title[YA],border_title[XB],border_title[YB],TFT_BLACK);
  tft.fillRect(title[XA],title[YA],title[XB],title[YB],TFT_RED);  
}
void drawBackground()
{
  tft.fillRect(fill[XA],fill[YA],fill[XB],fill[YB],TFT_TEAL);
}