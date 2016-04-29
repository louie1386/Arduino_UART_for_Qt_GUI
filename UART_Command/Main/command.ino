void Command_run(char *command_str){
  char opcode = char(command_str[0]);
  char data1 = char(command_str[1]);
  char data2 = char(command_str[2]);
  
  char cks;
  
  if(opcode == opcode_readstatus){  //output real value
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
      trigger_point_1 = readch_wellstatus(0, 6);
      trigger_point_2 = readch_wellstatus(6, 6);       
      //----connect command & well input----
      cks = 0;
      TXD(command_tag); //command_tag
      TXD(char(0x12)); //length
      cks += TXD(char(opcode_readstatus + opcode_ret_base)); //opcode
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
      cks += TXD(char(Fan3_sec));
      cks += TXD(char(Fan4_sec));
      cks += TXD(char(trigger_point_1)); //trigger_point
      cks += TXD(char(trigger_point_2)); //trigger_point
      cks = 0x100 - (cks % 0x100);
      TXD(char(cks)); //check sum
      }
    else if(opcode == opcode_writesetting){  //input setting value  
      Setting_H_temp = double(data1) + (double(data2)/10);
      cks = 0;
      TXD(command_tag); //command_tag
      TXD(char(0x04)); //length
      cks += TXD(char(opcode_writesetting + opcode_ret_base)); //opcode
      cks += TXD(char(data1)); //setting_temp_int
      cks += TXD(char(data2)); //setting_temp_deg
      cks = 0x100 - (cks % 0x100);
      TXD(char(cks)); //check sum
    }  
    else if(opcode == opcode_liftup){
      writech_leftup(0, data1, 6);
      writech_leftup(6, data2, 6);
      trigger_point_1 &= data1;
      trigger_point_2 &= data2;
      }
    else if(opcode == opcode_ledsw){
      if(data1 == 1){
        digitalWrite(LED_en,true);
        Log_LED(data1);
      }
      else if(data1 == 0){
        digitalWrite(LED_en,false);
        Log_LED(data1);
      }
    }
    else if(opcode == opcode_pdget){
      int PD_V; 
      cks = 0;
      TXD(command_tag); //command_tag
      TXD(char(PD_Ch_Num * 2 + 2)); //length
      cks += TXD(char(opcode_pdget + opcode_ret_base)); //opcode
      
      for(int i=0;i<PD_Ch_Num;i++){
        PD_V = readch_PD(i,PD_Ch_St);
        cks += TXD(highByte(PD_V)); 
        cks += TXD(lowByte(PD_V));     
      }
      cks = 0x100 - (cks % 0x100);
      TXD(char(cks)); //check sum
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

