
//  dav                            БЕРЕМО ПОГОДУ З САЙТУ  https://www.weatherbit.io                                                 //
//===============================================================================================================================//
void getWeatherData0() {
  location_name = "";  location_region = "";  location_country = "";  location_localtime = "";  location_temp = 0;  location_app_temp = 0;  location_rh = 0;  location_pres = 0;  location_wind_spd = 0;  location_wind_cdir_full = "";  location_sunrise = "";  location_sunset = "";  location_clouds = 0;  location_vis = 0;  location_uv = 0;  location_weather_description = "";
  if (!WIFI_connected) {    updateForecast++;    if (updateForecast >= 360) weatherString = tWeatrNot;    return;  }   if (printCom) {    printTime();    Serial.println("Getting weather forecast for City " +  cityID0 + "...");  }  //печать в каом порт если разрешена
 
  //****//
  
   WiFi.hostByName(weatherHost0.c_str(), pogodaIP);
  // if(WiFi.hostByName(weatherHost0.c_str(), pogodaIP)){printStringWithShift("ok ", 25);} else  {printStringWithShift("no ", 25);} 
   //Serial.print("pogodaIP ="); Serial.println(pogodaIP); 
  //if (ESPclient.connect(weatherHost0.c_str(), 80)) {
  if (ESPclient.connect(pogodaIP, 80)) {//Serial.println("conect yas");
  } else {  if (printCom) Serial.println("          Not connection server!!!"); updateForecast++;  if (updateForecast >= 360) weatherString = tWeatrNot;   return;  }

  HTTPClient http;
  String line = "";  String reqline = "http://" + weatherHost0 + "/v2.0/current/daily?city=" + urlencode(cityID0) + "&lang=" + weatherLang + "&key=" + weatherKey0;
  if (printCom) {    Serial.println("=======================================================");    Serial.println(reqline);    Serial.println("=======================================================");  }
  if (http.begin(ESPclient, reqline)) 
  { // HTTP
    int httpCode = http.GET();
    if (httpCode > 0) 
    {  //printStringWithShift("Yes2 ", 25); 
      if (printCom) {Serial.printf("[HTTP] GET... code: %d\n", httpCode);}  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {  line = http.getString();  }
    } else
    {  //printStringWithShift("No2 ", 25); 
       if (printCom){Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());}    
    }
    http.end();
    //printStringWithShift("Yes ", 25); 
  } 
  else   
  { 
      if (printCom){ Serial.printf("[HTTP} Unable to connect\n");}  
      //printStringWithShift("No ", 25); 
  }
  
  if (printCom) { Serial.print("line ="); Serial.println(line);  }
  
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(37) + 1128; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if (!doc.capacity()) {  printStringWithShift("No123 ", 25);  if (printCom) Serial.println("          Parse weather forecast - FAILED!!!"); updateForecast++; if (updateForecast >= 360) weatherString = tWeatrNot;  return;  }
  JsonObject data = doc["data"][0];
  location_rh = data["rh"];                       //Влажность   
  location_pres = data["pres"];                    //давление 999.3
  //String wr_pog= "Давление"+String(location_pres);  printStringWithShift(wr_pog.c_str(), 25);
  //delay(1000);
  //String wr_pog2=String(weatherKey0);   printStringWithShift(wr_pog2.c_str(), 25);  
  
  if (location_pres==0) 
  {
    String wr_pog= "Давление"+String(location_pres);  printStringWithShift(wr_pog.c_str(), 25);
    String wr_pog2=String(weatherKey0);   printStringWithShift(wr_pog2.c_str(), 25);  
    delay(1000);
  }
  /////if (pressSys == 1) location_pres /= 1.3332239; //если флаг установлен то делим давление чтобы разных измирениях выводить
  location_pres /= 1.3332239; //временно чтобы при преключении не мнялся вывод
  const char* data_timezone = data["timezone"]; // "Europe/Kiev"
  location_region = data_timezone;                        //регион
  const char* data_ob_time = data["ob_time"];              // "2019-09-19 17:57"
  location_localtime = data_ob_time;                       //время
  const char* data_country_code = data["country_code"];    // "UA"
  location_country = data_country_code;                  //код страны "UA"
  location_clouds = data["clouds"];                     //процент облачности 
  const char*   data_city_name = data["city_name"]; // "Kiev"
  location_name = data_city_name;                //Название города
  if (location_name == "Frankfurt am Main") location_name = "Франкфурт на Майні";
  location_wind_spd = data["wind_spd"];                      // 1  //скорось ветра
  const char* data_wind_cdir_full = data["wind_cdir_full"]; // "пі́вдень-пі́вдень-схід"
  location_wind_cdir_full = data_wind_cdir_full;  //направление ветра
  location_vis = data["vis"];                   // 5    Видимость
  const char*   data_sunset = data["sunset"]; // "16:01"
  location_sunset = data_sunset;                //время Заката
  location_uv = data["uv"];                        // какойто индекм UV
  int           data_wind_dir = data["wind_dir"]; // 166
  const char*   data_sunrise = data["sunrise"]; // "03:39"
  location_sunrise = data_sunrise;   //int         data_dni = data["dni"]; // Расвет
 // int    data_dni = data["dni"];
  JsonObject    data_weather = data["weather"];
  const char*   data_weather_description = data_weather["description"]; // "ясного неба"
  location_weather_description = data_weather_description;           //п
  location_temp = data["temp"];                                     // 10.6 //текущая температура
  location_app_temp = data["app_temp"];                             // 10.6 //температура ощущаеться

  String windDegString;
  if (data_wind_dir >= 345 || data_wind_dir <= 22)  windDegString = "\211 Северный "; // "\211 Северный ";           //"Північний";
  if (data_wind_dir >= 23  && data_wind_dir <= 68)  windDegString = "\234 Северо-Восточный  "; //"\234 Северо-Восточный  ";   //"Північно-східний";
  if (data_wind_dir >= 69  && data_wind_dir <= 114) windDegString = "\230 Восточный "; //"\230 Восточный ";             //"Східний";
  if (data_wind_dir >= 115 && data_wind_dir <= 160) windDegString = "\235 Южно-Восточный "; //"\235 Южно-Восточный ";   //"Південно-східний";
  if (data_wind_dir >= 161 && data_wind_dir <= 206) windDegString = "\210 Южный "; // "\210 Южный ";//           //"Південний";
  if (data_wind_dir >= 207 && data_wind_dir <= 252) windDegString = "\232 Южно-Западный  "; //"\232 Южно-Западный";   //"Південно-західний";
  if (data_wind_dir >= 253 && data_wind_dir <= 298) windDegString = "\231 Западный "; //"\231 Западный ";             //"Західний";
  if (data_wind_dir >= 299 && data_wind_dir <= 344) windDegString = "\233 Северо-Западный "; //"\233 Северо-Западный ";   //"Північно-західний";
  weatherString = "        ";
  if (displayCityName) {    String PCN = personalCityName;    if (PCN.length() > 0) weatherString += PCN;    else weatherString += String(location_name);    weatherString += ", ";  }  //вывод названия города 
  //  location_sunset +" "  + " "+  location_sunrise+" " +
   if (displayForecastNow) {  weatherString += "Сейчас:  " + String(location_temp, 1) + ("\202") + "C";    //------------------Сейчас---------------------ПППППППППППППППППППППППП---
   if (location_temp!=location_app_temp) { weatherString +=" ("+ String(location_app_temp,1)+ ("\202") + "C)";}      //если температура текущая и как ущучаеться одинаковые то вторую не выводим
    //weatherString += "Сейчас:  " + String(location_temp, 1) + ("\202") + "C ("+ String(location_app_temp,1)+ ("\202") + "C)";  //------------------Сейчас---------------------ПППППППППППППППППППППППП---
    weatherString += "  \213 " + String(location_rh) + "%";
    //weatherString += "  \215 " + String((location_pres), 0) + (pressSys == 1 ? tPress : tPress0);  ///// 215 Давление
    weatherString += "  \215 " + String((location_pres), 0) +  tPress ;  ///// 215 Давление 
    weatherString += "  \214 " + windDegString +" "+ String(location_wind_spd, 1) + tSpeed;
    weatherString += "   " + String (data_weather_description)+" "  + String(location_clouds) + "%  " +   "           ";
  }
  updateForecast = 0;
}
//===============================================================================================================================//
//                              БЕРЕМО ПОГОДУ З САЙТУ  openweathermap.org                                                     //
//===============================================================================================================================//
void getWeatherData1() {
  location_name = ""; location_region = ""; location_country = ""; location_localtime = "";       location_temp = 0;       location_app_temp = 0;  location_rh = 0;    location_pres = 0; 
  location_wind_spd = 0;  location_wind_cdir_full = "";  location_sunrise = "";  location_sunset = "";  location_clouds = 0;  location_vis = 0;  location_uv = 0;  location_weather_description = "";  
  if (!WIFI_connected) {    updateForecast++;    if (updateForecast >= 360) weatherString = tWeatrNot;    return;  }  if (printCom) {    printTime();  Serial.println("Getting weather forecast for City " +  cityID1 + "...");  }
  if (ESPclient.connect(weatherHost1.c_str(), 80))  {  } else {  if (printCom) Serial.println("          Not connection server!!!"); updateForecast++;  if (updateForecast >= 360) weatherString = tWeatrNot;  return;  }
 
  HTTPClient http;
  String line = "";  String reqline = "http://" + weatherHost1 + "/data/2.5/weather?id=" + urlencode(cityID1) + "&lang=" + weatherLang + "&units=metric&appid=" + weatherKey1;
  if (printCom) {    Serial.println("=======================================================");    Serial.println(reqline);    Serial.println("=======================================================");  }
  if (http.begin(ESPclient, reqline)) { // HTTP

    int httpCode = http.GET();
    if (httpCode > 0) {
      if (printCom) {Serial.printf("[HTTP] GET... code: %d\n", httpCode);}
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {        line = http.getString();      }
    } else { if (printCom){Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());}  }
    http.end();
  } else {   if (printCom){Serial.printf("[HTTP} Unable to connect\n");}  }
  
  if (printCom) {    Serial.print("line =");    Serial.println(line);  }
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 2 * JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(13) + 751; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if (!doc.capacity()) {
    if (printCom) Serial.println("          Parse weather forecast - FAILED!!!");
    updateForecast++;
    if (updateForecast >= 360) weatherString = tWeatrNot;
    return;
  }
  JsonObject weather_0 = doc["weather"][0];
  const char*   data_weather_description = weather_0["description"]; // "fog"
  location_weather_description = data_weather_description;
  JsonObject main = doc["main"];
  location_temp = main["temp"]; // 10.34
  location_pres = main["pressure"]; // 1023
  if (pressSys == 1) location_pres /= 1.3332239;
  location_rh = main["humidity"]; // 100
  float location_temp_min = main["temp_min"]; // 7
  float location_temp_max = main["temp_max"]; // 12.22
  location_vis = doc["visibility"]; // 1000
  location_vis /= 1000;
  location_wind_spd = doc["wind"]["speed"]; // 1
  int data_wind_dir = doc["wind"]["deg"]; // 230
  location_clouds = doc["clouds"]["all"]; // 20
  JsonObject sys = doc["sys"];
  const char* data_country_code = sys["country"]; // "UA"
  location_country = data_country_code;
  const char*   data_city_name = doc["name"]; // "Kyiv"
  location_name = data_city_name;
  if (location_name == "Frankfurt am Main") location_name = "Франкфурт на Майні";
  String windDegString;
  if (data_wind_dir >= 345 || data_wind_dir <= 22)  windDegString = "\211 Северный ";   //"Північний";
  if (data_wind_dir >= 23  && data_wind_dir <= 68)  windDegString = "\234 Северо-Восточный  ";   //"Північно-східний";
  if (data_wind_dir >= 69  && data_wind_dir <= 114) windDegString = "\230 Восточный ";   //"Східний";
  if (data_wind_dir >= 115 && data_wind_dir <= 160) windDegString = "\235 Южно-Восточный ";   //"Південно-східний";
  if (data_wind_dir >= 161 && data_wind_dir <= 206) windDegString = "\210 Южный ";   //"Південний";
  if (data_wind_dir >= 207 && data_wind_dir <= 252) windDegString = "\232 Южно-Западный ";   //"Південно-західний";
  if (data_wind_dir >= 253 && data_wind_dir <= 298) windDegString = "\231 Западный ";   //"Західний";
  if (data_wind_dir >= 299 && data_wind_dir <= 344) windDegString = "\233 Северо-Западный ";   //"Північно-західний";
  weatherString = "        ";
  if (displayCityName) {  String PCN = personalCityName; if (PCN.length() > 0) weatherString += PCN; else weatherString += String(location_name); weatherString += ", ";  }
  if (displayForecastNow) {
    weatherString += "Сейчас:** " + String(location_temp, 1) + " (" + String(location_temp_min, 1) + "..." + String(location_temp_max, 1) + ")" + ("\202") + "C";  //------------------------------------------------------
    weatherString += "     \213 " + String(location_rh) + "%";
    weatherString += "     \215 " + String((location_pres), 0) + (pressSys == 1 ? tPress : tPress0) ;
    weatherString += "     \214 " + windDegString + String(location_wind_spd, 1) + tSpeed;
    weatherString += "     \216 " + String(location_clouds) + "%     " + data_weather_description + "                ";
  }
  updateForecast = 0;
}
// ============================================================================//
//               Беремо ПРОГНОЗ!!! погоди з сайту https://www.weatherbit.io     //
// ============================================================================//
void getWeatherDataz0() {
  if (!WIFI_connected) {    updateForecasttomorrow++;    if (updateForecast >= 360) weatherStringZ = "";    return; }
  //if(printCom) printTime();
  //if(printCom) Serial.println("Getting weather forecast for tomorrow...");
  HTTPClient http;
  String line = "";
  String reqline = "http://" + weatherHost0 + "/v2.0/forecast/daily?city=" + urlencode(cityID0) + "&lang=" + weatherLang + "&days=2&key=" + weatherKey0;
  if (printCom) {    Serial.println("=======================================================");    Serial.println(reqline);    Serial.println("=======================================================");  }
  if (http.begin(ESPclient, reqline)) { // HTTP

    int httpCode = http.GET();
    if (httpCode > 0) {
      if (printCom){Serial.printf("[HTTP] GET... code: %d\n", httpCode);}
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {        line = http.getString();      }
    } else {
      if (printCom){Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());}
    }
    http.end();
  } else { if (printCom){   Serial.printf("[HTTP} Unable to connect\n");  }}
  if (printCom) {Serial.print("line2=");  Serial.println(line); }
