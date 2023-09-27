//==========ВИВІД НА ЕКРАН ТЕМПЕРАТУРИ В БУДИНКУ НЕ ИСПОЛЬЗУЮ для вывода отладки=========yyyyy===============================
void showTest() {    fl_dom=1;  wrem_tm="  "+String(t0Lux);      printStringWithShift( wrem_tm.c_str(), 40); }

//==========ДРУКУВАННЯ БІГУЧОЇ СТРОКИ *s - текст, shiftDelay - швидкість==========================================
void printStringWithShift(const char* s, int shiftDelay) {                                                    // коли працює ця функція, основний цикл зупиняється
  while (*s) 
  {  printCharWithShift(*s, shiftDelay);  s++;    
     server.handleClient();                                      // зберігаемо можливість відповіді на HTML запити під час бігучої стоки
     //MPR121_update(); 
      if (pr_bip_vre3) {bip_vre4();}  //звук с веба
      if (f_kuku) {bip_vre4();}  // кушка когда бежит стока стобы тоже звук воспроизводить
      if (one_fl==1||one_f2==1||one_f3==1||one_f4==1) {bip_vre4();} 
     buttonInter(); if(butMode !=0) {  clr(); refreshAll(); runningLine=0; return; } //анадлиз интрвала нажатия хлавиши //при любом нажатии прекращаем бегущую сторку наверное
  }
}
//==========ДРУКУВАННЯ БІГУЧОГО СИМВОЛУ с - символ, shiftDelay - швидкість=====================================
void printCharWithShift(unsigned char c, int shiftDelay) {
  c = convert_UA_RU_PL_DE(c);   if (c < ' ') return;
  c -= 32;  int w = showChar(c, fontUA_RU_PL_DE);
  for (int i = 0; i < w + 1; i++) { delay(shiftDelay); 
 // for (int i = w + 1; i > 0; i--) { delay(shiftDelay); 
  scrollLeft();  
 // scrollPrav();
  refreshAll();  }
}
//======================================================================================
int showChar(char ch, const uint8_t *data) {
  int len = pgm_read_byte(data);
  int i, w = pgm_read_byte(data + 1 + ch * len);
  for (i = 0; i < w; i++)   scr[NUM_MAX * 8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
  scr[NUM_MAX * 8 + i] = 0;
  return w;
}
//==========================================vvv============================================
void showDigit(char ch, int col, const uint8_t *data) {
  if ((dy < -8) | (dy > 8)) return;
  int len = pgm_read_byte(data);
  int w = pgm_read_byte(data + 1 + ch * len);
  col += dx;
  for (int i = 0; i < w; i++) {
    if (col + i >= 0 && col + i < 8 * NUM_MAX) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if (!dy) scr[col + i] = v; else scr[col + i] |= dy > 0 ? v >> dy : v << -dy;
    }
  }
}
//======================================================================================
void setCol(int col, byte v) 
{ if ((dy < -8) | (dy > 8)) return;
  col +=dx;   if (col>=0 && col<8 * NUM_MAX) { if(!dy) scr[col]=v; else scr[col] |= dy > 0 ? v >> dy : v << -dy;  }
}
//======================================================================================//======================================================================================//======================================================================================
//======================================================================================//======================================================================================//======================================================================================
//======================================================================================//======================================================================================//======================================================================================
//========== ГОДИННИКА=======================================
void showAnimClock2() {             // byte fontCLOCK = 0;      // 0-крупный, 1-крупный цифровой, 2-полу жирный, 3-полу жирный цифровой, 4-обычный, 5-обычный цифровой, 6-узкий, 7-узкий цифровой.
signed int  indent = (hour < 10 ? 12 : 15) + 4 * (NUM_MAX - 4);
signed int digPos[5] = {(indent - 14), (indent -  7 ), (indent + 3), (indent +  10 ), indent};
  int digHt = 16;
  int num = hour < 10 ? 1 : 0;
  int i;
  if (del == 0) {
    del = digHt;
    for (i = num; i < 4; i++) digold[i] = dig[i]; //стары цифры копируем в масив
    dig[0] = hour / 10;
    dig[1] = hour % 10;
    dig[2] = minute / 10;
    dig[3] = minute % 10;
    for (i = num; i < 4; i++)  digtrans[i] = (dig[i] == digold[i]) ? 0 : digHt; //цифры для движения наверное
  } else del--;
  clr();
  for (i = num; i < 4; i++) {
    if (digtrans[i] == 0) {
      dy = 0;
      if (fontCLOCK == 0) showDigit(dig[i], digPos[i], dig6x8);
      else if (fontCLOCK == 1) showDigit(dig[i], digPos[i], dig6x8dig);
      else if (fontCLOCK == 2) showDigit(dig[i], digPos[i], dig5x8rn);
      else if (fontCLOCK == 3) showDigit(dig[i], digPos[i], dig5x8rndig);
      else if (fontCLOCK == 4) showDigit(dig[i], digPos[i], dig5x8);
      else if (fontCLOCK == 5) showDigit(dig[i], digPos[i], dig5x8dig);
      else if (fontCLOCK == 6) showDigit(dig[i], digPos[i], dig4x8);
      else if (fontCLOCK == 7) showDigit(dig[i], digPos[i], dig4x8dig);
    } else {
      dy = digHt - digtrans[i];
      if (fontCLOCK == 0) showDigit(digold[i], digPos[i], dig6x8);
      else if (fontCLOCK == 1) showDigit(digold[i], digPos[i], dig6x8dig);
      else if (fontCLOCK == 2) showDigit(digold[i], digPos[i], dig5x8rn);
      else if (fontCLOCK == 3) showDigit(digold[i], digPos[i], dig5x8rndig);
      else if (fontCLOCK == 4) showDigit(digold[i], digPos[i], dig5x8);
      else if (fontCLOCK == 5) showDigit(digold[i], digPos[i], dig5x8dig);
      else if (fontCLOCK == 6) showDigit(digold[i], digPos[i], dig4x8);
      else if (fontCLOCK == 7) showDigit(digold[i], digPos[i], dig4x8dig);
      dy = - digtrans[i];
      if (fontCLOCK == 0) showDigit(dig[i], digPos[i], dig6x8);
      else if (fontCLOCK == 1) showDigit(dig[i], digPos[i], dig6x8dig);
      else if (fontCLOCK == 2) showDigit(dig[i], digPos[i], dig5x8rn);
      else if (fontCLOCK == 3) showDigit(dig[i], digPos[i], dig5x8rndig);
      else if (fontCLOCK == 4) showDigit(dig[i], digPos[i], dig5x8);
      else if (fontCLOCK == 5) showDigit(dig[i], digPos[i], dig5x8dig);
      else if (fontCLOCK == 6) showDigit(dig[i], digPos[i], dig4x8);
      else if (fontCLOCK == 7) showDigit(dig[i], digPos[i], dig4x8dig);
      digtrans[i]--;
    }
  }
  dy = 0;
  int flash = millis() % 1000;
  if (!alarm_stat) 
  {// мерегтіння двокрапок в годиннику підвязуємо до личильника циклів
    if ((flash >= 180 && flash < 360) || flash >= 540) { setCol(digPos[4], WIFI_connected ? 0x66 : 0x60); setCol(digPos[4] + 1, WIFI_connected ? 0x66 : 0x60); }
    if (statusUpdateNtpTime) 
    {                                                                                                      // якщо останнє оновленя часу було вдалим, то двокрапки в годиннику будуть анімовані
      if (flash >= 0 && flash < 180)                   { setCol(digPos[4], WIFI_connected ? 0x24 : 0x20); setCol(digPos[4] + 1, WIFI_connected ? 0x42 : 0x40); }
      if (flash >= 360 && flash < 540)                 { setCol(digPos[4], WIFI_connected ? 0x42 : 0x40); setCol(digPos[4] + 1, WIFI_connected ? 0x24 : 0x20); }
    }
    if (updateForecast && WIFI_connected) setCol(00, flash < 500 ? 0x80 : 0x00);
    if (updateForecasttomorrow && WIFI_connected) setCol((NUM_MAX * 8 - 1), flash < 500 ? 0x80 : 0x00);
  } else {    setCol(digPos[4], 0x66);    setCol(digPos[4] + 1, 0x66);  }
  refreshAll();
}
//============================================
void showLux2() {
  if (WiFi.status() == WL_CONNECTED) {   
    byte indent = (NUM_MAX * 8) - 32;   dx = dy = 0;    clr();   showDigit((t6 < 0.0 ? 18 : 17), 0 + indent, znaki5x8); //друкуємо H+ альбо H-
    //float t6t = t6 * (t6 >= 0 ? 1 : -1);
    //t1Lux = int(t0Lux);
    if (t0Lux >= 10000) {
      showDigit((int) t0Lux % 10, 28 + indent, dig4x8);
      showDigit((int)(t0Lux / 10) % 10, 23 + indent, dig4x8);   
      showDigit((int)(t0Lux / 100) % 10, 18 + indent, dig4x8);   
      showDigit((int)(t0Lux / 1000) % 10, 13 + indent, dig4x8); 
      showDigit((int)(t0Lux / 10000) % 10, 8 + indent, dig4x8);
    } 
    else if (t0Lux >= 1000) {
      showDigit((int)(t0Lux * 10) % 10, 28 + indent, dig4x8); 
      showDigit(2, 26 + indent, znaki5x8);
      showDigit((int) t0Lux % 10, 21 + indent, dig4x8);
      showDigit((int)(t0Lux / 10) % 10, 16 + indent, dig4x8);
      showDigit((int)(t0Lux / 100) % 10, 11 + indent, dig4x8);
      showDigit((int)(t0Lux / 1000) % 10, 6 + indent, dig4x8);
    } else {
      showDigit((int)(t0Lux * 100) % 10, 28 + indent, dig4x8);  
      showDigit((int)(t0Lux * 10) % 10, 23 + indent, dig4x8);  
      showDigit(2, 21 + indent, znaki5x8);  
      showDigit((int)t0Lux % 10, 16 + indent, dig4x8);
      if (t0Lux >= 10) showDigit((int)(t0Lux / 10) % 10, 11 + indent, dig4x8);
      if (t0Lux >= 100) showDigit((int)(t0Lux / 100) % 10, 6 + indent, dig4x8);
    }
    refreshAll();
  }
}
//======================================================================================//======================================================================================//======================================================================================
void showLux() {
  byte indent = aliData * (NUM_MAX - 4);  dx = dy = 0;  clr(); 
  //t1Lux = atof(t0_lux)/10;
  t1Lux = int(t0Lux);
  t2Lux = int(t0Lux * 10 *  (t0Lux > 0 ? 1 : -1)) % 10;
  tt2Lux= int(t0Lux * 100 * (t0Lux > 0 ? 1 : -1)) % 10;
 
  //showDigit((t1kux < 0.0 ? 28 : 27), indent, znaki5x8); // друкуємо К+ альбо К-
 // showDigit(29, 5+indent, znaki5x8); // БУКВА МАЛЕНЬКАЯ у
  //showDigit(30, 9+indent, znaki5x8); //  МАЛЕНЬКАЯ х
  //if (t2Lux <= -10.0 || t2Lux >= 10) showDigit((t1Lux < 0 ? (t2Lux * -1) / 10 : t2Lux / 10), 13 + indent, dig5x8); //смичаем первую цифру в завсимотсти от значения стоб не выводить 0
  //showDigit((t2Lux < 0 ? (t2Lux * -1) % 10 : t2Lux % 10), 19 + indent, dig5x8);
  //showDigit(2, 25 + indent, znaki5x8);    //точка
  //showDigit(t3Lux, 27 + indent, dig5x8);   //дробная часть

  //showDigit((t0Lux < 0.0 ? 4 : 3), indent, znaki5x8); // друкуємо D+ альбо D-
  if (t1Lux <= -10.0 || t1Lux >= 10) showDigit((t1Lux < 0 ? (t1Lux * -1) / 10 : t1Lux / 10), 13 + indent, dig5x8); //смичаем первую цифру в завсимотсти от значения стоб не выводить 0
  showDigit((t1Lux < 0 ? (t1Lux * -1) % 10 : t1Lux % 10), 19 + indent, dig5x8);
  showDigit(2, 25 + indent, znaki5x8);    //точка
  showDigit(t2Lux, 27 + indent, dig5x8);   //дробная часть
  //showDigit(tt2Lux, 27 + indent, dig5x8);   //дробная часть



  refreshAll();
}
//==========ВИВІД НА ЕКРАН ТЕМПЕРАТУРИ В комнаты ========================================
void showSimpleTempKom() {
  byte indent = aliData * (NUM_MAX - 4);  dx = dy = 0;  clr(); 
  showDigit((t1kom < 0.0 ? 28 : 27), indent, znaki5x8); // друкуємо К+ альбо К-
  showDigit(22, 5+indent, znaki5x8); // БУКВА МАЛЕНЬКАЯ о
  showDigit(31, 9+indent, znaki5x8); //  МАЛЕНЬКАЯ типа м
  if (t2kom <= -10.0 || t2kom >= 10) showDigit((t1kom < 0 ? (t2kom * -1) / 10 : t2kom / 10), 13 + indent, dig5x8); //смичаем первую цифру в завсимотсти от значения стоб не выводить 0
  showDigit((t2kom < 0 ? (t2kom * -1) % 10 : t2kom % 10), 19 + indent, dig5x8);
  showDigit(2, 25 + indent, znaki5x8);    //точка
  showDigit(t3kom, 27 + indent, dig5x8);   //дробная часть
  refreshAll();
}
//==========ВИВІД НА ЕКРАН ТЕМПЕРАТУРИ В Кухня ========================================
void showSimpleTempKux() {
  byte indent = aliData * (NUM_MAX - 4);  dx = dy = 0;  clr(); 
  showDigit((t1kux < 0.0 ? 28 : 27), indent, znaki5x8); // друкуємо К+ альбо К-
  showDigit(29, 5+indent, znaki5x8); // БУКВА МАЛЕНЬКАЯ у
  showDigit(30, 9+indent, znaki5x8); //  МАЛЕНЬКАЯ х
  if (t2kux <= -10.0 || t2kux >= 10) showDigit((t1kux < 0 ? (t2kux * -1) / 10 : t2kux / 10), 13 + indent, dig5x8); //смичаем первую цифру в завсимотсти от значения стоб не выводить 0
  showDigit((t2kux < 0 ? (t2kux * -1) % 10 : t2kux % 10), 19 + indent, dig5x8);
  showDigit(2, 25 + indent, znaki5x8);    //точка
  showDigit(t3kux, 27 + indent, dig5x8);   //дробная часть
  refreshAll();
}

