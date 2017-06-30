//YUN ROBOT Thinger.io
//  Arduino YUN+MotorShield: 4 motores DC, conectados a un control motor de 2
#define _DEBUG_

#include <YunClient.h>
#include <ThingerYun.h>



#define USERNAME "user"
#define DEVICE_ID "robot"
#define DEVICE_CREDENTIAL "password"

ThingerYun thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

//PIN definition
const int //Los pins de arduino para el motor
PWM_A   = 3,//Speed
DIR_A   = 12,//Dirección: alante/atras
BRAKE_A = 9;// Freno

const int
PWM_B   = 11,//Speed
DIR_B   = 13,//Dirección: alante/atras
BRAKE_B = 8;// Freno

const int SENSOR= 10;

const int SENSORB= 5;

//Variables
int direccion = 0;
int previous_direccion = 0;
int previous_velocidad=0;
int braked=0;
int velocidad= 80;
long pulse, sensed;
int cm=0;
int distancia=0;

void setup()
{
  Serial.begin(9600);
  Bridge.begin();
  
  // Configure the A output
  pinMode(BRAKE_A, OUTPUT);  // Brake pin on channel A
  pinMode(DIR_A, OUTPUT);    // Direction pin on channel A
  // Configure the B output
  pinMode(BRAKE_B, OUTPUT);  // Brake pin on channel B
  pinMode(DIR_B, OUTPUT);    // Direction pin on channel B

  pinMode(SENSOR,INPUT);
  
  pinMode(SENSORB,INPUT);


  

// //Thinger Resource input 
   thing["speed"] << [](pson& in)
    {
       if (in.is_empty()){
        in = velocidad;
    }
    else {
      velocidad = in;
    }
     };
  
 thing["direction"] << [](pson& in){
    if (in.is_empty()){
        in = direccion;
    }
    else {
      direccion = in;
    }
 };

  // Thinger Output
    thing["cm"] >> [](pson& out){ out = cm; };
}

void read_sensor()
{
  pulse=pulseIn(SENSOR, HIGH);
  sensed=pulse/147;//no mide menos de 15cm
  cm=sensed*2.54;
}
void read_sensor_back()
{
  pulse=pulseIn(SENSORB, HIGH);
  sensed=pulse/147;//no mide menos de 15cm
  distancia=sensed*2.54;
}

void loop()
{
   thing.handle();
   read_sensor();
   read_sensor_back();
 

   if ((previous_direccion != direccion) || (previous_velocidad != velocidad)||(cm < 20)|| ((braked=1)&&(cm>20))||(distancia<20)||((distancia>20)&&(braked=1)))
   {
     previous_direccion = direccion;
     previous_velocidad = velocidad;

     switch (direccion) {
      case 0:
        Serial.println("stop");
        digitalWrite(BRAKE_A, HIGH);  // raise the brake
        digitalWrite(BRAKE_B, HIGH);  // raise the brake
        break;
      case 1:
        Serial.println("adelante");
        //read_sensor();

        if (cm>20)// Si más de 20 cm, que se mueva, si menor de 20 cm, por defecto debe frenar
          {
            analogWrite(PWM_A, velocidad);     // Set the speed of the motor, 255 is the maximum value
            analogWrite(PWM_B, velocidad);     // Set the speed of the motor, 255 is the maximum value
            digitalWrite(BRAKE_A, LOW);  // setting brake LOW disable motor brake
            digitalWrite(BRAKE_B, LOW);  // setting brake LOW disable motor brake
  
            digitalWrite(DIR_A, HIGH);   // setting direction to HIGH the motor will spin forward
            digitalWrite(DIR_B, HIGH);   // setting direction to HIGH the motor will spin forward
            braked=0;
          }
        else
        {
            digitalWrite(BRAKE_A, HIGH);  // raise the brake
            digitalWrite(BRAKE_B, HIGH);  // raise the brake
            braked=1;
        }          
        break;
        
      case 2:
      
      if (distancia>20)
      {
        Serial.println("atras");
        analogWrite(PWM_A, velocidad);     // Set the speed of the motor
        analogWrite(PWM_B, velocidad);     // Set the speed of the motor
        digitalWrite(BRAKE_A, LOW);  // setting againg the brake LOW to disable motor brake
        digitalWrite(BRAKE_B, LOW); 
        digitalWrite(DIR_A, LOW);    // now change the direction to backward setting LOW the DIR_A pin
        digitalWrite(DIR_B, LOW);    // now change the direction to backward setting LOW the DIR_A pin
        braked=0;
      }
      else
      {
         digitalWrite(BRAKE_A, HIGH);  // raise the brake
         digitalWrite(BRAKE_B, HIGH);  // raise the brake
         braked=1;  
      }
      
        break;
        
      case 3:
      if (cm>20)
      {
        Serial.println("derecha");
        analogWrite(PWM_A, 240);     // Set the speed of the motor
        analogWrite(PWM_B, 0);     // Set the speed of the motor    
        digitalWrite(BRAKE_A, LOW);  // setting againg the brake LOW to disable motor brake
        digitalWrite(BRAKE_B, HIGH); 
        digitalWrite(DIR_A, HIGH);    // now change the direction to backward setting LOW the DIR_A pin
        //digitalWrite(DIR_B, LOW);    // now change the direction to backward setting LOW the DIR_A pin
        braked=0;
      }
      else
      {
         digitalWrite(BRAKE_A, HIGH);  // raise the brake
         digitalWrite(BRAKE_B, HIGH);  // raise the brake
         braked=1;  
      }

        break;
        
      case 4:

     if (cm>20)
     {
        Serial.println("izquierda");
        analogWrite(PWM_A, 0);     // Set the speed of the motor
        analogWrite(PWM_B, 240);     // Set the speed of the motor
        digitalWrite(BRAKE_A, HIGH);  // setting againg the brake LOW to disable motor brake
        digitalWrite(BRAKE_B, LOW); 
        //digitalWrite(DIR_A, LOW);    // now change the direction to backward setting LOW the DIR_A pin
        digitalWrite(DIR_B, HIGH);    // now change the direction to backward setting LOW the DIR_A pin
        braked=0;
     }
     else
     {
         digitalWrite(BRAKE_A, HIGH);  // raise the brake
         digitalWrite(BRAKE_B, HIGH);  // raise the brake
         braked=1;    
     }


        break;
        
      default: 
        // if nothing else matches, do the default
        // default is optional
        analogWrite(PWM_A, 0);     // Set the speed of the motor
        analogWrite(PWM_B, 0);     // Set the speed of the motor
      break;
    }  // end switch
  }// end if
  


}//end loop
 
