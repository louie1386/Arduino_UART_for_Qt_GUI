void Mux_PD_setup(){
  pinMode(PD_bit0,OUTPUT);
  pinMode(PD_bit1,OUTPUT);
  pinMode(PD_bit2,OUTPUT);
  pinMode(PD_bit3,OUTPUT);
  digitalWrite(PD_bit0,false);
  digitalWrite(PD_bit1,false);
  digitalWrite(PD_bit2,false);
  digitalWrite(PD_bit3,false);
  pinMode(LED_en,OUTPUT);
  digitalWrite(LED_en,false);
}

int readch_PD(int chnum, int times){
  if(chnum > 8)
    chnum = 8;  
  int ret;
  int sum = 0;

  digitalWrite(PD_bit0, bitRead(chnum, 0));
  digitalWrite(PD_bit1, bitRead(chnum, 1));
  digitalWrite(PD_bit2, bitRead(chnum, 2));
  digitalWrite(PD_bit3, bitRead(chnum, 3));
  delay(1);
  for(int j = 0;j < times;j++){
    sum += analogRead(PDmuxinput);     
    delay(1);  
  }
  ret = sum / times;
  return ret;
}
