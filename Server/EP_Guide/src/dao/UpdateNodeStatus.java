package dao;

import constant.ConstantCode;
import tool.DataTransform;

public class UpdateNodeStatus {
	public static byte[] update(byte[] msgbody) {
		// -- 解析消息体--//
		int readPos = 0;// 字符数组读指针初始化
		byte[] nodeID = new byte[6];
		for (int k = 0; k < 6; k++) {
			nodeID[k] = msgbody[readPos++];
		}
		byte[] nodeStatus = new byte[] { msgbody[readPos++], msgbody[readPos++] };
		byte[] userID = new byte[4];
		for (int k = 0; k < 4; k++) {
			userID[k] = msgbody[readPos++];
		}

		// -- 初始化数据--//
		DBHelper db = new DBHelper();
		String sqlexcu = null;
		byte[] result = ConstantCode.Failed;

		// -- 数据库操作--//
		try {
			long start_time = System.currentTimeMillis();
			sqlexcu = "UPDATE parking_spaces SET user_ID=?,status=?,start_time=? WHERE garage_ID=? And pSpace_ID=?";
			db.preState = db.con.prepareStatement(sqlexcu);
			db.preState.setString(1, DataTransform.bytesToString(userID));
			db.preState.setInt(2, DataTransform.byteGroupToInt(nodeStatus));
			db.preState.setLong(3, start_time);
			db.preState.setInt(4, 1);// 默认1号车库
			db.preState.setString(5, DataTransform.bytesToString(nodeID));
			int orptRow = db.preState.executeUpdate();
			if (orptRow > 0) {
				result = ConstantCode.Succeed;
				System.out.println("Info , 更改车位状态成功 ，车位ID：" +  DataTransform.bytesToString(nodeID));
			} else {
				System.out.println("Info , 更改车位状态车位失败 ，车位ID：" +  DataTransform.bytesToString(nodeID));
			}

		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 更改车位状态时数据库异常： ");
			e.printStackTrace();
			return result;
		}

		try {
			db.sql.close();
			db.con.close();
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB :更改车位状态时关闭数据库异常： ");
			e.printStackTrace();
		}
		return result;
	}

}
