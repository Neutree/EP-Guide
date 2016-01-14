package request;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import net.sf.json.JSONObject;

public class SendParkStatus {

	/**
	 * 使用post方式登陆
	 * 
	 * @param username
	 * @param password
	 * @return
	 */
	public static void Send(String cmd,String carId) {
		HttpURLConnection conn = null;
		try {
			// 创建一个URL对象
			URL mURL = new URL("http://localhost:8080/EP-Guide/parkMove");
			// 调用URL的openConnection()方法,获取HttpURLConnection对象
			conn = (HttpURLConnection) mURL.openConnection();

			conn.setRequestMethod("POST");// 设置请求方法为post
			conn.setReadTimeout(5000);// 设置读取超时为5秒
			conn.setConnectTimeout(10000);// 设置连接网络超时为10秒
			conn.setDoOutput(true);// 设置此方法,允许向服务器输出内容

			JSONObject jsonData = new JSONObject();
			// post请求的参数
			String action = cmd;
			String car_ID = carId;
			jsonData.put("action", action);
			jsonData.put("car_ID", car_ID);
			String sendmsg = "data=" + jsonData.toString();
			System.out.println("send: " + sendmsg);
			// 获得一个输出流,向服务器写数据,默认情况下,系统不允许向服务器输出内容
			OutputStream out = conn.getOutputStream();// 获得一个输出流,向服务器写数据
			out.write(sendmsg.getBytes());
			out.flush();
			out.close();

			int responseCode = conn.getResponseCode();// 调用此方法就不必再使用conn.connect()方法
			System.out.println("code :" + responseCode);
			if (responseCode == 200) {
				InputStream is = conn.getInputStream();
				String state = getStringFromInputStream(is);
				System.out.println("请求刷新停车动画成功");
				return;
			} else {
				System.out.println("请求刷新停车动画失败");
			}

		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (conn != null) {
				conn.disconnect();// 关闭连接
			}
		}

		return;
	}

	/**
	 * 根据流返回一个字符串信息 *
	 * 
	 * @param is
	 * @return
	 * @throws IOException
	 */
	private static String getStringFromInputStream(InputStream is) throws IOException {
		ByteArrayOutputStream os = new ByteArrayOutputStream();
		// 模板代码 必须熟练
		byte[] buffer = new byte[1024];
		int len = -1;
		// 一定要写len=is.read(buffer)
		// 如果while((is.read(buffer))!=-1)则无法将数据写入buffer中
		while ((len = is.read(buffer)) != -1) {
			os.write(buffer, 0, len);
		}
		is.close();
		String state = os.toString();// 把流中的数据转换成字符串,采用的编码是utf-8(模拟器默认编码)
		os.close();
		return state;
	}
}
