#ifndef __MATH_TOOL_H
#define __MATH_TOOL_H
class MathToll
{
public:
	/*
	////////////////////////////
	///生成CRC16校验码(还存在BUG)
	///@param data 需要用来计算CRC校验码的原始数据
	///@param len 数据长度（字节）
	////////////////////////////
	static unsigned short CRC16(unsigned char *data,int len)
	{
		unsigned short crc = 0xffff;
		int i_byte = 0,i_bit = 0;
		unsigned char t = 0;
		for (i_byte = 0; i_byte < len; i_byte++)
		{
			crc = crc ^ data[i_byte];
			for(i_bit = 0; i_bit < 8; i_bit++)
			{	
				t = crc & 0x0001;
				crc >>= 1;
				crc &=  0x7fff;
				if(t==1)
					crc = crc ^ 0xa001;
			}
		}
		return crc;
	}
	*/
	
	/////////////////////////////
	///和校验
	///@param data 需要用来计算和校验码的原始数据
	///@param len 数据长度（字节）
	/////////////////////////////
	static unsigned char CheckSum8(unsigned char *data,int len)
	{
		unsigned int sum=0;
		for(int i=0;i<len;++i)
			sum+=data[i];
		return sum&0xff;
	}
	
	
};
#endif
