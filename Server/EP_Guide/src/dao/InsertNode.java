package dao;

import java.util.ArrayList;
import java.util.List;

import constant.ConstantCode;
import tool.DataTransform;

public class InsertNode {
	public static byte[] insert(byte[] msgbody) {
		// -- 解析消息体--//
		int readPos = 0;// 字符数组读指针初始化
		byte[] nodeNum = new byte[] { msgbody[readPos++], msgbody[readPos++] };
		List<String> nodeList = new ArrayList<String>();
		for (int i = 0; i < DataTransform.byteGroupToInt(nodeNum); i++) {
			byte[] nodeID = new byte[6];
			for (int k = 0; k < 6; k++) {
				nodeID[k] = msgbody[readPos++];
			}
			nodeList.add(DataTransform.bytesToString(nodeID));
		}

		String sqlexcu = null;
		DBHelper db = new DBHelper();
		byte[] result = ConstantCode.Failed;

		for (int i = 0; i < nodeList.size(); i++) {
			try {
				sqlexcu = "Insert Into paring_spaces (pSpace_ID,garage_ID) Values(?,?)";
				db.preState = db.con.prepareStatement(sqlexcu);
				db.preState.setString(1, nodeList.get(i));
				db.preState.setString(2, "1");// 默认1号库
				int isSucs = db.preState.executeUpdate();
				if (isSucs > 0) {
					result = ConstantCode.Succeed;
				}
			} catch (Exception e) {
				// TODO: handle exception
				System.out.println("Abnormal , DB : 节点添加时插入数据库异常： ");
				e.printStackTrace();
				return result;
			}
		}
		try {

			db.sql.close();
			db.con.close();
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB : 节点添加时关闭数据库异常： ");
			e.printStackTrace();
		}

		return result;
	}
}
