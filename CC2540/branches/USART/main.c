/***************************************/
/*           WeBee�Ŷ�                 */
/*     BlueTooth 4.0 ѧϰ����          */
/*�������ƣ� ����ͨѶ2                 */
/*����ʱ�䣺2013/03/13                 */
/*����������abc#��ʽ���ͣ�#Ϊ��������  */
/*      ����abc�������ʣ�115200bps     */
/***************************************/
#include<ioCC2540.h>
#include <string.h>

#define uint unsigned int
#define uchar unsigned char

//�������LED�ƵĶ˿�
#define LED1 P1_0	//����LED1ΪP10�ڿ���
#define LED2 P1_1

//��������
void Delayms(uint xms);		//��ʱ����
void InitLed(void);		//��ʼ��P1��
void InitUart0();              //��ʼ������
void Uart_Send_String(char *Data,int len);

char Rxdata[50];
uchar RXTXflag = 1; 
char temp; 
uchar  datanumber = 0;

/****************************
          ��ʱ����
*****************************/
void Delayms(uint xms)   //i=xms ����ʱi���� (16M����ʱ���Լ����32M��Ҫ�޸ģ�ϵͳ���޸�Ĭ��ʹ���ڲ�16M)
{
 uint i,j;
 for(i=xms;i>0;i--)
   for(j=587;j>0;j--);
} 


/****************************
//��ʼ������
*****************************/
void InitLed(void)
{
  P1DIR |= 0x03; //P1_0��P1_1����Ϊ���
  LED1 = 0;       //LED1��Ϩ��
  LED2=0;    
}
/**************************************************************** 
   ���ڳ�ʼ������     
***********************************************************/
void InitUart0()
{
  CLKCONCMD &= ~0x40;                          // ����ϵͳʱ��ԴΪ 32MHZ����
  while(CLKCONSTA & 0x40);                     // �ȴ������ȶ� 
  CLKCONCMD &= ~0x47;                          // ����ϵͳ��ʱ��Ƶ��Ϊ 32MHZ
  
  PERCFG = 0x00;        //λ��1 P0�� 
  P0SEL = 0x3c;        //P0_2,P0_3,P0_4,P0_5��������,�ڶ����� 
  P2DIR &= ~0XC0;      //P0 ������ΪUART0 �����ȼ�
  
  U0CSR |= 0x80;       //UART ��ʽ 
  U0GCR |= 11;         //U0GCR��U0BAUD���     
  U0BAUD |= 216;       // ��������Ϊ115200 
  UTX0IF = 0;          //UART0 TX �жϱ�־��ʼ��λ1  ���շ�ʱ��
  U0CSR |= 0X40;       //������� 
  IEN0 |= 0x84;        // �����жϣ������ж�    
}

/**************************************************************** 
���ڷ����ַ�������    
****************************************************************/ 
void Uart_Send_String(char *Data,int len) 
{
  int j; 
  for(j=0;j<len;j++) 
  { 
    U0DBUF = *Data++; 
    while(UTX0IF == 0); //������ɱ�־λ
    UTX0IF = 0; 
  } 
}

/***************************
//������
***************************/
void main(void)
{
  InitLed();		//���ó�ʼ������ 
  InitUart0();
  while(1)
  {
     if(RXTXflag == 1)     //����״̬ 
     { 
        LED1=1;       //����״ָ̬ʾ 
        if( temp != 0) 
        { 
           if((temp!='#')&&(datanumber<50)) //'��'������Ϊ�����ַ�������ܽ���50���ַ�           
           Rxdata[datanumber++] = temp; 
           else 
           { 
             RXTXflag = 3;                     //���뷢��״̬ 
             LED1=0; //��ָʾ��
            } 
            temp  = 0;
         }
      }
      if(RXTXflag == 3)     //����״̬ 
      { 
       LED2= 1;                           
       U0CSR &= ~0x40;      //��ֹ���� 
       Uart_Send_String(Rxdata,datanumber); //�����Ѽ�¼���ַ�����
       U0CSR |= 0x40;      //������� 
       RXTXflag = 1;       // �ָ�������״̬ 
       datanumber = 0;     //ָ���0 
       LED2 = 0;           //�ط���ָʾ 
      }    
    }
}

/**************************************************************** 
���ڽ���һ���ַ�: һ�������ݴӴ��ڴ���CC2540, ������жϣ������յ������ݸ�ֵ������temp. 
****************************************************************/ 
#pragma vector = URX0_VECTOR 
  __interrupt void UART0_ISR(void) 
 { 
  URX0IF = 0;    // ���жϱ�־ 
  temp = U0DBUF;                           
 }
