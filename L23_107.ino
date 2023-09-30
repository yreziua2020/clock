//SDK ниже 3 вывбрать в менежере плат иначе не компелируеться потом надо разобраться почему
const int  _Dkurs=0;  ///выводиться информация в ком порт //////////////////////////////////////////////////////////////////////////////////////
const short UserID=3;
  //подчеркивание означает тест сдесь не используеться шина i2s
//из 27 но вроде все есть
//почемуто компилируеться только на второй верси  в менеджере плат библиотеке 8266 
//09102022  залил в часы
#include "Arduino.h" ///когда выдавала ошибку error 'uint8_t' does not name a type поставил выше чем  MPR121
//#include <MPR121.h>
//#include <BH1750.h>
#include <WiFiClient.h>
#include <Wire.h>

#include <IRremoteESP8266.h>
#include <IRsend.h>
int16_t ir_flag1,ir_flag2; // переменаю для работы с пультом
uint16_t rawData_sleep[71] = {9050, 4438,  634, 1616,  632, 488,  632, 488,  632, 488,  628, 492,  628, 492,  628, 488,  634, 488,  628, 492,  628, 1614,  628, 1616,  632, 1614,  632, 1610,  632, 1614,  632, 1614,  634, 1614,  628, 492,  628, 1614,  632, 488,  632, 488,  632, 488,  632, 488,  628, 1614,  632, 488,  628, 1616,  632, 488,  632, 1610,  634, 1614,  632, 1610,  632, 1616,  632, 488,  634, 1606,  632, 40014,  9054, 2172,  634};  // NEC 807F42BD
uint16_t rawData_on_off[71] = {9046, 4436,  634, 1614,  634, 488,  628, 494,  628, 494,  628, 488,  632, 488,  628, 492,  656, 460,  628, 492,  628, 1610,  632, 1616,  628, 1614,  628, 1614,  632, 1616,  630, 1612,  632, 1616,  628, 1616,  632, 1614,  628, 494,  626, 494,  628, 1610,  632, 490,  632, 1610,  634, 488,  632, 488,  628, 494,  628, 1616,  628, 1616,  628, 494,  628, 1614,  628, 492,  628, 1610,  628, 40006,  9050, 2174,  628};  // NEC 807FCA35
const uint16_t kIrLed = 16; //ПИН ИК передатчика 16
IRsend irsend(kIrLed);

#ifdef ESP32
    #include "FS.h"   //??? думал дир
    #include "SPIFFS.h" // Needed for ESP32 only
    #include <WebServer.h>
    #include <WiFi.h>
    #include <HTTPClient.h>
    //#include <Tone32.h>     //т
    #include <ESPmDNS.h>
    WebServer server(80);
    
#else
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
    #include <ESP8266HTTPUpdateServer.h>  //для обновления
    #include <ESP8266HTTPClient.h>
    ESP8266HTTPUpdateServer httpUpdater;  //для обновления
    ESP8266WebServer server(80); 
    //#include <DNSServer.h>
#endif

#define _ZVUK    //чтобы звук отключить закоментровать
#define _ip_adr      //без коментарии 102 если закоментировать то 103

#define _ipad 104
#define _getv 199
IPAddress local_IP(192, 168, 1, _ipad);// Задаем статический IP-адрес:
IPAddress gateway(192, 168, 1, _getv);// Задаем IP-адрес сетевого шлюза:
IPAddress primaryDNS(192, 168, 1, _getv);   // опционально

String weatherHost0 = "api.weatherbit.io";

/*
#if   (_ip_adr==102)
IPAddress local_IP(192, 168, 1, 102);// Задаем статический IP-адрес:
IPAddress gateway(192, 168, 1, 199);// Задаем IP-адрес сетевого шлюза:
IPAddress primaryDNS(192, 168, 1, 199);   // опционально
#elif (_ip_adr==103)
IPAddress local_IP(192, 168, 1, 103);// Задаем статический IP-адрес:
IPAddress gateway(192, 168, 1, 199);// Задаем IP-адрес сетевого шлюза:
IPAddress primaryDNS(192, 168, 1, 199);   // опционально
#elif (_ip_adr==104)
IPAddress local_IP(192, 168, 1, 104);// Задаем статический IP-адрес:
IPAddress gateway(192, 168, 1, 199);// Задаем IP-адрес сетевого шлюза:
IPAddress primaryDNS(192, 168, 1, 199);   // опционально
#endif
*/



/*
#ifdef _ip_adr==2
IPAddress local_IP(192, 168, 1, 104);// Задаем статический IP-адрес:
//String weatherHost0 = "15.235.118.222";  //надо 158.69.116.36
String weatherHost0 = "api.weatherbit.io";
#endif
#ifdef _ip_adr==3
IPAddress local_IP(192, 168, 1, 104);// Задаем статический IP-адрес:
//String weatherHost0 = "15.235.118.222";  //надо 158.69.116.36
String weatherHost0 = "api.weatherbit.io";
#endif
*/

IPAddress subnet(255, 255, 255, 0);

IPAddress secondaryDNS(8, 8, 8, 8); // опционально

//#include <HTTPClient.h>
//#include <WiFiClientSecure.h>

#include <FS.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h> //для обновления Arduino
//---------------------------------плеер----------
//#include <SoftwareSerial.h>
//#include <DFPlayer_Mini_Mp3.h>
//SoftwareSerial mp3_com(0, 12); // RX, TX  5,4
//#define MP3_PIN   16
#define gromk 15 //17//11  //9
#define gromk2 0  //насколько увеличить громкость
static uint32_t myTimer_pl;
static uint8_t i_bat ;
//----------------------------------------------------------
#include "Adafruit_Si7021.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BMP085.h>
#include "BlueDot_BME280.h"
#include <SimpleDHT.h>


