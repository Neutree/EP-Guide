package constant;

public class ConstantCode {
	/*************************** 与服务器通信的消息命令常量 *******************/

	// 操作成功、失败
	public static final byte[] Succeed = { 0x00, 0x01 };
	public static final byte[] Failed = { 0x00, 0x00 };

	// | 基本请求 | 登录、心跳包等 |
	public static final int cReqGateWayLogin = 0x0101;
	public static final int cAckGateWayLogin = 0x0801;
	public static final int cReqLinkCheck = 0x0102;
	public static final int cAckLinkCheck = 0x0802;

	// | 节点操作 | 管理员操作时更改 |
	public static final int cReqAddNode = 0x0104;
	public static final int cAckAddNode = 0x0804;
	public static final int cReqNodeDel = 0x0105;
	public static final int cAckNodeDel = 0x0805;
	public static final int cReqNodeNameUpdate = 0x0106;
	public static final int cAckNodeNameUpdate = 0x0806;
	// | 控制 | 客户端预约车位时控制 |
	public static final int cReqNodeStatusUpdate = 0x0107;
	public static final int cAckNodeStatusUpdate = 0x0807;

	// | 同步 | 车位状态改变时同步 |
	public static final int cReqNotNodeInforSync = 0x0108;
	public static final int cAckNotNodeInforSync = 0x0808;

	// | 触发模式 | 车辆入库时触发 |
	public static final int cReqShortLoad = 0x0103;
	public static final int cAckShortLoad = 0x0803;
	public static final int cReqParkSpaceNum = 0x0109;
	public static final int cAckParkSpaceNum = 0x0809;
}
