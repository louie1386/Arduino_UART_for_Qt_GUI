void buzzer_setup(){
  pinMode(buzzer,OUTPUT);
  buzzer_time = 0;
  buzzer_subtime = Buzzer_HL_msTime;
}

void buzzer_en(){
  if(buzzer_time){
    if(!buzzer_subtime){
      digitalWrite(buzzer,!digitalRead(buzzer)); 
      buzzer_subtime=Buzzer_HL_msTime;
    } 
    else
      buzzer_subtime--; 
    buzzer_time--;        
  }
  else
    digitalWrite(buzzer, false);
}

void buzzer_dis(){
  digitalWrite(buzzer, false);
  buzzer_time = 0;
}

