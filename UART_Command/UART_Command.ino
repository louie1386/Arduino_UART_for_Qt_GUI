#define Qtserial Serial1
#define debugserial Serial
#define PIDserial Serial2

#define command_tag char(0xAA)
#define opcode_output char(0x00)
#define opcode_input char(0x01)
#define opcode_lu char(0x02)

#define opcode_ret_base char(0xA0)
#define opcode_fail char(0xFA)

#define wellpin1 5
#define wellpin2 4
#define wellpin3 3
#define wellpin4 2

#define leftuppin1 6
#define leftuppin2 7
#define leftuppin3 8
#define leftuppin4 9

char Command[10];
char RXD_buffer;
int RXD_counter = 0;
int RXD_length = 0;
int RXD_cks = 0;
int comm_num = 0;

double setting_temp = 25;
double real_temp = 25;

char trigger_point_1;
char trigger_point_2;

//----PID----
#include <PID_v1.h>
#define Data_size 10
#define SampleTime 100
#define DispTime 100
#define Transistor A14
#define Thermistor A7

// Add some consts of the signal filter
int index=0;
double Temp_array[Data_size];

//Define Variables we'll be connecting to
double PIDtargetTh = 94 , ThVolt, T, R;
const int ThTransistorPin = Transistor;                                                  // Analog output pin that the transistor is attached to
const int ThermistorPin = Thermistor;
int sensorValue = 0 ;
unsigned long LastTime = 0 ;

//Specify the links and initial tuning parameters
double Kp=256, Ki=0, Kd=0;
PID myPID(&T, &ThVolt, &PIDtargetTh, Kp, Ki, Kd, DIRECT);
//----PID----

void setup() {
  // put your setup code here, to run once:
  Qtserial.begin(9600);
  debugserial.begin(9600);
//----PID----
  PIDserial.begin(9600);
  myPID.SetOutputLimits(0, 255);
  myPID.SetSampleTime(SampleTime);
  myPID.SetMode(AUTOMATIC);
//----PID----
pinMode(wellpin1,INPUT);
pinMode(wellpin2,INPUT);
pinMode(wellpin3,INPUT);
pinMode(wellpin4,INPUT);
pinMode(leftuppin1,OUTPUT);
pinMode(leftuppin2,OUTPUT);
pinMode(leftuppin3,OUTPUT);
pinMode(leftuppin4,OUTPUT);
digitalWrite(leftuppin1,false);
digitalWrite(leftuppin2,false);
digitalWrite(leftuppin3,false);
digitalWrite(leftuppin4,false);
}

void loop() {
  // put your main code here, to run repeatedly:
  //----connect command & PID----
  PIDtargetTh = setting_temp;
  real_temp = T;
  //----connect command & PID----
  if (Qtserial.available() > 0) {
    RXD_buffer = RXD();
    if(RXD_buffer == command_tag && RXD_counter == 0){
      //RXD_counter = 0;
      RXD_length = 0;
      RXD_cks = 0;
      comm_num = 0;
      }
    else if(RXD_counter == 1){ //length
      RXD_length = RXD_buffer;
      }
    else if(RXD_counter == RXD_length+1){ //check sum
      if((RXD_cks + RXD_buffer) % 0x100 == 0){
        Command_run(Command);  
        }
      else{
        Fail_ret(0x00);   
        }
        RXD_counter = -1;
        comm_num = 0;
      }
    else{ //opcode, data
      Command[comm_num] = RXD_buffer;
      RXD_cks = (RXD_cks + RXD_buffer) % 0x100;
      comm_num++;
      }
    RXD_counter++;
    }
//----PID----
  thermistor();
  myPID.Compute();
  analogWrite(ThTransistorPin, ThVolt);
  if(millis()-LastTime >= DispTime){
    LastTime=millis();
    PIDserial.print("Temperature\t");
    PIDserial.print(T);
    PIDserial.print("\tVolt%\t");
    PIDserial.println(ThVolt); 
  }
//----PID----
    delay(20);
}
