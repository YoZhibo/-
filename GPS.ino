#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include <SoftwareSerial.h>

ESP8266WebServer server(80); //服务器
SoftwareSerial gps(D1, D2);  //
//char[100] ch;

void connect_wifi() {
  ESP8266WiFiMulti wifiMulti;
  wifiMulti.addAP("Redmi Note 10 Pro","abababab");  //通过addAp函数存储  WiFi名称 WiFi密码
  wifiMulti.addAP("west2","123456"); 
  wifiMulti.addAP("TP-LINK_FF7A", "123456");

  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数存储的WiFi。如果有多个，
    Serial.print(i++); Serial.print(' ');    // 将会连接信号最强的那一个WiFi信号。
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。
                                             // 用于判断是否跳出while循环。
 
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');                     // WiFi连接成功后
  Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址
}


void setup() {
  delay(1000);
  Serial.begin(9600);
  gps.begin(9600);   //gps模块
  connect_wifi();
  server.begin();
  Serial.println("HTTP server started");
}

char GGA[200];
void handleRoot() {
  //get the byte data from the GPS
  //Serial.write(gpsData);   //在串口查看
  server.send(200, "text/plain", GGA); //send(响应状态码, 响应内容类型（plain纯文本）, 响应内容)
}

int len=0;
char Data[1000];
int i=0;

void loop() {
  server.handleClient(); //检查有没有客户端设备通过网络向ESP8266网络服务器发送请求
  //delay(1000);
  if(gps.available() > 0){ //
    //get the byte data from the GPS
//    char gpsData = gps.read();
//    Serial.write(gpsData);   //在串口查看

    Data[i] = gps.read();
    i = (i+1)%200;
    //Serial.print(Data[i]);
       //Serial.write(Data[i]);   //在串口查看
       //if(Data[i]=='\n'){
         //Serial.print("Connected to ");
         //Serial.write('\n');   
         //break;       
  }

  if(i==199){
    int t=0;
    for(int j=0;j<200;j++){
      if(Data[j]=='G' && Data[j+1]=='G' && Data[j+2]=='A'){
        for(int k=j;k<j+40;k++){
          if(k==200) break;
          Serial.print(Data[k]);
          GGA[t]=Data[k];
          t++;
        }
        Serial.println();
        server.on("/",handleRoot); //on监听服务，每当有客户端向服务器发送HTTP请求时，我们可以利用on函数来设置HTTP请求回调函数。
        delay(3000);
        break;
      }
    }
  }

//
  
}
