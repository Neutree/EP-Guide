package socket;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class ServerHost {
	private static int GtwOnlineNum = 0;// 记录网关端的数量

	/*
	 * 基于TCP协议的Socket通信，实现用户登录 服务器端
	 */
	public static void main(String[] args) {
		try {
			// 1.创建一个服务器端Socket,即ServerSocket,指定绑定的端口，并监听此端口
			ServerSocket serverSocket = new ServerSocket(5080);
			Socket socket = null;

			// 2.调用accept()方法开始监听，等待客户端的连接
			System.out.println("Server Started，waiting for connect ：");
			while (true) {// 循环监听等待客户端的连接
				socket = serverSocket.accept();// 调用accept()方法开始监听，等待客户端的连接
				ServerThead serverThead = new ServerThead(socket);// 创建一个新的线程
				serverThead.start();// 启动线程
				showInfo(socket);

			}

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public static void showInfo(Socket socket) {
		GtwOnlineNum++;
		System.out.println("已连接网关数量：" + GtwOnlineNum);
		InetAddress address = socket.getInetAddress();
		System.out.println("新增网关IP地址：" + address.getHostAddress());
	}
}
