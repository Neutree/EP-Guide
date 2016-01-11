package decode;

import bean.Message;
import tool.ByteToInt;
import tool.OutputByteGrp;

public class DecodeMsg {
	byte[] buf = { (byte) 0xA4, 0x02, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x02, 0x01, 0x01, 0x00,
			0x00, 0x00 };

	/*
	 * 解析接受到的消息，返回消息体内容，如果消息包有误则返回空
	 * 
	 */
	public static Message Decode(byte[] recvMessage) {

		byte[] message = recvMessage;
		OutputByteGrp.output(message);
		int msgLen = message.length;
		if (msgLen < 15) {// 数据包缺位，返回空
			return null;
		}
		int readPos = 0;// 字符数组读指针初始化
		byte[] msgHead = new byte[] { message[readPos++], message[readPos++] };
		OutputByteGrp.output(msgHead);
		byte[] msgID = new byte[] { message[readPos++], message[readPos++] };
		OutputByteGrp.output(msgID);
		byte[] gtwMac = new byte[6];
		for (int k = 0; k < gtwMac.length; k++) {
			gtwMac[k] = message[readPos++];
		}
		OutputByteGrp.output(gtwMac);
		byte[] msgBodyLen = new byte[] { message[readPos++], message[readPos++] };
		OutputByteGrp.output(msgBodyLen);
		byte[] msgCmd = new byte[] { message[readPos++], message[readPos++] };
		OutputByteGrp.output(msgCmd);
		int bodyLen = ByteToInt.byteGroupToInt(msgBodyLen);
		byte[] msgBody = new byte[bodyLen];

		for (int i = 0; i < bodyLen; i++) {
			msgBody[i] = message[readPos++];
		}
		OutputByteGrp.output(msgBody);
		byte[] msgVrf = new byte[] { message[readPos++] };
		OutputByteGrp.output(msgVrf);
		Message msgBean = new Message(msgHead, msgID, gtwMac, msgBodyLen, msgCmd, msgBody, msgVrf);
		System.out.println("RecvBean: " + msgBean.toString());

		return msgBean;
	}

}
