package test;

import tool.AndVerify;

public class testAndVer {
	public static byte[] buf = { (byte) 0xA4, 0x02, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x02, 0x01, 0x01, 0x00,
			0x00, (byte)0xB7};
	/**
	 * 对接受到的数据包进行和校验，判断是否正确 
	 * @param recvMsg
	 * @return boolean 
	 */
	public static boolean checkVrf(byte[] recvMsg){
		byte[] checkByte=recvMsg;
		for (int i = 0; i < recvMsg.length-1; i++) {
			checkByte[i]=recvMsg[i];
		}
		int verify=AndVerify.SUM16(checkByte, checkByte.length);
		System.out.println("verify: "+verify);
		
		return true;
	}
	public static void main(String[] args) {
		System.out.println(checkVrf(buf));
	}
	
	/*
	 public static byte[] GtwLogin(Message msgBean) {

		byte[] bMsgBody = { 0x01 };
		byte[] gMsgLen = DataTransform.intToByteGroup(bMsgBody.length, 2);

		msgBean.setbMsgCmd(DataTransform.intToByteGroup(ConstantCode.cAckGateWayLogin, 2));
		msgBean.setgMsgLen(gMsgLen);
		msgBean.setbMsgBody(bMsgBody);
		msgBean.setbMsgVrf(DataTransform
				.intToByteGroup(AndVerify.SUM16(msgBean.toBytesNotCheck(), msgBean.toBytesNotCheck().length), 1));

		return msgBean.toBytes();
	}
	 */
}
