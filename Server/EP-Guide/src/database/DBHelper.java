package database;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;

import config.DBInf;

public class DBHelper {
	public Connection con;
	public Statement sql;
	public PreparedStatement preState;
	//public ResultSet rs;
	public DBHelper(){
		try{
			Class.forName("com.mysql.jdbc.Driver");
		}
		catch(ClassNotFoundException e){
			System.out.println("Abnormal , DB驱动异常 ： "+e);
		}
		//以下为数据库连接
		String uri=DBInf.getUri();
		String dblogName=DBInf.getLoginName();
		String dbpassword=DBInf.getPassword();
		try {
			con=DriverManager.getConnection(uri,dblogName,dbpassword);
			sql=con.createStatement();
				
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	

}
