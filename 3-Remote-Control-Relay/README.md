# 1-Remote-Control-LED

远程控制 LED

## 接线图

![remote-control-led](./doc/Remote-Control-LED.jpg)

## 代码

```c
//定义引脚,高电平-开关闭合,低电平-形状打开；视模块跳线帽而定
int RELAY_Pin = 7;  //继电器信号端S

int incomingByte = 0;             // 接收到的 data byte
String inputString = "";          // 用来储存接收到的内容
boolean newLineReceived = false;  // 前一次数据结束标志
boolean startBit = false;         //协议开始标志

String returntemp = "";  //存储返回值

void setup() {
  pinMode(RELAY_Pin, OUTPUT);  //初始化RELAY IO口为输出方式

  Serial.begin(9600);  //波特率9600 （Wifi通讯设定波特率）

  digitalWrite(RELAY_Pin, LOW);  //RELAY初始化低电平
}
/**
* Function       loop
* @brief         按照接收的数据根据协议完成相应的功能 
*/
void loop() {
  // $RELAY-1^
  // 使用 serialEvent 函数异步读取串口数据，并根据协议格式 $...^ 来判断是否接收到完整数据包。
  // 将处理结果通过串口返回给发送端，返回的数据包格式为 $RELAY-<status>#。
  // 协议说明：
  // 命令格式：$RELAY-<1/0>^
  // 命令示意：$RELAY-1^
  // $ 表示数据包开始。
  // RELAY 表示控制继电器的命令。
  // -1 表示关闭继电器，-0 表示打开继电器。
  // ^ 表示数据包结束。
  while (newLineReceived) {
    if (inputString.indexOf("RELAY") == -1)  //如果要检索的字符串值“RELAY”没有出现
    {
      returntemp = "$RELAY-2,#";  //返回不匹配
      Serial.print(returntemp);   //返回协议数据包
      inputString = "";           // clear the string
      newLineReceived = false;
      break;
    }

    if (inputString[7] == '1')  //继电器开关闭合
    {
      int i = inputString.indexOf("-");      //从接收到的数据中检索字符串“-”出现的位置
      int ii = inputString.indexOf("^", i);  //从接收到的数据中以i为起始位置检索字符串“^”的位置
      if (ii > i && ii > 0 && i > 0)         //如果ii和i的顺序对了并且检索到ii与i存在
      {
        digitalWrite(RELAY_Pin, HIGH);  //向继电器S信号端输出高电平，继电器开关闭合
        returntemp = "$RELAY-1#";       //返回匹配失败
        Serial.print(returntemp);       //返回协议数据包
        inputString = "";               // clear the string
      } else                            //检索到ii和i的顺序不对或者没有检索到ii或者i
      {
        //            returntemp = "$RELAY-1#";  //返回匹配失败
        //            Serial.print(returntemp); //返回协议数据包
        //            inputString = "";   // clear the string
        //            newLineReceived = false;
        //            break;
      }
    } else  //继电器开关打开
    {
      digitalWrite(RELAY_Pin, LOW);  //向继电器S信号端输出低电平
      returntemp = "$RELAY-0#";      //返回匹配成功
      Serial.print(returntemp);      //返回协议数据包
    }
    inputString = "";  // clear the string
    newLineReceived = false;
  }
}
/**
* Function       serialEvent
* @brief         串口接收中断
*/
void serialEvent() {
  // 使用 serialEvent 函数异步读取串口数据，并根据协议格式 $...^ 来判断是否接收到完整数据包。
  // 将处理结果通过串口返回给发送端，返回的数据包格式为 $RELAY-<status>#。
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
    if (incomingByte == '^') {
      newLineReceived = true;  //如果到来的字节是'^'，读取结束
      startBit = false;
    }
  }
}

```