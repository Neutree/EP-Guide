package dao;

import java.sql.ResultSet;

import constant.ConstantCode;
import tool.DataTransform;

public class QueryLogin {
	public static byte[] pair(byte[] mac, byte[] pwd) {
		String gtwMac = DataTransform.bytesToString(mac);
		String password = DataTransform.bytesToString(pwd);
		DBHelper db = new DBHelper();
		ResultSet rs = null;
		String sqlexcu = null;
		byte[] result = ConstantCode.Failed;
		try {
			sqlexcu = "Select * From gateway where gtwmac=? And password=?";
			db.preState = db.con.prepareStatement(sqlexcu);
			db.preState.setString(1, gtwMac);
			db.preState.setString(2, password);
			rs = db.preState.executeQuery();
			if (rs.next()) {
				// 说明rs查询到了与之匹对数据，表示登录成功
				System.out.println("网关登录成功：");
				System.out.println("mac: " + gtwMac);
				System.out.println("pwd: " + password);
				result=ConstantCode.Succeed;
			}
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 网关登录时数据库异常： ");
			e.printStackTrace();
			return result;
		}

		try {
			rs.close();
			db.sql.close();
			db.con.close();
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 网关登录时关闭数据库异常： ");
			e.printStackTrace();
		}

		return result;
	}
}
