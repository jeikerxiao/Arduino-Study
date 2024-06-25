/**
* @par Copyright (C): 2024-2024, company
* @file         17、远程超声波测距
* @author       jeiker
* @version      V1.0
* @date         2024.06.04
* @brief        AR套件微信实验
* @details
* @par History  见如下说明
*
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  //0x27   0x3F

#define LED_Pin 7  // 报警指示灯管脚

#define TRIGGER_PIN 2                                // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 3                                   // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400                             // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  // NewPing setup of pins and maximum distance.

int incomingByte = 0;             // 接收到的 data byte
String inputString = "";          // 用来储存接收到的内容
boolean newLineReceived = false;  // 前一次数据结束标志
boolean startBit = false;         // 协议开始标志


String returntemp = "";  //存储返回值
int g_UTL = 0;
int g_UTLThreshold = 103;

/*printf格式化字符串初始化*/
int serial_putc(char c, struct __file*) {
  Serial.write(c);
  return c;
}
void printf_begin(void) {
  fdevopen(&serial_putc, 0);
}
/**
* Function       setup
* @author        jeiker
* @date          2024.06.04
* @brief         初始化配置
* @param[in]     void
* @retval        void
* @par History   无
*/
void setup() {
  // 初始化LED IO口为输出方式
  pinMode(LED_Pin, OUTPUT);    //初始化LED IO口为输出方式
  digitalWrite(LED_Pin, HIGH); //LED灭
  // 初始化LCD
  lcd.init();                  //初始化LCD
  lcd.backlight();             //打开背光
  // 初始化串口
  Serial.begin(9600);          //波特率9600 （WIFI通讯设定波特率）
  printf_begin();              //初始化printf
}
/**
* Function       loop
* @author        jeiker
* @date          2024.06.04
* @brief         根据收到的协议设置超声波报警距离并决定是否报警，最后返回数据协议包
* @param[in]     void
* @retval        void
* @par History   无
*/

void loop() {
  bool retry = true;
  while (retry) {
    delay(100);
    unsigned int uS = sonar.ping(); // 发送超声波并获取回声时间
    g_UTL = uS / US_ROUNDTRIP_CM;   // 将回声时间转换为距离值（单位：厘米）
    if (g_UTL != 0) {
      retry = false;
    }
  }

  // 设置LCD的显示内容
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(0, 1);
  lcd.print("             ");
  lcd.setCursor(9, 1);
  lcd.print(g_UTL);
  lcd.setCursor(12, 1);
  lcd.print("cm");

  if (g_UTL < g_UTLThreshold)  //如果检测到的超声波距离值小于收到设定的超声波距离值
  {
    digitalWrite(LED_Pin, LOW);                      //LED亮
    printf("$UTL-%d-%d-1#", g_UTL, g_UTLThreshold);  //打印超声波距离值和阈值
  } else                       //检测到的超声波距离值大于收到设定的超声波距离值
  {
    digitalWrite(LED_Pin, HIGH);                     //LED灭
    printf("$UTL-%d-%d-0#", g_UTL, g_UTLThreshold);  //打印超声波距离值和阈值
  }
  // 处理接收到的数据
  while (newLineReceived) {
    //    Serial.println(inputString);
    if (inputString.indexOf("UTL") == -1)  //如果要检索的字符串值“UTL”没有出现
    {
      returntemp = "$UTL-2#";    // 返回不匹配
      Serial.print(returntemp);  // 返回协议数据包
      inputString = "";          // 清空字符串
      newLineReceived = false;   // 前一次数据结束标志
      break;
    }
    //$UTL-XXX-123-X^
    int i = inputString.indexOf("-", 9);  // 从接收到的数据中以第9位为起始位置检索字符串“-”的位置
    if (i > 0)                            // 如果检索到了
    {
      String temp = inputString.substring(9, i);  //提取字符串中介于指定下标9到i之间的字符赋值给temp
      g_UTLThreshold = temp.toInt();              //将字符串temp转为整型赋值给g_GSThreshold
    }

    if (g_UTL < g_UTLThreshold)     //如果检测到的超声波距离值小于收到设定的超声波距离值
    {
      digitalWrite(LED_Pin, LOW);   //LED亮（报警）
      printf("$UTL-%d-%d-1#", g_UTL, g_UTLThreshold);
    } else                          //检测到的超声波距离值大于收到设定的超声波距离值
    {
      digitalWrite(LED_Pin, HIGH);  //LED灭(关闭报警)
      printf("$UTL-%d-%d-0#", g_UTL, g_UTLThreshold);
    }

    inputString = "";               //清空字符串
    newLineReceived = false;        //前一次数据结束标志
  }
  delay(2000);  // 延时2秒
}
/**
* Function       serialEvent
* @author        jeiker
* @date          2024.06.04
* @brief         串口接收中断   
* @param[in]     void
* @retval        void
* @par History   无
*/
void serialEvent() {
  bool isReadingPacket = false;    // 是否正在读取数据包
  while (Serial.available()) {     // 如果串口接收到数据则进入循环
    char incomingByte = Serial.read(); // 读取一个字节
    if (incomingByte == '$') {     // 如果到来的字节是'$'，开始读取数据包
      isReadingPacket = true;
      inputString = "";            // 重置字符串以开始新的数据包
    }
    if (isReadingPacket) {         // 如果正在读取数据包
      inputString += (char)incomingByte; // 将字节添加到数据包字符串中
    }
    if (incomingByte == '^') {     // 如果到来的字节是'^'，结束读取数据包
      isReadingPacket = false;
      newLineReceived = true;     // 标记数据包接收完成    
  }
}
