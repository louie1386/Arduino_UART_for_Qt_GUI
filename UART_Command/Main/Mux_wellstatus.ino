void Mux_wellstatus_setup(){
  pinMode(well_bit0,OUTPUT);
  pinMode(well_bit1,OUTPUT);
  pinMode(well_bit2,OUTPUT);
  pinMode(well_bit3,OUTPUT);
  digitalWrite(well_bit0,false);
  digitalWrite(well_bit1,false);
  digitalWrite(well_bit2,false);
  digitalWrite(well_bit3,false);
  pinMode(wellmuxinput,INPUT);
}

char readch_wellstatus(int startbit, int chnum){
  char ret = 0;
  if(chnum > 8)
    chnum = 8;
  for(int i = startbit;i < (chnum + startbit);i++){
    digitalWrite(well_bit0, bitRead(i, 0));
    digitalWrite(well_bit1, bitRead(i, 1));
    digitalWrite(well_bit2, bitRead(i, 2));
    digitalWrite(well_bit3, bitRead(i, 3));
    delay(1);
    bitWrite(ret, (i - startbit), digitalRead(wellmuxinput));
  }
  return ret;
}

