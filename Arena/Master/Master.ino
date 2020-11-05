
/*Adeepa Rajamanthri
 * Title: Master
 * Purpose: This program will collect all the data from slave arduinos and process them through 12c communication protocols
 * Date: 11/10/2018
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define T1 "41001558216874129"   
#define T2 "41051558216874129"
#define T3 12345678
#define POWERUP_NUM 3

 
bool check =false;
bool in = false;
const int ledPin =53;
const int interruptPin = 3;
uint8_t collisionBuffer[POWERUP_NUM][2]; //[tank ID, powerup type]
int cycle[POWERUP_NUM]; 
//
LiquidCrystal_I2C lcd(0x27,16,2);  
void setup() 
{
  Wire.begin();
  Wire.onReceive(receiveEvent); // from GUI to master
  lcd.backlight();
  lcd.init();  
  Serial.begin(115200);//set baud rate
  Serial.println("Serial Start");
  pinMode(ledPin, OUTPUT);
  pinMode(13, OUTPUT);
  
 pinMode(interruptPin, INPUT_PULLUP);// for TESTING PURPOSE
}

void loop() 
{
    int ii;
   if(!digitalRead(interruptPin)) // testing
   {
    ii =1;
    cycle[ii-1]++;
    cycle[ii-1] = cycle[ii-1]%6; 
    Serial.println("Testing Transmission:");
   int a[] = {1,15,cycle[ii]};
   transmissionEvent(a); // For Slave 1, set duration to be 5 seconds for speed powerup
   Serial.print(collisionBuffer[ii-1][0]);
   Serial.println(collisionBuffer[ii-1][1]);
   }
   
  for(int jj=1;jj<3;jj++)
  //if(0)
  {
      Wire.requestFrom(jj,7);
      receiving(jj);
      // insert code for transmit data to IDE
  }
  //printloop(); not sure what this does so ill leave it in

  emptyBuffer();
}

void printHex(unsigned int numberToPrint)
{
  if (numberToPrint >= 16)
    printHex(numberToPrint / 16);
  Serial.println("0123456789ABCDEF"[numberToPrint % 16]);
  //return 
  
}

/*Adeepa Rajamanthri
 * Name: receiving
 * Purpose: Sets the data to zero and read in the 8 bits from the respective wire
 */
void receiving(int jj)
{ 
  String str = "";
  int tankID[7];
  delay(5);

  while (Wire.available()>6)
  {
    // loop through all but the last
    for (int ii=0; ii < 7; ii++)
    {
      tankID[ii] = Wire.read(); // receive byte as a character
      str += tankID[ii]; // store ID as a string
    }

    if(str != "0255255255255255255") // if ID not invalid 
    {
      Serial.println(str);
      collisionBuffer[jj-1][0] = tankIdentify(str);
    }
  }
    
 }

 int tankIdentify(String str)
 {
    int tank = 0;
    if (str == T1)
    {
      tank  = 1;
    }
    if (str == T2)
    {
      tank  = 2;
    }
    if (str == T3)
    {
      tank  = 3;
    }
    Serial.println(tank);
    return tank;
 }
 
 
  void receiveEvent()
  {
    String input = "";
    int powerup_info[3]; // Name,Duration,type
    while (0 < Wire.available())
     {
          powerup_info[0] = Wire.read();// Slave Number
          powerup_info[1]=Wire.read();// Duration for power up
          powerup_info[2] = Wire.read();// Powerup Type
          transmissionEvent(powerup_info); //
     }
  }
  void transmissionEvent(int trans[])
  {
      Wire.beginTransmission(trans[0]); // powerup number
      Wire.write(trans[1]); // the duration
      Wire.write(trans[2]); // powerupType
      Wire.endTransmission(trans[0]);
      Serial.println("Sent val");
      collisionBuffer[trans[0]][1]=trans[2]; // for powerup , store powertype
  }
  
   void emptyBuffer()
   {
      for(intii=0;ii<POWER_NUM; ii++)
      {
        if(collisionBuffer[ii][0]>0)
        {
          Wire.write(ii);//Powerup_Num
          Wire.write(collisionBuffer[0]);//Tank
          Wire.write(collisionBuffer[1])//PowerType
          
          collisionBuffer[ii][0] = 0;//clear buffer
        }
      }
   }
