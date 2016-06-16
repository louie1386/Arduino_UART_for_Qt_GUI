void PID_setup(){
  HPID.SetOutputLimits(0, 255);
  HPID.SetSampleTime(SampleTime);
  HPID.SetMode(AUTOMATIC);
  CPID.SetOutputLimits(0, 255);
  CPID.SetSampleTime(SampleTime);
  CPID.SetMode(AUTOMATIC);

  //HaTune.SetNoiseBand(1);
  //HaTune.SetLookbackSec(1000);
  //HaTune.SetOutputStep(100); 
  HaTune.SetControlType(1);
  //debugserial.println(HaTune.GetNoiseBand()); 
  //debugserial.println(HaTune.GetLookbackSec()); 
  //debugserial.println(HaTune.GetOutputStep());  
}

double thermistor(int pin){
  double R, Te;
  int sensorValue = 0 ;
  sensorValue = analogRead(pin);   
  if(sensorValue == 1023)
     sensorValue = 1022;        
  R = (4.7*sensorValue)/(1023-sensorValue);
  Te = 162.1-33.76*log(R)+0.18898*log(R)*log(R)*log(R);
  if(Te<0)
    Te = 0;
  return Te;
}


double tempICget(int pin){
  double volt = 0;
  volt = double(analogRead(pin))/1024*5000;
  double temp = (volt-500)/10;
  if(temp<0)
    temp = 0;  
  return temp;
}

double T_avg(double Tarray[Data_size]){
  double T = 0;
  for(int i=0;i<Data_size;i++){
    T+=Tarray[i];
  }
  T=T/Data_size;
  return T;
}

double realtempget(int mode, int pin){
  if(mode==mode_thermistor)
    return thermistor(pin);
  else if(mode==mode_tempIC)
    return tempICget(pin);
}

