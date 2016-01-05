package test;

import org.json.JSONException;
import org.json.JSONObject;



public class testJson {
	public static void main(String[] args) {
		String RecData=null;
		try {
			JSONObject Recv=new JSONObject(RecData);
			JSONObject result=Recv.getJSONObject("result");
			JSONObject citynow=result.getJSONObject("citynow");
			String city=result.getString("city");
			System.out.println("city:"+city);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
