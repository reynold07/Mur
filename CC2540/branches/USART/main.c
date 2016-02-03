/***************************************/
/*           WeBee团队                 */
/*     BlueTooth 4.0 学习例程          */
/*例程名称： 串口通讯2                 */
/*建立时间：2013/03/13                 */
/*描述：例以abc#方式发送，#为结束符，  */
/*      返回abc。波特率：115200bps     */
/***************************************/
#include<ioCC2540.h>
#include <string.h>

#define uint unsigned int
#define uchar unsigned char

//定义控制LED灯的端口
#define LED1 P1_0	//定义LED1为P10口控制
#define LED2 P1_1

//函数声明
void Delayms(uint xms);		//延时函数
void InitLed(void);		//初始化P1口
void InitUart0();              //初始化串口
void Uart_Send_String(char *Data,int len);

char Rxdata[50];
uchar RXTXflag = 1; 
char temp; 
uchar  datanumber = 0;

/****************************
          延时函数
*****************************/
void Delayms(uint xms)   //i=xms 即延时i毫秒 (16M晶振时候大约数，32M需要修改，系统不修改默认使用内部16M)
{
 uint i,j;
 for(i=xms;i>0;i--)
   for(j=587;j>0;j--);
} 


/****************************
//初始化程序
*****************************/
void InitLed(void)
{
  P1DIR |= 0x03; //P1_0、P1_1定义为输出
  LED1 = 0;       //LED1灯熄灭
  LED2=0;    
}
/**************************************************************** 
   串口初始化函数     
***********************************************************/
void InitUart0()
{
  CLKCONCMD &= ~0x40;                          // 设置系统时钟源为 32MHZ晶振
  while(CLKCONSTA & 0x40);                     // 等待晶振稳定 
  CLKCONCMD &= ~0x47;                          // 设置系统主时钟频率为 32MHZ
  
  PERCFG = 0x00;        //位置1 P0口 
  P0SEL = 0x3c;        //P0_2,P0_3,P0_4,P0_5用作串口,第二功能 
  P2DIR &= ~0XC0;      //P0 优先作为UART0 ，优先级
  
  U0CSR |= 0x80;       //UART 方式 
  U0GCR |= 11;         //U0GCR与U0BAUD配合     
  U0BAUD |= 216;       // 波特率设为115200 
  UTX0IF = 0;          //UART0 TX 中断标志初始置位1  （收发时候）
  U0CSR |= 0X40;       //允许接收 
  IEN0 |= 0x84;        // 开总中断，接收中断    
}

/**************************************************************** 
串口发送字符串函数    
****************************************************************/ 
void Uart_Send_String(char *Data,int len) 
{
  int j; 
  for(j=0;j<len;j++) 
  { 
    U0DBUF = *Data++; 
    while(UTX0IF == 0); //发送完成标志位
    UTX0IF = 0; 
  } 
}

/***************************
//主函数
***************************/
void main(void)
{
  InitLed();		//调用初始化函数 
  InitUart0();
  while(1)
  {
     if(RXTXflag == 1)     //接收状态 
     { 
        LED1=1;       //接收状态指示 
        if( temp != 0) 
        { 
           if((temp!='#')&&(datanumber<50)) //'＃'被定义为结束字符，最多能接收50个字符           
           Rxdata[datanumber++] = temp; 
           else 
           { 
             RXTXflag = 3;                     //进入发送状态 
             LED1=0; //关指示灯
            } 
            temp  = 0;
         }
      }
      if(RXTXflag == 3)     //发送状态 
      { 
       LED2= 1;                           
       U0CSR &= ~0x40;      //禁止接收 
       Uart_Send_String(Rxdata,datanumber); //发送已记录的字符串。
       U0CSR |= 0x40;      //允许接收 
       RXTXflag = 1;       // 恢复到接收状态 
       datanumber = 0;     //指针归0 
       LED2 = 0;           //关发送指示 
      }    
    }
}

/**************************************************************** 
串口接收一个字符: 一旦有数据从串口传至CC2540, 则进入中断，将接收到的数据赋值给变量temp. 
****************************************************************/ 
#pragma vector = URX0_VECTOR 
  __interrupt void UART0_ISR(void) 
 { 
  URX0IF = 0;    // 清中断标志 
  temp = U0DBUF;                           
 }
