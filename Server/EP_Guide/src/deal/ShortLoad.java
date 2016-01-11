package deal;

import java.util.ArrayList;
import java.util.List;

import bean.Message;
import bean.Node;
import constant.ConstantCode;
import tool.AndVerify;
import tool.DataTransform;

public class ShortLoad {
	/**
	 * 配置最短路径
	 * 
	 * @param msgBean
	 * @return
	 */
	public static byte[] update(Message msgBean) {

		byte[] bMsgBody = getShortLoad(msgBean.getbMsgBody());// 获取要返回的消息体内容
		byte[] gMsgLen = DataTransform.intToByteGroup(bMsgBody.length, 2);// 消息体长度

		msgBean.setbMsgCmd(DataTransform.intToByteGroup(ConstantCode.cAckShortLoad, 2));
		msgBean.setgMsgLen(gMsgLen);
		msgBean.setbMsgBody(bMsgBody);
		msgBean.setbMsgVrf(DataTransform
				.intToByteGroup(AndVerify.SUM16(msgBean.toBytesNotCheck(), msgBean.toBytesNotCheck().length), 1));

		return msgBean.toBytes();// 返回数据包
	}

	public static byte[] getShortLoad(byte[] msgbody) {
		// -- 解析消息体--//
		int readPos = 0;// 字符数组读指针初始化
		byte[] nodeNum = new byte[] { msgbody[readPos++], msgbody[readPos++] };
		byte[] emptyNodeNum = new byte[] { msgbody[readPos++], msgbody[readPos++] };
		List<Node> nodeList = new ArrayList<Node>();
		byte[] result = null;
		for (int i = 0; i < DataTransform.byteGroupToInt(emptyNodeNum); i++) {
			byte[] nodeID = new byte[6];
			for (int k = 0; k < 6; k++) {
				nodeID[k] = msgbody[readPos++];
			}
			byte[] nodeStatus = new byte[] { msgbody[readPos++], msgbody[readPos++] };
			byte[] userID = null;
			Node node = new Node(DataTransform.bytesToString(nodeID), DataTransform.byteGroupToInt(nodeStatus),
					DataTransform.bytesToString(userID));
			nodeList.add(node);
		}
		for (int i = 0; i < nodeList.size(); i++) {
			if (nodeList.get(i).getStatus() == 0) {
				result = nodeList.get(i).toBytes();
			}
		}

		return result;
	}
}
