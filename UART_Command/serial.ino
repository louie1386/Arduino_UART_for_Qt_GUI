char RXD(){
  char ret;
  ret = Qtserial.read();
  debugserial.print(ret);
  return ret;
  }

char TXD(char txstr){
  Qtserial.print(txstr);
  debugserial.print(txstr);
  return txstr;
  }
