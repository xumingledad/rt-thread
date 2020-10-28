#include "stdlib.h"
#include <xpt2046.h>
#include <board.h>

/*
static uint16_t Xdown=0; 		 
static uint16_t Ydown=0;	     //触摸屏被按下就返回的的坐标值
static uint16_t Xup=0;
static uint16_t Yup=0; 			   //触摸屏被按下之后抬起返回的的坐标值 
*/

uint8_t  dir;			

uint8_t  CMD_RDX=0XD0;
uint8_t  CMD_RDY=0X90;

/*
float xFactor=0.06671114;	 //竖屏校准参数 
float yFactor=0.09117551;
short xOffset=-11;          //xOffset和yOffset 可以根据实际手头的电阻屏适当调整到精准
short yOffset=-18;
*/

/**************以下为普通IO模拟SPI通信XPT2046***************/
/****************************************************************************
* 名    称: void SPI_Write_Byte(u8 num) 
* 功    能：SPI写数据,向触摸屏IC写入1byte数据 
* 入口参数：num:要写入的数据
* 返回参数：无
* 说    明：       
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
		TCLK = 1;		 //上升沿有效	        
	}		 			    
} 	


/****************************************************************************
* 名    称: u16 SPI_Read_AD(u8 CMD)
* 功    能：SPI读数据 ,从触摸屏IC读取adc值
* 入口参数：CMD:指令
* 返回参数: 读到的数据	   
* 说    明：       
****************************************************************************/  
uint16_t SPI_Read_AD(uint8_t CMD)	  
{ 	 
	uint8_t count=0; 	  
	uint16_t Num=0; 
	TCLK=0;		//先拉低时钟 	 
	TDIN=0; 	//拉低数据线
	TCS=0; 		//选中触摸屏IC 2046
	SPI_Write_Byte(CMD);//发送命令字
	rt_hw_us_delay(6); 
	TCLK=0; 	     	    
	rt_hw_us_delay(1);    	   
	TCLK=1;		 
	rt_hw_us_delay(1);    
	TCLK=0; 	     	    
	for(count=0;count<16;count++)//读出16位数据,只有高12位有效 
	{ 				  
		Num<<=1; 	 
		TCLK=0;	 //下降沿有效  	    	   
		rt_hw_us_delay(1);    
 		TCLK=1;
 		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//高12位才有效
	TCS=1;		  //释放片选	 
	return(Num);   
}

/**************以上为普通IO模拟SPI通信XPT2046***************/
/****************************************************************************
* 名    称: u16 RTouch_Read_XorY(u8 xy)
* 功    能：读取一个坐标值(x或者y)
* 入口参数：xoy:指令（CMD_RDX/CMD_RDY
* 返回参数: 读到的数据	   
* 说    明：连续读取5次数据,对这些数据升序排列, 然后去掉最低和最1个数,取平均值       
****************************************************************************/ 
u16 RTouch_Read_XorY(uint8_t xoy)
{
	uint16_t i, j;
	uint16_t buf[5];
	uint16_t sum=0;
	uint16_t temp;
	
	for(i=0;i<5;i++)buf[i] = SPI_Read_AD(xoy);		 		    
	for(i=0;i<5-1; i++)  //排序
	{
		for(j=i+1;j<5;j++)
		{
			if(buf[i]>buf[j])//升序 
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
* 名    称: u8 RTouch_Read_XY(u16 *x,u16 *y)
* 功    能：读取x,y坐标
* 入口参数：x,y:读取到的坐标值
* 返回参数: 返回值:0,失败;1,成功   
* 说    明：最小值不能少于50.       
****************************************************************************/ 
u8 RTouch_Read_XY(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=RTouch_Read_XorY(CMD_RDX);
	ytemp=RTouch_Read_XorY(CMD_RDY);	  												   
	if(xtemp<50||ytemp<50)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}

/****************************************************************************
* 名    称: u8 RTouch_Read_XY2(u16 *x,u16 *y)
* 功    能：连续2次读取触摸屏IC
* 入口参数：x,y:读取到的坐标值
* 返回参数: 返回值:0,失败;1,成功   
* 说    明：        
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
    if(((x2<=x1&&x1<x2+50)||(x1<=x2&&x2<x1+50))  //前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+50)||(y1<=y2&&y2<y1+50)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  


//xpt2046初始化  		    
void XPT2046_Init(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;	
  
	 __HAL_RCC_GPIOA_CLK_ENABLE(); //使能GPIOA,B,C,F时钟
	 __HAL_RCC_GPIOB_CLK_ENABLE();
	 __HAL_RCC_GPIOC_CLK_ENABLE();
	 __HAL_RCC_GPIOF_CLK_ENABLE();

	 GPIO_InitStructure.Pin =GPIO_PIN_2; //设置为上拉输入
	 GPIO_InitStructure.Mode = GPIO_MODE_INPUT;//输入模式
	 //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	 GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;//100MHz
	 GPIO_InitStructure.Pull = GPIO_PULLUP;//上拉
	 HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化

	 GPIO_InitStructure.Pin = GPIO_PIN_11; 
	 HAL_GPIO_Init(GPIOF, &GPIO_InitStructure); 
	
	 GPIO_InitStructure.Pin =GPIO_PIN_0|GPIO_PIN_1;//设置为推挽输出
	 GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //输出模式
	 HAL_GPIO_Init(GPIOB, &GPIO_InitStructure); 

	 GPIO_InitStructure.Pin = GPIO_PIN_5; 
	 HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 	
}

INIT_DEVICE_EXPORT(XPT2046_Init);