//#include "Fonts2.h"    ------------------------------------------------------------------------------------------------------

#define DIN_PIN   13                                                                    //GPIO 13 / D7
#define CS_PIN    15                                                                    //GPIO 15 / D8
#define CLK_PIN   14                                                                    //GPIO 14 / D5
//#define buzzerPin 12            пустой будет пуль                                                         //GPIO 12 / D6
#include "fonts.h" 

WiFiClient ESPclient;
//WiFiClientSecure httpsClient;

//BH1750 lightMeter; 

// Веб сервер
File fsUploadFile;
IPAddress apIP(192, 168, 4, 1);
#include "P_js.h"
#include "P_css.h"
#include "P_index.h"
#include "P_time.h"
#include "P_weath.h"
#include "P_setup.h"
#include "P_help.h"
#include "P_mqtt.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* DB_host = "dev.ibc-composite.com";  //host_name
const int   Port = 80;
const char* database_pass = "nsq8VHqjzt1u"; //пароль пользователя базы данных
boolean f_otpr_skl; //флаг отправки пакетов запуска по времени для серверов 8.30
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char * host2 = "api.privatbank.ua";
const int httpsPort2 = 443;  //HTTPS= 443 and HTTP = 80
//SHA1 finger print of certificate use web browser to view and copy
const char fingerprint[] PROGMEM = "D9 52 CF 21 14 87 71 E5 7A 86 23 99 19 41 5C 4B 4E 7E FA EC";
String datarx,Kurs_disp;

float USDbuy ;  
float USDsale ;  
float EURbuy ;  
float EURsale ;  
float BTCbuy ;  
float BTCsale ;  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------
float t0Lux;    //для яркости
int t1Lux = 0;               // цела
int t2Lux = 0, tt2Lux = 0; 

uint8_t f_mp3;  //флаг  понять что запучен проиграш песен для случайного выбора песни
unsigned long tim_mp3 = millis();

#define fol_papk_pes   15   //команда для проигрывания песни из папки  

float pred_dav, raz_dav; //предыдущее давление  //разница в далениях
float max_dav, min_dav;   //минимальное и максимальное давление

bool dav_pov;   //давление повышаеться 1
int nask_dav; //насколько давление поменялось

//bool outForecast = true;
 String wrem_tm,dav_tmp;
 bool fl_per, fl_dat, fl_dom,fl_dom2, fl_uli, fl_dav, fl_dav3, fl_vla;// флаг для вывода даты ЧТОБЫ ВЛЕТАЛО В ВЫВОД ТОЛЬКО ОДИН РАЗ А НЕ НЕ НЕСКОЛЬКО ПОКА ВРЕМЯ НЕ ИСЧТИЧЕТ ВЫВОДА
 byte perek;
int dav_po_chasu[9]; //дельта давления по каждому часу для начала последних  6 часов
int kol_chasov; //сколько часов предыдущего давления помнить
 unsigned long  pressure2,aver_pressure, pressure_array[6], time_array[6];

unsigned long sumX, sumY, sumX2, sumXY;
float a, b;
boolean  move_arrow;
uint8_t   kol_dav;
int   angle, delta, pressure_del[6];
bool outKurs = false;
int updateKursPrivat = 0;
String kursString = "";
const char* kursPrivat2 = "api.privatbank.ua";//https://api.privatbank.ua/p24api/pubinfo?json&exchange&coursid=5
//String tMes, tNow, tPress, tSpeed, tMin, tTom, tKurs, tSale, tYour, tPoint;
String tKurs,tSale;
float buy;
float sale;
float ttmp;

String ssid = "home";                                                         // Назва локального WiFi
String password = "30011980";                                               // Пароль локального WiFi
String ssidAP      = "WiFi-Clock";                                                      // Назва точки доступу
String passwordAP  = "1234";                                                                // Пароль точки доступу
boolean weatherHost = 0;
//String weatherHost0 = "api.weatherbit.io";
//String weatherHost0 = "15.235.118.221";  //надо 158.69.116.36
String weatherHost1 = "api.openweathermap.org";

#ifdef _ip_adr 
String weatherKey0  = "665466c1c88b4674992485ea92a2c8e6";
//String weatherKey0
String weatherKey1  = "11111111111111111111111111111111";
String cityID0      = "Dnipro";
String cityID1      = "709930"; 
char personalCityName[51] = "Dnipro";
#else 
String weatherKey0  = "466b049f144f47e0a472cd2f07936497";
String weatherKey1  = "11111111111111111111111111111111";
String cityID0      = "Hulyaypole";
String cityID1      = "707898"; 
char personalCityName[51] = "Hulyaypole";
#endif

