#define Qtserial Serial1
#define debugserial Serial

//----PID----
#include <PID_v1.h>
#define Data_size 10
#define SampleTime 100
#define DispTime 100

#define H_Transistor A14
#define H_Thermistor A15

#define C_Transistor A12
#define C_Thermistor A13

#define I_Thermistor A11
#define case_Thermistor A10
#define cham_Thermistor A9
//----PID----

#define command_tag char(0xAA)
#define opcode_output char(0x00)
#define opcode_input char(0x01)
#define opcode_lu char(0x02)

#define opcode_ret_base char(0xA0)
#define opcode_fail char(0xFA)

#define posi_bit0 22
#define posi_bit1 24
#define posi_bit2 26
#define posi_bit3 28

#define wellmuxinput  30

#define leftup_bit0 23
#define leftup_bit1 25
#define leftup_bit2 27
#define leftup_bit3 29

#define leftupmuxoutput  31

#define TXRX_log  0
#define HPID_log  1
#define CPID_log  2

int debug_print_flag = TXRX_log;

char Command[20];
char RXD_buffer;
int RXD_counter = 0;
int RXD_length = 0;
int RXD_cks = 0;
int comm_num = 0;

double Setting_H_temp = 25;
double Real_H_temp = 25;

double Setting_C_temp = 10;
double Real_C_temp = 25;

double Real_I_temp = 20.5;
double Real_case_temp = 20.5;
double Real_cham_temp = 20.5;

char trigger_point_1;
char trigger_point_2;

//----PID----
int index=0;
double H_Temp_array[Data_size];
double C_Temp_array[Data_size];
double I_Temp_array[Data_size];
double case_Temp_array[Data_size];
double cham_Temp_array[Data_size];

double H_ThVolt;
double C_ThVolt;

unsigned long LastTime = 0 ;
//Specify the links and initial tuning parameters
double HKp=256, HKi=0, HKd=0;
double CKp=256, CKi=0, CKd=0;
PID HPID(&Real_H_temp, &H_ThVolt, &Setting_H_temp, HKp, HKi, HKd, DIRECT);
PID CPID(&Real_C_temp, &C_ThVolt, &Setting_C_temp, CKp, CKi, CKd, REVERSE);
//----PID----

void setup() {
  // put your setup code here, to run once:
  Qtserial.begin(9600);
  debugserial.begin(9600);
//----PID----
  HPID.SetOutputLimits(0, 255);
  HPID.SetSampleTime(SampleTime);
  HPID.SetMode(AUTOMATIC);
  CPID.SetOutputLimits(0, 255);
  CPID.SetSampleTime(SampleTime);
  CPID.SetMode(AUTOMATIC);
//----PID----
pinMode(posi_bit0,OUTPUT);
pinMode(posi_bit1,OUTPUT);
pinMode(posi_bit2,OUTPUT);
pinMode(posi_bit3,OUTPUT);
digitalWrite(posi_bit0,false);
digitalWrite(posi_bit1,false);
digitalWrite(posi_bit2,false);
digitalWrite(posi_bit3,false);
pinMode(wellmuxinput,INPUT);

pinMode(leftup_bit0,OUTPUT);
pinMode(leftup_bit1,OUTPUT);
pinMode(leftup_bit2,OUTPUT);
pinMode(leftup_bit3,OUTPUT);
digitalWrite(leftup_bit0,false);
digitalWrite(leftup_bit1,false);
digitalWrite(leftup_bit2,false);
digitalWrite(leftup_bit3,false);
pinMode(leftupmuxoutput,OUTPUT);
digitalWrite(leftupmuxoutput,false);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (debugserial.available() > 0) {
    debug_print_flag = debugserial.read() - 48;
  }
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
  H_Temp_array[index] = thermistor(H_Thermistor);
  Real_H_temp = T_avg(H_Temp_array);
  
  C_Temp_array[index] = thermistor(C_Thermistor);
  Real_C_temp = T_avg(C_Temp_array);

  I_Temp_array[index] = thermistor(I_Thermistor);
  Real_I_temp = T_avg(I_Temp_array);

  case_Temp_array[index] = thermistor(case_Thermistor);
  Real_case_temp = T_avg(case_Temp_array);

  cham_Temp_array[index] = thermistor(cham_Thermistor);
  Real_cham_temp = T_avg(cham_Temp_array);
  
  index = (index + 1)%Data_size;
  
  HPID.Compute();
  CPID.Compute();
  
  if(H_ThVolt == NAN)
    H_ThVolt = 255;
  if(C_ThVolt == NAN)
    C_ThVolt = 255;
    
  analogWrite(H_Transistor, H_ThVolt);
  analogWrite(C_Transistor, C_ThVolt);
  
  if(millis()-LastTime >= DispTime){
    LastTime=millis();
    if(debug_print_flag == HPID_log){
      debugserial.print("Temperature Th\t");
      debugserial.print(Real_H_temp);
      debugserial.print("\tVolt%\t");
      debugserial.println(H_ThVolt); 
    }
    else if(debug_print_flag == CPID_log){
      debugserial.print("Temperature TE\t");
      debugserial.print(Real_C_temp);
      debugserial.print("\tVolt%\t");
      debugserial.println(C_ThVolt); 
    }
  }
//----PID----
    delay(20);
}
