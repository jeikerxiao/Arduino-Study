/**
* @par Copyright (C): 2024-2024, company
* @file         10、远程烟雾报警
* @author       jeiker
* @version      V1.0
* @date         2024.06.04
* @brief        AR套件微信实验
* @details
* @par History  见如下说明
*/
#define GS_Pin A0  // 烟雾传感器管脚，电压值0-1024。
#define LED_Pin 3  // 报警指示灯管脚


int incomingByte = 0;             // 接收到的 data byte
String inputString = "";          // 用来储存接收到的内容
boolean newLineReceived = false;  // 前一次数据结束标志
boolean startBit = false;         //协议开始标志


String returntemp = "";  //存储返回值
int g_GS = 0;
int g_GSThreshold = 50;

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
  pinMode(LED_Pin, OUTPUT);  //初始化LED IO口为输出方式
  Serial.begin(9600);        //波特率9600 （WIFI通讯设定波特率）
  printf_begin();            //初始化printf
}
/**
* Function       loop
* @author        jeiker
* @date          2024.06.04
* @brief         根据收到的协议设置烟雾报警浓度并决定是否报警，最后返回数据协议包
* @param[in]     void
* @retval        void
* @par History   无
*/

void loop() {
  int val = analogRead(GS_Pin);  //读取A0口的电压值并赋值到val

  g_GS = map(val, 0, 1024, 0, 100);  //0-1024 划分到 0-100
  if (g_GS > g_GSThreshold)          //如果检测到的烟雾浓度值大于收到微信界面发送时设定的烟雾浓度值
  {
    digitalWrite(LED_Pin, LOW);      //报警
    printf("$SMOKE-0-1-%d#", g_GS);  //打印当前烟雾浓度值
  } else                             //检测到的烟雾浓度值小于收到微信界面发送时设定的烟雾浓度值
  {
    digitalWrite(LED_Pin, HIGH);     //LED灭
    printf("$SMOKE-0-0-%d#", g_GS);  //打印当前烟雾浓度值
  }
  while (newLineReceived) {
    //  Serial.println(inputString);
    if (inputString.indexOf("SMOKE") == -1)  //如果要检索的字符串值“SMOKE”没有出现
    {
      returntemp = "$SMOKE-2-#";  //返回不匹配
      Serial.print(returntemp);   //返回协议数据包
      inputString = "";           // clear the string
      newLineReceived = false;    // 前一次数据结束标志
      break;
    }
    //$SMOKE-50-0-0#
    int i = inputString.indexOf("-", 7);  //从接收到的数据中以第7位为起始位置检索字符串“-”的位置
    if (i > 0)                            //如果检索到了
    {
      String temp = inputString.substring(7, i);  //提取字符串中介于指定下标7到i之间的字符赋值给temp
      g_GSThreshold = temp.toInt();               //将字符串temp转为整型赋值给g_GSThreshold
    }
    // Serial.println(g_GSThreshold);
    if (g_GS > g_GSThreshold)  //如果检测到的烟雾浓度值大于收到微信界面发送时设定的烟雾浓度值
    {
      digitalWrite(LED_Pin, LOW);      //LED亮（报警）
      printf("$SMOKE-0-1-%d#", g_GS);  //打印当前烟雾浓度值
    } else                             //检测到的烟雾浓度值小于收到微信界面发送时设定的烟雾浓度值
    {
      digitalWrite(LED_Pin, HIGH);     //LED灭
      printf("$SMOKE-0-0-%d#", g_GS);  //打印当前烟雾浓度值
    }

    inputString = "";  // clear the string
    newLineReceived = false;
  }
  delay(2000);
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
  while (Serial.available())  //如果串口接收到数据则进入循环
  {
    incomingByte = Serial.read();  //一个字节一个字节地读，下一句是读到的放入字符串数组中组成一个完成的数据包
    if (incomingByte == '$')       //如果到来的字节是'$'，开始读取
    {
      startBit = true;
    }
    if (startBit == true) {
      inputString += (char)incomingByte;  // 全双工串口可以不用在下面加延时，半双工则要加的//
    }
    if (incomingByte == '^')  //如果到来的字节是'^'，读取结束
    {
      newLineReceived = true;
      startBit = false;
    }
  }
}
