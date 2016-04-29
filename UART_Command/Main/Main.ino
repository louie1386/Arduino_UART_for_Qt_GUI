#define Qtserial Serial2
#define debugserial Serial

//---Pin def---------------------------------------------------------------------------
#define H_Transistor    51
#define C_Transistor    49

#define H_Thermistor    A13
#define C_Thermistor    A12
#define I_Thermistor    A11
#define case_Thermistor A10
#define cham_Thermistor A9

#define well_bit0       22
#define well_bit1       24
#define well_bit2       26
#define well_bit3       28
#define wellmuxinput    30

#define leftup_bit0     23
#define leftup_bit1     25
#define leftup_bit2     27
#define leftup_bit3     29
#define leftupmuxoutput 31

#define Fan1            21
#define Fan2            20
#define Fan3            19
#define Fan4            18
#define Fan_Power_en    53

#define PD_bit0       46
#define PD_bit1       48
#define PD_bit2       50
#define PD_bit3       52
#define PDmuxinput    A15
#define LED_en        44

//---Op code---------------------------------------------------------------------------
#define command_tag           char(0xAA)
#define opcode_readstatus     char(0x00)
#define opcode_writesetting   char(0x01)
#define opcode_liftup         char(0x02)
#define opcode_ledsw          char(0x03)
#define opcode_pdget          char(0x04)

#define opcode_ret_base char(0xA0)
#define opcode_fail     char(0xFA)

//---Log IO ---------------------------------------------------------------------------
#define TXRX_log  '0'
#define HPID_log  '1'
#define CPID_log  '2'
#define FanC_log  '3'
char debug_print_flag = TXRX_log;

#define NoTest    '.'
#define LEDon     'n'
#define LEDoff    'f'
#define PDget     'p'
char Test_flag = NoTest;

//---TXRX-------------------------------------------------------------------------------
char Command[20];
char RXD_buffer;
int RXD_counter = 0;
int RXD_length = 0;
int RXD_cks = 0;
int comm_num = 0;

//----Well Status-----------------------------------------------------------------------
char trigger_point_1;
char trigger_point_2;

//----PID def---------------------------------------------------------------------------
#include <PID_v1.h>
#define Data_size 10
#define SampleTime 100
#define DispTime 1000

int index=0;
double H_Temp_array[Data_size];
double C_Temp_array[Data_size];
double I_Temp_array[Data_size];
double case_Temp_array[Data_size];
double cham_Temp_array[Data_size];

double Setting_H_temp = 94;
double Real_H_temp = 25;

double Setting_C_temp = 10;
double Real_C_temp = 25;

double Real_I_temp = 20.5;
double Real_case_temp = 20.5;
double Real_cham_temp = 20.5;

double H_ThVolt;
double C_ThVolt;

unsigned long LastTime = 0 ;
//Specify the links and initial tuning parameters
double HKp=256, HKi=0, HKd=0;
double CKp=256, CKi=0, CKd=0;
PID HPID(&Real_H_temp, &H_ThVolt, &Setting_H_temp, HKp, HKi, HKd, DIRECT);
PID CPID(&Real_C_temp, &C_ThVolt, &Setting_C_temp, CKp, CKi, CKd, REVERSE);

//----Fan------------------------------------------------------------------------------
unsigned int Fan1_sum = 0;
unsigned int Fan2_sum = 0;
unsigned int Fan3_sum = 0;
unsigned int Fan4_sum = 0;

unsigned int Fan1_sec = 0;
unsigned int Fan2_sec = 0;
unsigned int Fan3_sec = 0;
unsigned int Fan4_sec = 0;

//----PhotoDiode-----------------------------------------------------------------------
#define PD_Ch_Num  2
#define PD_Ch_St   3

//----End------------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  serial_setup();
  PID_setup();
  Fan_setup();
  Mux_wellstatus_setup();
  Mux_leftup_setup();
  Mux_PD_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  
//---SerialPort--------------------------------------------
  if(debugserial.available() > 0) {
    int debugread = debugserial.read();
    if((debugread-48)<=3){
      debug_print_flag = (char)debugread;
      Log_Input(debug_print_flag);
    }
    else{
      Test_flag = (char)debugread;
      Test_active(Test_flag);
    }
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
    
//----PID------------------------------------------------------
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
  
  analogWrite(H_Transistor, H_ThVolt);
  analogWrite(C_Transistor, C_ThVolt);
  
//----Timer(1sec)-----------------------------------------------
  if(millis()-LastTime >= DispTime){
    LastTime=millis();
    
    FanCounterOutput();    
    Log_Output(debug_print_flag);
    }
    delay(1);
}
