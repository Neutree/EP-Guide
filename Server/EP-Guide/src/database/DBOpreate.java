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
		     if (rs.next()) {
		    	 db_uname=rs.getString(1);
				 db_pwd=rs.getString(2);
				 db_token=rs.getString(3);
				 System.out.println("db_uname:"+db_uname);
		     	 System.out.println("db_pwd:"+db_pwd);
			     System.out.println("db_token:"+db_token);
			}else {
				return "6002";//此用户未注册
			}
			 
			 if (db_token!=null && !db_token.isEmpty()) {
				//-- 每一次通过账号密码登录都更新一次token --//
				 db_token=Encrypt.md5(username+System.currentTimeMillis());//token为用户名与登录时间Md5加密后的值
			 }
			 else{
				 return null;//登录失败，反空,token为空，有毒
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
	
	public static String register(String username ,String password,String car_ID) {
		 String token=null;
		 String sqlexcu="Select * From user where username=?  ";
		 DBHelper db=new DBHelper(sqlexcu);
		 ResultSet rs = null;
		 
    	 //System.out.println();
		 try {
			 db.preState.setString(1, username);
		     rs = db.preState.executeQuery(); 
		     if(!rs.next()){
		         //说明rs查询到的数据是空,没有注册过
		    	 System.out.println("car_ID2:"+car_ID);
		    	 sqlexcu="Insert Into user (userName,password,token,car_ID) Values(?,?,?,?)";
		    	 token=Encrypt.md5(username+System.currentTimeMillis());//token为用户名与登录时间Md5加密后的值
		    	 db.preState = db.con.prepareStatement(sqlexcu);
			     db.preState.setString(1, username);
				 db.preState.setString(2, password);
				 db.preState.setString(3, token);
				 db.preState.setString(4, car_ID);
				 db.preState.execute(); 
		     }
		     else{
		    	 return "7002";//说明已经注册
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
	
	public static String tokenLogin(String username ,String token) {
		 String sqlexcu="Select * From user where username=?  And token=?";
		 DBHelper db=new DBHelper(sqlexcu);
		 ResultSet rs = null;
		 try {
			 db.preState.setString(1, username);
			 db.preState.setString(2, token);
		     rs = db.preState.executeQuery(); 
		     if(rs.next()){
		         String carId=rs.getString(4);
		    	 System.out.println("carID:"+carId);
		    	 System.out.println("Info , token登录成功 ！ username: "+username);
		     }
		     else{
		    	 return "6002";//说明未注册
		     }	
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 二次登时时查询数据库异常： ");
			e.printStackTrace();
			//return "2003";
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
	
	public static String queryParkSpaces(String token ,String garageID) {
		 String sqlexcu=null;
		 DBHelper db=new DBHelper(sqlexcu);
		 ResultSet rs = null;
		 //-- 初次查询，判断token是否合法 --//
		 try {
			 sqlexcu="Select * From user where token=?  ";
			 db.preState.setString(1, token);
		     rs = db.preState.executeQuery(); 
		     if(rs.next()){
		         String username=rs.getString(1);
		    	 System.out.println("Info , 查询车库所有车位信息成功 ， username: "+username);
		     }
		     else{
		    	 return "3001";//非法用户访问
		     }	
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 查询车库所有车位信息时数据库异常： ");
			e.printStackTrace();
		}
		//-- 二次查询，通过获取车库Id获取所有车位信息 --//
		 try {
			 sqlexcu="Select * From garage where garageID=?  ";
			 db.preState.setString(1, garageID);
		     rs = db.preState.executeQuery(); 
		     if(rs.next()){
		         String garage_name=rs.getString(2);
		    	 System.out.println("Info , 查询车库所有车位信息成功 ， garage_name = "+garage_name);
		     }
		     else{
		    	 return "6001";//没有此车库ID
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
		
		return bean;
	}	
	
}
