void Log_Input(char flag){
  debugserial.println("");
  debugserial.print("Output log: ");
  debugserial.println(flag);
}

void Log_Output(char flag){
  if(flag == HPID_log){
      debugserial.print("Temperature Th\t");
      debugserial.print(Real_H_temp);
      debugserial.print("\tVolt%\t");
      debugserial.println(H_ThVolt); 
    }
    else if(flag == CPID_log){
      debugserial.print("Temperature TE\t");
      debugserial.print(Real_C_temp);
      debugserial.print("\tVolt%\t");
      debugserial.println(C_ThVolt); 
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

