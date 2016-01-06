package control;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONException;
import org.json.JSONObject;

import config.ConstantCode;
import database.DBOpreate;

public class QueryParkSpaces extends HttpServlet{
	
	public void doGet(HttpServletRequest request,HttpServletResponse response) throws ServletException,IOException{
		doPost(request, response);
	}

	public void doPost(HttpServletRequest request,HttpServletResponse response) throws ServletException,IOException{
		//-- 变量初始化 --//
		String receiveData;
		int action=0;//所需操作的命令字
		int result=0;//操作结果
		String garageID=null;
		String token=null;//用户token
		JSONObject jsonData = new JSONObject();
		JSONObject backnews=new JSONObject();
		
		//-- 处理接受到的json数据，并验证 --//
		request.setCharacterEncoding("UTF-8");
		receiveData=request.getParameter("data");//接受客户端的数据，即用户名和密码
			// 1. 验空判断
		if (receiveData==null) {
			result=ConstantCode.Res_Fail_OrderPark;//失败则状态码返回2004
			try {
				backnews.put("result", result);
			} catch (Exception e) {
				e.printStackTrace();
			}
			response.getWriter().write(backnews.toString());
			return;
		}
			// 2. json数据解析
		try {
			jsonData = new JSONObject(receiveData);
			System.out.println("Info , RecData : "+jsonData.toString());
			action=jsonData.getInt("action");
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
			// 2.1  判断是否请求命令字是否相符
		if (action!=ConstantCode.Req_OrderPark) {
			try {
					result=ConstantCode.Res_Fail_OrderPark;
					backnews.put("result", result);
				} catch (Exception e) {
				e.printStackTrace();
			}
			response.getWriter().write(backnews.toString());
			return;
		}
		try {
			garageID=jsonData.getString("garageID");
			token=jsonData.getString("token");
			
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
			// 2.2  查询数据库信息
		token = DBOpreate.queryParkSpaces(garageID,token);
		if (token!=null && !token.isEmpty()) {
			if (token.equals("3001") ) {
				result=ConstantCode.Res_Illegal_User;
			}
			else if (token.equals("2004") ) {
				result=ConstantCode.Res_Fail_OrderPark;
			}
			else if (token.equals("6004") ) {
				result=ConstantCode.Res_NotGrageID;
			}
			else{
				result=ConstantCode.Res_OrderPark;
			}
		}
	    
		//-- 把需返回的参数转换成json格式并返回给请求的客户端 --//
		 backnews=new JSONObject();
			try {
				backnews.put("result", result);
				backnews.put("token", token);
			} catch (Exception e) {
				e.printStackTrace();
			}
			response.setCharacterEncoding("UTF-8");
			response.getWriter().write(backnews.toString());
	}

}
