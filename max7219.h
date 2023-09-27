// MAX7219 commands:
#define CMD_NOOP   0
#define CMD_DIGIT0 1
#define CMD_DIGIT1 2
#define CMD_DIGIT2 3
#define CMD_DIGIT3 4
#define CMD_DIGIT4 5
#define CMD_DIGIT5 6
#define CMD_DIGIT6 7
#define CMD_DIGIT7 8
#define CMD_DECODEMODE  9
#define CMD_INTENSITY   10
#define CMD_SCANLIMIT   11
#define CMD_SHUTDOWN    12
#define CMD_DISPLAYTEST 15
byte scr[136];    //количество симолов в скроле стороке
//======================================================================================================
void sendCmdAll(byte cmd, byte data) {
  digitalWrite(CS_PIN, LOW);
  for(int i = NUM_MAX - 1; i >= 0; i--) {
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, cmd);
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, data);
  }
  digitalWrite(CS_PIN, HIGH);
}
//======================================================================================================берем первую строчку и заганяем по столбцам в обратном порядке
void refreshAll() {         // for(int sd= 0; sd < 8; sd++){  scr[sd] = scr[sd]>>8 ; }
  byte mask = (rotate == 270? 0x01 : 0x80);   //00000001  //10000000 //for(int b = 0; b < 8; b++){   bt = bt>> 1;   if(scr[i * 8 + b] & mask) stol |= 0x08;  }  // bt =bt | 0x01;//00000001  //столбцы         //sprintf (srt,"1234" ) 

  for(int ctr = 0; ctr < 8; ctr++)  //колисечество строк рисуес по строкам для 270 с низу
 {   digitalWrite(CS_PIN, LOW);
  for(int i = NUM_MAX - 1; i >= 0; i--)   //насколько протолкнуть от первого индикатора где  кабель подключен от 3,2,1,0
 //   for(int i = 0; i <= NUM_MAX - 1; i++)   //насколько протолкнуть от первого индикатора где  кабель подключен от 3,2,1,0  для перееворота с 270 градусами
    { byte bt = 0;    
      if(rotate == 270)     //для 270 градусов с0 внизу начинает стоки с низу //за заход выплевуем всю стороку для всех индикаторов
      { 
        for(int b = 0; b < 8; b++  ){  bt = bt<< 1;   if(scr[i * 8 + b] & mask){ bt |= 0x01;}  }  // bt =bt | 0x01;//00000001  //столбцы
        shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, CMD_DIGIT0 + ctr);       //Последовательность вывода бит MSBFIRST — слева или LSBFIRST — справа.
        shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, bt);
      //for(int cc = 0; cc < 10; cc++) {Serial.print(String (   scr[i*8+cc])+" ");} Serial.println(" ");  
    // Serial.print( bt,BIN);  Serial.println(" ");
      } 
      else if(rotate == 90) //ПОВОРОТ ПО ЧАСОВОЙ СТРЕЛКИ КАЖДОГОИНДЕКАТОРА
      { for(int b = 0; b < 8; b++){  bt >>= 1;    if(scr[i * 8 + b] & mask) bt |= 0x80;  }//10000000    //сдвигам в столбце  логический или устанвит бти в 1
        shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, CMD_DIGIT0 + ctr);
        shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, bt);
      } 
      else 
      { 
        shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, CMD_DIGIT0 + ctr);   // с количество строк наверное
        shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, scr[i * 8 + ctr]);    //scr[i * 8 + c]  scr -прсто масив с данными 0*8 1*8 8 2*8 16   может быть i 2 1 0 количество матри ц минус одну 
      }
  }   //количество матриц
    digitalWrite(CS_PIN, HIGH);
    if(rotate == 270) mask <<= 1;
    if(rotate == 90) mask >>= 1;

  } //количество строк 
}
//======================================================================================================
void clr() {
  for(int i = 0; i < NUM_MAX * 8; i++) scr[i] = 0;
}
//======================================================================================================
void scrollLeft() {
  for(int i = 0; i < NUM_MAX * 8 + 7; i++) scr[i] = scr[i + 1];
}
//======================================================================================================
void scrollPrav() {
  for(int i = NUM_MAX * 8 + 7; i > 0 ; i--) scr[i] = scr[i - 1];
}
//======================================================================================================
void invert() {
  for(int i = 0; i < NUM_MAX * 8; i++) scr[i] =~ scr[i];
}
//======================================================================================================
void initMAX7219() {
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  delay(500);
  sendCmdAll(CMD_DISPLAYTEST, 0);
  sendCmdAll(CMD_SCANLIMIT, 7);
  sendCmdAll(CMD_DECODEMODE, 0);
  sendCmdAll(CMD_INTENSITY, 0);
  sendCmdAll(CMD_SHUTDOWN, 0);
  clr();
  refreshAll();
}
