package test;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class ServerMuliThread {
	/*
	 * 基于TCP协议的Socket通信，实现用户登录 服务器端
	 */
	public static void main(String[] args) {
		try {
			// 1.创建一个服务器端Socket,即ServerSocket,指定绑定的端口，并监听此端口
			ServerSocket serverSocket = new ServerSocket(5088);
			Socket socket = null;
			// 记录客户端的数量
			int count = 0;
			// 2.调用accept()方法开始监听，等待客户端的连接
			System.out.println("服务器即将启动，等待客户端的连接");
			// 循环监听等待客户端的连接
			while (true) {
				// 调用accept()方法开始监听，等待客户端的连接
				socket = serverSocket.accept();
				// 创建一个新的线程
				ServerThead serverThead = new ServerThead(socket);
				// 启动线程
				serverThead.start();
				count++;
				System.out.println("客户端数量：" + count);
				InetAddress address = socket.getInetAddress();
				System.out.println("IP地址：" + address.getHostAddress());
			}

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
