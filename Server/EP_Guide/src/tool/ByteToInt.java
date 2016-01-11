package tool;

public class ByteToInt {
	/**
	 * 将byte数组转换为int,若数组为null或者长度为0，返回0
	 */
	public static int byteGroupToInt(byte[] b) {
		if (b != null) {
			int value = 0;
			// 由高位到低位
			for (int i = 0; i < b.length; i++) {
				int shift = (b.length - 1 - i) * 8;
				value += (b[i] & 0x000000FF) << shift;// 往高位游
			}
			return value;
		}
		return 0;
	}

}
