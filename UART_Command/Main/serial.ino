char RXD(){
  char rxstr;
  rxstr = Qtserial.read();
  debug_serial_print(rxstr, true);
  return rxstr;
  }

char TXD(char txstr){
  Qtserial.print(txstr);
  debug_serial_print(txstr, false);
  return txstr;
  }

void debug_serial_print(char str, bool txrx){
  if(byte(str) == 0xAA){
    debugserial.println(" ");
    if(txrx)
      debugserial.print("--> ");
     else
      debugserial.print("<-- ");
    }
  if(byte(str) < 0x10)
    debugserial.print(0);
  debugserial.print(byte(str), HEX);
  debugserial.print(" ");
  }