String weatherLang = "ru";                                                             // Мова отримання прогнозу погоди
String location_name = "";
String location_region = "";
String location_country = "";
String location_localtime = "";
float  location_temp;
float  location_app_temp;
int    location_rh;
float  location_pres;
float  location_wind_spd;
String location_wind_cdir_full = "";
String location_sunrise = "";
String location_sunset = "";
int    location_clouds;
int    location_vis;
int    location_uv;
String location_weather_description = "";
// ----------змінні для роботи з mqtt сервером
char mqtt_server[21] = "m13.cloudmqtt.com";                                             // Имя сервера MQTT
int  mqtt_port = 13011;                                                                 // Порт для подключения к серверу MQTT
char mqtt_user[25] = "222222222";
char mqtt_pass[25] = "3333333333333";                                               // Пароль от сервера MQTT
char mqtt_name[25] = "ESP-chasy";
char mqtt_sub_inform[25] = "Inform/mess";
char mqtt_sub1[25] = "Ulica/temp";
char mqtt_sub2[25] = "Ulica/hum";
char mqtt_sub3[25] = "Ulica/press";
char mqtt_pub_temp[25] = "ESPzimmer/temp";
char mqtt_pub_tempUl[25] = "ESPzimmer/tempUl";
char mqtt_pub_hum[25] = "ESPzimmer/hum";
char mqtt_pub_press[25] = "ESPzimmer/press";
char mqtt_pub_alt[25] = "ESPzimmer/alt";
bool mqttOn = false;
// --------------------------------------------
String uuid = "44444444444444444444444444444444";
String api_key = "55555555555555";
int sensors_ID0 = 0;    //88733 Frankfurt
int sensors_ID1 = 3300;   //88459 Frankfurt
int sensors_ID2 = 0;
// =====================================================================================
bool printCom =  false;
bool priin;
#define MAX_DIGITS 16
byte NUM_MAX = 4;
byte fontCLOCK = 0;      // 0-крупный, 1-крупный цифровой, 2-полу жирный, 3-полу жирный цифровой, 4-обычный, 5-обычный цифровой, 6-узкий, 7-узкий цифровой.
byte aliData = 8;   //наверное сколько линий в матрице
byte volBrightnessD  = 0;  //яркость днем
byte volBrightnessN  = 0;  //яркость ночью
bool volBrightnessAuto = 0;
byte levelBridhtness = 0;
byte timeDay = 5;
byte timeNight = 24;
int rotate = 90;//90
byte timeScrollStart = 6;//время с которого начинать показывть погогду
byte timeScrollStop = 23; //время полсе которого не показывать
byte timeScrollSpeed = 50;


bool buzzerSet = 0;   //Бузер активный или пасивный 0 пасивный
//#define brightPin A0
OneWire  ds(0); //   0                                                                     // DS18B20 подключен к 10 пину (резистор на 4.7к обязателен)
int pinDHT = 2;
SimpleDHT11 dht11;
SimpleDHT22 dht22;

// ----------
#include "max7219.h"
//#include "fonts.h"
boolean WIFI_connected = false;
// ---------- Настройка оновлення часу
IPAddress timeServerIP;
IPAddress pogodaIP; 
String ntpServerName = "ntp.time.in.ua";   //
const int NTP_PACKET_SIZE = 48; // Размер буфера для принимаемого пакетов
byte packetBuffer[NTP_PACKET_SIZE];
//----------------------------------------------------
char packetBuffer2[ NTP_PACKET_SIZE]; //Буфер хранения пакетов char
//------------------------------------------------
WiFiUDP udp;
unsigned int localPort = 2390;
unsigned int localPort2 = 1152;
unsigned long epochNM;
static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};        // Кількість днів у місяцях
#define LEAP_YEAR(Y) (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100)||!((1970+Y)%400)))   // Високосні літа
String weatherMain = "";
String weatherDescription = "";
String weatherLocation = "";
String country;
String tempz;
int clouds;
int windDeg;
float windSpeed;
String dw, _month;
String jsonConfig = "{}";
String jsonTime = "{}";
// ---------- Змінні для роботи локального годинника ЧАССЫ----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------
float timeZone = 4.0;     //было 2                                                                    //  часовий пояс  
float hourCorr;
bool isDayLightSaving = true;
long localEpoc = 0;
long localMillisAtUpdate = 0;
int hour = 16, minute = 40, second = 42, month = 8, day = 23, dayOfWeek = 1, year = 2020;    //2,40,42,4,6,5,2018
int g_hour, g_minute, g_second, g_month = 1, g_day, g_dayOfWeek, g_year;
bool statusUpdateNtpTime = 0;                                                               // якщо не "0" - то останнє оновленя часу було вдалим
String y, mon, wd, d, h, m, s, mes;
uint8_t hourTest[3], minuteTest[3];
int secFr, lastSecond, lastMinute;
String date;
byte kuOn = 7;
byte kuOff = 23;
bool clockNight = 0;  //вночное время выводим только часы
// ----------
String cityName;
//int cityId;
String weatherString;
String weatherStringZ;
String weatherStringZav;  //прогноз погоды на завтра строка
byte perk_sehas_segof_zavt; //преключение между сейчас сегодня завтра 
String vivod_date;
// ----------
byte dig[MAX_DIGITS] = {0};
byte digold[MAX_DIGITS] = {0};
byte digtrans[MAX_DIGITS] = {0};
int dx = 0;
int dy = 0;
byte del = 0;
int updateOTA = 1;  //разрешашаем обновлять по сети
bool displayForecast = true;    //Отображать погоду на экране
bool displayCityName = false;   //выводить город
bool displayForecastNow = true;
bool displayForecastTomorrow = true;
int updateForecast = 0;
int updateForecasttomorrow = 0;
float t0 = -85.0;         // температура в доме со знаком и плавающей запятой
int t1 = 0;               // температура в доме целая беззнаковая часть
int t2 = 0, tt2 = 0;      // температура в доме дробная беззнаковая часть
float t3 = -85.0;         // температура на улице со знаком и плавающей запятой
int t4 = 0, t5 = 0;     
float t6 = 0.0;           // температура доп. со знаком и плавающей запятой
int t7 = 0, t8 = 0; 
float t1kom = 0.0;        // температура комнаты со знаком и плавающей запятой
int t2kom, t3kom ; 
float t1kux = 0.0;         // температура комнаты со знаком и плавающей запятой
int t2kux, t3kux ; 

