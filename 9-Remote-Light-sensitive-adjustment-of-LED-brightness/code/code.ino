/**
* @par Copyright (C): 2024-2024, company
* @file         9、远程光敏控制LED调节不同光照灯的亮度
* @author       jeiker
* @version      V1.0
* @date         2024.06.04
* @brief        AR套件微信实验
* @details
* @par History  见如下说明
*/
#define GM_Pin A0  //光敏管脚，电压值：10000-0
#define LED_Pin 3  // LED管脚


int incomingByte = 0;             // 接收到的 data byte
String inputString = "";          // 用来储存接收到的内容
boolean newLineReceived = false;  // 前一次数据结束标志
boolean startBit = false;         //协议开始标志


String returntemp = "";  //存储返回值
int g_GM = 0;
int g_LT = 0;

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
  //pinMode(GM_Pin, INPUT_PULLUP);
  pinMode(LED_Pin, OUTPUT);  //初始化LED IO口为输出方式

  Serial.begin(9600);  //波特率9600 （WIFI通讯设定波特率）

  printf_begin();  //初始化printf
}
/**
* Function       loop
* @author        jeiker
* @date          2024.06.04
* @brief         根据光敏电阻读取到的数值控制LED调节不同光照灯的亮度
* @param[in]     void
* @retval        void
* @par History   无
*/

void loop() {
  int val = analogRead(GM_Pin);  //读取A0口的电压值并赋值到val
  /* AD: x: 0~1024   Lux: y: 10000~0
   y = -(10000/1024) x + 10000;
   y = -9.76x + 10000; 约等于y = -10x+10000   */
  g_GM = -10 * val + 10000;
  g_LT = map(g_GM, 10000, 0, 0, 255);       //10000-0 划分到 0-255
  analogWrite(LED_Pin, 255 - g_LT);         //将划分好后的值模拟写入LED管脚
                                            // while (1)//newLineReceived)
                                            // {
                                            //       if(inputString.indexOf("LTCTR") == -1)    //如果要检索的字符串值“LTCTR”没有出现
                                            //       {
                                            //           returntemp = "$LTCTR-2,#";  //返回不匹配
                                            //           Serial.print(returntemp); //返回协议数据包
                                            //           inputString = "";   // clear the string
                                            //           newLineReceived = false; // 前一次数据结束标志
                                            //           break;
                                            //       }
  g_LT = map(g_LT, 0, 255, 100, 0);         //0-255 划分到 0-100
  printf("$LTCTR-LX%d-LT%d#", g_GM, g_LT);  //打印光强和亮度
  inputString = "";                         // clear the string
  delay(3000);
  //   newLineReceived = false;
  //   }
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
//void serialEvent()
//{
//  while (Serial.available())                   //如果串口接收到数据则进入循环
//  {
//    incomingByte = Serial.read();              //一个字节一个字节地读，下一句是读到的放入字符串数组中组成一个完成的数据包
//    if(incomingByte == '$')                    //如果到来的字节是'$'，开始读取
//    {
//      startBit= true;
//    }
//    if(startBit == true)
//    {
//       inputString += (char) incomingByte;     // 全双工串口可以不用在下面加延时，半双工则要加的//
//    }
//    if (incomingByte == '^')                  //如果到来的字节是'#'，读取结束
//    {
//       newLineReceived = true;
//       startBit = false;
//    }
//  }
//}
