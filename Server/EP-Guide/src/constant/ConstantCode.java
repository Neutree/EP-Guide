package constant;

public class ConstantCode {
	//-- 客户端请求命令字 --//
	public static final int Req_Login=1001; //登录请求
	public static final int Req_Resgiter=1002; //注册请求
	public static final int Req_TokenLogin=1003; //二次登录请求
	public static final int Req_OrderPark=1004; //预约停车请求
	public static final int Req_OrderParkSpace=1005; //预约车位请求
	public static final int Req_HistoryParkQuery=1006; //停车记录查询请求
	
	//-- 服务器响应成功命令字 --//
	public static final int Res_Login=8001; //登录响应
	public static final int Res_Resgiter=8002; //注册响应
	public static final int Res_TokenLogin=8003; //二次登录响应
	public static final int Res_OrderPark=8004; //预约停车响应
	public static final int Res_OrderParkSpace=8005; //预约车位响应
	public static final int Res_HistoryParkQuery=8006; //停车记录查询响应
	
	//-- 服务器响应失败命令字 --//
	public static final int Res_Fail_Login=2001; //登录响应
	public static final int Res_Fail_Resgiter=2002; //注册响应
	public static final int Res_Fail_TokenLogin=2003; //二次登录响应
	public static final int Res_Fail_OrderPark=2004; //预约停车响应
	public static final int Res_Fail_OrderParkSpace=2005; //预约车位响应
	public static final int Res_Fail_HistoryParkQuery=2006; //停车记录查询响应
	
	//-- 其他 --//
	public static final int Res_Resgitered=7002; //已经注册
	public static final int Res_NotResgiter=6002; //未注册
	public static final int Res_Wrong_Pwd=4002; //未注册
	public static final int Res_Abnormal_TokenLogin=7003; //token登录账号异常，已有人在其他地方用帐密登录过
	public static final int Res_Illegal_User=3001; //非法用户访问
	public static final int legal_User=3000; //合法用户访问
	public static final int Res_NotGrageID=6004; //没有此车库ID
	public static final int Res_parkSpace_FUll=8800; //车位已被占用
	public static final int Res_parkSpace_Empty=8801; //车位空闲
	public static final int Res_NotPSpaceID=6005; //没有此车位ID
	public static final int Res_NotPSpaceHistory=6006; //没有此历史停车记录
	public static final int Res_Fail_QueryPSpaceStatus=6007; //查询车位状态失败
	
	
}
