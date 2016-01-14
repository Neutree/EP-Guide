package com.ep_guide.thread;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.ep_guide.bean.ParkHistory;
import com.ep_guide.bean.ParkSpace;

public class QueryParkHistory extends Thread {
	String url;
	String token = Data.token;
	String result,car_ID;
	final String urlPath="http://192.168.191.1:8080/EP-Guide/queryParkHistory";
	
	public QueryParkHistory(String car_ID)
	{
		this.car_ID = car_ID;
	}
	
	private void doPost() throws JSONException
	{
		try {
			JSONObject ClientKey = new JSONObject();
			ClientKey.put("action", "1006");
			ClientKey.put("token", token);
			ClientKey.put("car_ID", car_ID);
			
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
				System.out.println("hIs:"+result);
				JSONObject json = new JSONObject(result);
				Data_History.data_history = json.getInt("result");
				JSONArray jsonList = json.getJSONArray("dataList");
				List<ParkHistory> parkList = new ArrayList<ParkHistory>();
				for (int i = 0; i < 2; i++) {
		        	ParkHistory parkhistory = new ParkHistory();
		        	parkhistory.setGarage_ID(jsonList.getJSONObject(i).getString("garage_ID"));
		        	parkhistory.setpSpace_ID(jsonList.getJSONObject(i).getString("pSpace_ID"));
		        	parkhistory.setGetIn_time(jsonList.getJSONObject(i).getString("getIn_time"));
		        	parkhistory.setPark_time(jsonList.getJSONObject(i).getString("park_time"));
		        	parkList.add(parkhistory);
		        }
				
				Data_History.garage_ID = new String[3];
				Data_History.pSpace_ID = new String[3];
				Data_History.getIn_time = new String[3];
				Data_History.park_time = new String[3];
				for (int i = 0; i < parkList.size(); i++) {
					ParkHistory history = parkList.get(i);
					Data_History.garage_ID[i] = history.getGarage_ID();
					Data_History.pSpace_ID[i] = history.getpSpace_ID();
					Data_History.getIn_time[i] = history.getGetIn_time();
					Data_History.park_time[i] = history.getPark_time();
		        }
				
				
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
