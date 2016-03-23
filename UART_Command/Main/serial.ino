char RXD(){
  char rxstr;
  rxstr = Qtserial.read();
  debug_serial_print(rxstr);
  return rxstr;
  }

char TXD(char txstr){
  Qtserial.print(txstr);
  debug_serial_print(txstr);
  return txstr;
  }

void debug_serial_print(char str){
  if(byte(str) == 0xAA)
    debugserial.println(" ");
  if(byte(str) < 0x10)
    debugserial.print(0);
  debugserial.print(byte(str), HEX);
  debugserial.print(" ");
  }
