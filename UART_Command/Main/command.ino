void Command_run(char *command_str){
  char opcode = char(command_str[0]);
  char data1 = char(command_str[1]);
  char data2 = char(command_str[2]);
  
  char cks;
  
  if(opcode == opcode_output){  //output real value
      int real_H_temp_int = int(Real_H_temp);
      int real_H_temp_deg = int((Real_H_temp - real_H_temp_int)*10);
      int real_C_temp_int = int(Real_C_temp);
      int real_C_temp_deg = int((Real_C_temp - real_C_temp_int)*10);

      int real_I_temp_int = int(Real_I_temp);
      int real_I_temp_deg = int((Real_I_temp - real_I_temp_int)*10);
      int real_case_temp_int = int(Real_case_temp);
      int real_case_temp_deg = int((Real_case_temp - real_case_temp_int)*10);
      int real_cham_temp_int = int(Real_cham_temp);
      int real_cham_temp_deg = int((Real_cham_temp - real_cham_temp_int)*10);
      //----connect command & well input----
      trigger_point_1 = 0;
      trigger_point_2 = 0;
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
      //----connect command & well input----
      cks = 0;
      TXD(command_tag); //command_tag
      TXD(char(0x10)); //length
      cks += TXD(char(opcode_output + opcode_ret_base)); //opcode
      cks += TXD(char(real_H_temp_int));
      cks += TXD(char(real_H_temp_deg));
      cks += TXD(char(real_C_temp_int));
      cks += TXD(char(real_C_temp_deg));
      
      cks += TXD(char(real_I_temp_int));
      cks += TXD(char(real_I_temp_deg));  
      cks += TXD(char(real_case_temp_int));
      cks += TXD(char(real_case_temp_deg)); 
      cks += TXD(char(real_cham_temp_int));
      cks += TXD(char(real_cham_temp_deg));   
      cks += TXD(char(Fan1_sec));
      cks += TXD(char(Fan2_sec));
      cks += TXD(char(trigger_point_1)); //trigger_point
      cks += TXD(char(trigger_point_2)); //trigger_point
      cks = 0x100 - (cks % 0x100);
      TXD(char(cks)); //check sum
      }
    else if(opcode == opcode_input){  //input setting value  
      Setting_H_temp = double(data1) + (double(data2)/10);
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
        if(!(bitRead(data1, i))){
          digitalWrite(leftup_bit0, bitRead(i, 0));
          digitalWrite(leftup_bit1, bitRead(i, 1));
          digitalWrite(leftup_bit2, bitRead(i, 2));
          digitalWrite(leftup_bit3, bitRead(i, 3));   
          delay(1);       
          digitalWrite(leftupmuxoutput, true);
          delay(500); 
          digitalWrite(leftupmuxoutput, false);
          }
        }
        
      for(int i=6;i<12;i++){
        if(!(bitRead(data2, i-6))){
          digitalWrite(leftup_bit0, bitRead(i, 0));
          digitalWrite(leftup_bit1, bitRead(i, 1));
          digitalWrite(leftup_bit2, bitRead(i, 2));
          digitalWrite(leftup_bit3, bitRead(i, 3));
          delay(1);  
          digitalWrite(leftupmuxoutput, true);
          delay(500); 
          digitalWrite(leftupmuxoutput, false);
          }
        }
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

