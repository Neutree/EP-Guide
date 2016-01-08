# include "USART.h"
# include "MFRC522.h"
# include "GPIO.h"

GPIO rfidResetPin(GPIOA,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
GPIO ledGreen(GPIOB,6,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
GPIO ledYellow(GPIOB,7,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
USART com1(1,115200,true);
USART com2(2,9600,true);


MFRC522 rfid1(&com2);

//PICC默认密码(6个字节均为0xff)
const unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const unsigned char data1[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
unsigned char tagInfo[MFRC522_MaxReceiveLen];
u8 temp[20];
int main()
{
	//关两个LED
	ledYellow.SetLevel(1);
	ledGreen.SetLevel(1);
	//PCD复位
	rfid1.PCDReset();
	com1<<"start\n\n\n";
	u8 temp1=(0x37<<1)|0x80,temp2;
	while(1)
	{
		TaskManager::DelayMs(300);
//		if(spi1.ReadOrWriteByte(temp1,&temp2))
//			com1<<"send ok\t"<<temp1<<"\t"<<temp2<<"\n";
//		else
//			com1<<"send failed\n";
//		++temp1;
	
		if(temp1==0x37)
			temp1=0x38;
		else
			temp1=0x37;
		temp2=rfid1.ReadRawRC(temp1);
		com1<<temp1<<"\t"<<temp2<<"\n";
		temp2=11;
		com1<<temp2<<"\n\n\n";
		
		if(rfid1.WriteRawRC(0x02,9))
			com1<<"write ok\n";
		else
			com1<<"write fail\n";
		com1<<"value: "<<rfid1.ReadRawRC(2)<<"\n\n\n\n\n\n";
		
		
		if(rfid1.PcdRequest(MFRC522_PICC_REQALL,tagInfo))//寻到卡
		{
			//com1.SendData(tagInfo,2);//显示卡类型
			if(((u16)tagInfo[0]<<8|tagInfo[1])==MFRC522_PICC_MIFARE_ONE_S50)
				com1<<"\nMIFARE-M1-S50 card detected!\n";
			ledGreen.SetLevel(0);
			if(rfid1.PcdAntiColl(tagInfo))//防冲撞成功(找到一张卡序列号)
			{
				//显示ID
				com1<<"Card ID:\t";
				for(int i=0;i<4;++i)
					com1<<tagInfo[i]<<"\t";
				com1<<"\n";
				
				if(rfid1.PcdSelect(tagInfo))//选卡，卡号为前一步找到的卡号
				{
					
					if(rfid1.PcdAuthState(MFRC522_PICC_AUTHENT1A,1,(unsigned char*)DefaultKey,tagInfo))//校验A密匙
					{
						if(rfid1.PcdWrite(1,(u8*)data1))//向块1写入数据data1
						{
							
							if(rfid1.PcdRead(1,tagInfo))
							{
								com1<<"Read success,Data:\n";
								for(u8 i=0;i<16;++i)
									com1<<tagInfo[i]<<"\t";
								com1<<"\n";
								com1.SendData(tagInfo,16);//显示读出的块1的数据
								if(rfid1.PcdHalt())
									ledYellow.SetLevel(0);//点亮黄灯
								else
									ledYellow.SetLevel(1);//熄灭黄灯

							}
						}
						
					}
					
				}
				
			}
			
		}
		else
		{
			ledGreen.SetLevel(1);
			ledYellow.SetLevel(1);
			com1<<"find failed!\n";
		}

	}
}

