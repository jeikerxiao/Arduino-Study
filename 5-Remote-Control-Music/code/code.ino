/**
* @par Copyright (C): 2024-2024, company
* @file         5、远程音乐播放
* @author       jeiker
* @version      V1.0
* @date         2024.06.04
* @brief        AR套件微信实验
* @details
* @par History  见如下说明
*
*/
#include <MsTimer2.h>  //使用定时器2的库文件

int SPEAKER_Pin = 7;  //设置控制蜂鸣器播放音乐的数字IO脚
int I = 0;            //定义一个变量I



int incomingByte = 0;             // 接收到的 data byte
String inputString = "";          // 用来储存接收到的内容
boolean newLineReceived = false;  // 前一次数据结束标志
boolean startBit = false;         //协议开始标志


String returntemp = "";  //存储返回值
int g_Music_now = 0;     //存储当前正在唱的歌

/*所有音调*/
#define BL1 248
#define BL2 278
#define BL3 294
#define BL4 330
#define BL5 371
#define BL6 416
#define BL7 467

#define BA1 495
#define B2 556
#define B3 624
#define B4 661
#define B5 742
#define B6 833
#define B7 935

#define BH1 990
#define BH2 1112
#define BH3 1178
#define BH4 1322
#define BH5 1484
#define BH6 1665
#define BH7 1869

#define NTC1 262
#define NTC2 294
#define NTC3 330
#define NTC4 350
#define NTC5 393
#define NTC6 441
#define NTC7 495

#define NTCL1 131
#define NTCL2 147
#define NTCL3 165
#define NTCL4 175
#define NTCL5 196
#define NTCL6 221
#define NTCL7 248

#define NTCH1 525
#define NTCH2 589
#define NTCH3 661
#define NTCH4 700
#define NTCH5 786
#define NTCH6 882
#define NTCH7 990

#define NTD0 -1
#define NTD1 294
#define NTD2 330
#define NTD3 350
#define NTD4 393
#define NTD5 441
#define NTD6 495
#define NTD7 556

#define NTDL1 147
#define NTDL2 165
#define NTDL3 175
#define NTDL4 196
#define NTDL5 221
#define NTDL6 248
#define NTDL7 278

#define NTDH1 589
#define NTDH2 661
#define NTDH3 700
#define NTDH4 786
#define NTDH5 882
#define NTDH6 990
#define NTDH7 1112

#define NTE1 330
#define NTE2 350
#define NTE3 393
#define NTE4 441
#define NTE5 495
#define NTE6 556
#define NTE7 624

#define NTEL1 165
#define NTEL2 175
#define NTEL3 196
#define NTEL4 221
#define NTEL5 248
#define NTEL6 278
#define NTEL7 312

#define NTEH1 661
#define NTEH2 700
#define NTEH3 786
#define NTEH4 882
#define NTEH5 990
#define NTEH6 1112
#define NTEH7 1248

const int tune_1[] =  //根据简谱列出各频率
  {
    NTC1, NTC2, NTC3, NTC1,
    NTC1, NTC2, NTC3, NTC1,
    NTC3, NTC4, NTC5,
    NTC3, NTC4, NTC5,
    NTC5, NTC6, NTC5, NTC4, NTC3, NTC1,
    NTC5, NTC6, NTC5, NTC4, NTC3, NTC1,
    NTC2, NTCL5, NTC1,
    NTC2, NTCL5, NTC1
  };
const float durt_1[] =  //根据简谱列出各节拍
  {
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 2,
    1, 1, 2,
    0.75 /*(1+0.5)/2*/, 0.25, 0.75, 0.25, 1, 1,
    0.75 /*(1+0.5)/2*/, 0.25, 0.75, 0.25, 1, 1,
    1, 1, 2,
    1, 1, 2
  };

const int tune_2[] =  //根据简谱列出各频率
  {
    NTC5,
    NTC5,
    NTC6,
    NTC5,
    NTCH1,
    NTC7,
    NTC5,
    NTC5,
    NTC6,
    NTC5,
    NTCH2,
    NTCH1,
    NTC5,
    NTC5,
    NTCH5,
    NTCH3,
    NTCH1,
    NTC7,
    NTC6,
    NTCH4,
    NTCH4,
    NTCH3,
    NTCH1,
    NTCH2,
    NTCH1,
  };
