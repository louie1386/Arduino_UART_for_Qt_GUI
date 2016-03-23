void thermistor(){
  T=0;
  sensorValue = analogRead(ThermistorPin);            
  R = (4.7*sensorValue)/(1023-sensorValue);
  Temp_array[index] = 162.1-33.76*log(R)+0.18898*log(R)*log(R)*log(R);
  if(index<Data_size){
    index++;
  }
  else{
    index=0;
  }
  for(int i=0;i<Data_size;i++){
    T+=Temp_array[i];
  }
  T=T/Data_size;
}
