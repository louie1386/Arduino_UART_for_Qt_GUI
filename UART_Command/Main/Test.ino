void Test_active(char flag){
  if(flag == NoTest){
    
  }
  else if(flag == LEDon){
    digitalWrite(LED_en,true);
    debugserial.println("LED ON");
  }
  else if(flag == LEDoff){
    digitalWrite(LED_en,false);
    debugserial.println("LED OFF");
  }  
  else if(flag == PDget){
    int PD_V;   
    double ADCtoVol;
    debugserial.println("PD get");
    for(int i=0;i<PD_Ch_Num;i++){
      PD_V = readch_PD(i,PD_Ch_St);
      ADCtoVol = double(PD_V) / 1024 *5000;
      debugserial.print("Channel ");
      debugserial.print(i+1);
      debugserial.print(" = ");
      debugserial.println(ADCtoVol);
    }
  }
}