const float durt_2[] =  //根据简谱列出各节拍
  {
    0.5,
    0.5,
    1,
    1,
    1,
    2,
    0.5,
    0.5,
    1,
    1,
    1,
    2,
    0.5,
    0.5,
    1,
    1,
    1,
    1,
    2,
    0.5,
    0.5,
    1,
    1,
    1,
    2,
  };

const int tune_3[] =  //根据简谱列出各频率
  {
    B3,
    BA1,
    B2,
    BL6,
    B3,
    B2,
    BA1,
    B2,
    BL6,
    B3,
    BA1,
    B2,
    B2,
    B5,
    B3,
    BL7,
    BA1,
    BA1,
    BL7,
    BL6,
    BL7,
    BA1,
    B2,
    BL5,
    B6,
    B5,
    B3,
    B3,
    B2,
    BA1,
    B2,
    B3,
    B2,
    B3,
    B2,
    B3,
    B5,
    B5,
    B5,
    B5,
    B5,
    B5,
    B5,
  };
const float durt_3[] =  //根据简谱列出各节拍
  {
    1,
    1,
    1,
    1,
    0.5,
    0.5,
    0.5,
    0.5,
    2,
    1,
    1,
    1,
    1,
    0.5,
    0.5,
    1,
    1,
    0.5,
    0.5,

    1,
    0.5,
    0.5,
    1,
    1,
    0.5,
    0.5,
    1,
    1.5,
    0.5,
    1,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.25,
    0.25,
  };

const int tune_4[] =  //根据简谱列出各频率
  {
    NTC3,
    NTC5,
    NTC5,
    NTC3,
    NTC6,
    NTC6,
    NTC7,
    NTC6,
    NTC6,
    NTC6,
    NTC5,
    NTCH1,
    NTCH1,
    NTCH1,
    NTCH1,
    NTC6,
    NTCH1,
    NTC6,
    NTC5,

    NTC3,
    NTC5,
    NTC5,
    NTC5,
    NTC3,
    NTC6,
    NTC6,
    NTC7,
    NTC6,
    NTC6,
    NTC6,
    NTC5,
    NTCH1,
    NTCH1,
    NTCH1,
    NTCH1,
    NTC6,
    NTC6,
    NTCH1,
    NTCH2,

    NTCH5,
    NTCH5,
    NTCH5,
    NTCH5,
    NTCH5,
    NTCH3,
    NTCH2,
    NTCH1,
    NTCH1,
    NTC6,
    NTCH1,
    NTC6,
    NTCH1,
    NTCH2,
    NTCH2,
    NTCH2,
    NTCH2,
    NTCH2,
    NTCH1,
    NTCH3,
    NTCH2,
    NTCH2,

    NTCH3,
    NTCH3,
    NTCH3,
    NTCH3,
    NTCH2,
    NTCH2,
    NTCH1,
    NTCH1,
    NTCH1,
    NTCH2,
    NTCH1,
    NTC6,
    NTC5,
    NTC5,
    NTC5,
    NTC5,
    NTC6,
    NTC5,
    NTCH2,
    NTCH3,
    NTCH1,

  };
const float durt_4[] =  //根据简谱列出各节拍
  {
    0.5,
    0.5,
    1.5,
    0.5,
    0.5,
    0.5,
    0.5,
    1,
    0.5,
    0.5,
    0.5,
    1,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    2,

    0.5,
    0.5,
    1,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    1,
    0.5,
    0.5,
    0.5,
    1,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    2,
    0.5,
    1,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    1,
    1,
    0.5,
    0.5,
    0.5,
    1,
    0.25,
    0.5,
    0.5,
    0.5,
    0.5,
    1,
    0.25,
    2,

    0.5,
    1,
    0.5,
    0.5,
    0.5,
    1,
    0.5,
    1,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    1,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    2,
  };