float h0 = 0.0; // влажность в доме со знаком и плавающей запятой
byte h1 = 0;    // влажность в доме целая беззнаковая часть
byte h2 = 0;    // влажность в доме дробная беззнаковая часть
float tMqtt1 = 0.0; //температура с mqtt со знаком и плавающей запятой
float tMqtt2 = 0.0; //влажность с mqtt со знаком и плавающей запятой
float tMqtt3 = 0.0; //давление с mqtt со знаком и плавающей запятой
float tempNM = 0.0;
bool pressSys = 0;
int p0;
byte p1;
byte p2;
byte p3;
byte p4;
byte p5;
int pd_ma0,pd_mi0;  //впеменыый для отображения давления максимум минимум с учетом корекции
int pd0;
  byte  pd1, pd2, pd3, pd4;
byte humidity;                        // влажность для прогноза
float pressure;                       // давление для прогноза
//----------------------------------------------------------------------------------------
float temp;                           // температура для прогноза
float tempDs18b20;                    // температура с датчика DS18B20
BlueDot_BME280 bme;
float tempBme = 0;
float humBme = 0;
float pressBme = 0;
float altBme = 0;
Adafruit_BMP280 bmp;
Adafruit_BMP085 bmp180;
float tempBmp = 0;
float pressBmp = 0;
float altBmp = 0;
bool bmp280 = false;
bool BMP180 = false;
bool bme280 = false;
float tempDht = 0;
float humiDht = 0;
Adafruit_Si7021 sensor = Adafruit_Si7021();
float hum = 0;
float humSi7021 = 0;
float celsiusSi7021 = 0;
bool si7021 = false;
float corrTempD = 0;  //-3.3
float corrTempU = 0;
float corrTempH = 0;
float corrHumi  = 0;
int   corrPress = 0;  //-21
byte sensorDom = 0;          //NONE = 0, DS18B20 = 1, Si7021 = 2, BMP280 = 3, BME280 = 4, DHT = 5, MQTT1 = 6, MQTT2 = 7, MQTT3 = 8;
byte sensorUl =0;           //NONE = 0, DS18B20 = 1, Si7021 = 2, BMP280 = 3, BME280 = 4, DHT = 5, MQTT1 = 6, MQTT2 = 7, MQTT3 = 8, NMon = 9;
byte sensorHome = 0;         //NONE = 0, DS18B20 = 1, Si7021 = 2, BMP280 = 3, BME280 = 4, DHT = 5, MQTT1 = 6, MQTT2 = 7, MQTT3 = 8, NMon = 9;
byte sensorHumi = 0;         //NONE = 0, NONE    = 1, Si7021 = 2, NONE   = 3, BME280 = 4, DHT = 5,  NONE = 6;
byte sensorPrAl = 3;         //NONE = 0, NONE    = 1, NONE   = 2, BMP280 = 3, BME280 = 4, NONE = 5, NONE = 6;
String tMes, tNow, tCurr, tPress, tPress0, tSpeed, tMin, tTom, tYour, tPoint, tIp, tPass, tWeatrNot, tWeatrTN;
bool alarm_stat = 0;
bool alarm_hold = 0;  //для кноки зачемто
byte alarm_numer = 255;
byte alarme[5][3] {{12, 30, 0}, {7, 15, 0}, {22, 55, 0}, {0, 30, 0}, {0, 0, 0}}; //1-часы, 2-минуты, 3-откл(0)/1раз(11)/пон-пят(8)/пон-сб(9)/сб-вс(10)/вс(1)/пон(2)/вто(3)/сре(4)/чет(5)/пят(6)/сб(7)/всегда(12)
byte memory_hour_start = 9;
byte memory_hour_end = 23;
byte m_date = 0;
byte memory_date[9][2] {{1, 1}, {2, 1}, {6, 1}, {7, 1}, {13, 1}, {19, 1}, {8, 3}, {1, 5}, {1, 9}}; //1-день, 2-месяц
char memory_date_mes0[51] = "Новый Год!!!";char memory_date_mes1[51] = "ПослеНовыйГод(((";char memory_date_mes2[51] = "Ночь перед Рождеством";char memory_date_mes3[51] = "Рождество";char memory_date_mes4[51] = "Старый Новый Год!";char memory_date_mes5[51] = "Крещение";char memory_date_mes6[51] = "День цветов";char memory_date_mes7[51] = "День шашлыка)))";char memory_date_mes8[51] = "Отведи ребенка в школу";
bool firstStart = 0;
byte amountNotStarts = 0;
String jsonLine = "";
bool rtcStat = false;   //jопределяет исрльзовать ли часы
int rtcAddr = 0x68;
int hour_rtc, minute_rtc, second_rtc, month_rtc, day_rtc, dayOfWeek_rtc, year_rtc;
#include "rtc.h"
byte errorRTC;
bool butStat = 1;
byte butMode = 0; // 0 - не нажата, 1 - нажата один раз, 2 - нажата два раза, 3 - 5 секунд нажата, 4 - 30 секунд нажата.
byte butFlag = 0; // 1 - кнопка нажата, 0 - не нажата
int butCount = 0; // счетчик времени нажатия кнопки
int butMillis = 0;
bool runningLine = 0;
//плеер  pppppp
bool fl_bud_mp;  //флаг для будильника мп3
uint16_t voll=2;   //громкость
byte trek=222;   //номер мелодии для будильника  //220

int tracks[2] = {0, 0};              // tracks in microSD

bool kom_bip;   //комада звука 1 - команда звука была
char dan [20]; //дляпллера пре даных
bool pr_bip_vre3; //пременая для бп чтобы отказаться от задержек
bool st_pler;  //для стастуса плеера

