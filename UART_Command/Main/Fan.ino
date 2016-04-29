void Fan_setup(){
  pinMode(Fan1,INPUT);
  pinMode(Fan2,INPUT);
  pinMode(Fan3,INPUT);
  pinMode(Fan4,INPUT);
  attachInterrupt(digitalPinToInterrupt(Fan1), FanCounter1, RISING);
  attachInterrupt(digitalPinToInterrupt(Fan2), FanCounter2, RISING);
  attachInterrupt(digitalPinToInterrupt(Fan3), FanCounter3, RISING);
  attachInterrupt(digitalPinToInterrupt(Fan4), FanCounter4, RISING);

  pinMode(Fan_Power_en,OUTPUT);
  digitalWrite(Fan_Power_en,true);
}

void FanCounterOutput(){
    Fan1_sec = Fan1_sum;
    Fan2_sec = Fan2_sum;
    Fan3_sec = Fan3_sum;
    Fan4_sec = Fan4_sum;
    
    Fan1_sum = 0;
    Fan2_sum = 0;
    Fan3_sum = 0;
    Fan4_sum = 0;
}

void FanCounter1(){
  Fan1_sum++;
}

void FanCounter2(){
  Fan2_sum++;
}

void FanCounter3(){
  Fan3_sum++;
}

void FanCounter4(){
  Fan4_sum++;
}