// const int tune_5[]=                 //根据简谱列出各频率
//{
//B5, B3, B5, BH1, B6, BH1, B5,
// B5, BA1, B2, B3, B2, BA1, B2,
//B5, B3, B5,BH1, B7, B6, BH1, B5,
// B5, B2, B3, B4, BL7, BA1,
//B6, BH1, BH1, B7, B6, B7, BH1,
// B6, B7, BH1, B6,B6, B5, B3, BA1, B2,
//};
//const float durt_5[]=                   //根据简谱列出各节拍
//{
//1, 0.5, 0.5, 2, 1, 1, 2,
//1, 0.5, 0.5, 1, 0.5, 0.5, 2,
//1, 0.5, 0.5, 2, 0.5, 1, 1, 2,
//1, 0.5, 0.5, 2, 0.5, 2,
//1, 1, 2, 1, 0.5, 0.5, 2,
//0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2,
//};
//
//const int tune_6[]=                 //根据简谱列出各频率
//{
//NTC3, NTC3, NTCH1, NTC7, NTCH1, NTC3, NTC3,
//NTC3, NTC5, NTC5, NTC6, NTC4,
// NTC2, NTC2, NTC7, NTC6, NTC7, NTC2, NTC2,
// NTC2, NTC5, NTC4, NTC4, NTC4, NTC5, NTC3,
//NTC3, NTC3, NTCH1, NTC7, NTCH1, NTCH1, NTC3,
//NTC3, NTC6, NTCH1, NTCH3, NTCH2, NTCH2,
//NTCH1, NTCH1, NTCH1, NTC7, NTCH2,NTCH1, NTCH1, NTC7,NTC7,
//NTC3, NTC3, NTCH1, NTC7, NTCH1, NTC3, NTC3, NTC3, NTC5, NTC5, NTC6, NTC4,
//NTC2, NTC2, NTC7, NTC6, NTC7, NTC2, NTC2, NTC2, NTC5, NTC4, NTC4, NTC4, NTC5, NTC3,
//};
//const float durt_6[]=                   //根据简谱列出各节拍
//{
//0.25, 0.25, 0.5, 0.5, 1.25, 0.5, 1,
// 0.5, 0.5, 1.25, 0.5, 0.5,
//0.25, 0.25, 0.5, 0.5, 1.25, 0.5, 0.75,
//0.25, 0.5, 0.5, 1, 0.5, 0.25, 0.25,
//0.25, 0.25, 0.5, 0.5, 0.75, 0.5, 0.5,
//0.5, 0.5, 0.5, 1, 0.5, 1.25,
//0.5, 0.75, 0.5, 1, 0.5, 0.25, 0.25, 0.5, 0.5,
//0.25, 0.25, 0.5, 0.5, 1.25,0.5, 1,
//0.5, 0.5, 1.25,  0.5, 0.5,
//0.25, 0.25, 0.5, 0.5, 1.25, 0.5, 0.75,
//0.25, 0.5, 0.5, 1, 0.25, 0.25, 0.5,
//0.25, 0.25, 0.5, 0.5,
//};



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
  pinMode(SPEAKER_Pin, OUTPUT);  //蜂鸣器初始化为输出模式
  pinMode(13, OUTPUT);

  Serial.begin(9600);  //波特率9600 （蓝牙通讯设定波特率）

  digitalWrite(SPEAKER_Pin, HIGH);
  MsTimer2::set(500, analysis);  // 中断设置函数，每 500ms 进入一次中断
  MsTimer2::start();             //开始计时
}

/**
* Function       loop
* @author        jeiker
* @date          2024.06.04
* @brief         按照接收的数据根据协议完成相应的功能 
* @param[in]     void
* @retval        void
* @par History   无
*/
void loop() {
  //$MUSIC-4-1^
  switch (g_Music_now)  //解析协议后判断当前播放哪首歌曲
  {
    case 0: digitalWrite(SPEAKER_Pin, HIGH); break;  //蜂鸣器置高电平
    case 1:
      Play1();
      g_Music_now = 0;
      break;  //播放歌曲1，播放后蜂鸣器置高电平
    case 2:
      Play2();
      g_Music_now = 0;
      break;  //播放歌曲2，播放后蜂鸣器置高电平
    case 3:
      Play3();
      g_Music_now = 0;
      break;  //播放歌曲3，播放后蜂鸣器置高电平
    case 4:
      Play4();
      g_Music_now = 0;
      break;         //播放歌曲4，播放后蜂鸣器置高电平
    default: break;  //如果g_Music_now等于其他的值则停止
  }
}

