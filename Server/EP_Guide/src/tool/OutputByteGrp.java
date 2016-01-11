package tool;

public class OutputByteGrp {
	public static void output(byte[] msg) {
		for (int k = 0; k < msg.length; k++) {
			System.out.print(msg[k]+" ");
		}
		System.out.println();
	}

}
