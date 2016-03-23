char RXD(){
  char ret;
  ret = Qtserial.read();
  if(byte(ret) == 0xAA)
    debugserial.println(" ");
  debugserial.print(byte(ret), HEX);
  debugserial.print(" ");
  return ret;
  }

char TXD(char txstr){
  Qtserial.print(txstr);
  if(byte(txstr) == 0xAA)
    debugserial.println(" ");
  debugserial.print(byte(txstr), HEX);
  debugserial.print(" ");
  return txstr;
  }
