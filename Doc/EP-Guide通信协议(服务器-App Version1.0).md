##App—Server通信协议（Version 1.0）
####修改记录：
	version 1.0  时间 2016-1-4 内容：基本协议的制定
    version 1.01 时间 待定 内容： 更新、新增、删除
####一、协议简介
	客户端和服务器之间采用TCP进行通信（客户端只需请求服务器、等待服务器应答）
	客户端请求端口：5090

####二、接口定义

#####用户登录
_ _ _

- 功能：App登录服务器
- 描述：客户端用MD5对密码进行加密，服务器将发来的的密码进行验证，并生成新的token
- 请求地址(url):http://www.doubleeggs.com:8080/EP-Guide/login
- 提交格式：{"action":"1001", "username":"dandan","password":"gg"}
  {"action":"1001","username":"dandan","password":"123456","token":"5200"}
  提交命令字：1001-请求登录
  提交参数：username,password
- 返回格式：{"result":"xxx","token":"xxx"};
  返回命令字：2001-登录失败
  			4002-密码错误
  			6002-账号未注册
　　　　　　  8001-登录成功
  返回参数：result,token
- 使用方法：解析JSON对象，先获取result，如果为2001，则data里数据为空

#####用户注册
_ _ _

- 功能：App向服务器进行注册
- 描述：客户端用MD5对密码进行加密，服务器将发来的的密码进行验证，并生成新的token
- 请求地址(url):http://www.doubleeggs.com:8080/EP-Guide/register
- 提交格式：{"action":"1002", "username":"dan","password":"gg","car_ID":"渝B666668"}
  提交命令字：1002-请求注册
  提交参数：username,password,car_ID(车牌号)
- 返回格式：{"result":xxx, "data":"{…}"};
  返回命令字：2002-失败
  		  7002-已注册
　　　　　　8002-成功
  返回参数：token
- 使用方法：解析JSON对象，先获取result，如果为2002，则代表注册失败，data里数据为空

#####二次登录(token)
_ _ _

- 功能：App通过本地缓存的token和username向服务器进行登录验证
- 描述：服务器将客户端发来的token与进行验证，如果不通过则强制用户重新登录
- 请求地址(url):http://www.doubleeggs.com:8080/EP-Guide/tokenLogin
- 提交格式：{"action":"1003", "username":"dandan","token":"ad1c0358ae459416e567c3405eb7b963"}
  提交命令字：1003-请求二次登录
  提交参数：username,token
- 返回格式：{"result":xxx, "data":"{…}"};
  返回命令字：2003-失败
  		  6002-账号未注册
　　　　　　7003-账号异常
　　　　　　8003-成功
  返回参数：null
- 使用方法：解析JSON对象，先获取result，如果为2003，则代表操作失败，7003则代表已有人在其他地方登录过此号，需让用户重新登录，如为8003则登录成功

#####预约停车
_ _ _

- 功能：App向服务器请求各个车位的信息
- 描述：
- 请求地址(url):http://localhost:8080/EP-Guide/queryParkSpaces
- 提交格式：{"action":"1004", "token":"078701c50c5f0fd4395ecd284c645439","garage_ID":"1"}
  提交命令字：1004-请求车库里所有车位信息
  提交参数：garage_ID,token
- 返回格式：{"result":xxx, "data":"{…}"};
  返回命令字：2004-失败
  　　　　　　3001-非法用户访问,token不正确，需重新登陆
  　　　　　　6004-没有此车库ID
　　　　　　8004-成功
  返回参数：datalist (List<bean>)
  返回格式：{"result":0,"dataList":"[{\"ID\":\"1\",\"garage_ID\":\"1\",\"park_ID\":\"1\",\"park_spaceName\":\"dan\",\"start_time\":123456,\"status\":\"1\"}]"}
- ParkSpace(Bean)构造：String ID,String park_ID, String park_spaceName, String garage_ID, String status, long start_time
- 使用方法：解析JSON对象，先获取result，如果为2004，则代表操作失败，data里数据为空，如果为8004，则返回一个带有List<bean>格式的队列,用Bean来逐个解析list

#####预约车位
_ _ _

- 功能：App向服务器进行请求更改某一个车位的状态信息
- 描述：
- 请求地址(url):http://localhost:8080/EP-Guide/orderParkSpace
- 提交格式：{"action":"1005", "token":"078701c50c5f0fd4395ecd284c645439","garage_ID":"1","car_ID":"1","pSpace_ID":"1"}
  提交命令字：1005-请求更改某一个车位的状态信息（空闲->占用）
  提交参数：garage_ID,pSpace_ID,car_ID,token
- 返回格式：{"result":xxx, "data":"{…}"};
  返回命令字：2005-失败
　　　　　　8005-成功
  返回参数：null
- 使用方法：解析JSON对象，先获取result，如果为2004，则代表操作失败，data里数据为空，如果为8004，则返回一个带有hashMap格式的队列,键值对格式为(pSpace_ID,pSpace_status),ID从1开始取值

#####停车记录查询
_ _ _

- 功能：App向服务器请求
- 描述：客户端用MD5对密码进行加密，服务器将发来的的密码进行验证，并生成新的token
- 请求地址(url):ep-guide/api/xxx.java
- 提交格式：{"action":xxx, "data":"{...}"};
  提交命令字：1006-请求查询停车记录
  提交参数：garage_ID,car_ID
- 返回格式：{"result":xxx, "data":"{…}"};
  返回命令字：2006-失败
　　　　　　8006-成功
  返回参数：park_history (List<Bean>)
  参数说明：
  Bean - parked
	```
	park_ID （停车位ID）
	garage_ID （车库ID）
	getIn_time(入库时间,时间戳,解析为年-月-日-分)
	park_time (停车时长,时间戳，解析为小时)
	```

- 使用方法：解析JSON对象，先获取result，如果为2002，则代表注册失败，data里数据为空,如果为8006，则返回一个停车历史纪录的List,需遍历逐一取出.

#####心跳请求
_ _ _

- 功能：建立客户端与服务器之间的心跳，判断双方是否在线
- 描述：因为Http是一种无状态协议，所有App需定时向服务器发送心跳请求，服务器收到后进行心跳响应，
- 请求地址(url):ep-guide/api/xxx.java
- 提交格式：{"action":xxx, "data":"{...}"};
  提交命令字：1007-请求
  提交参数：
- 返回格式：{"result":xxx, "data":"{…}"};
  返回命令字：2007-失败
　　　　　　8007-成功
　　　　　　9007-成功且有新的消息
  返回参数：Msg_backHome(String)
- 使用方法：解析JSON对象，先获取result，如果为2007，则代表注册失败，返回值为空，如果为8007则表示服务器收到请求，返回值为空，如果9007则表示有家人回家了

#####模板
_ _ _

- 功能：
- 描述：
- 请求地址(url):ep-guide/api/xxx.java
- 提交格式：{"action":xxx, "data":"{...}"};
  提交命令字：100x-请求
  提交参数：
- 返回格式：{"result":xxx, "data":"{…}"};
  返回命令字：200x-失败
　　　　　　800x-成功
  返回参数：
- 使用方法：解析JSON对象，先获取result，如果为2002，则代表注册失败，data里数据为空