//daff
  const size_t capacity = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(7) + 2 * JSON_OBJECT_SIZE(37) + 2321;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if (!doc.capacity()) {
   
    if (printCom) Serial.println("          Parse weather forecast for tomorrow - FAILED!!!");
    updateForecasttomorrow++;
    if (updateForecast >= 360) weatherStringZ = "";
    return;
  }
  JsonObject data_0 = doc["data"][0];
  JsonObject data_0_weather = data_0["weather"];
  const char* data_0_weather_description = data_0_weather["description"]; // "Помірний дощ"
  float data_0_max_temp = data_0["max_temp"]; // 13.4
  float data_0_min_temp = data_0["min_temp"]; // 10.9
  JsonObject data_1 = doc["data"][1];
  int data_1_rh = data_1["rh"]; // 75
  int data_1_clouds = data_1["clouds"]; // 58
  float data_1_wind_spd = data_1["wind_spd"]; // 3.75302
  JsonObject data_1_weather = data_1["weather"];
  const char* data_1_weather_description = data_1_weather["description"]; // "Світло душ дощ"
  float data_1_max_temp = data_1["max_temp"]; // 16.3
  float data_1_min_temp = data_1["min_temp"]; // 10
  
    weatherStringZ="  Сегодня " ;
    if (dayOfWeek==2) weatherStringZ+="Понедельник"; if(dayOfWeek==3) weatherStringZ+="Вторник"; if(dayOfWeek==4) weatherStringZ+="Среда"; if(dayOfWeek==5) weatherStringZ+="Четверг"; if(dayOfWeek==6) weatherStringZ+="Пятница"; if(dayOfWeek==7) weatherStringZ+="Суббота"; if(dayOfWeek==1) weatherStringZ+="Воскресенье"; 
    weatherStringZ+= " "+String (day)+" ";
    if(month==1) weatherStringZ+="Января"; if(month==2) weatherStringZ+="Февраля"; if(month==3) weatherStringZ+="Марта"; if(month==4) weatherStringZ+="Апреля"; if(month==5) weatherStringZ+="Мая"; if(month==6) weatherStringZ+="Июня"; if(month==7) weatherStringZ+="Июля"; if(month==8) weatherStringZ+="Августа"; if(month==9) weatherStringZ+="Сентября"; if(month==10) weatherStringZ+="Октября"; if(month==11) weatherStringZ+="Ноября"; if(month==12) weatherStringZ+="Декабря";

  if (hour < 18) weatherStringZ += ":";   //выводим просто : если еще нет 18//сегодня ----------------------------------------------------------------------------------------------------------------------------------------------------
  if (hour < 12) weatherStringZ += "   " + String(data_0_min_temp, 1) + "...." + String(data_0_max_temp, 1) + "\202" + "C  "; //Если мень 12 то выдаем прогноз на сегодня
  if (hour < 18) weatherStringZ += "   " + String(data_0_weather_description); //+ "       ";  //после 12 и до 18 выводим сегодня только облочность, а прогноз не выводим уже, а после 18 выводим только сегоднишнюю  день недели дату и месяц

  int sr = location_sunrise.substring(0, 2).toInt() + 3;  if(sr>23) sr -= 24;  if(sr<0) sr += 24;  String sunrise = String(sr) + location_sunrise.substring(2, 5);
  int ss =  location_sunset.substring(0, 2).toInt() + 3;  if(ss>23) ss -= 24;  if(ss<0) ss += 24;  String sunset =  String(ss) +  location_sunset.substring(2, 5); 
  //Serial.print(String (hourCorr));
  weatherStringZ +="  Расвет: " +sunrise+"  Закат: "+sunset;  weatherStringZ += "        ";         //чтобы уежала сторока  не стиралась
                                                                                                    //отображать погоду на завтра
   if (displayForecastTomorrow) 
   {weatherStringZav  = "  Завтра:  " + String(data_1_min_temp, 1) + "...." + String(data_1_max_temp, 1) + "\202" + "C";    //202 градусы
    weatherStringZav += "     \213 " + String(data_1_rh) + "%";                 //213 капелька
    weatherStringZav += "     \214 " + String(data_1_wind_spd, 1) + tSpeed;     //214-знак ветра
    weatherStringZav += "     " + String(data_1_weather_description);
    weatherStringZav += "             ";
   }
 
  if (printCom) {Serial.println("         Getting weather forecast for tomorrow - is OK.");}
   
  updateForecasttomorrow = 0;
}
// =======================================================================//
//               Беремо ПРОГНОЗ!!! погоди з сайту openweathermap.org      //
// =======================================================================//
void getWeatherDataz1() {
  if (!WIFI_connected) {    updateForecasttomorrow++;    if (updateForecast >= 360) weatherStringZ = "";    return;  }
  //if(printCom) printTime();
  //if(printCom) Serial.println("Getting weather forecast for tomorrow...");
  HTTPClient http;
  String line = "";
  String reqline = "http://" + weatherHost1 + "/data/2.5/forecast/daily?id=" + urlencode(cityID1) + "&units=metric&appid=" + weatherKey1 + "&lang=" + weatherLang + "&cnt=2";
  if (printCom) {    Serial.println("=======================================================");    Serial.println(reqline);    Serial.println("=======================================================");  }
  if (http.begin(ESPclient, reqline)) { // HTTP

    int httpCode = http.GET();
    if (httpCode > 0) {
      if (printCom){Serial.printf("[HTTP] GET... code: %d\n", httpCode);}
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        line = http.getString();
      }
    } else {      if (printCom){Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());    }}
    http.end();
  } else {
    if (printCom){Serial.printf("[HTTP} Unable to connect\n");}
  }
  if (printCom) {    Serial.print("line2=");    Serial.println(line);  }

  const size_t capacity = 2 * JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 3 * JSON_OBJECT_SIZE(6) + 2 * JSON_OBJECT_SIZE(10) + 1281;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if (!doc.capacity()) {
    if (printCom) Serial.println("          Parse weather forecast for tomorrow - FAILED!!!");
    updateForecasttomorrow++;
    if (updateForecast >= 360) weatherStringZ = "";
    return;
  }

  JsonObject city = doc["city"];
  JsonObject list_0 = doc["list"][0];
  JsonObject list_0_temp = list_0["temp"];
  float data_0_min_temp = list_0_temp["min"]; // 9.63
  float data_0_max_temp = list_0_temp["max"]; // 9.69
  JsonObject list_0_weather_0 = list_0["weather"][0];
  const char* data_0_weather_description = list_0_weather_0["description"]; // "облачно"
  JsonObject list_1 = doc["list"][1];
  JsonObject list_1_temp = list_1["temp"];
  float data_1_day_temp = list_1_temp["day"]; // 18.13
  float data_1_min_temp = list_1_temp["min"]; // 10.67
  float data_1_max_temp = list_1_temp["max"]; // 20.32
  int data_1_rh = list_1["humidity"]; // 56
  JsonObject list_1_weather_0 = list_1["weather"][0];
  const char* data_1_weather_description = list_1_weather_0["description"]; // "слегка облачно"
  float data_1_wind_spd = list_1["speed"]; // 0.7
  int data_1_clouds = list_1["clouds"]; // 38
  weatherStringZ = "";
  if (hour < 18) weatherStringZ += tCurr + ":--";  
  if (hour < 12) weatherStringZ += "   \212" + String(data_0_min_temp, 1) + "...." + String(data_0_max_temp, 1) + "\202" + "C  ";
  if (hour < 18) weatherStringZ += "   " + String(data_0_weather_description) + "     ";
  if (displayForecastTomorrow) {
    weatherStringZ += tTom + ":   \212" + String(data_1_min_temp, 1) + "...." + String(data_1_day_temp, 1) + "...." + String(data_1_max_temp, 1) + "\202" + "C";
    weatherStringZ += "     \213 " + String(data_1_rh) + "%";
    weatherStringZ += "     \214 " + String(data_1_wind_spd, 1) + tSpeed;
    weatherStringZ += "     \216 " + String(data_1_clouds) + "%     "  + String(data_1_weather_description);
    weatherStringZ += "                   ";
  }
  if (printCom) Serial.println("          Getting weather forecast for tomorrow - is OK.");
  updateForecasttomorrow = 0;
}

