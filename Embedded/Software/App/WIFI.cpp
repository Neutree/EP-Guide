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
char WIFI::mStationMac[6]={0x12,0x08,0x07,0x72,0x07,0x01};

//server
char mServerIPOrDomain[30]="192.168.191.1";
uint32_t mServerPort=5080;
	
/********************************************************/

bool WIFI::Init(esp8266 &wifi)
{
	if(!wifi.kick())//检测是否存在
		return false;

	wifi.setEcho(1);//关闭回显
	wifi.setOprToStationSoftAP(1,1);//设置为AP+station模式
	wifi.setStationMac(mStationMac,1);//set station mac adress
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
void WIFI::MacAddressStringToBytes(char* macStr,char** macAddress)
{
	char temp;
	
	for(uint8_t i=0;i<17;)
	{
		(*macAddress)[i]=(uint8_t)(macStr[i]-'0')<<8|(macStr[i+1]-'0');//字符串转字节
		i+=3;
	}
}

/**********************************************************************/

