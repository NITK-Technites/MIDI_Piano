#include<SPI.h>
#define ON 10
#define OFF 7
byte control;
int analogPin=3;
int thresh=700, val;
int dig1 = 5,dig2 = 6, dig3 = 7, dig4 = 8;
byte byte1 = 0x81,byte2 = 0x80,c=0x01,out, old1=0x00, old2=0x00;
int note[16]={1,2,3,4,5,6,7,8,9,10,12,13,14,15,16};
int sspin = 10;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(dig1,OUTPUT);
  pinMode(dig2,OUTPUT);
  pinMode(dig3,OUTPUT);
  pinMode(dig4,OUTPUT);
  SPI.begin();
  pinMode(sspin,OUTPUT);
  
}
int i=0;
/*
void midi(cmd,note,velocity)
{
  Serial.println(note);
}
*/

void loop() {
  // put your main code here, to run repeatedly:


// Check if this function is working, else make the function of int datatype and return the new byte1 and byte 2 values
read_all();
phonic();
SPI_send(byte1, byte2);
}


void phonic()
{ 
 c = 0x01;
for(i=0;i<8;i++){
if(byte1 & c)
{
  if(!(old1 & (0x01<<i)))
  {
    Serial.println(note[i]+1000);
    old1 |= 0x01<<(i);
  }
}
else
{
  Serial.println(note[i]);
  old1 &= ~(0x01<<(i));
}

if(byte2 & c)
{
  if(!(old2 & (0x01<<i)))
  {
    Serial.println(note[i+8]+1000);
    old2 |= 0x01<<(i);
  }
}
else
{
  Serial.println(note[8+i]);
  old2 &= ~(0x01<<(i));
}

c = c<<1;
/*if(c==0)
{
  c = 0x01;
}*/
}
}

void read_all()
{
  // Iterates through all the inputs and registers which bit above the threshold
for(control = 0; control < 16; control++)
{
 digitalWrite(dig1,control & 0x1);
 digitalWrite(dig1,control & 0x2);
 digitalWrite(dig1,control & 0x4);
 digitalWrite(dig1,control & 0x8);
 val=analogRead(analogPin);
 if(val>thresh)
 {
   if(control < 8)
   {
    byte1 |= (0x01<<control);
   }
   if(control >= 8)
   {
    byte2 |= (0x01<<(control-8));
   }
 }
 else
  {
   if(control < 8)
   {
    byte1 &= ~(0x01<<control);
   }
   if(control >= 8)
   {
    byte2 &= ~(0x01<<(control-8));
   }
  }
}
}

void SPI_send(byte first, byte second)
{
  digitalWrite(sspin,HIGH);
  delay(100);
  SPI.transfer(first);
  SPI.transfer(second);
  delay(100);
  digitalWrite(sspin,LOW);
}