/*
//=========================================================================================================
//                                  narodmon.ru
void getNarodmon() {
  if (!WIFI_connected)  return;
  if (printCom) {    printTime();    Serial.println("Connection to narodmon.ru");  }
  if (ESPclient.connect("http://narodmon.ru", 80)) { if (printCom) Serial.println("connection failed"); return; }
  if (!sensors_ID0) return;
  String line = "";
  String reqline = "http://narodmon.ru/api/sensorsValues?sensors=";
  if (sensors_ID0) reqline += String(sensors_ID0);
  if (sensors_ID1) reqline += "," + String(sensors_ID1);
  if (sensors_ID2) reqline += "," + String(sensors_ID2);
  reqline += "&uuid=" + uuid + "&api_key=" + api_key;
  if (printCom) {    Serial.println("=======================================================");    Serial.println(reqline);    Serial.println("=======================================================");  }
  HTTPClient http;
  if (http.begin(ESPclient, reqline)) { // HTTP
    int httpCode = http.GET();
    if (httpCode > 0) {
      if (printCom){Serial.printf("[HTTP] GET... code: %d\n", httpCode);}
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        line = http.getString();
      }
    } else {
      if (printCom){Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());}
    }
    http.end();
  } else {
    if (printCom){Serial.printf("[HTTP} Unable to connect\n");}
  }
  if (printCom) {
    Serial.print("answer=");
    Serial.println(line);
  }
  const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(6) + 340; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if (!doc.capacity()) {
    if (printCom) Serial.println("          Parse weather forecast - FAILED!!!");
    return;
  }
  JsonObject sensors_0 = doc["sensors"][0];
  float sensors_0_value = sensors_0["value"]; // 14.2
  long sensors_0_time = sensors_0["time"]; // 1571853360
  JsonObject sensors_1 = doc["sensors"][1];
  float sensors_1_value = sensors_1["value"]; // 14
  long sensors_1_time = sensors_1["time"]; // 1571853000
  JsonObject sensors_2 = doc["sensors"][2];
  float sensors_2_value = sensors_2["value"];
  long sensors_2_time = sensors_2["time"];

  long timestamp = epochNM + (millis() / 1000);
  if (printCom) {    printTime();    Serial.println("sensors_0 = " + String(sensors_0_value, 1) + "'C    sensors_1 = " + String(sensors_1_value, 1) + "'C    sensors_2 = " + String(sensors_2_value, 1) + "'C");    Serial.println("time_0 = " + String(timestamp - sensors_0_time) + "      time_1 = " + String(timestamp - sensors_1_time) + "      time_2 = " + String(timestamp - sensors_2_time));  }
  tempNM = 0;
  if (sensors_ID0) {
    if ((timestamp - sensors_0_time) > 3600) {
      sensors_0_value = 99;
    } else tempNM = sensors_0_value;
  }
  if (sensors_ID1) {
    if ((timestamp - sensors_1_time) > 3600) {
      sensors_1_value = 99;
    } else if (tempNM > sensors_1_value) tempNM = sensors_1_value;
  }
  if (sensors_ID2) {
    if ((timestamp - sensors_2_time) > 3600) {
      sensors_2_value = 99;
    } else if (tempNM > sensors_2_value) tempNM = sensors_2_value;
  }
  if (!tempNM && !updateForecast) tempNM = location_temp;
  if (printCom){Serial.println("tempNM = " + String(tempNM, 1) + "'C");}
}

*/
