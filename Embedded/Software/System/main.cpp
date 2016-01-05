# include "USART.h"
# include "Delay.h"
# include "MFRC522.h"
# include "GPIO.h"

GPIO rfidResetPin(GPIOA,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
USART com1(1,115200,true);
USART com2(2,9600,true);
MFRC522 rfid1(&com2);


unsigned char tagInfo[20];
u8 temp[20];
int main()
{
	
	rfid1.PCDReset();//PCD复位
	
	while(1)
	{
		//com1<<"\n-------\n";
		Delay::Ms(300);
		
		
		if(rfid1.PcdRequest(1,tagInfo))//寻到卡
		{
			//com1<<"found tag:..."<<(char*)tagInfo<<"...\n";
		}
		u8 size=com2.ReceiveBufferSize();
		if(size>0)
		{
			if(com2.GetReceivedData(temp,size))
				com1.SendData(temp,size);
		}
	}
}

