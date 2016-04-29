void Mux_leftup_setup(){
  pinMode(leftup_bit0,OUTPUT);
  pinMode(leftup_bit1,OUTPUT);
  pinMode(leftup_bit2,OUTPUT);
  pinMode(leftup_bit3,OUTPUT);
  digitalWrite(leftup_bit0,false);
  digitalWrite(leftup_bit1,false);
  digitalWrite(leftup_bit2,false);
  digitalWrite(leftup_bit3,false);
  pinMode(leftupmuxoutput,OUTPUT);
  digitalWrite(leftupmuxoutput,false);
}

char writech_leftup(int startbit, char data, int chnum){
  if(chnum > 8)
    chnum = 8;
  for(int i = startbit;i < (chnum + startbit);i++){
    if(!(bitRead(data, i - startbit))){
      digitalWrite(leftup_bit0, bitRead(i, 0));
      digitalWrite(leftup_bit1, bitRead(i, 1));
      digitalWrite(leftup_bit2, bitRead(i, 2));
      digitalWrite(leftup_bit3, bitRead(i, 3));   
      delay(1);       
      digitalWrite(leftupmuxoutput, true);
      delay(500); 
      digitalWrite(leftupmuxoutput, false);
    }
  }
}

