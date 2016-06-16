#define Qtserial Serial2
#define debugserial Serial

//---Pin def---------------------------------------------------------------------------
#define H_Transistor    12
#define C_Transistor    11

#define TempPin1        A13
#define TempPin2        A12
#define TempPin3        A11
#define TempPin4        A10
#define TempPin5        A9
#define TempPin6        A8

#define well_bit0       23
#define well_bit1       25
#define well_bit2       27
#define well_bit3       29
#define wellmuxinput    31

#define liftup_bit0     22
#define liftup_bit1     24
#define liftup_bit2     26
#define liftup_bit3     28
#define liftupmuxoutput 30

#define Fan1            21
#define Fan2            20
#define Fan3            19
#define Fan4            18
#define Fan_Power_en    53

#define PD_bit0         46
#define PD_bit1         48
#define PD_bit2         50
#define PD_bit3         52
#define PDmuxinput      A15
#define LED_en          44

#define buzzer          13

#define TempIC          A14

#define AutoResetPin    A0

//---Op code---------------------------------------------------------------------------
#define command_tag           char(0xAA)
#define opcode_readstatus     char(0x00)
#define opcode_writesetting   char(0x01)
#define opcode_liftup         char(0x02)
#define opcode_ledsw          char(0x03)
#define opcode_pdget          char(0x04)

#define opcode_tester         char(0xF0)

#define opcode_ret_base       char(0xA0)
#define opcode_fail           char(0xFA)

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
#define Autotune  'a'
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

//----liftup----------------------------------------------------------------------------
#define liftup_push_times   1
#define liftup_push_ms      10
#define liftup_release_ms   100
#define liftup_ch_ms        500

//----PID def---------------------------------------------------------------------------
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#define Data_size               10
#define SampleTime              100
#define DispTime                1000
#define AutoTune_TempDiffRange  1
#define AutoTune_FailmsTime     5000

#define mode_thermistor         0
#define mode_tempIC             1

#define temp1_mode  mode_thermistor
#define temp2_mode  mode_thermistor
#define temp3_mode  mode_thermistor
#define temp4_mode  mode_thermistor
#define temp5_mode  mode_thermistor
#define temp6_mode  mode_thermistor

int index=0;
double Temp_array1[Data_size];
double Temp_array2[Data_size];
double Temp_array3[Data_size];
double Temp_array4[Data_size];
double Temp_array5[Data_size];
double Temp_array6[Data_size];

double TempIC_array[Data_size];

double Setting_H_temp = 0;
double Setting_C_temp = 0;

double Real_Temp1 = 20.5;
double Real_Temp2 = 20.5;
double Real_Temp3 = 20.5;
double Real_Temp4 = 20.5;
double Real_Temp5 = 20.5;
double Real_Temp6 = 20.5;

double Real_TempIC = 20.5;

double H_ThVolt;
double C_ThVolt;

unsigned long LastTime = 0 ;
//Specify the links and initial tuning parameters
double HKp=200, HKi=0, HKd=70;
double CKp=256, CKi=0, CKd=0;
PID HPID(&Real_Temp1, &H_ThVolt, &Setting_H_temp, HKp, HKi, HKd, DIRECT);
PID_ATune HaTune(&Real_Temp1, &H_ThVolt);
PID CPID(&Real_Temp2, &C_ThVolt, &Setting_C_temp, CKp, CKi, CKd, REVERSE);

bool Autotune_en = false;

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
#define PD_Ch_Num  8 
#define PD_Ch_St   10

//----Buzzer---------------------------------------------------------------------------
#define Buzzer_EN_msTime  1000 
#define Buzzer_HL_msTime  100
int buzzer_time;
int buzzer_subtime;

//----Reset----------------------------------------------------------------------------
#define MaxTemp 120

//----End------------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  serial_setup();
  PID_setup();
  Fan_setup();
  Mux_wellstatus_setup();
  Mux_liftup_setup();
  Mux_PD_setup();
  buzzer_setup();
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
  Temp_array1[index] = realtempget(temp1_mode, TempPin1);
  Real_Temp1 = T_avg(Temp_array1);
  
  Temp_array2[index] = realtempget(temp2_mode, TempPin2);
  Real_Temp2 = T_avg(Temp_array2);

  Temp_array3[index] = realtempget(temp3_mode, TempPin3);
  Real_Temp3 = T_avg(Temp_array3);

  Temp_array4[index] = realtempget(temp4_mode, TempPin4);
  Real_Temp4 = T_avg(Temp_array4);

  Temp_array5[index] = realtempget(temp5_mode, TempPin5);
  Real_Temp5 = T_avg(Temp_array5);

  Temp_array6[index] = realtempget(temp6_mode, TempPin6);
  Real_Temp6 = T_avg(Temp_array6);

  TempIC_array[index] = realtempget(mode_tempIC, TempIC);
  Real_TempIC = T_avg(TempIC_array);
  
  index = (index + 1)%Data_size;

  byte val = HaTune.Runtime();      
  if(val == 1 && Autotune_en == true){
    HKp = HaTune.GetKp();
    HKi = HaTune.GetKi();
    HKd = HaTune.GetKd();
    HPID.SetTunings(HKp,HKi,HKd);
    Autotune_en = false;
  }
  else
    HPID.Compute();
  CPID.Compute();

  if(H_ThVolt<0)
    H_ThVolt=0;
  if(C_ThVolt<0)
    C_ThVolt=0;
    
  analogWrite(H_Transistor, H_ThVolt);
  analogWrite(C_Transistor, C_ThVolt);
  
//----Timer(1sec)-----------------------------------------------
  if(millis()-LastTime >= DispTime){
    LastTime=millis();
    
    FanCounterOutput();    
    Log_Output(debug_print_flag);
    }
  //buzzer_en();
  if(Real_Temp1 >= MaxTemp)
    AutoReset();
  delay(1);
}
