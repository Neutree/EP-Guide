# include "USART.h"
# include "MFRC522.h"
# include "GPIO.h"

GPIO rfidResetPin(GPIOA,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
GPIO ledGreen(GPIOB,6,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
GPIO ledYellow(GPIOB,7,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
USART com1(1,115200,true);
USART com2(2,9600,true);
MFRC522 rfid1(&com2,&rfidResetPin);

//PICC默认密码(6个字节均为0xff)
const unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

unsigned char tagInfo[MFRC522_MaxReceiveLen];
u8 temp[20];
int main()
{
	//关两个LED
	ledYellow.SetLevel(1);
	ledGreen.SetLevel(1);
	
	//PCD复位
	rfid1.PCDReset();
	
	while(1)
	{
		TaskManager::DelayMs(500);
		

		if(rfid1.PcdRequest(MFRC522_PICC_REQALL,tagInfo))//寻到卡
		{
			com1.SendData(tagInfo,2);
			ledGreen.SetLevel(0);
			if(rfid1.PcdAntiColl(tagInfo))//防冲撞成功(找到一张卡序列号)
			{
				com1.SendData(tagInfo,4);
				if(rfid1.PcdSelect(tagInfo))//选卡，卡号为前一步找到的卡号
				{
					
					if(rfid1.PcdAuthState(MFRC522_PICC_AUTHENT1A,1,(unsigned char*)DefaultKey,tagInfo))
					{
						ledYellow.SetLevel(0);
					}
					else
					{
						ledYellow.SetLevel(1);
					}
				}
				
			}
		}
		else
		{
			ledGreen.SetLevel(1);
			ledYellow.SetLevel(1);
		}

	}
}