//==========ВИВІД НА ЕКРАН ТЕМПЕРАТУРИ В БУДИНКУ 2 ========================================
void showSimpleTemp2() {
  fl_dom=1;
  byte indent = aliData * (NUM_MAX - 4);
  dx = dy = 0;  clr(); 
  showDigit((t0 < 0.0 ? 4 : 3), indent, znaki5x8); // друкуємо D+ альбо D-
  showDigit(22, 4+indent, znaki5x8); // БУКВА МАЛЕНЬКАЯ о
  showDigit(24, 8+indent, znaki5x8); //  МАЛЕНЬКАЯ _1
  if (t1 <= -10.0 || t1 >= 10) showDigit((t1 < 0 ? (t1 * -1) / 10 : t1 / 10), 13 + indent, dig5x8); //смичаем первую цифру в завсимотсти от значения стоб не выводить 0
  showDigit((t1 < 0 ? (t1 * -1) % 10 : t1 % 10), 19 + indent, dig5x8);
  showDigit(2, 25 + indent, znaki5x8);    //точка
  showDigit(t2, 27 + indent, dig5x8);   //дробная часть
  refreshAll();
}
//==========ВИВІД НА ЕКРАН ТЕМПЕРАТУРИ В БУДИНКУ 3 ========================================
void showSimpleTempDom1() {
  fl_dom=1;
  byte indent = aliData * (NUM_MAX - 4);
  dx = dy = 0;  clr(); 
  showDigit((t0 < 0.0 ? 4 : 3), indent, znaki5x8); // друкуємо D+ альбо D-
 // showDigit(22, 4+indent, znaki5x8); // БУКВА МАЛЕНЬКАЯ о
 // showDigit(24, 8+indent, znaki5x8); //  МАЛЕНЬКАЯ _1
   if (t1 <= -10.0 || t1 >= 10) showDigit((t1 < 0 ? (t1 * -1) / 10 : t1 / 10), 7 + indent, dig5x8); //смичаем первую цифру в завсимотсти от значения стоб не выводить 0
  showDigit((t1 < 0 ? (t1 * -1) % 10 : t1 % 10), 13 + indent, dig5x8);
  showDigit(2, 19 + indent, znaki5x8);    //точка
  showDigit(t2, 21 + indent, dig5x8);   //дробная часть
  showDigit(tt2, 27 + indent, dig5x8);   //дробная часть
  refreshAll();
}
//==========ВИВІД НА ЕКРАН ТЕМПЕРАТУРИ НА ВУЛИЦІ======yyy=================================
void showSimpleTempU() {
  if (WiFi.status() == WL_CONNECTED)   
  { byte indent = aliData * (NUM_MAX - 4);
    dx = dy = 0; fl_uli=1;    clr();
    showDigit((t3 < 0.0 ? 6 : 5), indent, znaki5x8); //друкуємо U+ альбо U-
    showDigit(26, 4+indent, znaki5x8); // БУКВА МАЛЕНЬКАЯ о
   // showDigit(24, 8+indent, znaki5x8); //  МАЛЕНЬКАЯ _1  
    if (t4 <= -10.0 || t4 >= 10) showDigit((t4 < 0 ? (t4 * -1) / 10 : t4 / 10), 13 + indent, dig5x8); //4//смичаем первую цифру в завсимотсти от значения стоб не выводить 0
    showDigit((t4 < 0 ? (t4 * -1) % 10 : t4 % 10), 19 + indent, dig5x8);    //10
    showDigit(2, 25 + indent, znaki5x8);    //16//точка
    showDigit(t5, 27 + indent, dig5x8);     //18//дробная часть
    refreshAll();
  }
}
//==========ВИВІД НА ЕКРАН ДОДАТКОВИХ ДАННИХ==== пример как выводить узкие символы цифры====================================
void showSimpleTempH() {
  if (WiFi.status() == WL_CONNECTED) {   
    byte indent = (NUM_MAX * 8) - 32;   dx = dy = 0;    clr();   showDigit((t6 < 0.0 ? 18 : 17), 0 + indent, znaki5x8); //друкуємо H+ альбо H-
    float t6t = t6 * (t6 >= 0 ? 1 : -1);
    if (t6t >= 10000) {
      showDigit((int)t6t % 10, 28 + indent, dig4x8);
      showDigit((int)(t6t / 10) % 10, 23 + indent, dig4x8);   
      showDigit((int)(t6t / 100) % 10, 18 + indent, dig4x8);   
      showDigit((int)(t6t / 1000) % 10, 13 + indent, dig4x8); 
      showDigit((int)(t6t / 10000) % 10, 8 + indent, dig4x8);
    } 
    else if (t6t >= 1000) {
      showDigit((int)(t6t * 10) % 10, 28 + indent, dig4x8); 
      showDigit(2, 26 + indent, znaki5x8);
      showDigit((int)t6t % 10, 21 + indent, dig4x8);
      showDigit((int)(t6t / 10) % 10, 16 + indent, dig4x8);
      showDigit((int)(t6t / 100) % 10, 11 + indent, dig4x8);
      showDigit((int)(t6t / 1000) % 10, 6 + indent, dig4x8);
    } else {
      showDigit((int)(t6t * 100) % 10, 28 + indent, dig4x8);  
      showDigit((int)(t6t * 10) % 10, 23 + indent, dig4x8);  
      showDigit(2, 21 + indent, znaki5x8);  
      showDigit((int)t6t % 10, 16 + indent, dig4x8);
      if (t6t >= 10) showDigit((int)(t6t / 10) % 10, 11 + indent, dig4x8);
      if (t6t >= 100) showDigit((int)(t6t / 100) % 10, 6 + indent, dig4x8);
    }
    refreshAll();
  }
}
//==========ВИВІД НА ЕКРАН ВОЛОГОСТІ В БУДИНКУ========================================
void showSimpleHum() {
  byte indent = aliData * (NUM_MAX - 4);  dx = dy = 0;  clr();
  showDigit(7, indent, znaki5x8);     // друкуємо знак вологості
  if (h1 >= 10) showDigit(h1 / 10, 12 + indent, dig5x8);
  showDigit((h1 - (h1 / 10) * 10), 18 + indent, dig5x8);
  //showDigit(2, 18 + indent, znaki5x8);
  //showDigit(h2, 20 + indent, dig5x8);
  showDigit(8, 26 + indent, znaki5x8);
  refreshAll();
}
//=========ВИВІД НА ЕКРАН ДАВЛЕНИЯ=======v==!!!!========Текущего давления=======================
void showSimplePre() {
  byte indent = aliData * (NUM_MAX - 4);  dx=dy=0;  clr();  
 //if (dav_pov) showDigit(21, 0 + indent, znaki5x8);  else showDigit(9, 0 + indent, znaki5x8);     //21 ДАВЛЕНИЕ ПОВЫСИИЛОСЬ ЗА ЧАС 9 давление ПОНИЗИЛОСЬ  // друкуємо знак тиску}
  //showDigit('1' , (p1 > 0 ? 0 : 0) + indent, dig5x8);  
  showDigit(9 ,  indent, znaki5x8);    // друкуємо знак давления -8 . потом смичение,  затем  указываем что это знак а не цифра
  showDigit(p1 , (p1 > 0 ? 7 : 7) + indent, dig5x8);  
  showDigit(p2 , (p1 > 0 ? 13 : 13) + indent, dig5x8);  
  showDigit(p3 , (p1 > 0 ? 19 : 19) + indent, dig5x8);
  showDigit(2, 25 , znaki5x8);  //точка
  showDigit(p4 , (p1 > 0 ? 27 : 27) + indent, dig5x8);    
  //showDigit( 19 ,  23 + indent, znaki5x8);  
  //showDigit( 20 ,  29 + indent, znaki5x8);
  refreshAll();
}
//==========ВИВІД НА ЕКРАН ДАВЛЕНИЯ========вывлд предыдужего давления=============================
void showSimplePre2() {
  byte indent = aliData * (NUM_MAX - 4);  dx=dy=0;  clr();  pd0 = (int)pred_dav;  if (sensorPrAl) pd0 += corrPress; //Выполняем корекцию при вывде
  pd1 = pd0 / 1000;  pd2 = (pd0 - pd1 * 1000) / 100;  pd3 = (pd0 - pd1 * 1000 - pd2 * 100) / 10;  pd4 = pd0 % 10;
  showDigit(pd2 , (p1 > 0 ? 10 : 5) + indent, dig5x8);  showDigit(pd3 , (p1 > 0 ? 16 : 11) + indent, dig5x8);  showDigit(pd4 , (p1 > 0 ? 22 : 17) + indent, dig5x8);  
  refreshAll();
}
//==========ВИВІД НА ЕКРАН ДАВЛЕНИЯ =========ДАВЛЕНИЯ== ВВиде строки =daf==================ДАВЛЕНИЯ===================
void showSimplePre3() { fl_dav3=1;
  if (WiFi.status() == WL_CONNECTED) 
  {   //  dav_tmp="    \215"+String(((int)pressBmp)+corrPress)+"\223" +" max"+  (int)max_dav +" min"+ (int)min_dav+"     ";  //215 знак давления  259
     //dav_tmp="    \237"+ String((int)max_dav+corrPress) +"   \236"+ String((int)min_dav+corrPress)+" ";  //215 знак давления  259
       dav_tmp="    \237"+ String((int)max_dav+corrPress) +"   \236"+ String((int)min_dav+corrPress)+" ";  //215 знак давления  259
     printStringWithShift( dav_tmp.c_str(), 30);
  }}
  //==========ВИВІД НА ЕКРАН ДАВЛЕНИЯ =========ДАВЛЕНИЯ== ВВиде строки =daf==================ДАВЛЕНИЯ===================
