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
				
		/*
		 JSONArray backmsg=JSONArray.fromObject(pSpaces);
		net.sf.json.JSONObject recvJson=new net.sf.json.JSONObject();
		recvJson.put("result", result);
		recvJson.put("dataList", pSpaces);
		System.out.println("json1"+recvJson.toString());
		
		//--将获取到的String类型的数据转换为JSONObject --//
		net.sf.json.JSONObject jb=net.sf.json.JSONObject.fromObject(recvJson.toString());
        JSONArray jsonList = jb.getJSONArray("dataList");     
        List<ParkSpace> pSpaceList = new ArrayList<ParkSpace>();

        // 循环添加pSpace对象（可能有多个）
        for (int i = 0; i < jsonList.size(); i++) {
        	ParkSpace pSpace = new ParkSpace();
        	pSpace.setID(jsonList.getJSONObject(i).getString("ID"));
        	pSpace.setGarage_ID(jsonList.getJSONObject(i).getString("garage_ID"));
        	pSpaceList.add(pSpace);
        }

       //--"将Json数据转换为pSpace对象："--//
        for (int i = 0; i < pSpaceList.size(); i++) {
        	ParkSpace space = pSpaceList.get(i);
        	System.out.println("id: "+space.getID());
        	System.out.println("gra ID:" +space.getGarage_ID());
        }
		 
		 */
				
		
	}
}
