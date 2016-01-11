package tool;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.List;

public class MergerByteList {
	/**
	 * 合并byte数组
	 * 
	 * @param srcArrays
	 * @return
	 * 
	 */
	public static byte[] streamCopy(List<byte[]> srcArrays) {
		byte[] destAray = null;
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		try {
			for (byte[] srcArray : srcArrays) {
				bos.write(srcArray);
			}
			bos.flush();
			destAray = bos.toByteArray();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				bos.close();
			} catch (IOException e) {
			}
		}
		return destAray;
	}
}
