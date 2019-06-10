//String SSID = "winwin";
//String PASS = "tx54LrN$!";
/*
 
String SSID = "Mahmoud Ibrahim";
String PASS = "0553664875";
//String IP = "192.168.1.5" ;
String IP = "192.168.1.3";
String port = "8680";*/
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>

String fileData = "";
File myFile;
StaticJsonBuffer<1200> jsonBuffer;
String _SSID = "";
String PASS = "";
String IP = "";
String port = "";
int pinCS = 53; 
int i=0;
int _length;
int* analogSensorspins;
int* SensorID;
String* AnalogSensorsTitles;
float* AnalogSensorScale;
float* AnalogSensorB;

String ReadData = "";
boolean SDoperating = false;
void setup() {

  Serial2.begin(115200); // for arduino mega
  Serial.begin(115200); //for serial port
  pinMode(pinCS, OUTPUT);  // SD pin
  SDSetup();
  SDoperating = SDCheck();
  Serial2.println("AT+RST");
  adjustConfig();
  checkModuleResponse();
  setWifiMode();
  accesspointConnect();
  TransportModeSet();
  initiateTCP();

}


void Send2Api(String data) {
  //String data="sensorid=1&value=" ;
  //data+=analogRead(A0);;// data sent must be under this form //name1=value1&name2=value2.
  // String cmd = "GET /products/5b28dc0100d6c22d7c2ac9b0 HTTP/1.1\r\n Host: " + IP + "\r\n\r\n";
  //Serial.println(data);
  String cmd = "POST / HTTP/1.1\r\n" ;
  cmd += "Host: ";
  cmd += IP;
  cmd += ":";
  cmd += port;
  cmd += "\r\n";
  cmd += "Accept: *" ;
  cmd += "/" ;
  cmd += "*\r\n" ;
  cmd += "Content-Length: ";
  cmd += data.length() ;
  cmd += "\r\n" ;
  cmd += "Content-Type: application/x-www-form-urlencoded\r\n" ;
  cmd += "\r\n" + data;
  Serial.print("AT+CIPSEND=4,");//determine how many bytes
  Serial.println(cmd.length());
  Serial2.print("AT+CIPSEND=4,");
  Serial2.println(cmd.length());

  if (Serial2.find("OK"))
  {
    Serial.println("OK");
  } else {
    Serial.println("ERROR");
  }

  Serial2.print(cmd);

  //printResponse();
  String message = "";
  while (Serial2.available()) {
    //int inByte = Serial2.read();
    message += Serial2.readString();
  }
  Serial.println(message);
}

void loop() {
 ReadData=sensorRead();
 Serial.println(ReadData);
 initiateTCP();
 Send2Api(ReadData);
}
String txt2String(String fileName) {
  String fileData2 = "";
  myFile = SD.open(fileName);
  if (myFile) {
    // Reading the whole file
    while (myFile.available()) {
      fileData2 = fileData2 + String(char(myFile.read()));
    }
    //Serial.println(fileData2);
    myFile.close(); // close the file
    return fileData2;
  }
  else {
    return " couldn't read file" + fileData2;
  }
}
void adjustConfig() {
  //fileData = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  fileData = txt2String("config.txt");
  //Serial.println(fileData);
  JsonObject& root = jsonBuffer.parseObject(fileData);
  _SSID = root["SSID"].as<String>();
  PASS = root["PASS"].as<String>();
  IP = root["IP"].as<String>();
  port = root["port"].as<String>();
  JsonArray& myArray = root["analogSensorspins"];
  _length=myArray.size();
  analogSensorspins = new int[_length];
  SensorID = new int[_length];
  AnalogSensorsTitles = new String[_length];
  AnalogSensorScale= new float[_length];
  AnalogSensorB=new float[_length];
  for (int i = 0; i < _length; i++) {
    analogSensorspins[i] = root["analogSensorspins"][i];
    SensorID[i] = root["SensorID"][i];
    AnalogSensorsTitles[i] = root["SensorName"][i].as<String>();
    AnalogSensorScale[i] = root["ASF"][i].as<float>();
    AnalogSensorB[i]= root["ASB"][i].as<float>();
  }
}

void SDSetup() {

  // SD Card Initialization
  digitalWrite(pinCS, HIGH);
  delay(2000);
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    delay(5000);
    return;
  }
}

boolean SDCheck() {
  // Create/Open file
  myFile = SD.open("pincon.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    myFile.close(); // close the file
    Serial.println("SD card check is Done.");
    return true;
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("There's an error creating / opening files on this SD");
    return false;
  }
}

void initiateTCP() {
  while (Serial2.find("OK") == false  )
  {
    String initCmd = "AT+CIPSTART=4,\"TCP\",\"";
    initCmd += IP;
    initCmd += "\","+port;
    Serial2.println(initCmd);
    Serial.println(initCmd);
    delay(500);
    if ( Serial2.find("ALREADY CONNECTED") == true)
      break;
    // printResponse();
  }
  Serial.println("linked");
}