byte caun_zv;   //счет чик звука для очерености проиграша
//======================================================================================//==================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================
#define Stat  0x42
#define Play  0x0D
#define Stop  0x0E
#define Next  0x01
#define Fold  0x0F
#define ADVE  0x13
#define Volu  0x06

uint8_t f_kuku;   //флаг для кукушки каждый час
uint8_t one_fl, one_f2, one_f3, one_f4; //флаг говорить превый раз время для напоминания чтобы сказать только раз
static uint8_t cmdbuf[8] = {0};
//static uint8_t cmdbuf2[9] = {0};
static uint8_t ansbuf[20] = {0};
//static uint8_t ansbuf_pred[20] = {0};

//static uint8_t doigral; //флаг того что плеер доиграл песню
//uint8_t  caun_tmp; 
//uint8_t per_otv;    //для того чтобы понят что был првый ответ
//static uint32_t nomer;
//static uint16_t nomer;
//uint8_t cont_net_kom; //счет количиства полученых не тех команд чтобы все начать с начала!!!! удалить
//uint32_t T_prav_otv;  //время ожидание нужного ответа, полсе него продожить отправку следуещей команды

//static uint8_t cout;
//static uint16_t  f_stop; //закончился проигр песни
//static uint16_t  f_ADVERT; //флаг вставки
//static uint16_t  f_otv; //флаг должен быть ответ , чтобы исключить дублирование ответа при выводе в ком порт
//static uint16_t  pusk_stop;

//static unsigned long zac;
//======================================================================================//==================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================
/*
void print_info () {
        Serial.println ("you send:");
        printHex (send_buf, 10);
        delay (100);
        int recv_leng = serialRead (mySerial, recv_buf, 10, 3);
        if (recv_leng) {
                Serial.println ("you get:");
                printHex (recv_buf, recv_leng);
        }
}
*/


