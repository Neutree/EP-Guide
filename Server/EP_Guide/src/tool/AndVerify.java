package tool;

public class AndVerify {

	/**
	 * 和校验 ,(需要用来计算和校验码的原始数据, 数据长度（字节）)
	 * 
	 * @param data
	 * @param len
	 * @return
	 */
	public static int SUM16(byte[] data, int len) {
		int sum = 0;
		for (int i = 0; i < len; ++i)
			sum += data[i];
		return sum & 0x00ff;
	}

	/**
	 * 对接受到的数据包进行和校验，判断是否正确
	 * 
	 * @param recvMsg
	 * @return boolean
	 */
	public static boolean checkVrf(byte[] recvMsg, byte[] vrf) {
		byte[] checkByte = new byte[recvMsg.length - 1];
		for (int i = 0; i < recvMsg.length - 1; i++) {
			checkByte[i] = recvMsg[i];
		}
		int verify = AndVerify.SUM16(checkByte, checkByte.length);
		if (verify != DataTransform.byteGroupToInt(vrf)) {
			return false;
		}
		return true;
	}
}
