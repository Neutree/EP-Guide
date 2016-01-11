package socket;

public interface DoWork {
public static final String APP_MAC = "app.macStr";
	
	/**
	 * 执行对SocketInfo的处理，返回要发送给客户端的数据
	 * */
	public byte[] doWork(byte[] recvMsg);
}
