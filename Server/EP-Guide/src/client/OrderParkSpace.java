package client;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONException;
import org.json.JSONObject;

import constant.ConstantCode;
import dao.DBOpreate;

public class OrderParkSpace extends HttpServlet {

	public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doPost(request, response);
	}

	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// -- 变量初始化 --//
		String garage_ID = null, pSpace_ID = null, receiveData;
		int action = 0;// 所需操作的命令字
		int result = 0;// 操作结果
		String car_ID = null;
		String token = null;// 用户token
		JSONObject jsonData = new JSONObject();
		JSONObject backnews = new JSONObject();

		// -- 处理接受到的json数据，并验证 --//
		request.setCharacterEncoding("UTF-8");
		receiveData = request.getParameter("data");// 接受客户端的数据，即用户名和密码
		// 1. 验空判断
		if (receiveData == null) {
			result = ConstantCode.Res_Fail_OrderParkSpace;
			backnews = resResult(result, backnews);
			response.getWriter().write(backnews.toString());
			return;
		}
		// 2. json数据解析
		try {
			jsonData = new JSONObject(receiveData);
			System.out.println("Info , RecData : " + jsonData.toString());
			action = jsonData.getInt("action");
			System.out.println("action=" + action);
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		// 2.1 判断是否请求命令字是否相符
		if (action != ConstantCode.Req_OrderParkSpace) {
			result = ConstantCode.Res_Fail_OrderParkSpace;// 失败则状态码返回2005
			System.out.println("命令字有误:" + action);
			backnews = resResult(result, backnews);
			response.getWriter().write(backnews.toString());
			return;
		}
		try {
			garage_ID = jsonData.getString("garage_ID");
			pSpace_ID = jsonData.getString("pSpace_ID");
			car_ID = jsonData.getString("car_ID");
			token = jsonData.getString("token");
			System.out.println("car_ID1:" + car_ID + "请求停车");
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

		// 2.2 判断接受到的数据是否为空，不为空才操作数据库
		if (!garage_ID.isEmpty() && !pSpace_ID.isEmpty() && !car_ID.isEmpty() && !token.isEmpty()) {
			int isLegal = DBOpreate.checkToken(token);
			if (isLegal == ConstantCode.Res_Illegal_User) {
				System.out.println("非法用户！ 拒绝访问！");
				result = ConstantCode.Res_Illegal_User;
				backnews = resResult(result, backnews);
				response.getWriter().write(backnews.toString());
				return;
			} else {// 操作数据库
				int backRes = DBOpreate.queryPSpaceStatus(garage_ID, pSpace_ID);
				if (backRes == ConstantCode.Res_parkSpace_Empty) {
					boolean isSucs = DBOpreate.orderParkSpace(Integer.valueOf(garage_ID), pSpace_ID, car_ID);// 预定车位
					if (isSucs) {
						result = ConstantCode.Res_OrderParkSpace;
					} else {
						result = ConstantCode.Res_Fail_OrderParkSpace;
					}
				}
				else if(backRes == ConstantCode.Res_parkSpace_FUll){
					result = ConstantCode.Res_parkSpace_FUll;//如果车位已满则返回8800
				}else if(backRes == ConstantCode.Res_NotPSpaceID){
					result = ConstantCode.Res_NotPSpaceID;
				}
			}
		} else {
			System.out.println("接受到的数据有空，操作失败");
			result = ConstantCode.Res_Fail_OrderParkSpace;
			backnews = resResult(result, backnews);
			response.getWriter().write(backnews.toString());
			return;
		}
		// -- 把需返回的参数转换成json格式并返回给请求的客户端 --//
		backnews = resResult(result, backnews);
		response.setCharacterEncoding("UTF-8");
		response.getWriter().write(backnews.toString());
	}

	private JSONObject resResult(int result, JSONObject backnews) {
		try {
			backnews.put("result", result);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return backnews;
	}
}
