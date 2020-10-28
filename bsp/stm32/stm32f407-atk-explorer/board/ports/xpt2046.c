#include "stdlib.h"
#include <xpt2046.h>
#include <board.h>

/*
static uint16_t Xdown=0; 		 
static uint16_t Ydown=0;	     //�����������¾ͷ��صĵ�����ֵ
static uint16_t Xup=0;
static uint16_t Yup=0; 			   //������������֮��̧�𷵻صĵ�����ֵ 
*/

uint8_t  dir;			

uint8_t  CMD_RDX=0XD0;
uint8_t  CMD_RDY=0X90;

/*
float xFactor=0.06671114;	 //����У׼���� 
float yFactor=0.09117551;
short xOffset=-11;          //xOffset��yOffset ���Ը���ʵ����ͷ�ĵ������ʵ���������׼
short yOffset=-18;
*/

/**************����Ϊ��ͨIOģ��SPIͨ��XPT2046***************/
/****************************************************************************
* ��    ��: void SPI_Write_Byte(u8 num) 
* ��    �ܣ�SPIд����,������ICд��1byte���� 
* ��ڲ�����num:Ҫд�������
* ���ز�������
* ˵    ����       
****************************************************************************/
void SPI_Write_Byte(uint8_t num)    
{  
	uint8_t count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)
			TDIN = 1;  
		else TDIN = 0;   
		num<<=1;    
		TCLK=0; 
		rt_hw_us_delay(1);
		TCLK = 1;		 //��������Ч	        
	}		 			    
} 	


/****************************************************************************
* ��    ��: u16 SPI_Read_AD(u8 CMD)
* ��    �ܣ�SPI������ ,�Ӵ�����IC��ȡadcֵ
* ��ڲ�����CMD:ָ��
* ���ز���: ����������	   
* ˵    ����       
****************************************************************************/  
uint16_t SPI_Read_AD(uint8_t CMD)	  
{ 	 
	uint8_t count=0; 	  
	uint16_t Num=0; 
	TCLK=0;		//������ʱ�� 	 
	TDIN=0; 	//����������
	TCS=0; 		//ѡ�д�����IC 2046
	SPI_Write_Byte(CMD);//����������
	rt_hw_us_delay(6); 
	TCLK=0; 	     	    
	rt_hw_us_delay(1);    	   
	TCLK=1;		 
	rt_hw_us_delay(1);    
	TCLK=0; 	     	    
	for(count=0;count<16;count++)//����16λ����,ֻ�и�12λ��Ч 
	{ 				  
		Num<<=1; 	 
		TCLK=0;	 //�½�����Ч  	    	   
		rt_hw_us_delay(1);    
 		TCLK=1;
 		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//��12λ����Ч
	TCS=1;		  //�ͷ�Ƭѡ	 
	return(Num);   
}

/**************����Ϊ��ͨIOģ��SPIͨ��XPT2046***************/
/****************************************************************************
* ��    ��: u16 RTouch_Read_XorY(u8 xy)
* ��    �ܣ���ȡһ������ֵ(x����y)
* ��ڲ�����xoy:ָ�CMD_RDX/CMD_RDY
* ���ز���: ����������	   
* ˵    ����������ȡ5������,����Щ������������, Ȼ��ȥ����ͺ���1����,ȡƽ��ֵ       
****************************************************************************/ 
u16 RTouch_Read_XorY(uint8_t xoy)
{
	uint16_t i, j;
	uint16_t buf[5];
	uint16_t sum=0;
	uint16_t temp;
	
	for(i=0;i<5;i++)buf[i] = SPI_Read_AD(xoy);		 		    
	for(i=0;i<5-1; i++)  //����
	{
		for(j=i+1;j<5;j++)
		{
			if(buf[i]>buf[j])//���� 
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=1;i<5-1;i++)sum+=buf[i];
	temp=sum/(5-2*1);
	return temp;   
} 

/****************************************************************************
* ��    ��: u8 RTouch_Read_XY(u16 *x,u16 *y)
* ��    �ܣ���ȡx,y����
* ��ڲ�����x,y:��ȡ��������ֵ
* ���ز���: ����ֵ:0,ʧ��;1,�ɹ�   
* ˵    ������Сֵ��������50.       
****************************************************************************/ 
u8 RTouch_Read_XY(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=RTouch_Read_XorY(CMD_RDX);
	ytemp=RTouch_Read_XorY(CMD_RDY);	  												   
	if(xtemp<50||ytemp<50)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}

/****************************************************************************
* ��    ��: u8 RTouch_Read_XY2(u16 *x,u16 *y)
* ��    �ܣ�����2�ζ�ȡ������IC
* ��ڲ�����x,y:��ȡ��������ֵ
* ���ز���: ����ֵ:0,ʧ��;1,�ɹ�   
* ˵    ����        
****************************************************************************/
u8 RTouch_Read_XY2(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	u8 flag;    
    flag=RTouch_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=RTouch_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+50)||(x1<=x2&&x2<x1+50))  //ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+50)||(y1<=y2&&y2<y1+50)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  


//xpt2046��ʼ��  		    
void XPT2046_Init(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;	
  
	 __HAL_RCC_GPIOA_CLK_ENABLE(); //ʹ��GPIOA,B,C,Fʱ��
	 __HAL_RCC_GPIOB_CLK_ENABLE();
	 __HAL_RCC_GPIOC_CLK_ENABLE();
	 __HAL_RCC_GPIOF_CLK_ENABLE();

	 GPIO_InitStructure.Pin =GPIO_PIN_2; //����Ϊ��������
	 GPIO_InitStructure.Mode = GPIO_MODE_INPUT;//����ģʽ
	 //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	 GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;//100MHz
	 GPIO_InitStructure.Pull = GPIO_PULLUP;//����
	 HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��

	 GPIO_InitStructure.Pin = GPIO_PIN_11; 
	 HAL_GPIO_Init(GPIOF, &GPIO_InitStructure); 
	
	 GPIO_InitStructure.Pin =GPIO_PIN_0|GPIO_PIN_1;//����Ϊ�������
	 GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //���ģʽ
	 HAL_GPIO_Init(GPIOB, &GPIO_InitStructure); 

	 GPIO_InitStructure.Pin = GPIO_PIN_5; 
	 HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 	
}

INIT_DEVICE_EXPORT(XPT2046_Init);
