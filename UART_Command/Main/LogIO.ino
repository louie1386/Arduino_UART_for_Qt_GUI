void Log_Input(char flag){
  debugserial.println("");
  debugserial.print("Output log: ");
  debugserial.println(flag);
}

void Log_Output(char flag){
  if(flag == HPID_log){
      debugserial.print("Target Th\t");
      debugserial.print(Setting_H_temp);
      debugserial.print("\tTemperature Th\t");
      debugserial.print(Real_Temp1);
      debugserial.print("\tVolt%\t");
      debugserial.print(H_ThVolt); 
      debugserial.print("\tKp\t");
      debugserial.print(HKp); 
      debugserial.print("\tKi\t");
      debugserial.print(HKi);   
      debugserial.print("\tKd\t");
      debugserial.println(HKd);   
    }
    else if(flag == CPID_log){
      debugserial.print("Target TE\t");
      debugserial.print(Setting_C_temp);      
      debugserial.print("\tTemperature TE\t");
      debugserial.print(Real_Temp2);
      debugserial.print("\tVolt%\t");
      debugserial.print(C_ThVolt); 
      debugserial.print("\tKp\t");
      debugserial.print(CKp); 
      debugserial.print("\tKi\t");
      debugserial.print(CKi);   
      debugserial.print("\tKd\t");
      debugserial.println(CKd);
    }
    else if(flag == FanC_log){
      debugserial.print(" Fan1: ");
      debugserial.print(Fan1_sec);
      debugserial.print(" Fan2: ");
      debugserial.print(Fan2_sec);
      debugserial.print(" Fan3: ");
      debugserial.print(Fan3_sec);
      debugserial.print(" Fan4: ");
      debugserial.println(Fan4_sec);
    }
}

void Log_LED(int LED_status){
  if(LED_status == 1)
    debugserial.println("LED ON");
  else if(LED_status == 0)
    debugserial.println("LED OFF");
}

