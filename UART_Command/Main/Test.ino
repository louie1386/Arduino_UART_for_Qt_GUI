void lefupTest(){
  digitalWrite(leftup_bit0, false);
  digitalWrite(leftup_bit1, false);
  digitalWrite(leftup_bit2, false);
  digitalWrite(leftup_bit3, false);   
  delay(1);       
  digitalWrite(leftupmuxoutput, true);
  delay(500); 
  digitalWrite(leftupmuxoutput, false);
}

