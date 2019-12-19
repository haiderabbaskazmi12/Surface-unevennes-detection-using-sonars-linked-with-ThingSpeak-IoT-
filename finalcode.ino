String myAPIkey = "SBT82FLOAF506N2T";    // for data upload to internet
#include <NewPing.h>  // for sonar
#include <SoftwareSerial.h>
#define TRIGGER_PINL  11
#define ECHO_PINL     12
#define TRIGGER_PINR  9
#define ECHO_PINR     10
#define MAX_DISTANCE 200
NewPing sonarL(TRIGGER_PINL, ECHO_PINL, MAX_DISTANCE);// built in library functions
NewPing sonarR(TRIGGER_PINR, ECHO_PINR, MAX_DISTANCE);
SoftwareSerial ESP8266(2, 3); // Tx,  Rx  for transmitting and recieving
int sonar1,sonar2;   
long writingTimer = 17; 
long startTime = 0;
long waitTime = 0;
boolean relay1_st = false; 
boolean relay2_st = false; 
unsigned char check_connection = 0;
unsigned char times_check=0;

boolean error;

void setup()
{
  Serial.begin(115200); 
  ESP8266.begin(115200); 
  startTime = millis(); 
  ESP8266.println("AT+CWMODE=3");    // List valid modes = （station, AP, station + AP）             
  ESP8266.println("AT+RST");    // reset on start
  delay(2000);
  Serial.println("Connecting to Wifi");
  while(check_connection==0)
  {
    Serial.print(".");
    ESP8266.println("AT+CWJAP=\"mate10\",\"qwertyuiop\"\r\n"); //Commands ESP8266 to connect a SSID with supplied password.
    ESP8266.setTimeout(10000);
    if(ESP8266.find("WIFI CONNECTED\r\n")==1)
    {
      Serial.println("WIFI CONNECTED");
      
      break;
    }
    times_check++;
    if(times_check>3) 
    {
      times_check=0;
      Serial.println("Trying to Reconnect..");
    }
  }
}

void loop()
{ 
  waitTime = millis()-startTime;   
  if (waitTime > (writingTimer*1000)) 
  {
    readSensors();
    writeThingSpeak();
    startTime = millis();   
  }
}


void readSensors(void)
{
//  sonar1 = 5.00;
//  sonar2 = 7.00;
    sonar1 = sonarL.ping_cm();
    sonar2 = sonarR.ping_cm();
}


void writeThingSpeak(void)
{
  startThingSpeakCmd();
  // preparacao da string GET
  String getStr = "GET /update?api_key=";
  getStr += myAPIkey;
  getStr +="&field1=";
  getStr += String(sonar1);
  getStr +="&field2=";
  getStr += String(sonar2);
  getStr += "\r\n\r\n";
  GetThingspeakcmd(getStr); 
}

void startThingSpeakCmd(void)
{
  ESP8266.flush(); //returning true indicates that output data have effectively been sent, and false that a timeout has occurred.
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com IP address
  cmd += "\",80";
  ESP8266.println(cmd);
  Serial.print("Start Commands: ");
  Serial.println(cmd);

  if(ESP8266.find("Error"))
  {
    Serial.println("AT+CIPSTART error");
    return;
  }
}

String GetThingspeakcmd(String getStr)
{
  String cmd = "AT+CIPSEND=";   // send data
  cmd += String(getStr.length());
  ESP8266.println(cmd);
  Serial.println(cmd);

  if(ESP8266.find(">"))
  {
    ESP8266.print(getStr);
    Serial.println(getStr);
    delay(500);
    String messageBody = "";
    while (ESP8266.available()) 
    {
      String line = ESP8266.readStringUntil('\n');
      if (line.length() == 1) 
      {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
        messageBody = ESP8266.readStringUntil('\n');
      }
    }
    Serial.print("MessageBody received: ");
    Serial.println(messageBody);
    return messageBody;
  }
  else
  {
    ESP8266.println("AT+CIPCLOSE");     // Close TCP (tranmission control protocol) or UDP connection.For single connection mode
    Serial.println("AT+CIPCLOSE");   //tcp works with an ip (defines how data packets are send to each other b/w computers)
  } 
}
