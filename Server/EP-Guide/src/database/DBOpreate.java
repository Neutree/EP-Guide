package database;

import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.List;

import bean.ParkSpace;
import config.ConstantCode;
import tools.Encrypt;

public class DBOpreate {
	/**
	 * 用户登录：验证客户端发的账密是否与本地服务器一致,如果正确则更新数据库中的token，并返回token值，否则返空
	 * 
	 * @param username
	 * @param password
	 * @return null || token
	 */
	public static String queryLogin(String username, String password) {
		// -- 变量初始化 --//
		String db_uname = null;
		String db_pwd = null;
		String db_token = null;
		String sqlexcu = null;
		DBHelper db = new DBHelper();
		ResultSet rs = null;

		// -- 核对账号是否已注册 --//
		try {
			sqlexcu = "Select * From user where username=?";
			db.preState = db.con.prepareStatement(sqlexcu);
			db.preState.setString(1, username);
			rs = db.preState.executeQuery();
			if (rs.next()) {
				System.out.println("Info , Login: 此账号已注册");
			} else {
				return "6002";// 此用户未注册
			}

		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 账号查询数据库异常： ");
			e.printStackTrace();
		}

		// -- 核对账号密码--//
		try {
			sqlexcu = "Select * From user where username=? And password=? ";
			db.preState = db.con.prepareStatement(sqlexcu);
			db.preState.setString(1, username);
			db.preState.setString(2, password);
			rs = db.preState.executeQuery();
			if (rs.next()) {
				db_uname = rs.getString(1);
				db_pwd = rs.getString(2);
				db_token = rs.getString(3);
				System.out.println("db_uname:" + db_uname);
				System.out.println("db_pwd:" + db_pwd);
				System.out.println("db_token:" + db_token);
			} else {
				return "4002";// 密码错误
			}

		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 账号密码登录查询数据库异常： ");
			e.printStackTrace();
		}

		if (db_token != null && !db_token.isEmpty()) {
			// -- 每一次通过账号密码登录都更新一次token --//
			db_token = Encrypt.md5(username + System.currentTimeMillis());// token为用户名与登录时间Md5加密后的值
		} else {
			return null;// 登录失败，反空,token为空，有毒
		}

		try {
			sqlexcu = "UPDATE user SET token=? WHERE username=? And password=?";
			db.preState = db.con.prepareStatement(sqlexcu);
			db.preState.setString(1, db_token);
			db.preState.setString(2, db_uname);
			db.preState.setString(3, db_pwd);
			int orptRow=db.preState.executeUpdate();
			if (orptRow>0) {
				System.out.println("Info , 更改token成功 ，token："+db_token);
			}
			else {
				db_token="2001";
				System.out.println("Info , 更改token失败 ");
			}
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 账号密码登录更新数据库token异常！ ");
			e.printStackTrace();
		}
		try {
			rs.close();
			db.sql.close();
			db.con.close();
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 关闭数据库异常： ");
			e.printStackTrace();
		}

		return db_token;
	}

	public static String register(String username, String password, String car_ID) {
		String token = null;
		String sqlexcu = null;
		DBHelper db = new DBHelper();
		ResultSet rs = null;

		// System.out.println();
		try {
			sqlexcu = "Select * From user where username=?";
			db.preState = db.con.prepareStatement(sqlexcu);
			db.preState.setString(1, username);
			rs = db.preState.executeQuery();
			if (!rs.next()) {
				// 说明rs查询到的数据是空,没有注册过
				System.out.println("car_ID2:" + car_ID);
				sqlexcu = "Insert Into user (userName,password,token,car_ID) Values(?,?,?,?)";
				token = Encrypt.md5(username + System.currentTimeMillis());// token为用户名与登录时间Md5加密后的值
				db.preState = db.con.prepareStatement(sqlexcu);
				db.preState.setString(1, username);
				db.preState.setString(2, password);
				db.preState.setString(3, token);
				db.preState.setString(4, car_ID);
				db.preState.execute();
			} else {
				return "7002";// 说明已经注册
			}
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 注册时插入数据库异常： ");
			e.printStackTrace();
			return "2002";
		}

		try {
			rs.close();
			db.sql.close();
			db.con.close();
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 账号密码登录时关闭数据库异常： ");
			e.printStackTrace();
		}

		return token;
	}

