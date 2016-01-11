package client;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONException;
import org.json.JSONObject;

import bean.ParkSpace;
import constant.ConstantCode;
import dao.DBOpreate;
import net.sf.json.JSONArray;

public class QueryParkSpaces extends HttpServlet {

	public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doPost(request, response);
	}

	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// -- 变量初始化 --//
		String receiveData;
		int action = 0;// 所需操作的命令字
		int result = 0;// 操作结果
		String garage_ID = null;
		String token = null;// 用户token
		List<ParkSpace> pSpaces = new ArrayList<ParkSpace>();
		JSONObject jsonData = new JSONObject();
		net.sf.json.JSONObject backnews = new net.sf.json.JSONObject();

		// -- 处理接受到的json数据，并验证 --//
		request.setCharacterEncoding("UTF-8");
		receiveData = request.getParameter("data");// 接受客户端的数据，即用户名和密码
		// 1. 验空判断
		if (receiveData == null) {
			result = ConstantCode.Res_Fail_OrderPark;// 失败则状态码返回2004
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
			System.out.println("Info , RecData : " + jsonData.toString());
			action = jsonData.getInt("action");
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		// 2.1 判断是否请求命令字是否相符
		if (action != ConstantCode.Req_OrderPark) {
			try {
				result = ConstantCode.Res_Fail_OrderPark;
				backnews.put("result", result);
			} catch (Exception e) {
				e.printStackTrace();
			}
			response.getWriter().write(backnews.toString());
			return;
		}
		try {
			garage_ID = jsonData.getString("garage_ID");
			token = jsonData.getString("token");

		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		// 2.2 查询数据库信息
		// -- 验证token的合法性 --//
		int isLegal = DBOpreate.checkToken(token);
		if (isLegal == ConstantCode.Res_Illegal_User) {
			System.out.println("非法用户！ 拒绝访问！");
			try {
				result = ConstantCode.Res_Illegal_User;
				backnews.put("result", result);
			} catch (Exception e) {
				e.printStackTrace();
			}
			response.getWriter().write(backnews.toString());
			return;
		}
		// -- 查询所有的车位信息 --//
		pSpaces = DBOpreate.queryParkSpaces(garage_ID);// 从数据库中查询所有车位信息
		System.out.println("pSpaces Num:" + pSpaces.size());
		if (pSpaces.size() > 0) {
			result = ConstantCode.Res_OrderPark;
		}
		// -- 把需返回的参数转换成json格式 --//
		JSONArray dataList = JSONArray.fromObject(pSpaces);
		backnews.put("result", result);
		backnews.put("dataList", dataList);
		System.out.println("backmsg : " + backnews.toString());

		// -- 返回给请求的客户端 --//
		response.setCharacterEncoding("UTF-8");
		response.getWriter().write(backnews.toString());
	}

}
