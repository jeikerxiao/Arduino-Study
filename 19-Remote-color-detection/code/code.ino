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
#include <TimerOne.h>  //the header file

#define S0     3   // Please notice the Pin's define
#define S1     4  //Combination of S0 and S1 decision output signal frequency scaling factor
#define S2     5  //The combination of S2 and S3 decided to let the red, green, blue, what kind of light through a filter
#define S3     6  
#define OUT    2  


int incomingByte = 0;                    // 接收到的 data byte
String inputString = "";                 // 用来储存接收到的内容
boolean newLineReceived = false;         // 前一次数据结束标志
boolean startBit  = false;               //协议开始标志


String returntemp = "";           //存储返回值 
int g_UTL = 0;
int g_UTLThreshold = 103;


int   g_count = 0;    // count the frequecy
int   g_array[3];     // store the RGB value
int   g_flag = 0;     // filter of RGB queue
float g_SF[3];        // save the RGB Scale factor

// Init TSC230 and setting Frequency.
void TSC_Init()
{
 //set the pins as OUTPUT
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  //OUTPUT Frequency Scaling 2%
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH); 
}

// Select the filter color
void TSC_FilterColor(int Level01, int Level02)
{
  if(Level01 != 0)
    Level01 = HIGH;

  if(Level02 != 0)
    Level02 = HIGH;

  digitalWrite(S2, Level01); 
  digitalWrite(S3, Level02); 
}
//interrupt function, calculate TCS3200 output signal of pulse number
void TSC_Count()
{
  g_count ++ ;
}
/*
name:TSC_Callback()
function:Timer interrupt function, each 1 s after the interruption, the time of the red, green, blue three kinds of light through a filter
 TCS3200 output signal of pulse number stored in the array g_array[3] in the corresponding element variables 
 */
void TSC_Callback()
{
  switch(g_flag)
  {
  case 0:
//    Serial.println("->WB Start");
    TSC_WB(LOW, LOW);              //Filter output Red         
    break;
  case 1:
 //   Serial.print("->Frequency R=");
 //   Serial.println(g_count);
    g_array[0] = g_count;  //Stored within 1 s red light through the filter, TCS3200 output pulse number
    TSC_WB(HIGH, HIGH);            //Filter output Green        
    break;    
  case 2:
 //   Serial.print("->Frequency G=");        
 //   Serial.println(g_count);
    g_array[1] = g_count;  //Stored within 1s green light through the filter, TCS3200 output pulse number
    TSC_WB(LOW, HIGH);             //Filter output Blue
    break; 
  case 3:
   // Serial.print("->Frequency B=");
   // Serial.println(g_count);
   // Serial.println("->WB End");
    g_array[2] = g_count;  //Stored within 1s blue light through the filter, TCS3200 output pulse number
    TSC_WB(HIGH, LOW);             //Clear(no filter)   
    break;
  default:     
    g_count = 0;  //clear count      
    break; 
  }
} 

void TSC_WB(int Level0, int Level1)  //White Balance
{
  g_count = 0;
  g_flag ++;
  TSC_FilterColor(Level0, Level1);
  Timer1.setPeriod(1000000);//1000000 microseconds(1s)
}



/*printf格式化字符串初始化*/
int serial_putc( char c, struct __file * )
{
  Serial.write( c );
  return c;
}
void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
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
void setup()
{
  TSC_Init();
  Timer1.initialize();             // default is 1s
  Timer1.attachInterrupt(TSC_Callback);  
  attachInterrupt(0, TSC_Count, RISING);   
  delay(4000); 
  
//  for(int i=0; i<3; i++)
//    Serial.println(g_array[i]);
//    
  g_SF[0] = 255.0/ g_array[0];     //R Scale factor
  g_SF[1] = 255.0/ g_array[1] ;    //G Scale factor
  g_SF[2] = 255.0/ g_array[2] ;    //B Scale factor
//  
//  Serial.println(g_SF[0]);
//  Serial.println(g_SF[1]);
//  Serial.println(g_SF[2]); 
  
  Serial.begin(9600);	          //波特率9600 （WIFI通讯设定波特率）  
  printf_begin();                 //初始化printf
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

void loop() 
{   

  g_flag = 0;
  printf("$CLO-R%d-G%d-B%d#",int(g_array[0] * g_SF[0]),int(g_array[1] * g_SF[1]),int(g_array[2] * g_SF[2]));

       inputString = "";   // clear the string
       newLineReceived = false;   

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
void serialEvent()
{
  while (Serial.available())                   //如果串口接收到数据则进入循环
  {    
    incomingByte = Serial.read();              //一个字节一个字节地读，下一句是读到的放入字符串数组中组成一个完成的数据包
    if(incomingByte == '$')                    //如果到来的字节是'$'，开始读取
    {
      startBit= true;
    }
    if(startBit == true)
    {
       inputString += (char) incomingByte;     // 全双工串口可以不用在下面加延时，半双工则要加的//
    }  
    if (incomingByte == '^')                  //如果到来的字节是'^'，读取结束
    {
       newLineReceived = true; 
       startBit = false;
    }
  }
}



