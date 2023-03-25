void Jump(){
  if(squatPosition == false){
    squat(0);
    Serial.print("Jongkok");    
    Serial.print(",");
    Serial.println(squatPosition);
  }
  else if(squatPosition == true && (wait_time_function(1000))){
    squat(1);    
    Serial.print("Ngadek");    
    Serial.print(",");
    Serial.println(squatPosition);
  }
}
