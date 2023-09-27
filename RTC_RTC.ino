//RTC -вроде писваиваеться значения только когда обовляеться врмя из интрнета, можеа убарь присваивания значениея при чтения с компа искать RRTTCC
//  updateTime идет всевремя в цикле
//==========ОНОВЛЕННЯ ЛОКАЛЬНОГО ЧАСУ (ЛОКАЛЬНІ ЧАСИ)===============================================================
void updateTime() {
  long curEpoch = localEpoc + ((millis() - localMillisAtUpdate) / 1000);
  long epoch = round(curEpoch + 86400L);
  epoch = (epoch % 86400L);
  hour = ((epoch % 86400L) / 3600) % 24;
  minute = (epoch % 3600) / 60;
  second = epoch % 60;
}
//==========ОНОВЛЕННЯ МЕРЕЖЕВОГО ЧАСУ (перевірка в три проходи)===nntp=================================================
void timeUpdateNTP() {
  if (!WIFI_connected) return;
 // if (rtcStat&& day_rtc==day)   { if (printCom) {Serial.println("RTC yes, Time no internet"); return;}} //при чтении с комп не сехронизируеться день недели   //day_rtc==day когда день rtc равен дню часов тогда не читаем с интернета
 if (rtcStat)   { if (printCom) {Serial.println("RTC yes, Time no internet"); return;}} 
  if (printCom) printTime();
  statusUpdateNtpTime = 1;
  for (int timeTest = 0; timeTest < 3; timeTest++) 
  {
    getNTPtime(); if (printCom) {if (timeTest) Serial.print("          "); Serial.println("Proba #" + String(timeTest + 1) + "   " + String(g_hour) + ":" + ((g_minute < 10) ? "0" : "") + String(g_minute) + ":" + ((g_second < 10) ? "0" : "") + String(g_second));    }
   
    // updateTime();
    hourTest[timeTest] = g_hour;
    minuteTest[timeTest] = (g_minute || (g_minute == 59 ? 0 : g_minute++));
  
    if (statusUpdateNtpTime == 0) {  /////mp3_play_file_in_fol (11,6);    delay(500);  
    if (printCom) {        printTime();        Serial.print("ERROR TIME!!1!\r\n");    }      return;    }
    if (timeTest > 0) 
    {
       if ((hourTest[timeTest] != hourTest[timeTest - 1] || minuteTest[timeTest] != minuteTest[timeTest - 1])) 
       { if (printCom) {  printTime(); Serial.print("ERROR TIME!2!!\r\n"); }
         /////mp3_play_file_in_fol (11,6);
         delay(500);     
         statusUpdateNtpTime = 0;    return;
       }
    }
  }
  hour=g_hour;minute=g_minute;second=g_second;day=g_day;dayOfWeek=g_dayOfWeek;month=g_month;year=g_year;
   //Serial.println("плучаем время из интернета");
   //hour=10;minute=59;second=g_second;day=g_day;dayOfWeek=g_dayOfWeek;month=g_month;year=g_year;
  if (rtcStat) { hour_rtc=hour;minute_rtc=minute;second_rtc= second;month_rtc=month;day_rtc=day;dayOfWeek_rtc=dayOfWeek;year_rtc=year;setRTCDateTime();if (printCom) {Serial.println("Time save RTC OK.");  }} //Устанавливанм время в РТС если включено использовать
  localMillisAtUpdate = millis();
  localEpoc = (hour * 60 * 60 + minute * 60 + second);
  saveTime();
  /////mp3_play_file_in_fol (11,11);delay(200);
  if (printCom) {printTime();Serial.println((day < 10 ? "0" : "") + String(day) + "." + (month < 10 ? "0" : "") + String(month) + "." + String(year) + " DW = " + String(dayOfWeek));    Serial.println("          Time update OK.");  }
  
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void printTime() {  if (printCom) Serial.print((hour < 10 ? "0" : "") + String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second) + "  ");}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte alarms() { //dav
  for (byte i = 0; i < 5; i++) 
  {   if (alarme[i][0]==hour && alarme[i][1]==minute && (alarme[i][2]==dayOfWeek || (alarme[i][2]==8 && (dayOfWeek>1 && dayOfWeek<7)) || (alarme[i][2]==9 && dayOfWeek>1) || (alarme[i][2]==10 && (dayOfWeek==1 || dayOfWeek==7)) || alarme[i][2]>10)) 
      {  alarm_numer=i;  if (printCom){ Serial.println( String(alarm_numer)+" "+String(alarm_hold)+" "+String (alarm_stat));}      return 1;   }
  }
  alarm_numer = 255;  return 0;
}
//==========ОТРИМАННЯ ДАТИ ТА ЧАСУ ВІД СЕРВЕРА ТОЧНОГО ЧАСУ =============================================================
void getNTPtime() {
   
  WiFi.hostByName(ntpServerName.c_str(), timeServerIP);   //определяет по именм айпи в timeServerIP может возвращать ошибку
  int cb;
  for (int i = 0; i < 3; i++) {  /////mp3_play_file_in_fol (11,8);
  delay(200);
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;    packetBuffer[1] = 0;    packetBuffer[2] = 6;    packetBuffer[3] = 0xEC;    packetBuffer[12] = 49;    packetBuffer[13] = 0x4E;    packetBuffer[14] = 49;    packetBuffer[15] = 52;
    udp.beginPacket(timeServerIP, 123);                     //NTP порт 123
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();
    delay(800);                                             // чекаємо пів секуни
    cb = udp.parsePacket();
    if (!cb && printCom) Serial.println("          no packet yet..." + String (i + 1));
    if (!cb && i == 2) {   statusUpdateNtpTime = 0;  udp.begin(localPort2);    return; }    // якщо час не отримано       // вихіз з getNTPtime()
    if (cb) i = 3;
  }
  if (cb) {                                                  // якщо отримали пакет з серверу
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;        // Unix час станом на 1 січня 1970. в секундах, то 2208988800:
    unsigned long epoch = secsSince1900 - seventyYears;
    epochNM = epoch - (millis() / 1000);
    boolean summerTime;
    if (month < 3 || month > 10) summerTime = false;            // не переходимо на літній час в січні, лютому, листопаді і грудню
    if (month > 3 && month < 10) summerTime = true;             // Sommerzeit лічимо в квіні, травні, червені, липні, серпені, вересені
    if (month == 3 && (hour + 24 * day) >= (3 + 24 * (31 - (5 * year / 4 + 4) % 7)) || month == 10 && (hour + 24 * day) < (3 + 24 * (31 - (5 * year / 4 + 1) % 7))) summerTime = true;
    epoch += (int)(timeZone * 3600 + (3600 * (isDayLightSaving && summerTime)));
    hourCorr = timeZone + (isDayLightSaving && summerTime);
    g_year = 0;
    int days = 0;
    uint32_t time;
    time = epoch / 86400;
    g_hour = (epoch % 86400L) / 3600;
    g_minute = (epoch % 3600) / 60;
    g_second = epoch % 60;
    g_dayOfWeek = (((time) + 4) % 7) + 1;
    while ((unsigned)(days += (LEAP_YEAR(g_year) ? 366 : 365)) <= time) {
      g_year++;
    }
    days -= LEAP_YEAR(g_year) ? 366 : 365;
    time -= days;
    days = 0;
    g_month = 0;
    uint8_t monthLength = 0;
    for (g_month = 0; g_month < 12; g_month++) {
      if (g_month == 1) {
        if (LEAP_YEAR(g_year)) monthLength = 29;
        else monthLength = 28;
      }
      else monthLength = monthDays[g_month];
      if (time >= monthLength) time -= monthLength;
      else break;
    }
    g_month++;
    g_day = time + 1;
    g_year += 1970;
    udp.begin(localPort2);
    return;
  }
  udp.begin(localPort2);  if (printCom) Serial.println("Нет времени(((");
}
//===============================================================================================================================//
