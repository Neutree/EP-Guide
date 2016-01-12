# include "WIFI.h"

/******************常量初始化****************************/
//AP
char WIFI::mSoftApName[13]="EP-Guide";
char WIFI::mSoftApPassword[20]="1208077207";
char WIFI::mSoftApIP[16]="192.168.4.1";
char WIFI::mSoftApGateway[16]="192.168.4.1";
char WIFI::mSoftApNetmask[16]="255.255.255.0";
uint32_t WIFI::mSoftApPort=345;
uint16_t WIFI::mSoftApClientTimeOut=180;//s

//Station
char WIFI::mJoinApName[13]="nomore";
char WIFI::mJoinApPassword[20]="morenimei0";
//char WIFI::mJoinApName[13]="ICanHearYou";
//char WIFI::mJoinApPassword[20]="1208077207";
unsigned char WIFI::mStationMac[18]="12:08:07:72:07:01";

//server
char WIFI::mServerIPOrDomain[30]="192.168.191.1";
uint32_t WIFI::mServerPort=5080;


//node
uint32_t WIFI::mNodePort=345;
	
/********************************************************/

bool WIFI::Init(esp8266 &wifi)
{
	if(!wifi.kick())//检测是否存在
		return false;

	wifi.setEcho(1);//关闭回显
	wifi.setOprToStationSoftAP(1,1);//设置为AP+station模式
	wifi.setStationMac((char*)mStationMac,1);//set station mac adress
	if(!wifi.joinAP(mJoinApName,mJoinApPassword,1))//join ap
	{
		
	}
	wifi.setSoftAPParam(mSoftApName,mSoftApPassword,5,3,1);//set station param
//	wifi.setStationIp(mSoftApIP,mSoftApGateway,mSoftApNetmask,1);//set station's address (AP mode)
//	wifi.setAPIp();
	wifi.setDHCP(2,1,1);//enable softap and station DHCP 开启DHCP后,AP依然会使用上一句设置的IP和网关地址
//	wifi.setDHCP(0,0,1);//enable softap and station DHCP 开启DHCP后,AP依然会使用上一句设置的IP和网关地址
	wifi.enableMUX();// enable Mux(允许多连接)
	wifi.startServer(mSoftApPort);//config server(AP) port
	wifi.setTCPServerTimeout(mSoftApClientTimeOut);

	wifi.getAPIp();
	if(wifi.getStationIp()==0)//获取作为station的IP，即连接到WIFI后被分配到的地址
		return false;
	return true;
}


/**************************WIFI相关工具********************************/
////////////////////////
///将字符串形式的MAC地址转换为字节形式的，6个字节
///////////////////////
void WIFI::MacAddressStringToBytes(char* macStr,unsigned char macAddress[6])
{
	char j = 0;
	for (unsigned char i = 0; i<17;)
	{
		macAddress[j] = (((unsigned short)macStr[i] - '0') * 16)+ (macStr[i + 1] - '0');//字符串转字节
		i += 3;
		++j;
	}
}


///////////////////////////////
///将IP字符串转换为数组
///@param IPStr 字符串
///@param IP 存放字节类型的IP地址的数组
///@retval 有效IP地址字符串的长度
///////////////////////////////
unsigned char WIFI::IPStringToBytes(char* IPStr, unsigned char IP[4])
{
	char i = 0;
	char j = 0;
	char k = 0;
	unsigned char length=0;
	IP[0] = 0;
	IP[1] = 0;
	IP[2] = 0;
	IP[3] = 0;
	while (((IPStr[i] >= '0'&&IPStr[i] <= '9')||IPStr[i]=='.')&&j<4)
	{
		if (IPStr[i] == '.')
		{
			++j;
			k = 0;
			++i;
			++length;
			continue;
		}
		if (k != 0)
			IP[j] *= 10;
		IP[j] += (IPStr[i] - '0');
		++length;
		++k;
		++i;
		if (k > 3)//字符串格式错误
			return 0;
	}
	if (i < 8)
		return 0;
	return length;

}

//////////////////////////////////////////
///提取包含多个IP地址的字符串中的IP地址到数组，每个IP4个字节
///@param IPStr 字符串
///@param IP 存放字节类型的IP地址的数组
///@retval 找到正确格式IP的个数
/////////////////////////////////////////
unsigned char WIFI::IPStringsToBytes(char* IPStr, unsigned char IP[][4])
{
	short i=0,j=0;
	char* pStr = IPStr;
	unsigned char ipStrlength=0;
	while (IPStr[i] != '\0')
	{
		ipStrlength = IPStringToBytes(pStr, IP[j]);
		if (ipStrlength!=0)
		{
			++j;
			pStr += ipStrlength;
		}
		++pStr;
		++i;
	}
	return j;
}



///////////////////////////////
///将IP字符串转换为数组
///@param IPStr 字符串
///@param IP 存放字节类型的IP地址的数组
///@retval 有效IP地址字符串的长度
///////////////////////////////
void WIFI::IPBytesToString( unsigned char IP[4],char* IPStr)
{
	unsigned short temp;
	unsigned char length=0;
	for(char i = 0; i<4; ++i)
	{
		temp = IP[i];
		unsigned char  len = 0, data[3];
		do
		{
			len++;
			data[3 - len] = temp % 10 + '0';
			temp = temp / 10;
		} while (temp);
		length += len;
		memcpy(IPStr+length-len+i,data + 3 - len, len);
		IPStr[length+i] = '.';
	}
	IPStr[length + 3] = '\0';
}

/**********************************************************************/

