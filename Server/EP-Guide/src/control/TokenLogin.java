package control;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONObject;

import config.ConstantCode;
import database.DBOpreate;

public class TokenLogin extends HttpServlet {
	public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doPost(request, response);
	}

	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// -- 变量初始化 --//
		String username = null, receiveData;// 用户名客户端接收的json数据
		int action;// 所需操作的命令字
		int result = 0;// 操作结果
		String token = null;// 用户token
		JSONObject jsonData = new JSONObject();
		JSONObject backnews = new JSONObject();

		// -- 处理接受到的json数据，并验证 --//
		request.setCharacterEncoding("UTF-8");
		receiveData = request.getParameter("data");// 接受客户端的数据，即用户名和密码
		// 1. 验空判断
		if (receiveData == null) {
			result = ConstantCode.Res_Fail_TokenLogin;// 失败则状态码返回2003
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
			// 2.1 判断是否请求命令字是否相符
			if (action != ConstantCode.Req_TokenLogin) {
				try {
					result = ConstantCode.Res_Fail_TokenLogin;
					backnews.put("result", result);
				} catch (Exception e) {
					e.printStackTrace();
				}
				response.getWriter().write(backnews.toString());
				return;
			}

			// 2.2 判断账号和token是否正确
			username = jsonData.getString("username");
			token = jsonData.getString("token");
			String backResult = DBOpreate.tokenLogin(username, token);
			if (backResult != null && !backResult.isEmpty()) {
				System.out.println("backRes:" + backResult);
				if (token.equals("2003")) {
					result = ConstantCode.Res_Fail_TokenLogin;
				} else if (backResult.equals("6002")) {
					result = ConstantCode.Res_NotResgiter;
				} else if (backResult.equals("7003")) {
					result = ConstantCode.Res_Abnormal_TokenLogin;
				} else if (backResult.equals("8003")) {
					result = ConstantCode.Res_TokenLogin;
				} else {
					result = ConstantCode.Res_Fail_TokenLogin;
				}
			}
		} catch (Exception e1) {
			System.out.println("Abnormal , tokenLogin异常 ：");
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
