double thermistor(int pin){
  double R, Te;
  int sensorValue = 0 ;
  sensorValue = analogRead(pin);            
  R = (4.7*sensorValue)/(1023-sensorValue);
  Te = 162.1-33.76*log(R)+0.18898*log(R)*log(R)*log(R);
  return Te;
}

double T_avg(double Tarray[Data_size]){
  double T = 0;
  for(int i=0;i<Data_size;i++){
    T+=Tarray[i];
  }
  T=T/Data_size;
  return T;
}

