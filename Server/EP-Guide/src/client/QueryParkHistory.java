package client;

import java.io.IOException;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONException;
import org.json.JSONObject;

import bean.ParkHistory;
import config.ConstantCode;
import database.DBOpreate;
import net.sf.json.JSONArray;

public class QueryParkHistory extends HttpServlet {
	public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doPost(request, response);
	}

	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// -- 变量初始化 --//
		String car_ID = null, token = null, receiveData;
		int action = 0;// 所需操作的命令字
		int result = 0;// 操作结果
		JSONObject jsonData = new JSONObject();
		net.sf.json.JSONObject backnews=new net.sf.json.JSONObject();

		// -- 处理接受到的json数据，并验证 --//
		request.setCharacterEncoding("UTF-8");
		receiveData = request.getParameter("data");
		// 1. 验空判断
		if (receiveData == null) {
			result = ConstantCode.Res_Fail_HistoryParkQuery;
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
		if (action != ConstantCode.Req_HistoryParkQuery) {
			try {
				System.out.println("Info , 查询历史停车记录 action 有误");
				result = ConstantCode.Res_Fail_HistoryParkQuery;
				backnews.put("result", result);
			} catch (Exception e) {
				e.printStackTrace();
			}
			response.getWriter().write(backnews.toString());
			return;
		}
		// 2.2 获取所需数据
		try {
			car_ID = jsonData.getString("car_ID");
			token = jsonData.getString("token");
			System.out.println("car_ID:" + car_ID);
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

		// 2.3 验证token的合法性
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
		// 2.4 查询对应car_ID的停车记录
		List<ParkHistory> hList = DBOpreate.queryHistory(car_ID);
		System.out.println("list: " + hList.toString());
		if (hList.size() > 0) {
			result = ConstantCode.Res_HistoryParkQuery;
		}
		// -- 把需返回的参数转换成json格式 --//
		JSONArray dataList = JSONArray.fromObject(hList);
		backnews.put("result", result);
		backnews.put("dataList", dataList);
		System.out.println("backmsg : " + backnews.toString());
		
		//-- 返回给请求客户端 --//
		response.setCharacterEncoding("UTF-8");
		response.getWriter().write(backnews.toString());
	}
}