//==================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================
//*********************************************************************************************************************************************************************************************************************************************************************
//*********************************************************************************************************************************************************************************************************************************************************************
//*********************************************************************************************************************************************************************************************************************************************************************
//*********************************************************************************************************************************************************************************************************************************************************************
void loop() {  
    //MPR121_update(); 
    //if(mp3_com.available()){ answer(100);}
 
       if (second==45) {  if (!f_otpr_skl){f_otpr_skl=1;
       //bd_sql();
       }
       }  else   { f_otpr_skl=0;}    
    
#ifdef _ZVUK    
    if (pr_bip_vre3) {bip_vre4();} //звук с веба   дублируеться когда бежит строка чтоб не замолк
    if (f_kuku) {bip_vre4();}      //звук скукушка дублируеться когда бежит строка чтоб не замолк
    if (one_fl==1||one_f2==1||one_f3==1||one_f4==1) {bip_vre4();}      //постоянно влетает пока незакончет произношение 0 не играет 1- запускает голос 2- неиграет и ждет сброса
   

 // if ( (f_mp3)&& (millis() -  tim_mp3 > 1000) ) { tim_mp3 = millis(); if (digitalRead(MP3_PIN)==1) { if (hour > 5 && hour !=23) {mp3_play_file_in_fol (12,random(100));} } } // каждые полсекунды будет проерять закончился лм проиграш
 //vver(); 
  //if (digitalRead(MP3_PIN)==1)
#endif 
  if (updateOTA) ArduinoOTA.handle();
  //if (priin) {command(3,0,222);priin=0;}
    server.handleClient();   
     
  updateTime();   
  buttonInter();                     // дозволяємо HTTP серверу відповідать на запити // оновлюємо час//анадлиз интрвала нажатия хлавиши
  
    int cb2 = udp.parsePacket(); 
  if (cb2) 
  {  udp.read(packetBuffer2, NTP_PACKET_SIZE);   String  req2=(packetBuffer2); //char reg3=(packetBuffer2);
    if (req2.indexOf("63") != -1)   // yyyyy    Осуществляет поиск символа или подстроки в строке (req2)  -1 если 63 не найдено (можно указать и определенную позицию, с которой необходимо начать поис через запятую)
    {  
      char myStr8[10];  req2=req2.substring(2);req2.toCharArray(myStr8, req2.length()+1);t3 = atof(myStr8)/10;   //возращает значение начиная с совторо символа, можно указать и позиц символа чере запатую// копирование String в массив myStr8 //преобразуем char в флоат
      t4 = int(t3);  t5 = int(t3 * 10 * (t3 > 0 ? 1 : -1)) % 10;  
    } 
    if (req2.indexOf("64") != -1)  {  char myStr8[10];  req2=req2.substring(2);req2.toCharArray(myStr8, req2.length()+1);t1kom = atof(myStr8)/10; t2kom = int(t1kom); t3kom = int(t1kom * 10 * (t1kom > 0 ? 1 : -1)) % 10; } 
    if (req2.indexOf("65") != -1)  {  char myStr8[10];  req2=req2.substring(2);req2.toCharArray(myStr8, req2.length()+1);t1kux = atof(myStr8)/10; t2kux = int(t1kux); t3kux = int(t1kux * 10 * (t1kux > 0 ? 1 : -1)) % 10; } 
    if (req2.indexOf("60") != -1)  {  char myStr8[10];  req2=req2.substring(2);req2.toCharArray(myStr8, req2.length()+1);t1kom = atof(myStr8)/10; t2kom = int(t1kom); t3kom = int(t1kom * 10 * (t1kom > 0 ? 1 : -1)) % 10; } 
  // byte  
     memset(packetBuffer2, 0, NTP_PACKET_SIZE);  //очищаем буфер для приема следующей команды   //зполняем packetBuffer нулем(0) по размеру NTP_PACKET_SIZE
  }
                                                                      
 if (second != lastSecond) 
 {                                                           
    lastSecond = second;      //елсли регулеровка включена автоматически то оправляем яркость в зависимости от освечения  иначе если ЧАС больше дневного и Меньше Ночного часма то оправляем яркость дненую, инач яркость ночную
    if (volBrightnessAuto) { /*levelBridhtness=map(analogRead(brightPin),1023,0,2,15);*/ sendCmdAll(CMD_INTENSITY, levelBridhtness);} else {if (hour>=timeDay && hour<timeNight) sendCmdAll(CMD_INTENSITY, volBrightnessD);  else sendCmdAll(CMD_INTENSITY, volBrightnessN);  }
 
   secFr = 0;// на початку нової секунди скидаємо secFr в "0"

    if (lastMinute != minute) 
    { if (hour == 0 && minute == 0 && second == 0) 
      {  day++; if (day==32 || (day==31 && (month==4 || month==6 || month==9 || month==11)) || (month==2 && ((day==29&&year%4 !=0) || (day==30 && year%4 ==0)))) { day=1;month++;  if (month > 12) {month = 1; year++; } }
        dayOfWeek++;if (dayOfWeek > 7) dayOfWeek = 1;   //день недели
      }      
      lastMinute = minute;
    }
 } else secFr++;                                                                                                                          //пока секуда не закочилась увеличивает значение  тогла нарощуємо лічильник циклів secFr     //за секуду гдето 2180                                                       
  //---------Сигнал Каждый час  если минуты и секунды  и незакончилась первая секуда и если час больше чем кукушка вклечина и час меньше чем укушка выключена подаем сигнал полного часа
 //if (minute==0 && second==0 && secFr==0 && (hour>=kuOn && hour<kuOff)) { bip();bip();}
 if (minute==0 && second==0 && secFr==0 && (hour>=kuOn && hour<kuOff)) { f_kuku=1;}

 
 //на 10 секунде читаем значение датчиков
 if (secFr == 0 && second == 25 && !alarm_stat) {sensorsAll();  /*dav_opros();kol_dav++;*/}//читаем значеение датчков  if (printCom){Serial.println("Обновление датчиков");};

  //----------- РОБОТА З БУДИЛЬНИКОМ------------------------------------------------------
  if (secFr==0)  {   if (second>0 && alarms()) { if (!alarm_stat && alarm_numer!=255 && !alarm_hold)  alarm_stat=1; }  
  else if (alarm_stat)  
  {alarm_stat = 0; voll=2; fl_bud_mp=0;  bip_bud_vs(); //будильник всавка часов если играет или посто говорит время
   if (alarme[alarm_numer][2] == 11) alarme[alarm_numer][2] = 0;  
  } else if (alarm_hold != 0){ Serial.println("что то связано с выключением думаю");}}
    //------------- РОБОТА ЗІ СВЯТКОВИМИ ДАТАМИ ---------------------------------------------
  if (secFr == 0) {  //КАЖДУЮ СЕКУДУ
    //if (minute == 0) { // minute % 5 == 1   //1, 6, 11, 16...56 хв.
     if (minute==1 || minute==30) { 
      if (hour >= memory_hour_start && hour <= memory_hour_end && second < 30 && second > 2 && !alarm_stat) {
        for (byte i = 0; i < 9; i++) 
        {   if (memory_date[i][0] == day && memory_date[i][1] == month) 
            { m_date = i; bip_prazn();
              printStringWithShift(("     "), timeScrollSpeed); //такая запись в место свич экономит 30 байт  ПЗУ или скетча гдето 
              printStringWithShift((m_date == 0 ? memory_date_mes0 : m_date == 1 ? memory_date_mes1 : m_date == 2 ? memory_date_mes2 : m_date == 3 ? memory_date_mes3 : m_date == 4 ? memory_date_mes4 : m_date == 5 ? memory_date_mes5 : m_date == 6 ? memory_date_mes6 : m_date == 7 ? memory_date_mes7 : memory_date_mes8), timeScrollSpeed);
              printStringWithShift(("     "), timeScrollSpeed);     // break;    нужен только если отображать одно напоминание на день
            }
        }
      }
    } else m_date = 0;
  
  }  buttonHandling();
   //Serial.print(String(secFr)); 
  //------------- НАШ ЧАС ----------------------nach------------------------------------------
 // if (hour == 0 && minute == 51) {     bip();       /*printStringWithShift(("       22:55 \200\200\200 " + tMes + " \200\200\200").c_str(), timeScrollSpeed);     return;*/  }
 
 if (_ipad==103) {
    if (hour == 6 && minute == 10 ){if (one_fl==0) {one_fl=1;}      }  else  {one_fl=0;}    /*printStringWithShift(("       22:55 \200\200\200 " + tMes + " \200\200\200").c_str(), timeScrollSpeed);     return;*/ 
    if (hour == 6 && minute == 20 ){if (one_f2==0) {one_f2=1;}      }  else  {one_f2=0;}
    if (hour == 6 && minute == 25 ){if (one_f3==0) {one_f3=1;}      }  else  {one_f3=0;}
    if (hour == 6 && minute == 30 ){if (one_f4==0) {one_f4=1;}      }  else  {one_f4=0;}
  }

 if (_ipad==102) {
 if (hour==6 && minute==5)  
 {
    if (ir_flag2==0) 
    {   
        ir_flag2=1; 
      //irsend.sendRaw(rawData_sleep,71,38); delay(300); irsend.sendRaw(rawData_sleep,71,38); delay(300); irsend.sendRaw(rawData_sleep,71,38);delay(300);irsend.sendRaw(rawData_sleep, 71, 38); delay(300);
     // irsend.sendRaw(rawData_on_off,71,38); 
    }    
    }  else  {ir_flag2=0;}
 }
 
 
 // if (minute % 5 == 1) {if ( pred_dav!=pressBmp) {if (pressBmp>pred_dav){nask_dav=int(pressBmp-pred_dav); dav_pov=1;} else {dav_pov=0; nask_dav=int(pred_dav-pressBmp);} pred_dav=pressBmp;}}
 
  // -------------------------------------------------------------------- ВИВІД НА ЕКРАН ГОДИННИКА АБО ТЕМПЕРАТУРИ ЧИ ВОЛОГОСТІ-------------------------------daf-----------------------------------------------------------------------------
    if (!alarm_stat && millis() % 50 == 0) //ноль когда 50 100 150 ... 
  {  
    if ((clockNight && (timeDay<=timeNight?(hour>=timeDay && hour<=timeNight):(hour>=timeDay || hour<timeNight))) || !clockNight) 
   {
    if(second>=10){
        if (second % 2 == 0 )
        {if(fl_per==0)
          { fl_per=1;//perek++;
            //t0Lux = lightMeter.readLightLevel();  //читаем значение яркости
            
            switch (++perek)    //  showSimplePre();  showSimplePre3();   showLux2(); showSimpleDate();
            {
           // case 1:{showSimpleTempDom1();  break;}
            //case 1:{showSimplePre(); perek=0; break;}
            case 1:{showSimpleDate(); break;}
            case 2:{showAnimClock();   break;}
            case 3:{showSimplePre();   break;}
            case 4:{showAnimClock();   break;}
            //case 5:{Kurs_disp();   break;}
            case 5:{showSimpleTempDom1();   break;}
            case 6:{ showAnimClock(); break;}  
            case 7:{showSimpleHum(); perek=0;break;}
            //case 2:{ perek=0; break;}
           // case 2:{showSimpleTempDom1();  break;}
           // case 3:{showSimpleHum();   break;} 
           // case 4:{showSimpleHum();   break;}
     //       case 5:{ perek=0; break;}
           // case 5:{showSimplePre4();     break;}  //showLux2();showSimplePre4();
    //        case 6:{showSimplePre();  break;}
   //         case 7:{/*showSimplePre();*/  break;}
            case 8:{showSimplePre();  break;}
            case 9:{showSimpleDate();   break;}
            case 10:{showAnimClock();  break;}  
            case 11:{showSimpleTempDom1();  break;}  
            case 12:{showSimpleTempDom1();   break;}
            case 13:{showSimpleHum(); break;}
            case 14:{/*showSimpleHum();*/ break;}
            case 15:{showSimplePre4();   break;}
            case 16:{  break;}
            case 17:{showSimplePre();  break;}
            case 18:{showSimplePre3();  break;}
            case 19:{showSimpleDate2();   break;}
            case 20:{showSimpleDate2(); break;}
            case 21:{showSimpleDate2();  break;}  
            case 22:{ perek=0; break;}
            default:{  break;}
            }//switch
          }
        } else fl_per=0;
      } else { perek=0; showAnimClock();}
   } else showAnimClock();  //чтобы вночное время показывать только часы 

  } 
  else   if (alarm_stat) 
  {  if (millis() % 50 == 0) showAnimClock(); 
     if(secFr==0 && second>1 && second<=59) 
    {     clr(); delay(100); refreshAll();       bip_budil();  }  
  }  //----ббб-------------------будильник сработал и мигание времени /*clr(); refreshAll();bip();bip();*/
  
  // --------------------------------------------------------------------------------------------------------////секуда еще не закончилась   обновляем RTC в 3:01:10 //если используемfl_bud_mp=0;
  if (secFr == 0)                                                                                                                                                                         
  {  if (hour==3 && minute==1 && second==10 )  {   if (rtcStat) {getRTCDateTime(); hour=hour_rtc; minute=minute_rtc; second=second_rtc;day=day_rtc; month=month_rtc; year=year_rtc; dayOfWeek=dayOfWeek_rtc; if (printCom) {Serial.println("RTC update: "+String(hour)+":"+String(minute)+":"+String(second)+"   "+String(day)+"."+String(month)+"."+String(year)+" D="+String(dayOfWeek));}  } }
   
    // ---------- 10 секунда - виводимо дату/погоду---------------pat-------------------------------------------
                                                                                                                                                          //  if (((minute!=0 && second==5)||(minute=0 && second==10))  && !alarm_stat && hour >= timeScrollStart && hour < timeScrollStop)                     //если нет аварий и на екране не чего не выводиться //в полный час выводим погоду позже чтобы отработал информация
    if ( second==3  && !alarm_stat && hour >= timeScrollStart && hour < timeScrollStop)                                                                                                           //если нет аварий и на екране не чего не выводиться //в полный час выводим погоду позже чтобы отработал информация
    {                                                                                                                                                                                           //елси время вывода разрешает отображать то выводим прогноз 
      if((timeDay<=timeNight?(hour>=timeDay && hour<=timeNight):(hour>=timeDay || hour<timeNight)) || !clockNight) 
      { if (updateForecast > 0 && updateForecast < 360) {printStringWithShift(("  (" + tWeatrTN + String(updateForecast) + tMin + ")        ").c_str(), timeScrollSpeed); perk_sehas_segof_zavt=1;}//нет обновления погоды сколькото минут, выводит соообщение
        switch (++perk_sehas_segof_zavt) 
         {  case 1: {printStringWithShift(   weatherString.c_str(), timeScrollSpeed); break;}                              //сейча выводим
            //case 2: {printStringWithShift(  Kurs_disp.c_str(), timeScrollSpeed); break;}                             
            case 2: {printStringWithShift(  weatherStringZ.c_str(), timeScrollSpeed); break;}                             //сегодня
            case 3: {printStringWithShift(weatherStringZav.c_str(), timeScrollSpeed); perk_sehas_segof_zavt=0; break;}    //завтра
         }
        //if (minute % 2 == 0 || !displayForecast)  { printStringWithShift(weatherString.c_str(), timeScrollSpeed); }  //по четным сейчас  //не понятно зачем displayForecast
      } else showAnimClock();           
                                                                       //;pd0=(int)pressBmp+corrPress           
       /////if (sensorPrAl) {udp.beginPacket({ 192, 168, 1, 12 }, 1152);pd0=(int)pressBmp; udp.print("60"+String(pd0));  udp.endPacket(); } //udp.stop();//udp.begin(1152);  //uuddpp //раз в менуту отправлеем давление если есь датчик
      // if (rtcStat) {getRTCDateTime(); hour=hour_rtc; minute=minute_rtc; second=second_rtc;day=day_rtc; month=month_rtc; year=year_rtc; dayOfWeek=dayOfWeek_rtc; mp3_set_volume (18);delay(100);  mp3_play (hour);  delay(1400);  mp3_play (minute+100);  }     
    }
   
    // ---------- 30 перевірка доступності WiFi мережі ---// повторне підк. до WiFi кожну 1, 6, 11, 16...56 хв.
    if ((second>30 && second<38) && (WiFi.status() != WL_CONNECTED || !WIFI_connected) && !alarm_stat && firstStart)  { WIFI_connected = false;  WiFi.disconnect();  if (minute % 5 == 1) {   wifiConnect();  if (WiFi.status() == WL_CONNECTED) WIFI_connected = true;    }    }
   
    // ---------- 43 секунда оновленя мережевого часу кожну хвилину або в 5 хвилину кожного часу
    //if (((year==2018) && (statusUpdateNtpTime == 0 && second == 43) || (minute == 02 && second == 43)) && !alarm_stat) timeUpdateNTP();
    if (hour ==01 && minute == 02 && second == 43 && !alarm_stat) timeUpdateNTP(); //обновляем раз в сутки
    //if (minute == 02 && second == 43 && !alarm_stat) {  if (rtcStat) {getRTCDateTime(); hour=hour_rtc; minute=minute_rtc; second=second_rtc;day=day_rtc; month=month_rtc; year=year_rtc; dayOfWeek=dayOfWeek_rtc; if (printCom) {Serial.println("RTC update: "+String(hour)+":"+String(minute)+":"+String(second)+"   "+String(day)+"."+String(month)+"."+String(year)+" D="+String(dayOfWeek));}  }}
    // ---------- 46 cек. оновлюємо прогноз погоди -------------------------------------
 //   if (second == 46 && hour >= timeScrollStart && hour <= timeScrollStop && !alarm_stat) 
 //1, 6, 11, 16...56 хв.
    if (second == 46 && hour >= timeScrollStart && hour <= timeScrollStop && !alarm_stat)     //(minute % 5 == 1) && 
    {
    if (minute % 2 == 0 )  {dav_opros();kol_dav++; if(kol_dav>5) {kol_dav=0;}}
    
     // if (minute == 1 || minute == 31 || updateForecast || updateForecasttomorrow ||updateKursPrivat) 
      if (minute == 1  || updateForecast || updateForecasttomorrow ||updateKursPrivat) 
      {
        //if ((minute == 1 ||  minute == 31  ) && displayForecast)  
        if ((minute == 1 ) && displayForecast)  
        {
            /// dav_opros();kol_dav++; if(kol_dav>5) {kol_dav=0;}
               
          if (!weatherHost) {getWeatherData0();} else {getWeatherData1();   }                                  if (!weatherHost) {getWeatherDataz0(); } else {getWeatherDataz1();  }     
          } 
        else  
        {
          if (updateForecast) { if (!weatherHost) {getWeatherData0();} else {getWeatherData1(); }  }  if (updateForecasttomorrow) {  if (!weatherHost) {getWeatherDataz0(); } else {getWeatherDataz1();} }     
        }
      }
    }


    
  }  //secFr

  
}
//*********************************************************************************************************************************************************************************************************************************************************************
//*********************************************************************************************************************************************************************************************************************************************************************
//*********************************************************************************************************************************************************************************************************************************************************************
//*********************************************************************************************************************************************************************************************************************************************************************
//=========================================================================================================

//------------ function urlencode for weather parameters --------------------
String urlencode(String str) {    // функция взята от http://forum.amperka.ru/members/benben.19545/
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield();
  }
  return encodedString;
}
//------------ function chr_to_str --------------------
String chr_to_str(String str) {   String chr_to_str = "";  for (int i = 0; i < str.length(); i++) {    chr_to_str += str.charAt(i);  }   return chr_to_str;} 

//--------------------------Язык---------------------------
void lang() {if(weatherLang=="ru") {tNow="Сейчас"; tPress0="гПа"; tPress="мР"; tSpeed="м/с"; tMin="м."; tCurr="Сегодня"; tTom="Завтра"; tYour="Ваш"; tIp="и введите в браузере адрес"; tPass="пароль";
tWeatrNot=" Нет обновления погоды более 6 часов!!!   "; tWeatrTN="нет об.пог.-";  } }
