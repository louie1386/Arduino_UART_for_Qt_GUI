void Command_run(char *command_str){
  char opcode = char(command_str[0]);
  char data1 = char(command_str[1]);
  char data2 = char(command_str[2]);
  
  char cks;
  
  if(opcode == opcode_output){  //output real value
      //real_temp = dummy_real_temp(setting_temp, real_temp, 2);
      int real_temp_int = int(real_temp);
      int real_temp_deg = int((real_temp - real_temp_int)*10);
      //----connect command & well input----
      for(int i=0;i<6;i++){
        digitalWrite(posi_bit0, bitRead(i, 0));
        digitalWrite(posi_bit1, bitRead(i, 1));
        digitalWrite(posi_bit2, bitRead(i, 2));
        digitalWrite(posi_bit3, bitRead(i, 3));
        delay(1);
        bitWrite(trigger_point_1, i, digitalRead(wellmuxinput));
        }
      for(int i=6;i<12;i++){
        digitalWrite(posi_bit0, bitRead(i, 0));
        digitalWrite(posi_bit1, bitRead(i, 1));
        digitalWrite(posi_bit2, bitRead(i, 2));
        digitalWrite(posi_bit3, bitRead(i, 3));
        delay(1);
        bitWrite(trigger_point_2, (i-6), digitalRead(wellmuxinput));
        }        
      /*
      trigger_point_1 = B00000000;
      trigger_point_2 = B00000000;
      trigger_point_1 = trigger_point_1 | (B00000001 * digitalRead(wellpin1));
      trigger_point_1 = trigger_point_1 | (B00000010 * digitalRead(wellpin2));
      trigger_point_1 = trigger_point_1 | (B00000100 * digitalRead(wellpin3));
      trigger_point_1 = trigger_point_1 | (B00001000 * digitalRead(wellpin4));
      */
      //----connect command & well input----
      cks = 0;
      TXD(command_tag); //command_tag
      TXD(char(0x06)); //length
      cks += TXD(char(opcode_output + opcode_ret_base)); //opcode
      cks += TXD(char(real_temp_int)); //real_temp_int
      cks += TXD(char(real_temp_deg)); //real_temp_deg
      cks += TXD(char(trigger_point_1)); //trigger_point
      cks += TXD(char(trigger_point_2)); //trigger_point
      cks = 0x100 - (cks % 0x100);
      TXD(char(cks)); //check sum
      }
    else if(opcode == opcode_input){  //input setting value  
      setting_temp = double(data1) + (double(data2)/10);
      cks = 0;
      TXD(command_tag); //command_tag
      TXD(char(0x04)); //length
      cks += TXD(char(opcode_input + opcode_ret_base)); //opcode
      cks += TXD(char(data1)); //setting_temp_int
      cks += TXD(char(data2)); //setting_temp_deg
      cks = 0x100 - (cks % 0x100);
      TXD(char(cks)); //check sum
    }  
    else if(opcode == opcode_lu){
      for(int i=0;i<6;i++){
        digitalWrite(leftup_bit0, bitRead(i, 0));
        digitalWrite(leftup_bit1, bitRead(i, 1));
        digitalWrite(leftup_bit2, bitRead(i, 2));
        digitalWrite(leftup_bit3, bitRead(i, 3));     
        if(!(bitRead(data1, i))){
          digitalWrite(leftupmuxoutput,true);
          }
        else if(bitRead(data1, i)){
          digitalWrite(leftupmuxoutput,false);
          }
        }
        
      for(int i=6;i<12;i++){
        digitalWrite(leftup_bit0, bitRead(i, 0));
        digitalWrite(leftup_bit1, bitRead(i, 1));
        digitalWrite(leftup_bit2, bitRead(i, 2));
        digitalWrite(leftup_bit3, bitRead(i, 3));     
        if(!(bitRead(data2, (i-6)))){
          digitalWrite(leftupmuxoutput,true);
          }
        else if(bitRead(data2, (i-6))){
          digitalWrite(leftupmuxoutput,false);
          }
        }
      /*
      digitalWrite(leftuppin1, !(bitRead(data1, 0)));
      digitalWrite(leftuppin2, !(bitRead(data1, 1)));
      digitalWrite(leftuppin3, !(bitRead(data1, 2)));
      digitalWrite(leftuppin4, !(bitRead(data1, 3)));
      */
      trigger_point_1 &= data1;
      trigger_point_2 &= data2;
      }
  }
  
void Fail_ret(char fail_type){
  char cks = 0;
  cks += TXD(char(command_tag));
  cks += TXD(char(0x03));
  cks += TXD(char(opcode_fail));
  cks += TXD(char(fail_type));
  cks = 0x100 - (cks % 0x100);
  TXD(char(cks));
  }  

