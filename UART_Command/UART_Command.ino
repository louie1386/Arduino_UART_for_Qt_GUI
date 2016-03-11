#define command_tag char(0xAA)
#define opcode_output char(0x00)
#define opcode_input char(0x01)

#define opcode_ret char(0xA0)
#define opcode_fail char(0xFA)

char Command[20];
char RXD_buffer;
int RXD_counter = 0;
int RXD_length = 0;
int RXD_cks = 0;

int setting_temp_int = 94;
int setting_temp_deg = 00;
int setting_time_min = 10;
int setting_time_sec = 00;

int real_temp_int = 90;
int real_temp_deg = 1;
int real_time_min = 9;
int real_time_sec = 30;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    RXD_buffer = RXD();
    if(RXD_buffer == command_tag){
      //Command = ""; 
      RXD_counter = 0;
      RXD_length = 0;
      RXD_cks = 0;
      }
    else{
      if(RXD_counter == 1){ //length
        RXD_length = RXD_buffer;
        }
      else if(RXD_counter == RXD_length+1){ //check sum
        //Serial.print(RXD_cks + RXD_buffer);
        if((RXD_cks + RXD_buffer) % 0x100 == 0){
          Command_run(Command);  
          }
        else{
          Fail_ret(0x00);   
          }
        }
      else{ //opcode, data
        Command[RXD_counter - 2] = RXD_buffer;
        RXD_cks = (RXD_cks + RXD_buffer) % 0x100;
        }
      }
    RXD_counter++;
    }
    delay(10);
}

char RXD(){
  char ret;
  ret = Serial.read();
  return ret;
  }

void Command_run(char command_str[20]){
  char opcode = char(command_str[0]);
  char data1 = char(command_str[1]);
  char data2 = char(command_str[2]);
  char data3 = char(command_str[3]);
  char data4 = char(command_str[4]);
  char cks;
  //Serial.print(data1);
  
  if(opcode == opcode_output){  //output real value
      Serial.print(command_tag); //command_tag
      Serial.print(char(0x06)); //length
      Serial.print(char(opcode_output + 0xA0)); //opcode
      Serial.print(char(real_temp_int)); //real_temp_int
      Serial.print(char(real_temp_deg)); //real_temp_deg
      Serial.print(char(real_time_min)); //real_time_min
      Serial.print(char(real_time_sec)); //real_time_sec
      cks = 0x100 - ((opcode_output + 0xA0 + real_temp_int + real_temp_deg + real_time_min + real_time_sec) % 0x100);
      Serial.print(char(cks)); //check sum
      }
    else if(opcode == opcode_input){  //input setting value  
      setting_temp_int = data1;
      setting_temp_deg = data2;
      setting_time_min = data3;
      setting_time_sec = data4;
      Serial.print(command_tag); //command_tag
      Serial.print(char(0x06)); //length
      Serial.print(char(opcode_input + 0xA0)); //opcode
      Serial.print(char(setting_temp_int)); //setting_temp_int
      Serial.print(char(setting_temp_deg)); //setting_temp_deg
      Serial.print(char(setting_time_min)); //setting_time_min
      Serial.print(char(setting_time_sec)); //setting_time_sec
      cks = 0x100 - ((opcode_input + 0xA0 + setting_temp_int + setting_temp_deg + setting_time_min + setting_time_sec) % 0x100);
      Serial.print(char(0x00)); //check sum
    }  
  }
  
void Fail_ret(char fail_type){
  char cks;
  Serial.print(char(command_tag));
  Serial.print(char(0x03));
  Serial.print(char(opcode_fail));
  Serial.print(char(fail_type));
  cks = 0x100 - ((opcode_fail + fail_type) % 0x100);
  Serial.print(char(cks));
  }  
