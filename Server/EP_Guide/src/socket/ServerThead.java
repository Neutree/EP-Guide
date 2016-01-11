package socket;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

import deal.DealMsgCmd;
import tool.OutputByteGrp;

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
	protected byte[] backMsg = null;
	protected byte[] recv = null;
	DealMsgCmd dealMsgCmd = new DealMsgCmd();// 处理数据

	public ServerThead(Socket socket) {
		this.socket = socket;
	}

	// 线程执行的操作，响应客户端的请求
	public void run() {
		readByte();// 读取数据
		backMsg = dealMsgCmd.doWork(recv);
		System.out.println("backMsg:");
		OutputByteGrp.output(backMsg);
		write(backMsg);// 写入需响应的数据
		
		try {
			sleep(5000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		close();// 关闭资源
		System.out.println("socket已关闭");
		
	}

	protected void readByte() {
		try {
			// 获取输入流，并读取客户端信息
			is = socket.getInputStream();
			isr = new InputStreamReader(is);
			recv = readInputStream(is);
			OutputByteGrp.output(recv);
			//socket.shutdownInput();// 关闭输入流
		} catch (Exception e) {
			System.out.println("Info , Socket : 读取socket信息异常 ：");
			e.printStackTrace();
			
		}

	}

	public static byte[] readInputStream(InputStream ins) {
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		try {
			int count =0;		
			while((count = ins.available()) == 0) ;
			System.out.println("recv len :"+count);
			byte[] buffer = new byte[count];
			int readCount = 0; // 已经成功读取的字节的个数
			while (readCount < count) {
				readCount += ins.read(buffer, readCount, count - readCount);
			}
			bos.write(buffer, 0, count);
			OutputByteGrp.output(buffer);
			
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		} 
		return bos.toByteArray();
	}

	public void deal() {

	}

	protected void write(byte[] msg) {
		try {
			// 获取输出流,响应客户端请求
			os = socket.getOutputStream();
			
			os.write(msg);
			os.flush();// 调用flush()方法将缓冲输出
			System.out.println("backnews:");
			OutputByteGrp.output(backMsg);
			
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
}
