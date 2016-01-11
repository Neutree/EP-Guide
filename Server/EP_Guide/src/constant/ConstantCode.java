package constant;

public class ConstantCode {
	/***************************与服务器通信的消息命令常量*******************/
	
	//| 基本请求 | 登录、心跳包等 |
	public static final byte[] cReqGateWayLogin={(byte) 0x0101};
	public static final byte[] cAckGateWayLogin={(byte) 0x0801};
	public static final byte[] cReqLinkCheck={(byte) 0x0102};
	public static final byte[] cAckLinkCheck={(byte) 0x0802};
	
	//| 节点操作 | 管理员操作时更改 | 
	public static final byte[] cReqAddNode={(byte) 0x0104};
	public static final byte[] cAckAddNode={(byte) 0x0804};
	public static final byte[] cReqNodeDel={(byte) 0x0105};
	public static final byte[] cAckNodeDel={(byte) 0x0805};
	public static final byte[] cReqNodeNameUpdate={(byte) 0x0106};
	public static final byte[] cAckNodeNameUpdate={(byte) 0x0806};
	//| 控制 | 客户端预约车位时控制 |  
	public static final byte[] cReqNodeStatusUpdate={(byte) 0x0107};
	public static final byte[] cAckNodeStatusUpdate={(byte) 0x0807};
	
	//| 同步 | 车位状态改变时同步 | 
	public static final byte[] cReqNotNodeInforSync={(byte) 0x0108};
	public static final byte[] cAckNotNodeInforSync={(byte) 0x0808};
	
	//| 触发模式 | 车辆入库时触发 |   
	public static final byte[] cReqShortLoad={(byte) 0x0103};
	public static final byte[] cAckShortLoad={(byte) 0x0803};
	public static final byte[] cReqParkSpaceNum={(byte) 0x0109};
	public static final byte[] cAckParkSpaceNum={(byte) 0x0809};
}