void showSimplePre4() { //fl_dav3=1;
  if (WiFi.status() == WL_CONNECTED) 
  {   //  dav_tmp="    \215"+String(((int)pressBmp)+corrPress)+"\223" +" max"+  (int)max_dav +" min"+ (int)min_dav+"     ";  //215 знак давления  259
     //dav_tmp="    \237"+ String((int)max_dav+corrPress) +"   \236"+ String((int)min_dav+corrPress)+" ";  //215 знак давления  259
     dav_tmp=" K="+ String(kol_dav)+"del="+String(delta)+", "+ String((int)pressure_del[0])+"," + String((int)pressure_del[1])+"," + String((int)pressure_del[2])+","+ String((int)pressure_del[3])+","+ String((int)pressure_del[4])+","+String((int)pressure_del[5])+","+ String((int)pressure_array[6])+" ";  //215 знак давления  259
     printStringWithShift( dav_tmp.c_str(), 30);
  }}
 
  //==========ВИВІД НА ЕКРАН ДАВЛЕНИЯ =========ДАВЛЕНИЯ== ВВиде строки =daf==================ДАВЛЕНИЯ===================
void showSimplePre5() { //fl_dav3=1;
  if (WiFi.status() == WL_CONNECTED) 
  {   //  dav_tmp="    \215"+String(((int)pressBmp)+corrPress)+"\223" +" max"+  (int)max_dav +" min"+ (int)min_dav+"     ";  //215 знак давления  259
     //dav_tmp="    \237"+ String((int)max_dav+corrPress) +"   \236"+ String((int)min_dav+corrPress)+" ";  //215 знак давления  259
     dav_tmp="  Ch "+ String(dav_po_chasu[0]) + "."+ String(dav_po_chasu[1]) + "."+ String(dav_po_chasu[2]) + "."+ String(dav_po_chasu[3]) + "."+ String(dav_po_chasu[4]) +  "."+String(dav_po_chasu[5]) +"."+ String(dav_po_chasu[6])+"." + String(dav_po_chasu[7])+"."+ String(dav_po_chasu[8]);  //215 знак давления  259
     printStringWithShift( dav_tmp.c_str(), 30);
  }}
 
