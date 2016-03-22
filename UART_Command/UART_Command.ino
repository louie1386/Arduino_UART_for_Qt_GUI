#define Qtserial Serial1
#define debugserial Serial

#define command_tag char(0xAA)
#define opcode_output char(0x00)
#define opcode_input char(0x01)
#define opcode_lu char(0x02)

#define opcode_ret_base char(0xA0)
#define opcode_fail char(0xFA)

char Command[10];
char RXD_buffer;
int RXD_counter = 0;
int RXD_length = 0;
int RXD_cks = 0;
int comm_num = 0;

double setting_temp = 25;
double real_temp = 25;

char trigger_point_1 = B00000001;
char trigger_point_2 = B00000001;

void setup() {
  // put your setup code here, to run once:
  Qtserial.begin(9600);
  debugserial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
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
    delay(10);
}
