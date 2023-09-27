//---------------------------------------------------------------------------
void buttonInter() {
 /* if (digitalRead(BUT_PIN) == butStat && butCount == 0 && butFlag == 0  && butMode == 0)                                                    { butCount=millis();    butFlag=1;  }
  if ((millis()-butCount) >= 30000 && butFlag == 1 && butMode == 0)                                                                         { butMode=4; butFlag=0; butCount=0; }
  if (digitalRead(BUT_PIN)==!butStat && (millis()-butCount)>=10000 && butFlag == 1 && butMode == 0)                                         { butMode=3; butFlag=0; butCount=0; }
  if (digitalRead(BUT_PIN)==!butStat && (millis()-butCount)< 10000 && (millis()-butCount) > 800 && (butFlag==1 || butFlag==2) && butMode==0){ butMode=1; butFlag=0; butCount=0; }
  if (digitalRead(BUT_PIN)==!butStat && (millis()-butCount)<= 800  && butFlag == 1 && butMode == 0)                                           butFlag=2;
  if (digitalRead(BUT_PIN)== butStat && (millis()-butCount)<= 800  && butFlag == 2)                                                         { butMode=2; butFlag=0; butCount=0; }
  */
}
//----------------------------------------------------------------
void buttonHandling() { //вызываеться якаждую секунду// если будильник работает, то любое нажатие выключает его
  if (alarm_stat &&  butMode)  {alarm_stat=0; alarm_hold=1; butMode=0; if(alarme[alarm_numer][2]==11) alarme[alarm_numer][2]=0; }
  if (butMode == 4) { // если кнопка нажата была более 30 секунд то возврат к заводским установкам
    butMode = 0;              //  Serial.println("Ta to jest KAPUT!!!!!!");  /* bip();bip();bip();bip(); */  SPIFFS.remove("/config.json");
   Serial.println("Ta to jest KAPUT!!!!!!");
    //if (printCom) {printTime(); Serial.println("ESP erase Config file"); } delay(8000);  ESP.reset();
  }
  if (butMode == 3) { butMode = 0;  if (printCom) {Serial.println("Reset ESP!!!");}  /*bip();    bip();  bip(); */  
#ifdef ESP32
 // ESP.restart();
#else
  // ESP.reset(); 
#endif
  } // если кнопка была нажата более 10 секунд но менее 30, то будет рестарт часов
    if (butMode == 1) 
  {/* bip(); */clr(); refreshAll();
    //if (sensorDom) { showSimpleTemp(); delay(1500);}
    if (sensorUl)  { showSimpleTempU();delay(1500);}
    if (sensorHome){ showSimpleTempH();delay(1500);}
    if (sensorHumi){ showSimpleHum();  delay(1500);}
    if (sensorPrAl){ showSimplePre();  delay(1500);}
    butMode=0;  clr();  refreshAll();
  }   // При двойном нажатии на кнопку выводится прогноз погоды
  if (butMode==2) {/*bip();bip();*/ butMode=0; clr(); refreshAll(); printStringWithShift(weatherString.c_str(),timeScrollSpeed); printStringWithShift(weatherStringZ.c_str(),timeScrollSpeed); printStringWithShift(weatherStringZav.c_str(),timeScrollSpeed); clr();refreshAll(); }
  }
