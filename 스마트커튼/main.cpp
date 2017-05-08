#include <DHT11.h>
#include <Stepper.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(3, 2); //  TX, RX
const int stepsPerRevolution = 64; 
Stepper myStepper(stepsPerRevolution, 11,9,10,8); 

int flag = 0;
int address = 0;
char BT;
int err;
float temp, humi;
char data;
int pin=4;
DHT11 dht11(pin); 
void setup()
{
  // 모터
  myStepper.setSpeed(300);

  Serial.begin(9600);
  BTSerial.begin(9600);

  flag = 0; // flag 설정
}
void loop()
{ 
  // 온도 측정 code 
  if((err=dht11.read(humi, temp))==0)
  {
    Serial.print("temperature:");
    Serial.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    Serial.println();
  }
  Serial.println("Mode Select!!");
  if(BTSerial.available())
  {
    char BT;
    BT = BTSerial.read(); 
    Serial.println(BT);
    if(BT == 'a')
    {
      Serial.println("Auto Mode");
      while(1)
      {      
        data = BTSerial.read();
        Auto_Mode();
        if(BTSerial.available()){data = BTSerial.read();}
        if(data == 'b'){
          BT = 'b';
          break;
        }
      }
    }
    else if(BT == 'b')
    {
      Serial.println("Hand Mode");
      while(1)
      {      
        Hand_Mode();
        if(BTSerial.available()){data = BTSerial.read();}
        if(data == 'a')
        {
          BT = 'a';
          break;
        }
      }      
    }
  }  
}
void Hand_Mode()
{
  data = '0';
  Serial.print(data);
  Serial.print(" ");
  Serial.println(flag);
  delay(50);
  if(BTSerial.available())
  {
    data = BTSerial.read();
    Serial.print(data);
    Serial.print(" ");
    Serial.println(flag);
    // up
    if(data == 'u'&& flag == 0)
    {
      Cutton_Off();
      flag = 1;
    }
    // down
    else if(data == 'd'&& flag == 1)
    {
      Cutton_On();
      flag = 0;
    }
  }
}
void Auto_Mode()
{
  if( temp > 13 && flag == 0)
  {
    Cutton_Off();
    flag = 1;
    Serial.println(flag);
  }
  else if( temp <= 13 && flag == 1)
  {
    Cutton_On();
    flag = 0;
    Serial.println(flag);
  }
} 
void Cutton_On()
{ 
  for(int i=0; i<32; i++) {  // 64 * 32 = 2048 한바퀴
    myStepper.step(-stepsPerRevolution);
    }
  delay(500);
}
  
void Cutton_Off()
{ 
  for(int i=0; i<32; i++) {  // 64 * 32 = 2048 한바퀴
      myStepper.step(stepsPerRevolution);
    }
    delay(500);  
}
