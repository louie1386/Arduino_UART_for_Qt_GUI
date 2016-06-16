void Command_run(char *command_str){
  char opcode = char(command_str[0]);
  char data1 = char(command_str[1]);
  char data2 = char(command_str[2]);
  
  char cks;
  
  if(opcode == opcode_readstatus){  //output real value
      int Real_Temp1_int = int(Real_Temp1);
      int Real_Temp1_deg = int((Real_Temp1 - Real_Temp1_int)*10);
      int Real_Temp2_int = int(Real_Temp2);
      int Real_Temp2_deg = int((Real_Temp2 - Real_Temp2_int)*10);

      int Real_Temp3_int = int(Real_Temp3);
      int Real_Temp3_deg = int((Real_Temp3 - Real_Temp3_int)*10);
      int Real_Temp4_int = int(Real_Temp4);
      int Real_Temp4_deg = int((Real_Temp4 - Real_Temp4_int)*10);
      int Real_Temp5_int = int(Real_Temp5);
      int Real_Temp5_deg = int((Real_Temp5 - Real_Temp5_int)*10);
      int Real_Temp6_int = int(Real_Temp6);
      int Real_Temp6_deg = int((Real_Temp6 - Real_Temp6_int)*10);
      //----connect command & well input----
      trigger_point_1 = readch_wellstatus(0, 6);
      trigger_point_2 = readch_wellstatus(6, 6);       
      //----connect command & well input----
      cks = 0;
      TXD(command_tag); //command_tag
      TXD(char(0x14)); //length
      cks += TXD(char(opcode_readstatus + opcode_ret_base)); //opcode
      cks += TXD(char(Real_Temp1_int));
      cks += TXD(char(Real_Temp1_deg));
      cks += TXD(char(Real_Temp2_int));
      cks += TXD(char(Real_Temp2_deg));
      
      cks += TXD(char(Real_Temp3_int));
      cks += TXD(char(Real_Temp3_deg));  
      cks += TXD(char(Real_Temp4_int));
      cks += TXD(char(Real_Temp4_deg)); 
      cks += TXD(char(Real_Temp5_int));
      cks += TXD(char(Real_Temp5_deg));   
      cks += TXD(char(Real_Temp6_int));
      cks += TXD(char(Real_Temp6_deg));  
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
      //buzzer_dis();
      writech_liftup(0, data1, 6);
      writech_liftup(6, data2, 6);
      //if((trigger_point_1 | trigger_point_2) != 0)
      //  buzzer_time = Buzzer_EN_msTime;
      trigger_point_1 = readch_wellstatus(0, 6);
      trigger_point_2 = readch_wellstatus(6, 6);       
      }
    else if(opcode == opcode_ledsw){
      if(data1 == 1){
        digitalWrite(LED_en,true);
        //Log_LED(data1);
      }
      else if(data1 == 0){
        digitalWrite(LED_en,false);
        //Log_LED(data1);
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
    else if(opcode == opcode_tester){
      char testdata[2];
      int temp_int = int(Real_TempIC);
      int temp_deg = int((Real_TempIC - temp_int)*10);
      
      testdata[0] = temp_int;
      testdata[1] = temp_deg;
      
      cks = 0;
      TXD(command_tag); //command_tag
      TXD(char(0x4)); //length
      cks += TXD(char(opcode_tester)); //opcode
      cks += TXD(char(testdata[0]));
      cks += TXD(char(testdata[1]));   
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

