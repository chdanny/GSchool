//可以上傳data to ThingSpeak
// http://lolwarden.pixnet.net/blog/post/82031214-%2A%E6%95%99%E5%AD%B8%2Aarduino-wifi%28esp8266%29-%E5%88%9D%E6%AC%A1%E4%B8%8A%E6%89%8B%E6%87%89%E7%94%A8
//

#include <SoftwareSerial.h>
#define _baudrate   9600
#define _baudrateESP   115200
#define _rxpin      10
#define _txpin      11
SoftwareSerial debug( _rxpin, _txpin ); // RX, TX
//*-- IoT Information
#define SSID "ada3721"
#define PASS "lindasuo"
#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149
// 使用 GET 傳送資料的格式
// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&filed2=[data 2]...;
String GET = "GET /update?key=HYU3RH7YLM7MX42N";

void setup() {
    Serial.begin( _baudrate );
    debug.begin( _baudrateESP );
    sendDebug("AT");
    delay(1000);
    if(debug.find("OK"))
    {
        Serial.println("RECEIVED: OK\nData ready to sent!");
        connectWiFi();
    }
    else{
    Serial.println("NO RESEPONCE!");
  }
}
void loop() {
    delay(5000);   // 60 second
    SentOnCloud( String(5), String(9) );
}
void SentOnCloud( String T, String H )
{
    // 設定 ESP8266 作為 Client 端
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += IP;
    cmd += "\",80";
    sendDebug(cmd);
    if( debug.find( "Error" ) )
    {
        Serial.print( "RECEIVED: Error\nExit1" );
        return;
    }
    cmd = GET + "&field1=" + T + "&field2=" + H +"\r\n";
    debug.print( "AT+CIPSEND=" );
    debug.println( cmd.length() );
    if(debug.find( ">" ) )
    {
        Serial.print(">");
        Serial.print(cmd);
        debug.print(cmd);
    }
    else
    {
        debug.print( "AT+CIPCLOSE" );
    }
    if( debug.find("OK") )
    {
        Serial.println( "RECEIVED: OK" );
    }
    else
    {
        Serial.println( "RECEIVED: Error\nExit2" );
    }
}
void sendDebug(String cmd)
{
    Serial.print("SEND: ");
    Serial.println(cmd);
    debug.println(cmd);
} 
 
boolean connectWiFi()
{
    debug.println("AT+CWMODE=1");
    delay(2000);
    String cmd="AT+CWJAP=\"";
    cmd+=SSID;
    cmd+="\",\"";
    cmd+=PASS;
    cmd+="\"";
    sendDebug(cmd);
    delay(5000);
    if(debug.find("OK"))
    {
        Serial.println("RECEIVED: OK");
        return true;
    }
    else
    {
        Serial.println("RECEIVED: Error");
        return false;
    }
    cmd = "AT+CIPMUX=0";
    sendDebug( cmd );
    if( debug.find( "Error") )
    {
        Serial.print( "RECEIVED: Error" );
        return false;
    }
}