/**
* Function       Play1
* @author        jeiker
* @date          2024.06.04
* @brief         播放歌曲1
* @param[in]     void
* @retval        void
* @par History   无
*/

void Play1() {
  int length = sizeof(tune_1) / sizeof(tune_1[0]);  //计算长度
  for (int x = 0; x < length; x++) {
    serialEvent();                //调用串口读写
    if (newLineReceived == true)  //如果串口接收一个完整的数据包
    {
      analysis();  //协议解析
    }
    if (g_Music_now != 1)  //如果不播放歌曲1
    {
      return;  //返回主函数
    }
    tone(SPEAKER_Pin, tune_1[x]);
    delay(500 * durt_1[x]);  //这里用来根据节拍调节延时，500这个指数可以自己调整，在该音乐中，我发现用500比较合适。
    noTone(SPEAKER_Pin);
  }
}

/**
* Function       Play1
* @author        jeiker
* @date          2024.06.04
* @brief         播放歌曲2
* @param[in]     void
* @retval        void
* @par History   无
*/

void Play2() {

  int length = sizeof(tune_2) / sizeof(tune_2[0]);  //计算长度
  for (int x = 0; x < length; x++) {
    serialEvent();                //调用串口读写
    if (newLineReceived == true)  //如果串口接收一个完整的数据包
    {
      analysis();  //协议解析
    }
    if (g_Music_now != 2)  //如果不播放歌曲2
    {
      return;  //返回主函数
    }
    tone(SPEAKER_Pin, tune_2[x]);
    delay(500 * durt_2[x]);  //这里用来根据节拍调节延时，500这个指数可以自己调整，在该音乐中，我发现用500比较合适。
    noTone(SPEAKER_Pin);
  }
}
/**
* Function       Play1
* @author        jeiker
* @date          2024.06.04
* @brief         播放歌曲3
* @param[in]     void
* @retval        void
* @par History   无
*/
void Play3() {
  int length = sizeof(tune_3) / sizeof(tune_3[0]);  //计算长度
  for (int x = 0; x < length; x++) {
    serialEvent();                //调用串口读写
    if (newLineReceived == true)  //如果串口接收一个完整的数据包
    {
      analysis();  //协议解析
    }
    if (g_Music_now != 3)  //如果不播放歌曲3
    {
      return;  //返回主函数
    }
    tone(SPEAKER_Pin, tune_3[x]);
    delay(500 * durt_3[x]);  //这里用来根据节拍调节延时，500这个指数可以自己调整，在该音乐中，我发现用500比较合适。
    noTone(SPEAKER_Pin);
  }
}
/**
* Function       Play1
* @author        jeiker
* @date          2024.06.04
* @brief         播放歌曲4
* @param[in]     void
* @retval        void
* @par History   无
*/
void Play4() {
  int length = sizeof(tune_4) / sizeof(tune_4[0]);  //计算长度
  for (int x = 0; x < length; x++) {
    serialEvent();                //调用串口读写
    if (newLineReceived == true)  //如果串口接收一个完整的数据包
    {
      analysis();  //协议解析
    }
    if (g_Music_now != 4)  //如果不播放歌曲4
    {
      return;  //返回主函数
    }
    tone(SPEAKER_Pin, tune_4[x]);
    delay(500 * durt_4[x]);  //这里用来根据节拍调节延时，500这个指数可以自己调整，在该音乐中，我发现用500比较合适。
    noTone(SPEAKER_Pin);
  }
}


