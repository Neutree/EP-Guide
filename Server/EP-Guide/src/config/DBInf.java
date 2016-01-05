package config;

public class DBInf {
	private static final String loginName="dandan";
	private static final String password="cool";
	private static final String uri="jdbc:mysql://www.doubleeggs.com:3306/ep-guide";
//	private static final String loginName="root";
//	private static final String password="gg";
//	private static final String uri="jdbc:mysql://localhost:3306/ep-guide";
	public static String getLoginName(){
		return loginName;
	}
	public static String getPassword(){
		return password;
	}
	public static String getUri(){
		return uri;
	}
}