void Send2Api() {
/*

POST /products/create HTTP/1.1
Host: localhost:1234
Content-Type: application/x-www-form-urlencoded
Cache-Control: no-cache
Postman-Token: 33783efb-eccd-41c4-9646-e3af22e82a6f

name=maged2&price=150
*/
String data="name=AhmedSamy&price=" ;
data+=50;// data sent must be under this form //name1=value1&name2=value2.
// String cmd = "GET /products/5b28dc0100d6c22d7c2ac9b0 HTTP/1.1\r\n Host: " + IP + "\r\n\r\n";

String cmd ="POST / HTTP/1.1\r\n" ;
cmd +="Host: ";
cmd +=IP;
cmd+=":";
cmd+=port;
cmd+="\r\n";
cmd +="Accept: *" ;
cmd +="/" ;
cmd +="*\r\n" ;
cmd +="Content-Length: ";
cmd += data.length() ;
cmd +="\r\n" ;
cmd +="Content-Type: application/x-www-form-urlencoded\r\n" ;
cmd +="\r\n" + data;
  Serial.print("AT+CIPSEND=4,");//determine how many bytes
  Serial.println(cmd.length());
  Serial2.print("AT+CIPSEND=4,");
  Serial2.println(cmd.length());
  
  if (Serial2.find("OK"))
  {
    Serial.println("OK");
  } else {
    Serial.println("ERROR");
  }
 // delay(150);
  Serial2.print(cmd);
  /*delay(250);
Serial.print("listening");
delay(5500);*/

//printResponse();
String message="";
 while (Serial2.available()) {
    //int inByte = Serial2.read();
    message+= Serial2.readString();   
  }
Serial.println(message);
}

void SendMessage() {
  while (Serial2.find("OK") == false)
  {
    String page = "AT+CIPSTART=4,\"TCP\",\"";
    page += IP;
    page += "\","+port;
    Serial2.println(page);
    Serial.println(page);
    delay(7000);
  }
}
  String sensorRead(){

  String cmd="[";
  //String cmd="";
  delay(10);
  for(int i=0;i<_length;i++){
    float SensorValue=AnalogSensorScale[i]*analogRead(analogSensorspins[i])+AnalogSensorB[i];
    //float temp=0.488*SensorValue;
    float temp=SensorValue;
    delay(10);
    //cmd += "{DevID:001,physID:"+String(i)+",Type:"+AnalogSensorsTitles[i]+",ID:"+String(SensorID[i])+",Value:"+String(temp)+"},\n";
    //cmd += "{DevID:\""+String(SensorID[i])+"\",Type:\""+AnalogSensorsTitles[i]+"\",Value:\""+String(temp)+"\"},\n";
    cmd += "{\"SensorID\":\""+String(SensorID[i])+"\",\"Type\":\""+AnalogSensorsTitles[i]+"\",\"Value\":\""+String(temp)+"\"},\n"; 
  }
  cmd = cmd.substring(0, cmd.length() - 2);
  cmd +="]\n";
  //cmd +="\n";
 // Serial.println(cmd);
  return cmd;
  }

void checkModuleResponse() {
  Serial2.println("AT");
  delay(500);
  while (!(Serial2.find("OK"))) {
    Serial.println("Module hasn't responded yet.");
    Serial2.println("AT");
  }
  Serial.println("Module is ready");
  delay(30);
}

void setWifiMode() {
  Serial2.println("AT+CWMODE=1"); // Set module to station mode
  delay(10);
  while ((Serial2.find("OK") == false)) {
    Serial.println("Module mode hasn't been set yet.");
    Serial2.println("AT+CWMODE=1"); // Set module to station mode
    delay(10);
  }
  Serial.println("Mode set");
}

void TransportModeSet() {
  Serial2.println("AT+CIPMUX=1"); // Set TCP mode to 4 multiple channels
  delay(30);
  while ((Serial2.find("OK") < 1)) {
    Serial.println("TCP mode hasn't been set yet.");
    Serial2.println("AT+CIPMUX=1"); // SETTING m
    delay(10);
  }
  Serial.println("Transport Layer Mode is set");
}

void accesspointConnect() {
  while (Serial2.find("OK") < 1)
  {
    String wifi = "AT+CWJAP=\"";
    wifi += _SSID;
    wifi += "\",\"";
    wifi += PASS;
    wifi += "\"\r\n";
    Serial.println(wifi);
    Serial2.println(wifi);
    delay(5000);
  }
  Serial2.println("AT+CIFSR");
  printResponse();

  Serial.println("Connected");
}

void printResponse() {
  delay(4500);
  while (Serial2.available() > 0) {
    char c = Serial2.read();
    Serial.write(c);
    delay(2);
  }
}
