package decode;

import bean.Message;
import tool.DataTransform;

public class DecodeMsg {
	byte[] buf = { (byte) 0xA4, 0x02, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x02, 0x01, 0x01, 0x00,
			0x00, 0x00 };

	/*
	 * 解析接受到的消息，返回消息体内容，如果消息包有误则返回空
	 * 
	 */
	public static Message Decode(byte[] recvMessage) {
		byte[] message = recvMessage;
		int msgLen = message.length;
		Message msgBean = null;
		try {
			if (msgLen < 15) {// 数据包缺位，返回空
				return null;
			}
			int readPos = 0;// 字符数组读指针初始化
			byte[] msgHead = new byte[] { message[readPos++], message[readPos++] };
			byte[] msgID = new byte[] { message[readPos++], message[readPos++] };
			byte[] gtwMac = new byte[6];
			for (int k = 0; k < gtwMac.length; k++) {
				gtwMac[k] = message[readPos++];
			}
			byte[] msgBodyLen = new byte[] { message[readPos++], message[readPos++] };
			byte[] msgCmd = new byte[] { message[readPos++], message[readPos++] };
			int bodyLen = DataTransform.byteGroupToInt(msgBodyLen);
			byte[] msgBody = new byte[bodyLen];
			for (int i = 0; i < bodyLen; i++) {
				msgBody[i] = message[readPos++];
			}
			byte[] msgVrf = new byte[] { message[readPos++] };
			
			msgBean = new Message(msgHead, msgID, gtwMac, msgBodyLen, msgCmd, msgBody, msgVrf);
			System.out.println("RecvBean: " + msgBean.toString());

		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("数据包异常 :");
			e.printStackTrace();
			return null;//返空
		}

		return msgBean;
	}

}
