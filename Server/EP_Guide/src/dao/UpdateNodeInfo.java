package dao;

import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.List;

import tool.DataTransform;
import tool.MergerByteList;

public class UpdateNodeInfo {
	public static byte[] update(byte[] msgbody) {
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

		// -- 初始化数据--//
		DBHelper db = new DBHelper();
		String sqlexcu = null;
		ResultSet rs = null;
		byte[] result = null;
		List<byte[]> orderedNode = new ArrayList<byte[]>();
		orderedNode.add(new byte[] { 0x00, 0x00 });// 默认0个
		int count = 0;
		// -- 数据库操作--//
		for (int i = 0; i < nodeList.size(); i++) {
			try {
				sqlexcu = "Select * From parking_spaces where garage_ID=? And pSpace_ID=? ";
				db.preState = db.con.prepareStatement(sqlexcu);
				db.preState.setInt(1, 1);
				db.preState.setString(2, nodeList.get(i));
				rs = db.preState.executeQuery();
				while (rs.next()) {
					int isOrder = rs.getInt(8);
					System.out.println("Info , 查询车库车位状态信息成功 ，  isOrder= " + isOrder);
					if (isOrder == 1) {
						orderedNode.add(DataTransform.StringToBytes(nodeList.get(i)));
						count++;
					}
				}
				if (orderedNode.size()>1) {
					orderedNode.set(0, DataTransform.intToByteGroup(count, 2));
					result = MergerByteList.streamCopy(orderedNode);
				}
			} catch (Exception e) {
				// TODO: handle exception
				System.out.println("Abnormal , DB : 查询车库车位状态信息时数据库异常： ");
				e.printStackTrace();
				return result;
			}
		}
		try {
			rs.close();
			db.sql.close();
			db.con.close();
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("Abnormal , DB :查询车库车位状态信息时关闭数据库异常： ");
			e.printStackTrace();
		}
		return result;
	}

}
