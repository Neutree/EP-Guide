package test;

import org.json.JSONObject;

public class testJson {
	public static void main(String[] args) {
		/*
		String RecData=null;
		try {
			JSONObject Recv=new JSONObject(RecData);
			JSONObject result=Recv.getJSONObject("result");
			JSONObject citynow=result.getJSONObject("citynow");
			String city=citynow.getString("city");
			System.out.println("city:"+city);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		*/
//		String test="data={\"action\":\"1001\",\"username\":\"dan1dan\",\"password\":\"gg\"}";
//		System.out.println(test);
		
		//以下为把需返回的参数转换成json格式
				JSONObject transfer=new JSONObject();
				try {
						transfer.put("result", "8001");
						transfer.put("user_id", "1");
						transfer.put("token", "198889");
						transfer.put("user_type", "gay");
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				System.out.println("jsonData=!! "+transfer.toString());
				String content="data="+transfer.toString();
				System.out.println(content);
				
				
				//data={"action":"1002", "username":"dan","password":"gg","car_ID":"渝B666668"}
				
		
	}
}