//==========ВИВІД НА ЕКРАН ДАТИ==============ddd===========================================
void showSimpleDate() {
  if (WiFi.status() == WL_CONNECTED) 
  {  fl_dat=1;  wrem_tm=" "+String(day/10)+String(day%10)+"\267";
      switch (dayOfWeek)    {case 1: {wrem_tm+="Вос"; break;} case 2: {wrem_tm+="Пон"; break;} case 3: {wrem_tm+="Вто"; break;} case 4: {wrem_tm+="Сре"; break;} case 5: {wrem_tm+="Чет"; break;} case 6: {wrem_tm+="Пят"; break;}  case 7: {wrem_tm+="Суб"; break;}  } 
      printStringWithShift( wrem_tm.c_str(), 15);
  }
}
//==========ВИВІД НА ЕКРАН ДАТИ 2=========================================================
void showSimpleDate2() {
  fl_dat=1;
  byte indent = aliData * (NUM_MAX - 4);
  dx = dy = 0;  clr();
  showDigit(day / 10, indent, dig4x8);
  showDigit(day % 10,  5 + indent, dig4x8);
  showDigit(month / 10, 12 + indent, dig4x8);
  showDigit(month % 10, 17 + indent, dig4x8);
  showDigit((year - 2000) / 10, 23 + indent, dig4x8);
  showDigit((year - 2000) % 10, 28 + indent, dig4x8);
  setCol(10 + indent, 0xC0);
  setCol(22 + indent, 0xC0);
  refreshAll();
}
//==========ВИВІД НА ЕКРАН АНІМАЦІЙНОГО ГОДИННИКА=======================================
void showAnimClock() {             // byte fontCLOCK = 0;      // 0-крупный, 1-крупный цифровой, 2-полу жирный, 3-полу жирный цифровой, 4-обычный, 5-обычный цифровой, 6-узкий, 7-узкий цифровой.
signed int  indent = (hour < 10 ? 12 : 15) + 4 * (NUM_MAX - 4);
signed int digPos[5] = {(indent - 14), (indent -  7 ), (indent + 3), (indent +  10 ), indent};
  int digHt = 16;
  int num = hour < 10 ? 1 : 0;
  int i;
  if (del == 0) {
    del = digHt;
    for (i = num; i < 4; i++) digold[i] = dig[i];
    dig[0] = hour / 10;
    dig[1] = hour % 10;
    dig[2] = minute / 10;
    dig[3] = minute % 10;
    for (i = num; i < 4; i++)  digtrans[i] = (dig[i] == digold[i]) ? 0 : digHt;
  } else del--;
  clr();
  for (i = num; i < 4; i++) {
    if (digtrans[i] == 0) {
      dy = 0;
      if (fontCLOCK == 0) showDigit(dig[i], digPos[i], dig6x8);
      else if (fontCLOCK == 1) showDigit(dig[i], digPos[i], dig6x8dig);
      else if (fontCLOCK == 2) showDigit(dig[i], digPos[i], dig5x8rn);
      else if (fontCLOCK == 3) showDigit(dig[i], digPos[i], dig5x8rndig);
      else if (fontCLOCK == 4) showDigit(dig[i], digPos[i], dig5x8);
      else if (fontCLOCK == 5) showDigit(dig[i], digPos[i], dig5x8dig);
      else if (fontCLOCK == 6) showDigit(dig[i], digPos[i], dig4x8);
      else if (fontCLOCK == 7) showDigit(dig[i], digPos[i], dig4x8dig);
    } else {
      dy = digHt - digtrans[i];
      if (fontCLOCK == 0) showDigit(digold[i], digPos[i], dig6x8);
      else if (fontCLOCK == 1) showDigit(digold[i], digPos[i], dig6x8dig);
      else if (fontCLOCK == 2) showDigit(digold[i], digPos[i], dig5x8rn);
      else if (fontCLOCK == 3) showDigit(digold[i], digPos[i], dig5x8rndig);
      else if (fontCLOCK == 4) showDigit(digold[i], digPos[i], dig5x8);
      else if (fontCLOCK == 5) showDigit(digold[i], digPos[i], dig5x8dig);
      else if (fontCLOCK == 6) showDigit(digold[i], digPos[i], dig4x8);
      else if (fontCLOCK == 7) showDigit(digold[i], digPos[i], dig4x8dig);
      dy = - digtrans[i];
      if (fontCLOCK == 0) showDigit(dig[i], digPos[i], dig6x8);
      else if (fontCLOCK == 1) showDigit(dig[i], digPos[i], dig6x8dig);
      else if (fontCLOCK == 2) showDigit(dig[i], digPos[i], dig5x8rn);
      else if (fontCLOCK == 3) showDigit(dig[i], digPos[i], dig5x8rndig);
      else if (fontCLOCK == 4) showDigit(dig[i], digPos[i], dig5x8);
      else if (fontCLOCK == 5) showDigit(dig[i], digPos[i], dig5x8dig);
      else if (fontCLOCK == 6) showDigit(dig[i], digPos[i], dig4x8);
      else if (fontCLOCK == 7) showDigit(dig[i], digPos[i], dig4x8dig);
      digtrans[i]--;
    }
  }
  dy = 0;
  int flash = millis() % 1000;
  if (!alarm_stat) {// мерегтіння двокрапок в годиннику підвязуємо до личильника циклів
    if ((flash >= 180 && flash < 360) || flash >= 540) { setCol(digPos[4], WIFI_connected ? 0x66 : 0x60); setCol(digPos[4] + 1, WIFI_connected ? 0x66 : 0x60); }
    if (statusUpdateNtpTime) {                                                                                                                                          // якщо останнє оновленя часу було вдалим, то двокрапки в годиннику будуть анімовані
      if (flash >= 0 && flash < 180)                   { setCol(digPos[4], WIFI_connected ? 0x24 : 0x20); setCol(digPos[4] + 1, WIFI_connected ? 0x42 : 0x40); }
      if (flash >= 360 && flash < 540)                 { setCol(digPos[4], WIFI_connected ? 0x42 : 0x40); setCol(digPos[4] + 1, WIFI_connected ? 0x24 : 0x20); }
    }
    if (updateForecast && WIFI_connected) setCol(00, flash < 500 ? 0x80 : 0x00);
    if (updateForecasttomorrow && WIFI_connected) setCol((NUM_MAX * 8 - 1), flash < 500 ? 0x80 : 0x00);
  } else {    setCol(digPos[4], 0x66);    setCol(digPos[4] + 1, 0x66);  }
  refreshAll();
}
//=======================================поидееи анимация попытки подключения к файфай==========
void showAnimWifi(byte probaWifi) {
  byte digPos[2] = {18, 25};
  int digHt = 16;  int num = 2;  int ii;
  if (del == 0) 
  { del = digHt;  for (ii = 0; ii < num; ii++) digold[ii] = dig[ii];
    dig[0] = probaWifi / 10;   dig[1] = probaWifi % 10;
    for (ii = 0; ii < num; ii++)  digtrans[ii] = (dig[ii] == digold[ii]) ? 0 : digHt;
  } else del--;
  clr();
  for (ii = 0; ii < num; ii++) 
  {   if (digtrans[ii] == 0) { dy = 0; showDigit(dig[ii], digPos[ii], dig6x8);  } 
      else {  dy = digHt - digtrans[ii]; showDigit(digold[ii], digPos[ii], dig6x8);   dy = - digtrans[ii];      showDigit(dig[ii], digPos[ii], dig6x8);     digtrans[ii]--;    }
  }
  dy = 0;
  refreshAll();
}



