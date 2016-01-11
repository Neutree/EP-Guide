package dao;

import java.util.ArrayList;
import java.util.List;

import bean.Node;
import constant.ConstantCode;
import tool.DataTransform;

public class UptNodeInfo {
	/**
	 * 已抛弃，无用！
	 * @param msgbody
	 * @return
	 */
	public static byte[] update(byte[] msgbody) {
		// -- 解析消息体--//
		int readPos = 0;// 字符数组读指针初始化
		byte[] nodeNum = new byte[] { msgbody[readPos++], msgbody[readPos++] };
		byte[] emptyNodeNum = new byte[] { msgbody[readPos++], msgbody[readPos++] };
		List<Node> nodeList = new ArrayList<Node>();
		for (int i = 0; i < DataTransform.byteGroupToInt(emptyNodeNum); i++) {
			byte[] nodeID = new byte[6];
			for (int k = 0; k < 6; k++) {
				nodeID[k] = msgbody[readPos++];
			}
			byte[] nodeStatus = new byte[] { msgbody[readPos++], msgbody[readPos++] };
			byte[] userID = new byte[4];
			for (int k = 0; k < 4; k++) {
				userID[k] = msgbody[readPos++];
			}
			Node node = new Node(DataTransform.bytesToString(nodeID), DataTransform.byteGroupToInt(nodeStatus),
					DataTransform.bytesToString(userID));
			nodeList.add(node);
		}

		// -- 初始化数据--//
		DBHelper db = new DBHelper();
		String sqlexcu = null;
		byte[] result = ConstantCode.Failed;

		// -- 数据库操作--//
		for (int i = 0; i < nodeList.size(); i++) {
			try {
				sqlexcu = "UPDATE parking_spaces SET user_ID=?,status=? WHERE garage_ID=? And pSpace_ID=?";
				db.preState = db.con.prepareStatement(sqlexcu);
				Node node = nodeList.get(i);
				db.preState.setString(1, node.getUser_ID());
				db.preState.setInt(2, node.getStatus());
				db.preState.setInt(3, 1);// 默认1号车库
				db.preState.setString(4, node.getNode_ID());
				int orptRow = db.preState.executeUpdate();
				if (orptRow > 0) {
					result = ConstantCode.Succeed;
					System.out.println("Info , 节点信息同步成功");
				} else {
					System.out.println("Info ,  节点信息同步失败 ");
				}

			} catch (Exception e) {
				// TODO: handle exception
				System.out.println("Abnormal , DB : 更改车位状态时数据库异常： ");
				e.printStackTrace();
				return result;
			}

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