	public static String tokenLogin(String username, String token) {
		String sqlexcu = null;
		DBHelper db = new DBHelper();
		ResultSet rs = null;
		try {
			sqlexcu = "Select * From user where username=?  And token=?";
			db.preState = db.con.prepareStatement(sqlexcu);
			db.preState.setString(1, username);
			db.preState.setString(2, token);
			rs = db.preState.executeQuery();
			if (rs.next()) {
				String carId = rs.getString(4);
				System.out.println("carID:" + carId);
				System.out.println("Info , token登录成功 ！ username: " + username);
			} else {
				return "6002";// 说明未注册
			}
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 二次登时时查询数据库异常： ");
			e.printStackTrace();
			// return "2003";
		}

		try {
			rs.close();
			db.sql.close();
			db.con.close();
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 账号密码登录时关闭数据库异常： ");
			e.printStackTrace();
		}

		return "8003";
	}

	public static List<ParkSpace> queryParkSpaces(String garageID) {
		// -- 变量初始化 --//
		String sqlexcu = null;
		DBHelper db = new DBHelper();
		ResultSet rs = null;
		List<ParkSpace> pSpaces = new ArrayList<ParkSpace>();

		// -- 通过获取车库Id获取所有车位信息 --//
		try {
			sqlexcu = "Select * From parking_spaces where garage_ID=?  ";
			db.preState = db.con.prepareStatement(sqlexcu);
			db.preState.setString(1, garageID);
			rs = db.preState.executeQuery();
			if (rs.next()) {
				String ID = rs.getString(1);
				String park_ID = rs.getString(2);
				String park_spaceName = rs.getString(3);
				String garage_ID = rs.getString(4);
				String status = rs.getString(5);
				long start_time = rs.getLong(6);
				ParkSpace pSpace = new ParkSpace(ID, park_ID, park_spaceName, garage_ID, status, start_time);
				pSpaces.add(pSpace);
				System.out.println("Info , 查询车库车位信息成功 ， pSpace = " + pSpace.toString());
			} else {
				return null;// 没有此车库ID
			}
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 查询车库所有车位信息时数据库异常： ");
			e.printStackTrace();
		}

		try {
			rs.close();
			db.sql.close();
			db.con.close();
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 账号密码登录时关闭数据库异常： ");
			e.printStackTrace();
		}
		return pSpaces;
	}

	public static boolean orderParkSpace(int garage_ID, String pSpace_ID, String car_ID) {
		// -- 变量初始化 --//
		String sqlexcu = null;
		boolean sucsFlag = false;
		DBHelper db = new DBHelper();

		// -- 通过获取车库Id获取所有车位信息 --//
		try {
			int isOrder=1,status=1;
			long start_time= System.currentTimeMillis();
			sqlexcu = "UPDATE parking_spaces SET car_ID=?,status=?,isOrder=?,start_time=? WHERE garage_ID=? And pSpace_ID=?";
			db.preState = db.con.prepareStatement(sqlexcu);
			db.preState.setString(1, car_ID);
			db.preState.setInt(2, status);
			db.preState.setInt(3, isOrder);
			db.preState.setLong(4, start_time);
			db.preState.setInt(5, garage_ID);
			db.preState.setString(6, pSpace_ID);
			int orptRow=db.preState.executeUpdate();
			if (orptRow>0) {
				sucsFlag=true;
				System.out.println("Info , 预约车位成功 ，车位ID："+pSpace_ID);
			}
			else {
				sucsFlag=false;
				System.out.println("Info , 预约车位失败 ，车位ID："+pSpace_ID);
			}
			
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 预约车位时更改车位表异常： ");
			e.printStackTrace();
			return false;
		}

		try {
			db.sql.close();
			db.con.close();
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 预约车位时关闭数据库异常： ");
			e.printStackTrace();
			return false;
		}
		return sucsFlag;
	}

	public static int checkToken(String token) {
		// -- 变量初始化 --//
		String sqlexcu = null;
		DBHelper db = new DBHelper();
		ResultSet rs = null;
		// -- 初次查询，判断token是否合法 --//
		try {
			sqlexcu = "Select * From user where token=?  ";
			db.preState = db.con.prepareStatement(sqlexcu);
			db.preState.setString(1, token);
			rs = db.preState.executeQuery();
			if (rs.next()) {
				System.out.println("Info , 合法用户请求");
				return ConstantCode.legal_User;// 合法用户
			} else {
				return ConstantCode.Res_Illegal_User;// 非法用户访问
			}
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 查询车库所有车位信息时数据库异常： ");
			e.printStackTrace();
		}
		return 0;// 表示操作失败
	}

}
