#ifndef __COMMUNICATE_H
#define __COMMUNICATE_H

#include "esp8266.h"

/***************************与服务器通信的消息命令常量*******************/
//| 基本请求 | 登录、心跳包等 |
#define TO_SERVER_cReqGateWayLogin      0x0101    //网关登录服务器请求
#define TO_SERVER_cAckGateWayLogin      0x0801    //服务器响应网关登录
#define TO_SERVER_cReqLinkCheck         0x0102    //链路请求(消息体空)
#define TO_SERVER_cAckLinkCheck         0x0802    //链路请求响应(消息体空)
                                        
//| 节点操作 | 管理员操作时更改 |       
#define TO_SERVER_cReqAddNode           0x0104    //节点添加请求
#define TO_SERVER_cAckAddNode           0x0804    //节点添加响应
#define TO_SERVER_cReqNodeDel           0x0105    //节点删除请求
#define TO_SERVER_cAckNodeDel           0x0805    //节点删除响应
#define TO_SERVER_cReqNodeNameUpdate    0x0106    //节点别名修改请求
#define TO_SERVER_cAckNodeNameUpdate    0x0806    //节点别名修改响应
                                        
//| 控制 | 客户端预约车位时控制 |       
#define TO_SERVER_cReqNodeStatusUpdate  0x0107    //更改车位状态请求
#define TO_SERVER_cAckNodeStatusUpdate  0x0807    //更改车位状态响应
                                        
//| 同步 | 车位状态改变时同步 |         
#define TO_SERVER_cReqNotNodeInforSync  0x0108    //节点信息同步通知
#define TO_SERVER_cAckNotNodeInforSync  0x0808    //节点信息同步通知响应
                                        
//| 触发模式 | 车辆入库时触发 |         
#define TO_SERVER_cReqShortLoad         0x0103    //最短路线配置请求
#define TO_SERVER_cAckShortLoad         0x0803    //最短路线配置请求
#define TO_SERVER_cReqParkSpaceNum      0x0109    //车位数量同步请求
#define TO_SERVER_cAckParkSpaceNum      0x0809    //车位数量同步响应

class Communicate
{
	
	public:
		/****************************通信*****************************************/
		////////////////////////////
		///发送一个字节到服务器
		///@param wifi wifi对象
		///@param ipAddress 服务器IP地址
		///@param port 服务器端口
		///@param data 需要发送的一个字节的数据
		///@param 是否发送成功（对方不一定接收到，只是发送成功）
		////////////////////////////
		static bool SendBytesToServer(esp8266 & wifi,char* ipAddress,uint32_t port,char* data,uint32_t len);
	
	
		//////////////////////////////
		///发送http请求到服务器
		///@param wifi wifi对象
		///@param ipAddress 服务器IP地址
		///@param port 服务器端口
		///@param data 请求内容
		///@param 是否发送成功（对方不一定接收到，只是发送成功）
		/////////////////////////////
		static bool HttpRequest(esp8266 & wifi,char* ipAddress,uint32_t port,char* data);
		/*******************************************************************************/
	
	public:
		/*****************************协议************************************************/
		///////////////////////
		///链路请求、响应包
		//////////////////////
		static char mToServerLinkCheckPack[15];
	
	
		/////////////////////////
		///记录消息ID号（递增）
		/////////////////////////
		static uint16_t ToServerGenerateMessageID();
		/**********************************************************************************/
};

#endif