//void Play5()
//{
//
//
//int length = sizeof(tune_5)/sizeof(tune_5[0]);   //计算长度
// for(int x=0; x < length;x++)
//  {
//    tone( SPEAKER_Pin,tune_5[x]);
//    delay(500*durt_5[x]);   //这里用来根据节拍调节延时，500这个指数可以自己调整，在该音乐中，我发现用500比较合适。
//    noTone( SPEAKER_Pin);
//  }
//
//
//
//}
//
//
//void Play6()
//{
//
//int length = sizeof(tune_6)/sizeof(tune_6[0]);   //计算长度
// for(int x=0; x < length;x++)
//  {
//    tone(SPEAKER_Pin,tune_6[x]);
//    delay(500*durt_6[x]);   //这里用来根据节拍调节延时，500这个指数可以自己调整，在该音乐中，我发现用500比较合适。
//    noTone(SPEAKER_Pin);
//  }
//}


/**
* Function       analysis
* @author        jeiker
* @date          2024.06.04
* @brief         协议分析
* @param[in]     void
* @retval        void
* @par History   无
*/
void analysis() {
  digitalWrite(13, HIGH);  //板载LED灯亮
  while (newLineReceived)  //当串口接受完一个完整的数据包
  {
    if (inputString.indexOf("MUSIC") == -1)  //如果要检索的字符串值“MUSIC”没有出现
    {
      returntemp = "$MUSIC-2,#";  //返回不匹配
      Serial.print(returntemp);   //返回协议数据包
      inputString = "";           // clear the string
      newLineReceived = false;
      break;
    }

    //解析开关
    int i = inputString.indexOf("^", 0);  //从接收的数据中第0位开始检索"^"的位置
    if (i != -1)                          //如果检索到了
    {
      if (inputString[i - 1] == '1')  //如果"#"的前面一位字符串值为'1'
      {
        if (inputString[7] == '1')  //如果接收的数据第七位字符串值为1
        {
          int i = inputString.indexOf("-");      //从接收到的数据中检索“，”出现的位置
          int ii = inputString.indexOf("^", i);  //从接收到的数据中以i为起始位置检索字符串“#”的位置
          if (ii > i && ii > 0 && i > 0)         //如果ii和i的顺序对了并且检索到ii与i存在
          {
            g_Music_now = 1;
          }
        }
        if (inputString[7] == '2')  //如果接收的数据第七位字符串值为2
        {
          int i = inputString.indexOf("-");      //从接收到的数据中检索“，”出现的位置
          int ii = inputString.indexOf("^", i);  //从接收到的数据中以i为起始位置检索字符串“#”的位置
          if (ii > i && ii > 0 && i > 0)         //如果ii和i的顺序对了并且检索到ii与i存在
          {
            g_Music_now = 2;
          }
        }
        if (inputString[7] == '3')  //如果接收的数据第七位字符串值为3
        {
          int i = inputString.indexOf("-");      //从接收到的数据中检索“，”出现的位置
          int ii = inputString.indexOf("^", i);  //从接收到的数据中以i为起始位置检索字符串“#”的位置
          if (ii > i && ii > 0 && i > 0)         //如果ii和i的顺序对了并且检索到ii与i存在
          {
            g_Music_now = 3;
          }
        }
        if (inputString[7] == '4')  //如果接收的数据第七位字符串值为4
        {
          int i = inputString.indexOf("-");      //从接收到的数据中检索“，”出现的位置
          int ii = inputString.indexOf("^", i);  //从接收到的数据中以i为起始位置检索字符串“#”的位置
          if (ii > i && ii > 0 && i > 0)         //如果ii和i的顺序对了并且检索到ii与i存在
          {
            g_Music_now = 4;
          }
        }
      } else  //“#”的前面一位字符串值不为'1'
      {
        g_Music_now = 0;
      }
    }

    //returntemp = "$MUSIC,0,#"; //返回匹配成功

    //  Serial.print(returntemp); //返回协议数据包
    inputString = "";  // clear the string
    newLineReceived = false;
  }
  digitalWrite(13, LOW);  //板载LED灯灭
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
  digitalWrite(13, HIGH);
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
    if (incomingByte == '^')  //如果到来的字节是'#'，读取结束
    {
      newLineReceived = true;
      startBit = false;
    }
  }
}
