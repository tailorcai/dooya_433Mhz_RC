// use special version of rcswitch
#include "RCSwitch.h"
RCSwitch myFa = RCSwitch();//定义发送端
RCSwitch myShou = RCSwitch();//定义接收端
unsigned long i = 0; //注意，这里的数据类型，不能用int

void setup(){
  myFa.enableTransmit(7);//发送端接6号口（或其它口）
  myFa.setProtocol(4);
 Serial.begin(9600);//打开串口调试

}

void loop(){
 i = i+1;
//  digitalWrite(7,LOW);
//  myFa.send(i, 24);//发送端发送数据，i为数字
 if( i==1 ) {
  // myFa.sendQuadState("0011010111F1F0000101");
  myFa.sendQuadState("0011010111F1F0000110");
  Serial.print("sending...");
  delay(8000);
  myFa.sendQuadState("0011010111F1F000FFFF");
  Serial.println( i );
 }
}
