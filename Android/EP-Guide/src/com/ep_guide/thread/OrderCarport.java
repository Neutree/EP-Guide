package com.ep_guide.thread;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import org.json.JSONException;
import org.json.JSONObject;

public class OrderCarport extends Thread {
	String url;
	String token = Data.token;
	String result,car_ID,pSpace_ID,garage_ID;
	final String urlPath="http://192.168.191.1:8080/EP-Guide/orderParkSpace";
	
	public OrderCarport(String garage_ID,String car_ID, String pSpace_ID){
		this.car_ID = car_ID;
		this.pSpace_ID = pSpace_ID;	
		this.garage_ID = garage_ID;
	}
	
	private void doPost() throws JSONException
	{
		try {
			JSONObject ClientKey = new JSONObject();
			ClientKey.put("action", "1005");
			ClientKey.put("token", token);
			ClientKey.put("garage_ID", garage_ID);
			ClientKey.put("car_ID", car_ID);
			ClientKey.put("pSpace_ID", pSpace_ID);
			
			//把JSON数据转换成String类型使用输出流向服务器写
			String content="data="+ClientKey.toString();
			System.out.println(content);
			URL HttpUrl = new URL(urlPath);
			try {
				HttpURLConnection conn = (HttpURLConnection) HttpUrl.openConnection();
				conn.setRequestMethod("POST");
				conn.setConnectTimeout(5000);
				OutputStream os = conn.getOutputStream();
				os.write(content.getBytes());

				os.close();
				//返回数据
				BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
				StringBuffer sb = new StringBuffer();
				String strData;
				
				while((strData=reader.readLine())!=null)
				{
					sb.append(strData);
				}
				result = sb.toString();
				System.out.println("StrORD:"+result);
				JSONObject json = new JSONObject(result);
				Data_Carport.data_carport=json.getInt("result");
				
				reader.close();
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	public void run() {

			try {
				doPost();
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		
	
	}

}
