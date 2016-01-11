package deal;

import bean.Message;
import constant.ConstantCode;
import dao.UpdateNodeStatus;
import tool.AndVerify;
import tool.DataTransform;

public class NodeStatusUpdate {
	/**
	 * 更改车位状态
	 * 
	 * @param msgBean
	 * @return
	 */
	public static byte[] update(Message msgBean) {

		byte[] bMsgBody = UpdateNodeStatus.update(msgBean.getbMsgBody());// 获取要返回的消息体内容
		byte[] gMsgLen = DataTransform.intToByteGroup(bMsgBody.length, 2);// 消息体长度

		msgBean.setbMsgCmd(DataTransform.intToByteGroup(ConstantCode.cAckNodeStatusUpdate, 2));
		msgBean.setgMsgLen(gMsgLen);
		msgBean.setbMsgBody(bMsgBody);
		msgBean.setbMsgVrf(DataTransform
				.intToByteGroup(AndVerify.SUM16(msgBean.toBytesNotCheck(), msgBean.toBytesNotCheck().length), 1));

		return msgBean.toBytes();// 返回数据包
	}
}
