package database;

import java.sql.ResultSet;

import tools.Encrypt;

public class DBOpreate {
	/**
	 * 用户登录：验证客户端发的账密是否与本地服务器一致,如果正确则更新数据库中的token，并返回token值，否则返空
	 * @param username
	 * @param password
	 * @return null || token 
	 */
	public static String queryLogin(String username ,String password) {
		 String db_uname=null;
		 String db_pwd=null;
		 String db_token=null;
		 String sqlexcu="Select * From user where username=? And password=? ";
		 DBHelper db=new DBHelper(sqlexcu);
		 ResultSet rs = null;
		 try {
			 db.preState.setString(1, username);
			 db.preState.setString(2, password);
		     rs = db.preState.executeQuery(); 			
			 while (rs.next()) {
				db_uname=rs.getString(1);
				db_pwd=rs.getString(2);
				db_token=rs.getString(3);
				System.out.println("db_uname:"+db_uname);
				System.out.println("db_pwd:"+db_pwd);
				System.out.println("db_token:"+db_token);
			 }
			 if (db_token!=null && !db_token.isEmpty()) {
				//-- 每一次通过账号密码登录都更新一次token --//
				 db_token=Encrypt.md5(username+System.currentTimeMillis());//token为用户名与登录时间Md5加密后的值
			 }
			 else{
				 return null;//登录失败，反空：数据库里无与之匹对账号或密码
			 }		
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 账号密码登录查询数据库异常： ");
			e.printStackTrace();
		}
		try {
			sqlexcu="UPDATE user SET token=? WHERE username=? And password=?";
			db.preState = db.con.prepareStatement(sqlexcu);
			db.preState.setString(1, db_token);
			db.preState.setString(2, db_uname);
			db.preState.setString(3, db_pwd);
			db.preState.execute(); 
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
			System.out.println("Abnormal , DB : 账号密码登录时关闭数据库异常： ");
			e.printStackTrace();
		}
		
		return db_token;
	}
}
