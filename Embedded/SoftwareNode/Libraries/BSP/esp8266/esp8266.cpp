#include "esp8266.h"

//USART usart3test(3,115200/*,true,0,2,7,1,3*/);//
	esp8266::esp8266(USART &usart, uint32_t baudRate )
	:mUsart(usart)
	{
		if(baudRate!=0)
		{
			usart.SetBaudRate(baudRate);
		}
		rx_empty();//empty receive buffer
	}

	void esp8266::rx_empty()
	{
		mUsart.ClearReceiveBuffer();
		
		for(u16 i=0;i<mReceiveBuffer.Size();++i)//将队列中存放数据的数组擦除
			mReceiveBuffer._buf[i]=0;
		mReceiveBuffer.Clear();//清除队列信息
	}


    /* 
     * Recvive data from uart. Return all received data if target found or timeout. 
     */
    char* esp8266::recvString(char const *target, float timeout)
	{
		u8 temp;
		//int i=1;
		double tartTime=TaskManager::Time();
		while((TaskManager::Time()-tartTime)<timeout)
		{
			while(mUsart.ReceiveBufferSize()>0)
			{
				mUsart.GetReceivedData(&temp,1);//从串口接收缓冲区接收数据
				if(temp=='\0')
					continue;
				mReceiveBuffer.Put(temp);//放入esp的缓冲区
				
			}
			
			if(strstr(mReceiveBuffer._buf,target))
				break;	
			//else
			//	i++;
			//if(i>6)
			//	break;
			//else			
				
		}
//		if(i)
//			usart3test<<"test2"<<"\r\n";
		//usart3test<<"1**************1\r\n";
		//usart3test<<mReceiveBuffer._buf;
		//usart3test<<"1**************2\r\n";
		if(mReceiveBuffer.Size()>0)//接收到了数据，加上结束标志
			mReceiveBuffer.Put('\0');
		return mReceiveBuffer._buf;
	}
    
    /* 
     * Recvive data from uart. Return all received data if one of target1 and target2 found or timeout. 
     */
    char* esp8266::recvString(char const *target1, const char* target2, float timeout )
	{
		u8 temp;
		double tartTime=TaskManager::Time();
		while((TaskManager::Time()-tartTime)<timeout)
		{
			while(mUsart.ReceiveBufferSize()>0)
			{
				mUsart.GetReceivedData(&temp,1);//从串口接收缓冲区接收数据
				if(temp=='\0')
					continue;
				mReceiveBuffer.Put(temp);//放入esp的缓冲区
			}
			if(strstr(mReceiveBuffer._buf,target1) || strstr(mReceiveBuffer._buf,target2))//找到目标数据
				break;
			
		}
		if(mReceiveBuffer.Size()>0)//接收到了数据，加上结束标志
			mReceiveBuffer.Put('\0');
		return mReceiveBuffer._buf;
	}
    
    /* 
     * Recvive data from uart. Return all received data if one of target1, target2 and target3 found or timeout. 
     */
    char* esp8266::recvString(char const *target1, const char* target2,const  char* target3, float timeout)
	{
		u8 temp;
		double tartTime=TaskManager::Time();
		while((TaskManager::Time()-tartTime)<timeout)
		{
			while(mUsart.ReceiveBufferSize()>0)
			{
				mUsart.GetReceivedData(&temp,1);//从串口接收缓冲区接收数据
				if(temp=='\0')
					continue;
				mReceiveBuffer.Put(temp);//放入esp的缓冲区
			}
			if(strstr(mReceiveBuffer._buf,target1) || strstr(mReceiveBuffer._buf,target2) || strstr(mReceiveBuffer._buf,target3))//找到目标数据
				break;
			
		}
		if(mReceiveBuffer.Size()>0)//接收到了数据，加上结束标志
			mReceiveBuffer.Put('\0');
		return mReceiveBuffer._buf;
	}
    
    /* 
     * Recvive data from uart and search first target. Return true if target found, false for timeout.
     */
    bool esp8266::recvFind(char const *target, float timeout)
	{
		char* data_temp=recvString(target,timeout);
		//mUsart<<data_temp<<"2"<<"\r\n";
		if(strstr(data_temp,target))
			return true;
		return false;
	}
    
    /* 
     * Recvive data from uart and search first target and cut out the substring between begin and end(excluding begin and end self). 
     * Return true if target found, false for timeout.
     */
    bool esp8266::recvFindAndFilter(char const *target, char const * begin, char const * end, char** Data, float timeout)
	{

		char* data_temp=recvString(target,timeout);
		if(strstr(data_temp,target))
		{
			char* index1=strstr(data_temp,begin);
			char* index2=strstr(data_temp,end);

			if(index1&&index2)
			{
				index1+=strlen(begin);
				*index2='\0';
				*Data=index1;
				return true;
			}
		}
		
		Data=0;
		return false;
	}
    
	bool esp8266::eAT(void)
	{
		rx_empty();
		mUsart<<"AT\r\n";
		return recvFind("OK");
	}
    bool esp8266::eATRST(void)
	{
		rx_empty();
		mUsart<<"AT+RST\r\n";
		return recvFind("OK");
	}
    bool esp8266::eATGMR(char **version)
	{

		rx_empty();
		mUsart<<"AT+GMR\r\n";
		return recvFindAndFilter("OK", "\r\n", "\r\nOK", version);
	}
	bool esp8266::eATGSLP(u32 time)
	{
		rx_empty();
		mUsart<<"AT+GSLP="<<(int)time<<"\r\n";
		return recvFind("OK");
	}
	bool esp8266::eATE(u8 mode)
	{
		rx_empty();
		mUsart<<"ATE"<<mode<<"\r\n";
		return recvFind("OK");
	}
	bool esp8266::eATRESTORE()
	{
		rx_empty();
		mUsart<<"AT+RESTORE"<<"\r\n";
		return recvFind("OK");
	}
	bool esp8266::eATSETUART(uint32_t baudrate,uint8_t pattern)
	{
		rx_empty();
		if(pattern>3||pattern<1){
			return false;
		}
		switch(pattern){
				case 3:
						mUsart<<"AT+UART=";
						break;
				case 2:
						mUsart<<"AT+UART_CUR=";
						break;
				case 1:
						mUsart<<"AT+UART_DEF=";
						 break;    
		}
		mUsart<<(int)baudrate<<",8,1,0,0\r\n";
		if(recvFind("OK"))
		{
			return true;
		}
		else 
			return false;
	}
	bool esp8266::qATCWMODE(char **mode,uint8_t pattern)
	{
		if (!pattern) {
				return false;
		}
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CWMODE_DEF?\r\n";
						break;
				case 2:
						mUsart<<"AT+CWMODE_CUR?\r\n";
						break;
				default:
						mUsart<<"AT+CWMODE?\r\n";
		}
		return recvFindAndFilter("OK", ":", "\r\nOK", mode); 
	}
	
	bool esp8266::eATCWMODE(char** list) 
	{
		rx_empty();
		mUsart<<"AT+CWMODE=?\r\n";
		return recvFindAndFilter("OK", "+CWMODE:(", ")\r\n\r\nOK", list);
	}
	bool esp8266::sATCWMODE(uint8_t mode,uint8_t pattern)
	{
		char* data_temp;
		if (!pattern) 
				return false;
		rx_empty();
		 switch(pattern)
		{
				case 1:
						mUsart<<"AT+CWMODE_DEF="<<mode<<"\r\n";
						break;
				case 2:
					mUsart<<"AT+CWMODE_CUR="<<mode<<"\r\n";
						break;
				default:
						mUsart<<"AT+CWMODE="<<mode<<"\r\n";
						break;
		}
		data_temp = recvString("OK", "ERROR");
		if(strstr(data_temp,"OK"))
			return true;
		else//strstr(ssid,"FAIL")
			return false;
		
	}
	
	bool esp8266::qATCWJAP(char** ssid,uint8_t pattern)
	{
	//	char * data_temp;
		if (!pattern) 
		return false;
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CWJAP_DEF?\r\n";
						break;
				case 2:
						mUsart<<"AT+CWJAP_CUR?\r\n";
						break;
				default:
						mUsart<<"AT+CWJAP?\r\n";
		}
		//data_temp = recvString("OK","No Ap");
		//if(strstr(data_temp,"OK")) 
			return recvFindAndFilter("OK", "\r\n", "\r\nOK", ssid);
		///if(strstr(data_temp,"No Ap"))
		//{
		//	*ssid="No Ap";
		//	return true;	
		//}			
		//return false;
	}
	
	bool esp8266::sATCWJAP(char* ssid, char* pwd,uint8_t pattern)
	{
		char* data_temp;
		if (!pattern) 
			return false;
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CWJAP_DEF=";
						break;
				case 2:
						mUsart<<"AT+CWJAP_CUR=";
						break;
				default:
						mUsart<<"AT+CWJAP=";
		}
		mUsart<<"\""<<ssid<<"\",\""<<pwd<<"\"\r\n";
		data_temp = recvString("OK", "ERROR",10);
		if(strstr(data_temp,"OK"))
			return true;
		else//strstr(ssid,"FAIL")
			return false;
	}
	
	bool esp8266::eATCWLAP(char** list)
	{
		rx_empty();
		mUsart<<"AT+CWLAP\r\n";
//		mUsart<<recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", list);
		return recvFindAndFilter("OK", "+CWLAP:(", "TP-LINK", list);
		//return 0;
	}

	
		bool esp8266::eATCWQAP()
	{
		char *data_temp;
		rx_empty();
		mUsart<<"AT+CWQAP\r\n";		
		data_temp = recvString("OK");
		if(strstr(data_temp,"OK"))
			return true;
		else//strstr(ssid,"FAIL")
			return false;
	}
	
	
	
	
	bool esp8266::qATCWSAP(char **data,uint8_t pattern)
	{
		if (!pattern) 
			return false;
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CWSAP_DEF?\r\n";
						break;
				case 2:
						mUsart<<"AT+CWSAP_CUR?\r\n";
						break;
				default:
						mUsart<<"AT+CWSAP?\r\n";
		}
		return recvFindAndFilter("OK", "+CWSAP:", "\r\n\r\nOK", data);			
	}

	
	bool esp8266::sATCWSAP(char*  ssid, char*  pwd, uint8_t chl, uint8_t ecn,uint8_t pattern)
	{
				char* data_temp;
		if (!pattern) 
			return false;
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CWSAP_DEF=";
						break;
				case 2:
						mUsart<<"AT+CWSAP_CUR=";
						break;
				default:
						mUsart<<"AT+CWSAP=";
		}
		mUsart<<"\""<<ssid<<"\",\""<<pwd<<"\","<<chl<<","<<ecn<<"\r\n";
		data_temp = recvString("OK", "ERROR");
		if(strstr(data_temp,"OK"))
			return true;
		else//strstr(ssid,"FAIL")
			return false;
	
	}
	bool esp8266::eATCWLIF(char **  list)
	{
		rx_empty();
		mUsart<<"AT+CWLIF\r\n";
		return recvFindAndFilter("OK", "\r\n", "\r\n\r\nOK", list);
		
	}
	
	
	bool esp8266::qATCIPSTAMAC(char** mac,uint8_t pattern)
	{
		if (!pattern) 
		return false;
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CIPSTAMAC_DEF?\r\n";
						break;
				case 2:
						mUsart<<"AT+CIPSTAMAC_CUR?\r\n";
						break;
				default:
						mUsart<<"AT+CIPSTAMAC?\r\n";
		}
		return recvFindAndFilter("OK", "+CIPSTAMAC:", "\r\n\r\nOK", mac);	
		
	}
			bool esp8266::sATCIPSTAMAC(char* mac,uint8_t pattern)
	{
						char* data_temp;

		if (!pattern) 
			return false;
		rx_empty();
		
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CIPSTAMAC_DEF=";
						break;
				case 2:
						mUsart<<"AT+CIPSTAMAC_CUR=";
						break;
				default:
						mUsart<<"AT+CIPSTAMAC=";
		}
		mUsart<<"\""<<mac<<"\""<<"\r\n";
		data_temp = recvString("OK", "ERROR");
		if(strstr(data_temp,"OK"))
			return true;
		else
			return false;					
	}
	bool esp8266::qATCIPAPMAC(char** mac,uint8_t pattern)
	{
		if (!pattern) 
		return false;
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CIPAPMAC_DEF?\r\n";
						break;
				case 2:
						mUsart<<"AT+CIPAPMAC_CUR?\r\n";
						break;
				default:
						mUsart<<"AT+CIPAPMAC?\r\n";
		}
		return recvFindAndFilter("OK", "+AT+CIPAPMAC:", "\r\n\r\nOK", mac);	
	}
	bool esp8266::sATCIPAPMAC(char* mac,uint8_t pattern)
	{
						char* data_temp;

		if (!pattern) 
			return false;
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CIPAPMAC_DEF=";
						break;
				case 2:
						mUsart<<"AT+CIPAPMAC_CUR=";
						break;
				default:
						mUsart<<"AT+CIPAPMAC=";
		}
		mUsart<<"\""<<mac<<"\""<<"\r\n";
		data_temp = recvString("OK", "ERROR");
		if(strstr(data_temp,"OK"))
			return true;
		else
			return false;					
	}
	
	bool esp8266::qATCIPSTA(char** ip,uint8_t pattern)
	{
		if (!pattern) 
		return false;
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CIPSTA?\r\n";
						break;
				case 2:
						mUsart<<"AT+CIPSTA?\r\n";
						break;
				default:
						mUsart<<"AT+CIPSTA?\r\n";
		}
		return recvFindAndFilter("OK", "+CIPSTA:", "\r\n\r\nOK", ip);	
	}
	bool esp8266::sATCIPSTA(char* ip,uint8_t pattern)
	{
		char* data_temp;

		if (!pattern) 
			return false;
		rx_empty();
		
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CIPSTA_DEF=";
						break;
				case 2:
						mUsart<<"AT+CIPSTA_CUR=";
						break;
				default:
						mUsart<<"AT+CIPSTA=";
		}
		mUsart<<"\""<<ip<<"\""<<"\r\n";
		data_temp = recvString("OK", "ERROR");
		if(strstr(data_temp,"OK"))
			return true;
		else
			return false;					
	}
	
	
	
	
	bool esp8266::sATCIPSERVER(uint8_t mode, uint32_t port)
	{
		char * data_temp;
		
		rx_empty();
		mUsart<<"AT+CIPSERVER="<<mode<<","<<port<<"\r\n";					
		data_temp = recvString("OK", "ERROR");
		if(strstr(data_temp,"OK"))
			return true;
		else//strstr(ssid,"FAIL")
			return false;
	
	}
	bool esp8266::qATCWDHCP(char**list, uint8_t pattern)
	{
		if (!pattern) 
			return false;
		rx_empty();
		 switch(pattern)
		{
				case 1:
						mUsart<<"AT+CWDHCP_DEF?\r\n";
						break;
				case 2:
					mUsart<<"AT+CWDHCP_CUR?\r\n";
						break;
				default:
						mUsart<<"AT+CWDHCP?\r\n";
						break;
		}
		return recvFindAndFilter("OK", "+CWDHCP:", "\r\nOK", list);
		
	
	}
	bool esp8266::sATCWDHCP(uint8_t mode, uint8_t en, uint8_t pattern)
	{
		char * data_temp;

			if (!pattern) 
				return false;
		rx_empty();
		 switch(pattern)
		{
				case 1:
						mUsart<<"AT+CWDHCP_DEF="<<mode<<","<<en<<"\r\n";
						break;
				case 2:
					mUsart<<"AT+CWDHCP_CUR="<<mode<<","<<en<<"\r\n";
						break;
				default:
						mUsart<<"AT+CWDHCP="<<mode<<","<<en<<"\r\n";
						break;
		}

		data_temp = recvString("OK", "ERROR");
		if(strstr(data_temp,"OK"))
			return true;
		else//strstr(ssid,"FAIL")
			return false;
	}

	bool esp8266::sATCIPMUX(uint8_t mode)
	{

		//char * data_temp;
		rx_empty();
		mUsart<<"AT+CIPMUX="<<mode<<"\r\n";							
		return recvFind("OK");
//		data_temp = recvString("OK", "ERROR");
//		if(strstr(data_temp,"OK"))
//			return true;
//		else
//			return false;

//		rx_empty();
//		mUsart<<"AT+CIPMUX="<<mode<<"\r\n";							
//		return recvFind("OK");

	}
		bool esp8266::sATCIPSTO(uint32_t timeout)
	{
		rx_empty();
		mUsart<<"AT+CIPSTO="<<timeout<<"\r\n";
		return recvFind("OK");
	}
	bool esp8266::qATCIPAP(char** ip,uint8_t pattern)
	{
		if (!pattern) 
			return false;
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						mUsart<<"AT+CIPAP_DEF?\r\n";
						break;
				case 2:
						mUsart<<"AT+CIPAP_CUR?\r\n";
						break;
				default:
						mUsart<<"AT+CIPAP?\r\n";
		}
		return recvFindAndFilter("OK", "+CIPAP:", "\r\n\r\nOK", ip);			
	}
	
	
	bool esp8266::sATCIPSTARTMultiple(uint8_t mux_id, char*  type, char*  addr, uint32_t port)
	{
		rx_empty();
		mUsart<<"AT+CIPSTART="<<mux_id<<",\""<<type<<"\",\""<<addr<<"\","<<port<<"\r\n";							
		return recvFind("OK");
	}
	
	bool esp8266::sATCIPSENDMultiple(uint8_t mux_id, char *buffer, uint32_t len)
	{
		rx_empty();
		mUsart<<"AT+CIPSEND="<<mux_id<<","<<len<<"\r\n";
		if(!recvFind("OK\r\n> "))
			return false;
		rx_empty();
		mUsart.SendData((uint8_t*)buffer,len);
		return recvFind("OK\r\n");
	}
	
	
	
	bool esp8266::sATCIPCLOSEMulitple(uint8_t mux_id)
	{
		rx_empty();
		mUsart<<"AT+CIPCLOSE="<<mux_id<<"\r\n";
		return recvFind("OK");
	}
	
	
	
	
	
	
	
	
	
	
	/** 
     * Verify ESP8266 whether live or not. 
     *
     * Actually, this method will send command "AT" to ESP8266 and waiting for "OK". 
     * 
     * @retval true - alive.
     * @retval false - dead.
     */
    bool esp8266::kick(void)
	{
		return eAT();
	}
    
    /**
     * Restart ESP8266 by "AT+RST". 
     *
     * This method will take 3 seconds or more. 
     *
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::restart(void)
	{
		u16 temp=0;
		if(eATRST())
		{
			while(!eAT())
			{
				TaskManager::DelayMs(500);
				++temp;
				if(temp>10)
					return false;
			}
			return 1;
		}
		return 0;
	}
    
    /**
     * Get the version of AT Command Set. 
     * 
     * @return the string of version. 
     */
    char* esp8266::getVersion()
	{
		
		char  *version=NULL;
		eATGMR(&version);		
		return version;
	}
	 /**
	 * Switch the echo function.    
	 * 
	 *  @param mode - 1 start echo -0 stop echo
	 *  @retval true - success. 
	 *  @retval false - failure. 
	 *  
	 */
	bool esp8266::setEcho(uint8_t mode)
	{
		return eATE(mode);
	}
	
	/**
		*  Restore factory.   
		*  @retval true - success.  
		*  @retval false - failure.  
		*  @note  The operation can lead to restart the machine.  
		*/
	bool esp8266::restore(void)
	{
		return eATRESTORE();
	}
	/**
	 * Set up a serial port configuration.  
	 *
	 * @param pattern -1 send "AT+UART=", -2 send "AT+UART_CUR=", -3 send "AT+UART_DEF=". 
	 * @param baudrate - the uart baudrate. 
	 * @retval true - success. 
	 * @retval false - failure. 
	 * @note  Only allows baud rate design, for the other parameters:databits- 8,stopbits -1,parity -0,flow control -0 . 
	 */
	bool esp8266::setUart(uint32_t baudrate,uint8_t pattern)
	{
		return eATSETUART(baudrate,pattern);
	}
    /**
     * Set operation mode to station. 
     *
     * @param   pattern1 -1, send "AT+CWMODE_DEF?",-2,send "AT+CWMODE_CUR?",-3,send "AT+CWMODE?". 
     * @param   pattern2 -1, send "AT+CWMODE_DEF=",-2,send "AT+CWMODE_CUR=",-3,send "AT+CWMODE=". 
     * @retval true - success.
     * @retval false - failure.
     * 
     */
	 bool esp8266::setOprToStation(uint8_t pattern1,uint8_t pattern2)
	{
		return sATCWMODE(1,pattern2);
	}
	

    
    /**
     * Set operation mode to softap.  
     * @param   pattern1 -1, send "AT+CWMODE_DEF?",-2,send "AT+CWMODE_CUR?",-3,send "AT+CWMODE?". 
     * @param   pattern2 -1, send "AT+CWMODE_DEF=",-2,send "AT+CWMODE_CUR=",-3,send "AT+CWMODE=". 
     * 
     * @retval true - success. 
     * @retval false - failure. 
     */
    bool esp8266::setOprToSoftAP(uint8_t pattern1,uint8_t pattern2)
	{
		return sATCWMODE(2,pattern2);
	}
   
    /**
     * Set operation mode to station + softap.  
     * @param   pattern1 -1, send "AT+CWMODE_DEF?",-2,send  "AT+CWMODE_CUR?",-3,send "AT+CWMODE?". 
     * @param   pattern2 -1, send "AT+CWMODE_DEF=",-2,send "AT+CWMODE_CUR=",-3,send "AT+CWMODE=". 
     * 
     * @retval true - success. 
     * @retval false - failure. 
     */
    bool esp8266::setOprToStationSoftAP(uint8_t pattern1,uint8_t pattern2)
	{
		return sATCWMODE(3,pattern2);
	}
    
    /**
     * Get the operation mode.  
     * @param   pattern1 -1, send "AT+CWMODE_DEF?",-2,send  "AT+CWMODE_CUR?",-3,send "AT+CWMODE?". 
     * 
     * @retval 0 - failure.
     * @retval 1 - mode Station.
     * @retval 2 - mode AP. 
     * @retval 3 - mode AP + station. 
     */
    char* esp8266::getOprMode(uint8_t pattern1)
	{		
			char* num;
			qATCWMODE(&num);
			return num;
		
	}
			 /**
     * Get the model values list.  
     * 
     * @return the list of model.  
     */ 
    char* esp8266::getWifiModeList(void)//AT+CWMODE=?
	{
		char* list;
		eATCWMODE(&list);
		return list;
	
	}
	
    
    /**
     * Search available AP list and return it.
     * 
     * @return the list of available APs. 
     * @note This method will occupy a lot of memeory(hundreds of Bytes to a couple of KBytes). 
     *  Do not call this method unless you must and ensure that your board has enough memery left.
     */
    char* esp8266::getAPList(void)
	{
		char *list;
		eATCWLAP(&list);
		return list;
		
	}
    
    /**
     * Search and returns the current connect AP. 
     * 
     * @param pattern -1, send "AT+CWJAP_DEF?",-2,send "AT+CWJAP_CUR?",-3,send "AT+CWJAP?". 
     * @return the ssid of AP connected now. 
     */ 
    char* esp8266::getNowConecAp(uint8_t pattern)
	{
		char *ssid;
		qATCWJAP(&ssid);
		return ssid;
		
	}
    
    /**
     * Join in AP. 
     *
     * @param pattern -1 send "AT+CWJAP_DEF=" -2 send "AT+CWJAP_CUR=" -3 send "AT+CWJAP=". 
     * @param ssid - SSID of AP to join in. 
     * @param pwd - Password of AP to join in. 
     * @retval true - success.
     * @retval false - failure.
     * @note This method will take a couple of seconds. 
     */
    bool esp8266::joinAP(char* ssid, char* pwd,uint8_t pattern)
	{
		return sATCWJAP(ssid,pwd,pattern);
	}
    
    /**
     * Leave AP joined before. 
     *
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::leaveAP(void)//存在BUG
	{
		return eATCWQAP();
	}
    
    /**
     * Set SoftAP parameters. 
     * 
     * @param pattern -1 send "AT+CWSAP_DEF=" -2 send "AT+CWSAP_CUR=" -3 send "AT+CWSAP=". 
     * @param ssid - SSID of SoftAP. 
     * @param pwd - PASSWORD of SoftAP. 
     * @param chl - the channel (1 - 13, default: 7). 
     * @param ecn - the way of encrypstion (0 - OPEN, 1 - WEP, 
     *  2 - WPA_PSK, 3 - WPA2_PSK, 4 - WPA_WPA2_PSK, default: 4). 
     * @retval true - success.
     * @retval false - failure.
     * @note This method should not be called when station mode. 
     */
    bool esp8266::setSoftAPParam(char* ssid, char* pwd, uint8_t chl , uint8_t ecn,uint8_t pattern)
	{
		return sATCWSAP(ssid,pwd,chl,ecn,pattern);
	}
    
    /**
     * get SoftAP parameters. 
     * 
     * @param pattern -1 send "AT+CWSAP_DEF?" -2 send "AT+CWSAP_CUR?" -3 send "AT+CWSAP?". 
     * @note This method should not be called when station mode. 
     */
    char* esp8266::getSoftAPParam(uint8_t pattern)
	{
		char *cwsap;
		qATCWSAP(&cwsap,pattern);
		return cwsap;
		 
	}
    
    /**
     * Get the IP list of devices connected to SoftAP. 
     * 
     * @return the list of IP.
     * @note This method should not be called when station mode. 
     */
	char* esp8266::getJoinedDeviceIP(void)//调试可行,运行不行
	{
		char * list;
		eATCWLIF(&list);
		return list;
	}
    
    /**
     * Get the current state of DHCP. 
     * 
     * @param pattern -1 send "AT+CWDHCP_DEF?" -2 send "AT+CWDHCP_CUR?"  -3 send "AT+CWDHCP?". 
     * @return the state of DHCP.
     * 
     */
    char* esp8266::getDHCP(uint8_t pattern)
	{
		char * list;
		qATCWDHCP(&list);
		return list;
	}
    
     /**
     * Set the  state of DHCP. 
     * @param pattern -1 send "AT+CWDHCP_DEF=" -2 send "AT+CWDHCP_CUR=" -3 send "AT+CWDHCP=". 
     * @param mode - set ap or set station or set ap + station. 
     * @param en - 0 disable DHCP  - 1 enable DHCP. 
     * @retval true - success.
     * @retval false - failure.
     */
     bool esp8266::setDHCP(uint8_t mode, uint8_t en, uint8_t pattern)
	 {
		 return sATCWDHCP(mode,en,pattern);
	 }
     
     /**
     * make boot automatically connected. 
     * @param en -1 enable  -0 disable. 
     * @retval true - success.
     * @retval false - failure.
     */
     bool esp8266::setAutoConnect(uint8_t en)
	 {
		 return false;
	 }
     
     /**
     * Get the station's MAC address. 
     * @param pattern -1 send "AT+CIPSTAMAC_DEF?=" -2 send "AT+CIPSTAMAC_CUR?" -3 send "AT+CIPSTAMAC?". 
     * @return mac address. 
     * @note This method should not be called when ap mode. 
     */
     char* esp8266::getStationMac(uint8_t pattern)
	 {
		 
		 
		 char *mac;
		 qATCIPSTAMAC(&mac,pattern);
		 return mac;
	 
	 }
     
     /**
     * Set the station's MAC address. 
     * @param pattern -1 send "AT+CIPAPMAC_DEF=" -2 send "AT+CIPAPMAC_CUR=" -3 send "AT+CIPAPMAC=". 
     * @param mac - the mac address of station. 
     * @retval true - success.
     * @retval false - failure.
     */
     bool esp8266::setStationMac(char* mac,uint8_t pattern)
	 {
		 
		 return sATCIPSTAMAC(mac,pattern);
	 }
	  /**
     * Get the AP's MAC address. 
     * @param pattern -1 send "AT+CIPAPMAC_DEF?=" -2 send "AT+CIPAPMAC_CUR?" -3 send "AT+CIPAPMAC?". 
     * @return mac address. 
     * @note This method should not be called when ap mode. 
     */
     char* esp8266::getApMac(uint8_t pattern)
	 {
		 
		 
		 char *mac;
		 qATCIPAPMAC(&mac,pattern);
		 return mac;
	 
	 }
     
     /**
     * Set the station's MAC address. 
     * @param pattern -1 send "AT+CIPSTAMAC_DEF=" -2 send "AT+CIPSTAMAC_CUR=" -3 send "AT+CIPSTAMAC=". 
     * @param mac - the mac address of station. 
     * @retval true - success.
     * @retval false - failure.
     */
     bool esp8266::setApMac(char* mac,uint8_t pattern)
	 {
		 
		 return sATCIPAPMAC(mac,pattern);
	 }
     
     /**
     * Get the station's IP. 
     * @param pattern -1 send "AT+CIPSTA_DEF?" -2 send "AT+CIPSTA_CUR?" -3 send "AT+CIPSTA?". 
     * @return the station's IP. 
     * @note This method should not be called when ap mode. 
     */
     char* esp8266::getStationIp(uint8_t pattern)
	 {
		char *ip;
		qATCIPSTA(&ip,pattern);
		if(ip!=0)
			recvFindAndFilter("gateway", "ip:\"", "\"+CIPSTA:gateway", &ip);
			
		return ip;
	 }
     
      /**
     * Set the station's IP. 
     * @param pattern -1 send "AT+CIPSTA_DEF=" -2 send "AT+CIPSTA_CUR=" -3 send "AT+CIPSTA=". 
     * @param ip - the ip of station. 
     * @param gateway -the gateway of station. 
     * @param netmask -the netmask of station.  
     * @retval true - success.
     * @retval false - failure.
     * @note This method should not be called when ap mode. 
     */
     bool esp8266::setStationIp(char* ip,char* gateway,char* netmask,uint8_t pattern)
	 {
			return sATCIPSTA(ip,pattern);
	 }
  /***********************************************已做到这**************************************************************************/   
     /**
     * Get the AP's IP. 
     * @param pattern -1 send "AT+CIPAP_DEF?" -2 send "AT+CIPAP_CUR?" -3 send "AT+CIPAP?". 
     * @return ap's ip. 
     * @note This method should not be called when station mode. 
     * 
     */
     char* esp8266::getAPIp(uint8_t pattern)
	 {
		 char* ip=NULL;
		 qATCIPAP(&ip,ESP8266_DEFAULT_PATTERN);
		 if(ip!=0)
			recvFindAndFilter("gateway", "ip:\"", "\"+CIPSTA:gateway", &ip);
		return ip;
	 }
     
     /**
     * Set the AP IP. 
     * @param pattern -1 send "AT+CIPAP_DEF=" -2 send "AT+CIPAP_CUR=" -3 send "AT+CIPAP=". 
     * @param ip - the ip of AP. 
     * @retval true - success.
     * @retval false - failure.
     * @note This method should not be called when station mode.
     */
     bool esp8266::setAPIp(char* ip,uint8_t pattern)
	 {
		 return false;
	 }
     
     /**
     * start smartconfig. 
     * @param type -1:ESP_TOUCH  -2:AirKiss. 
     * @retval true - success.
     * @retval false - failure.
     */
     bool esp8266::startSmartConfig(uint8_t type)
	 {
		 return false;
	 }
     
     /**
     * stop smartconfig. 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
     bool esp8266::stopSmartConfig(void)
	 {
		 return false;
	 }
    
    /**
     * Get the current status of connection(UDP and TCP). 
     * 
     * @return the status. 
     */
    char* esp8266::getIPStatus(void)
	{
		return 0;
	}
    
    /**
     * Get the IP address of ESP8266. 
     *
     * @return the IP list. 
     */
    char* esp8266::getLocalIP(void)
	{
		return 0;
	}
    
    /**
     * Enable IP MUX(multiple connection mode). 
     *
     * In multiple connection mode, a couple of TCP and UDP communication can be builded. 
     * They can be distinguished by the identifier of TCP or UDP named mux_id. 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::enableMUX(void)
	{
		return sATCIPMUX(1);
	}
    
    /**
     * Disable IP MUX(single connection mode). 
     *
     * In single connection mode, only one TCP or UDP communication can be builded. 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::disableMUX(void)
	{
		return sATCIPMUX(0);
	}
    
    /**
     * Create TCP connection in single mode. 
     * 
     * @param addr - the IP or domain name of the target host. 
     * @param port - the port number of the target host. 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::createTCP(char* addr, uint32_t port)
	{
		return false;
	}
    
    /**
     * Release TCP connection in single mode. 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::releaseTCP(void)
	{
		return false;
	}
    
    /**
     * Register UDP port number in single mode.
     * 
     * @param addr - the IP or domain name of the target host. 
     * @param port - the port number of the target host. 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::registerUDP(char* addr, uint32_t port)
	{
		return false;
	}
    
    /**
     * Unregister UDP port number in single mode. 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::unregisterUDP(void)
	{
		return false;
	}
  
    /**
     * Create TCP connection in multiple mode. 
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @param addr - the IP or domain name of the target host. 
     * @param port - the port number of the target host. 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::createTCPMutipleMode(uint8_t mux_id, char* addr, uint32_t port)
	{
		char type[]="TCP";
		return sATCIPSTARTMultiple(mux_id,type,addr,port);
	}
	
    
    /**
     * Release TCP connection in multiple mode. 
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::releaseTCPMutipleMode(uint8_t mux_id)
	{
		return false;
	}
    
    /**
     * Register UDP port number in multiple mode.
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @param addr - the IP or domain name of the target host. 
     * @param port - the port number of the target host. 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::registerUDPMutipleMode(uint8_t mux_id, char* addr, uint32_t port)
	{
		return false;
	}
    
    /**
     * Unregister UDP port number in multiple mode. 
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::unregisterUDPMutipleMode(uint8_t mux_id)
	{
		return false;
	}

    /**
     * Set the timeout of TCP Server. 
     * 
     * @param timeout - the duration for timeout by second(0 ~ 28800, default:180). 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::setTCPServerTimeout(uint32_t timeout)
	{
		return sATCIPSTO(timeout);
	}
    
    /**
     * Start TCP Server(Only in multiple mode). 
     * 
     * After started, user should call method: getIPStatus to know the status of TCP connections. 
     * The methods of receiving data can be called for user's any purpose. After communication, 
     * release the TCP connection is needed by calling method: releaseTCP with mux_id. 
     *
     * @param port - the port number to listen(default: 333).
     * @retval true - success.
     * @retval false - failure.
     *
     * @see char* getIPStatus(void);
     * @see uint32_t recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t len, uint32_t timeout);
     * @see u8 releaseTCP(uint8_t mux_id);
     */
    bool esp8266::startTCPServer(uint32_t port)
	{
		return sATCIPSERVER(1,port);
	}

    /**
     * Stop TCP Server(Only in multiple mode). 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::stopTCPServer(void)
	{
		return true;
	}
    
    /**
     *Set the module transfer mode
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::setCIPMODE(uint8_t mode)
	{
		return true;
	}
    
    /**
     * Start Server(Only in multiple mode). 
     * 
     * @param port - the port number to listen(default: 333).
     * @retval true - success.
     * @retval false - failure.
     *
     * @see char* getIPStatus(void);
     * @see uint32_t recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t len, uint32_t timeout);
     */
    bool esp8266::startServer(uint32_t port)
	{
		return sATCIPSERVER(1,port);
	}

    /**
     * Stop Server(Only in multiple mode). 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::stopServer(void)
	{
		return true;
	}
    /**
     * Save the passthrough links
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::saveTransLink (uint8_t mode,char* ip,uint32_t port)
	{
		return true;
	}
    
   /**
    * PING COMMAND. 
    * 
    * @retval true - success.
    * @retval false - failure.
    */
    bool esp8266::setPing(char* ip)
	{
		return true;
	}

    /**
     * Send data based on TCP or UDP builded already in single mode. 
     * 
     * @param buffer - the buffer of data to send. 
     * @param len - the length of data to send. 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::send(const uint8_t *buffer, uint32_t len)
	{
		return true;
	}
            
    /**
     * Send data based on one of TCP or UDP builded already in multiple mode. 
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @param buffer - the buffer of data to send. 
     * @param len - the length of data to send. 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::sendMultipleMode(uint8_t mux_id, char *buffer, uint32_t len)
	{
		return sATCIPSENDMultiple(mux_id,buffer,len);
	}
    
    /**
     * Send data based on TCP or UDP builded already in single mode. 
     * 
     * @param buffer - the buffer of data to send from flash memeory. 
     * @param len - the length of data to send. 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::sendFromFlash(const uint8_t *buffer, uint32_t len)
	{
		return true;
	}
            
    /**
     * Send data based on one of TCP or UDP builded already in multiple mode. 
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @param buffer - the buffer of data to send from flash memeory. 
     * @param len - the length of data to send. 
     * @retval true - success.
     * @retval false - failure.
     */
    bool esp8266::sendFromFlashMultipleMode(uint8_t mux_id, const uint8_t *buffer, uint32_t len)
	{
		return true;
	}
    
    /**
     * Receive data from TCP or UDP builded already in single mode. 
     *
     * @param buffer - the buffer for storing data. 
     * @param buffer_size - the length of the buffer. 
     * @param timeout - the time waiting data. 
     * @return the length of data received actually. 
     */
    uint32_t esp8266::recv(uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
	{
		return false;
	}
    
    /**
     * Receive data from one of TCP or UDP builded already in multiple mode. 
     *
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @param buffer - the buffer for storing data. 
     * @param buffer_size - the length of the buffer. 
     * @param timeout - the time waiting data. 
     * @return the length of data received actually. 
     */
    uint32_t esp8266::recvMultipleMode(uint8_t mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
	{
		return false;
	}
    
    /**
     * Receive data from all of TCP or UDP builded already in multiple mode. 
     *
     * After return, coming_mux_id store the id of TCP or UDP from which data coming. 
     * User should read the value of coming_mux_id and decide what next to do. 
     * 
     * @param coming_mux_id - the identifier of TCP or UDP. 
     * @param buffer - the buffer for storing data. 
     * @param buffer_size - the length of the buffer. 
     * @param timeout - the time waiting data. 
     * @return the length of data received actually. 
     */
    uint32_t esp8266::recvMultipleModeComingmux_id(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
	{
		return 0;
	}
	
	
	bool esp8266::eATCIFSR(char** list)
	{
		char* data_temp;
		rx_empty();
		mUsart<<"AT+CIFSR\r\n";
		data_temp = recvString("OK", "ERROR");
		if(strstr(data_temp,"OK"))
			return true;
		else//strstr(ssid,"FAIL")
			return false;
	}
	
	bool esp8266::CloseMulitpleSend(uint8_t mux_id)
	{
		return sATCIPCLOSEMulitple(mux_id);
	}

