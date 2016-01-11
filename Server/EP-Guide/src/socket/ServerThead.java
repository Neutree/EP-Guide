package socket;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

/*
 * 服务器端线程处理类
 */
public class ServerThead extends Thread {

	protected Socket socket = null;
	protected InputStream is = null;
	protected InputStreamReader isr = null;
	protected BufferedReader br = null;
	protected OutputStream os = null;
	protected PrintWriter pw = null;
	protected String recvData = null;
	protected String backMsg = "welcome";

	public ServerThead(Socket socket) {
		this.socket = socket;
	}

	// 线程执行的操作，响应客户端的请求
	public void run() {
		read();// 读取数据
		deal();// 处理数据
		write();// 写入需响应的数据
		close();// 关闭资源
	}

	protected void read() {
		try {
			// 获取输入流，并读取客户端信息
			is = socket.getInputStream();
			isr = new InputStreamReader(is);
			br = new BufferedReader(isr);

			String temp = null;
			boolean flag = true;
			while ((temp = br.readLine()) != null) {// 循环读取客户端的信息
				System.out.println("temp : " + temp);
				if (flag) {
					recvData = temp;
					flag = false;
				} else {
					recvData += temp;
				}
			}
			System.out.println("Info ， Socket receive :" + recvData);
			socket.shutdownInput();// 关闭输入流
		} catch (Exception e) {
			System.out.println("Info , Socket : 读取socket信息异常 ：");
			e.printStackTrace();
		}

	}

	public void deal() {

	}

	protected void write() {
		try {
			// 获取输出流,响应客户端请求
			os = socket.getOutputStream();
			pw = new PrintWriter(os);
			pw.write(backMsg);
			pw.flush();// 调用flush()方法将缓冲输出
			System.out.println("Info ， Socket send :" + backMsg);
		} catch (Exception e) {
			System.out.println("Abnormal , Socket : 写入数据异常 ：");
			e.printStackTrace();
		}

	}

	protected void close() {
		try {
			if (pw != null)
				pw.close();
			if (os != null)
				os.close();
			if (br != null)
				br.close();
			if (isr != null)
				isr.close();
			if (is != null)
				is.close();
			if (socket != null)
				socket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			System.out.println("Abnormal , Socket : 关闭socket资源异常 ：");
			e.printStackTrace();
		}
	}

}
