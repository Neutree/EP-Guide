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
import tools.Encrypt;

public class QueryParkHistory extends HttpServlet{
	public void doGet(HttpServletRequest request,HttpServletResponse response) throws ServletException,IOException{
		doPost(request, response);
	}

	public void doPost(HttpServletRequest request,HttpServletResponse response) throws ServletException,IOException{
		//-- 变量初始化 --//
		String username=null,password=null,receiveData;//用户名，密码，客户端接收的json数据
		int action=0;//所需操作的命令字
		int result=0;//操作结果
		String car_ID=null;
		String token=null;//用户token
		JSONObject jsonData = new JSONObject();
		JSONObject backnews=new JSONObject();
		
		//-- 处理接受到的json数据，并验证 --//
		request.setCharacterEncoding("UTF-8");
		receiveData=request.getParameter("data");//接受客户端的数据，即用户名和密码
			// 1. 验空判断
		if (receiveData==null) {
			result=ConstantCode.Res_Fail_Resgiter;//失败则状态码返回2001
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
		if (action!=ConstantCode.Req_HistoryParkQuery) {
			try {
					backnews.put("result", "action is not correct:"+jsonData.toString());
					
				} catch (Exception e) {
				e.printStackTrace();
			}
			response.getWriter().write(backnews.toString());
			return;
		}
		try {
			username=jsonData.getString("username");
			password=jsonData.getString("password");
			car_ID=jsonData.getString("car_ID");
			System.out.println("car_ID1:"+car_ID);
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		password=Encrypt.encrypt(password+"&A-402");//sha1加密
		System.out.println("Info , 密码 SHA1加密后： "+password);
			// 2.2  判断是否已被注册，没有则注册
		token = DBOpreate.register(username, password,car_ID);
		if (token!=null && !token.isEmpty()) {
			if (token.equals("7002") ) {
				result=ConstantCode.Res_Resgitered;
			}
			else if (token.equals("2002") ) {
				result=ConstantCode.Res_Fail_Resgiter;
			}
			else{
				result=ConstantCode.Res_Resgiter;
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
