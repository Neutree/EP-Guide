##网关—Server通信协议（Version 1.0）
#####修改记录：
	version 1.0  时间 2016-1-4 新增
    version 1.01 时间 待定

#####一、协议简介
	网关和服务器之间采用Socket进行通信（Socket是对TCP/IP协议的封装和应用）
	网关-服务端TCP监听端口：5080
	服务器-客户端通信监听端口：5090

#####二、	报文结构介绍
注:整个通信的报文采用16进制格式

| 传输层 |||| 业务层 |||
|--------|--------|
| 消息头 | 消息ID | 网关Mac地址 | 消息长度 | 命令字 | 消息体 | 校验字 |
| gMsgHead | gMsgID | gGtwMac | gMsgLen | bMsgCmd | bMsgBody | bMsgVrf |
| 2Byte | 2Byte | 8Byte | 2Byte | 2Byte | nByte | 1Byte |

#####三、 命令集

| 命令分类 | 命令描述 | 命令字 | 命令编码 | 传输方向 |
|--------|--------|
| 基本请求 | 登录、心跳包等 |
|    1    |网关登录服务器请求 | cReqGateWayLogin | 0x0101 | 网关-Server |
|    2    | 服务器响应网关登录 | cAckGateWayLogin | 0x0801 | Server-网关 |
|    3    | 链路请求 | cReqLinkCheck | 0x0102 | 网关-Server |
|    4    | 链路请求响应 | cAckLinkCheck | 0x0802 | Server-网关 |
| 节点操作 | 管理员操作时更改 |
|    1    | 节点添加请求 | cReqAddNode | 0x0104 | 网关-Server |
|    2    | 节点添加响应 | cAckAddNode | 0x0804 | Server-网关 |
|    3    | 节点删除请求 | cReqNodeDel | 0x0105 | 网关-Server |
|    4    | 节点删除响应 | cAckNodeDel | 0x0805 | Server-网关 |
|    5    | 节点别名修改请求 | cReqNodeNameUpdate | 0x0106 | 网关-Server |
|    6    | 节点别名修改响应 | cAckNodeNameUpdate | 0x0806 | Server-网关 |
| 控制 | 客户端预约车位时控制 |
|    1    | 更改车位状态请求 | cReqNodeStatusUpdate | 0x0107 | 网关-Server |
|    2    | 更改车位状态响应 | cAckNodeStatusUpdate | 0x0807 | Server-网关 |
| 同步 | 车位状态改变时同步 |
|    1    | 节点信息同步通知 | cReqNotNodeInforSync | 0x0108 | 网关-Server |
|    2    | 节点信息同步通知响应 | cAckNotNodeInforSync | 0x0808 | Server-网关 |
| 触发模式 | 车辆入库时触发 |
|    1    | 最短路线配置请求 | cReqShortLoad | 0x0103 | 网关-Server |
|    2    | 最短路线配置请求 | cAckShortLoad | 0x0803 | Server-网关 |
|    3    | 车位数量同步请求 | cReqShortLoad | 0x0109 | 网关-Server |
|    4    | 车位数量同步响应 | cAckShortLoad | 0x0809 | Server-网关 |