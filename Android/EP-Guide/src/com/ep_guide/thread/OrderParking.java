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

import android.widget.ListView;

import com.ep_guide.bean.ParkSpace;
import com.example.ep_guide.OrderParkActivity;

public class OrderParking extends Thread {
	String url;
	String token = Data.token;
	String result;
	ListView listView1;
	final String urlPath="http://192.168.191.1:8080/EP-Guide/queryParkSpaces";
	
	
	private void doPost() throws JSONException
	{
		try {
			System.out.println("token="+token);
			JSONObject ClientKey = new JSONObject();
			ClientKey.put("action", "1004");
			ClientKey.put("token", token);
			ClientKey.put("garage_ID", "1");
			
			//把JSON数据转换成String类型使用输出流向服务器写
			String content="data="+ClientKey.toString();
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
				System.out.println("Str:"+result);
				JSONObject json = new JSONObject(result);
				Data_Parking.data_parking = json.getInt("result");
				//System.out.println(json.getString("dataList"));
				JSONArray jsonList = json.getJSONArray("dataList");
				List<ParkSpace> pSpaceList = new ArrayList<ParkSpace>();
				 // 循环添加pSpace对象（可能有多个）
		        for (int i = 0; i < 2; i++) {
		        	ParkSpace pSpace = new ParkSpace();
		        	pSpace.setID(jsonList.getJSONObject(i).getString("ID"));
		        	pSpace.setPark_spaceName(jsonList.getJSONObject(i).getString("park_spaceName"));
		        	pSpace.setStatus(jsonList.getJSONObject(i).getString("status"));
		        	pSpace.setStart_time(jsonList.getJSONObject(i).getString("start_time"));
		        	pSpaceList.add(pSpace);
		        }
		        Data_Order.Park_ID = new String[3];
		        Data_Order.ParkspaceName = new String[3]; 
		        Data_Order.Start_Time = new String[3]; 
		        Data_Order.Status = new String[3]; 
		      //--"将Json数据转换为pSpace对象："--//
		        for (int i = 0; i < pSpaceList.size(); i++) {
		        	ParkSpace space = pSpaceList.get(i);
		        	Data_Order.Park_ID[i] = space.getID();
		        	Data_Order.ParkspaceName[i] = space.getPark_spaceName();
		        	Data_Order.Status[i] = space.getStatus();
		        	Data_Order.Start_Time[i] = space.getStart_time();
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
