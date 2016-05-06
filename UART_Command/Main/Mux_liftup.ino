void Mux_liftup_setup(){
  pinMode(liftup_bit0,OUTPUT);
  pinMode(liftup_bit1,OUTPUT);
  pinMode(liftup_bit2,OUTPUT);
  pinMode(liftup_bit3,OUTPUT);
  digitalWrite(liftup_bit0,false);
  digitalWrite(liftup_bit1,false);
  digitalWrite(liftup_bit2,false);
  digitalWrite(liftup_bit3,false);
  pinMode(liftupmuxoutput,OUTPUT);
  digitalWrite(liftupmuxoutput,false);
}

char writech_liftup(int startbit, char data, int chnum){
  if(chnum > 8)
    chnum = 8;
  for(int i = startbit;i < (chnum + startbit);i++){
    if(!(bitRead(data, i - startbit))){
      digitalWrite(liftup_bit0, bitRead(i, 0));
      digitalWrite(liftup_bit1, bitRead(i, 1));
      digitalWrite(liftup_bit2, bitRead(i, 2));
      digitalWrite(liftup_bit3, bitRead(i, 3));   
      delay(liftup_ch_ms);     
      for(int i=0;i<liftup_push_times;i++)
      {  
        digitalWrite(liftupmuxoutput, true);
        delay(liftup_push_ms);
        digitalWrite(liftupmuxoutput, false);
        delay(liftup_release_ms);
      }
    }
  }
}

