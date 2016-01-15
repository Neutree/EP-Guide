package localreq;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONObject;

public class ParkMove extends HttpServlet {

	public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doPost(request, response);
	}

	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// -- 变量初始化 --//
		String car_ID = null, receiveData;
		int action;// 所需操作的命令字
		int result = 0;// 操作结果
		JSONObject jsonData = new JSONObject();
		JSONObject backnews = new JSONObject();
		System.out.println("getIn sendStatus");
		// -- 处理接受到的json数据，并验证 --//
		request.setCharacterEncoding("UTF-8");
		receiveData = request.getParameter("data");// 接受客户端的数据，即用户名和密码
		// 1. 验空判断
		if (receiveData == null) {
			result = 0;// 失败则状态码返回2001
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
			car_ID = jsonData.getString("car_ID");
			if (action == 0) {// 车辆出库

				result = 1;
			}
			if (action == 1) {// 车辆进库
				System.out.println("action" + action);
				result = 1;
			}

		} catch (Exception e1) {

			e1.printStackTrace();
		}

		// -- 把需返回的参数转换成json格式并返回给请求的客户端 --//
		backnews = new JSONObject();
		try {
			backnews.put("result", result);
		} catch (Exception e) {
			e.printStackTrace();
		}
		response.setCharacterEncoding("UTF-8");
		response.getWriter().write(backnews.toString());
	}

}
