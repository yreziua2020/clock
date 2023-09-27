void setup() {
// randomSeed(analogRead(0)); Не понятно для чего
uint32_t seed = 0;
for (int i = 0; i < 16; i++) {
  seed *= 4;
  seed += analogRead(A0) & 3;
  randomSeed(seed); //устанавливаю какоето случайное чмсло для функции random проверял очень разные числа получаются 1414862165 290805013
}

  Wire.begin();
  
    // if (!MPR121.begin(0x5A)) { if (printCom){   Serial.println("error setting up MPR121");}     }
   Serial.begin(9600);  //ддля плеера 
   //mp3_com.begin(9600); 
   ////mp3_set_serial (mySerial); 

   #ifdef _ZVUK
   //mp3_set_serial (mp3_com);
    //mp3_set_serial (Serial);
   #endif 
  
    
  if (printCom) Serial.println("");
  pinMode(16,INPUT); 
  //digitalWrite(BUT_PIN, !butStat);  delay(500);
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN, 1);  sendCmdAll(CMD_INTENSITY, 1);
  SPIFFS.begin();
    
  //время  //rtcStat - флаг определяет включен ли модул временр
  loadConfig();  
  //loadTime();    //читаем время из файла// записываеться когда было удачное обновление из зинтрнета
  lang();   //языки

  //если работа смодулем разрешена то считываем данные смодуля
             // ищем модуль часовой ,на ходит модуль без кварца по адресу 68 , есили не искть то при rtcStat=1 будет виснуть на чтение данных из модуля
          Wire.beginTransmission(0x67); errorRTC = Wire.endTransmission();  if (errorRTC == 0){ rtcAddr = 0x67;  if (printCom) Serial.println("YES!!! find RTC module addr: 0x67!"); } 
  else  { Wire.beginTransmission(0x68); errorRTC = Wire.endTransmission();  if (errorRTC == 0){ rtcAddr = 0x68;  if (printCom) Serial.println("YES!!! find RTC module addr: 0x68!"); } else rtcStat = false;  }
  //при старте обновляем время
  if (rtcStat) { if (printCom)  Serial.println("RTC START");  getRTCDateTime(); hour=hour_rtc;minute=minute_rtc;second=second_rtc; day=day_rtc; month=month_rtc; year=year_rtc; dayOfWeek=dayOfWeek_rtc; if (printCom) {Serial.println("RTC update: "+String(hour)+":"+String(minute)+":"+String(second)+"    "+String(day)+"."+String(month)+"."+String(year)+" D=" + String(dayOfWeek)); }    } else if (printCom) Serial.println("RTC module off!");  // ------------------
  
  sensorsDht();
  if (bmp.begin()) {    if (printCom) Serial.println("YES!!! find BMP280 sensor!");  bmp280 = true;  sensorsBmp(); } else if (printCom) Serial.println("Did not find BMP280 sensor!");
 // if (bmp180.begin()) { if (printCom) Serial.println("YES!!! find BMP180 sensor!");  BMP180 = true;  sensorsBmp(); } else if (printCom) Serial.println("Did not find BMP180 sensor!");
  bme.parameter.communication = 0;                            //Подключение сенсора по I2C
  bme.parameter.I2CAddress = 0x76;                            //I2C Адрес сенсора или 0x77
  bme.parameter.sensorMode = 0b11;                            //0b00 спящий режим измерения не выполняются, 0b01: выполняется одно измерение, 0b11: датчик постоянно измеряет
  bme.parameter.IIRfilter = 0b100;                            //высокое значение коэффициента означает меньше шума, но измерения также менее чувствительны 0b000-0(off), 0b001-2, 0b010-4, 0b011-8, 0b100-16 (default value)
  bme.parameter.humidOversampling = 0b100;                    //коэффициент избыточной дискретизации для измерений влажности 0b000-0 (фильтр откл), 0b001-1, 0b010-2, 0b011-4, 0b100-8, 0b101-16 (максимум)
  bme.parameter.tempOversampling = 0b101;                     //коэффициент передискретизации для измерения температуры 0b000-0 (фильтр откл), 0b001-1, 0b010-2, 0b011-4, 0b100-8, 0b101-16 (максимум)
  bme.parameter.pressOversampling = 0b101;                    //коэффициент избыточной дискретизации для измерений давления. Для измерений высоты более высокий коэффициент обеспечивает более стабильные значения
  bme.parameter.pressureSeaLevel = 1013.25;                   //текущее давление, скорректированное для уровня моря 1013.25 hPa. Для точных измерений высоты
  bme.parameter.tempOutsideCelsius = 15;                      //средняя температура снаружи 15°C
  bme.parameter.tempOutsideFahrenheit = 59;                   //средняя температура снаружи 59°F
  if (bme.init() == 0x60) {   if (printCom) Serial.println("YES!!! find BME280 sensor!");   bme280 = true;    sensorsBme();  }   else if (printCom) Serial.println("Did not find BME280 sensor!");  
 // if (sensor.begin()) {       if (printCom) Serial.println("YES!!! find Si7021 sensor!");   si7021 = true;    sensorsSi7021(); } else if (printCom) Serial.println("Did not find Si7021 sensor!");
  sensorsAll(); 
  server_init();
  // ----------
  localMillisAtUpdate = millis(); localEpoc = (hour * 60 * 60 + minute * 60 + second);
  udp.begin(localPort);
  wifiConnect();// ---------- Підключення до WiFi
   
   if (updateOTA) {                                         // ***********  OTA SETUP
    ArduinoOTA.setPort(8266);    ArduinoOTA.setHostname("ESP-ZAL_clok");    ArduinoOTA.onEnd([]() {   bip_restart_up();   ESP.restart();    });
    ArduinoOTA.onError([](ota_error_t error) {if (printCom) {Serial.printf("Error[%u]: ", error); if (error == OTA_AUTH_ERROR && printCom) Serial.println("Auth Failed"); else if (error == OTA_BEGIN_ERROR && printCom) Serial.println("Begin Failed"); else if (error == OTA_CONNECT_ERROR && printCom) Serial.println("Connect Failed");      else if (error == OTA_RECEIVE_ERROR && printCom) Serial.println("Receive Failed");      else if (error == OTA_END_ERROR && printCom) Serial.println("End Failed"); }     ESP.restart();    });
    ArduinoOTA.begin();
  }
  if (WiFi.status() == WL_CONNECTED) {   if (displayForecast) {      if (!weatherHost) { getWeatherData0(); getWeatherDataz0(); } else {   getWeatherData1(); getWeatherDataz1();  }    }   }  //weatherHost -определяет с какого веб сайта отображать погоду
  
  
   // lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE_2);  //Serial.println(F("BH1750 Test"));
  //MPR121_inic();  //иницилизация MPR121
 
   pressure2 = aver_sens();          // найти текущее давление по среднему арифметическому
  for (byte i = 0; i < 6; i++) {   pressure_array[i] = pressure2;  time_array[i] = i; }            // забить массив времени числами 0 - 5 // забить весь массив текущим давлением
 
  #ifdef _ZVUK
   command2(Volu,0,gromk);
   //vver();
   #endif 
 
  //timeUpdateNTP();
//kurs_s= "USD buy=";
 // delay(1000);
  //  kurs();
  Serial.println("Start 13");
  randomSeed(analogRead(brightPin));
   Serial.println("analogRead="); Serial.println(analogRead(brightPin));
}