//==========КОНВЕРТАЦІЯ СИМВОЛІВ В РАЗІ ВИКОРИСТАННЯ УКРАЇНСЬКИХ ЛІТЕР==================
byte dualChar = 0;
unsigned char convert_UA_RU_PL_DE(unsigned char _c) {
  unsigned char c = _c;
  // конвертирование латиницы
  if (c==208) { dualChar=1; return 0;}  else if (c==209 || c==210) {dualChar=2;    return 0;  }
  if (c== 32 && dualChar != 3) {                                    dualChar=3;    return c;  }
  if (dualChar == 1) { if (c>=144 && c<192) {c += 48; }             dualChar=0;    return c;  }
  if (dualChar == 2) { if (c>=128 && c<144) {c += 112;}  switch (_c) { case 144: c=133; break; case 145: c=149; break; }    dualChar=0;    return c;  }
  // конвертирование польского и немецкого
  if (c == 195) {    dualChar=4;    return 0;  }
  if (c == 196) {    dualChar=5;    return 0;  }
  if (c == 197) {    dualChar=6;    return 0;  }
  if (dualChar==4) {switch (_c) { case 132: c=177; break; case 147: c=166; break; case 150: c=179; break; case 156: c = 181; break; case 159: c=183; break; case 164: c=178; break; case 179: c=167; break; case 182: c=180; break; case 188: c=182; break; }  dualChar=0;  return c;  }
  if (dualChar==5) {if(c>=132 && c<136) {c +=26; }   switch (_c) {  case 152: c = 168; break;  case 153: c = 169; break;  }     dualChar = 0;    return c;  }
  if (dualChar==6) {if(c>=129 && c<133) {c +=33; } if (c>=154 && c<156) {c +=16; } if (c>=185 && c<189) {c -=13;}    dualChar=0;    return c;  }
  //return 0; //добавил чтоб убрать ошибку
 }
/*
//==========
void saveChrMas(String string_t, byte lenght_off, byte number_s) {
  byte lenght = string_t.length();   if (lenght > lenght_off) return;
  const char *s = string_t.c_str();
  for (int i = 0; i < lenght; i++) {        s++;  }  //snprintf(*memory_date_mes[number_s], 1, "%s", *s);
}
*/
