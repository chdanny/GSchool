#include "DHT.h"
#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>
#include "SoftwareSerial.h"

#define WIFI_AP "liefyuan"
#define WIFI_PASSWORD "123456789"

// DHT
#define DHTPIN 4
#define DHTTYPE DHT11

char MqttServer[] = "101.200.46.138";

// 初始化以太网客户端对象 -- WiFiEspClient.h
WiFiEspClient espClient;

// 初始化DHT11传感器
DHT dht(DHTPIN, DHTTYPE);

// 初始化MQTT库PubSubClient.h的对象
PubSubClient client(espClient);

SoftwareSerial soft(2, 3); // RX, TX

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup() {

  Serial.begin(9600);
  dht.begin();

  InitWiFi();                                // 连接WiFi
  client.setServer( MqttServer, 1883 );      // 连接WiFi之后，连接MQTT服务器

  lastSend = 0;
}

void loop() {
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    while ( status != WL_CONNECTED) {
      Serial.print("[loop()]Attempting to connect to WPA SSID: ");
      Serial.println(WIFI_AP);
      // 连接WiFi热点
      status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      delay(500);
    }
    Serial.println("[loop()]Connected to AP");
  }

  if ( !client.connected() ) {
    reconnect();
  }

  if ( millis() - lastSend > 1000 ) { // 用于定时1秒钟发送一次数据
    getAndSendTemperatureAndHumidityData(); // 获取温湿度数据发送到MQTT服务器上去
    lastSend = millis();
  }

  client.loop();
}

/*
 * 
 * 读取温湿度数据，然后发送到MQTT服务器上去
 * 
 */
void getAndSendTemperatureAndHumidityData()
{
  Serial.println("Collecting temperature data.");

  // 大概250ms读取一次
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // 查看是否读取温湿度失败的
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

  String temperature = String(t);
  String humidity = String(h);

  // Just debug messages
  Serial.print( "Sending temperature and humidity : [" );
  Serial.print( temperature ); Serial.print( "," );
  Serial.print( humidity );
  Serial.print( "]   -> " );

  // 构建一个 JSON 格式的payload的字符串
  String payload = "{";
  payload += "\"temperature\":"; payload += temperature; payload += ",";
  payload += "\"humidity\":"; payload += humidity;
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );

  // boolean publish(const char* topic, const char* payload);

  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.print("[publish]-->>");
  Serial.println( attributes );
}

void InitWiFi()
{
  // 初始化软串口，软串口连接ESP模块
  soft.begin(9600);
  // 初始化ESP模块
  WiFi.init(&soft);
  // 检测WiFi模块在不在，宏定义：WL_NO_SHIELD = 255,WL_IDLE_STATUS = 0,
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  Serial.println("[InitWiFi]Connecting to AP ...");
  // 尝试连接WiFi网络
  while ( status != WL_CONNECTED) {
    Serial.print("[InitWiFi]Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    delay(500);
  }
  Serial.println("[InitWiFi]Connected to AP");
}

/**
 * 
 * MQTT客户端断线重连函数
 * 
 */

void reconnect() {
  // 一直循环直到连接上MQTT服务器
  while (!client.connected()) {
    Serial.print("[reconnect]Connecting to MQTT Server ...");
    // 尝试连接connect是个重载函数 (clientId, username, password)
    if ( client.connect("liefyuan", NULL, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ mqtt connect error code = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );// Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}






///-------------------------------------------------------------------------

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "liefyuan";            // your network SSID (name)
char pass[] = "123456789";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(115200);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
}

void loop()
{
  // print the network connection information every 10 seconds
  Serial.println();
  printCurrentNet();
  printWifiData();

  delay(10000);
}

void printWifiData()
{
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print("MAC address: ");
  Serial.println(buf);
}

void printCurrentNet()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to
  byte bssid[6];
  WiFi.BSSID(bssid);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[5], bssid[4], bssid[3], bssid[2], bssid[1], bssid[0]);
  Serial.print("BSSID: ");
  Serial.println(buf);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.println(rssi);
}

//-----------------------------------------------
//-------------------------------------------------



//#include "DHT.h"
#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>
#include "SoftwareSerial.h"

#define WIFI_AP "ada3721"
#define WIFI_PASSWORD "lindasuo"

// DHT
//#define DHTPIN 4
//#define DHTTYPE DHT11

char MqttServer[] = "47.93.19.134";

// 初始化以太网客户端对象 -- WiFiEspClient.h
WiFiEspClient espClient;

// 初始化DHT11传感器
//DHT dht(DHTPIN, DHTTYPE);

// 初始化MQTT库PubSubClient.h的对象
PubSubClient client(espClient);

SoftwareSerial soft(7, 8); // RX, TX

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup() {

  Serial.begin(9600);
 // dht.begin();

  InitWiFi();                                // 连接WiFi
  client.setServer( MqttServer, 1883 );      // 连接WiFi之后，连接MQTT服务器

  lastSend = 0;
}

void loop() {
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    while ( status != WL_CONNECTED) {
      Serial.print("[loop()]Attempting to connect to WPA SSID: ");
      Serial.println(WIFI_AP);
      // 连接WiFi热点
      status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      delay(500);
    }
    Serial.println("[loop()]Connected to AP");
  }

  if ( !client.connected() ) {
    reconnect();
  }
/*
  if ( millis() - lastSend > 1000 ) { // 用于定时1秒钟发送一次数据
    getAndSendTemperatureAndHumidityData(); // 获取温湿度数据发送到MQTT服务器上去
    lastSend = millis();
  }
*/
  client.loop();
}



void InitWiFi()
{
  // 初始化软串口，软串口连接ESP模块
  soft.begin(115200);
  // 初始化ESP模块
  WiFi.init(&soft);
  // 检测WiFi模块在不在，宏定义：WL_NO_SHIELD = 255,WL_IDLE_STATUS = 0,
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  Serial.println("[InitWiFi]Connecting to AP ...");
  // 尝试连接WiFi网络
  while ( status != WL_CONNECTED) {
    Serial.print("[InitWiFi]Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    delay(500);
  }
  Serial.println("[InitWiFi]Connected to AP");
}

/**
 * 
 * MQTT客户端断线重连函数
 * 
 */

void reconnect() {
  // 一直循环直到连接上MQTT服务器
  while (!client.connected()) {
    Serial.print("[reconnect]Connecting to MQTT Server ...");
    // 尝试连接connect是个重载函数 (clientId, username, password)
    if ( client.connect("liefyuan", "TEST", "TEST") ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ mqtt connect error code = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );// Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}