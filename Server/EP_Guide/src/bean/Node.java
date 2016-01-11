package bean;

import java.util.ArrayList;
import java.util.List;

import tool.DataTransform;
import tool.MergerByteList;

public class Node {
	private String node_ID;
	private int status;
	private String user_ID;

	public Node(String node_ID, int status, String user_ID) {
		super();
		this.node_ID = node_ID;
		this.status = status;
		this.user_ID = user_ID;
	}

	public String getNode_ID() {
		return node_ID;
	}

	public void setNode_ID(String node_ID) {
		this.node_ID = node_ID;
	}

	public int getStatus() {
		return status;
	}

	public void setStatus(int status) {
		this.status = status;
	}

	public String getUser_ID() {
		return user_ID;
	}

	public void setUser_ID(String user_ID) {
		this.user_ID = user_ID;
	}

	@Override
	public String toString() {
		return "Node [node_ID=" + node_ID + ", status=" + status + ", user_ID=" + user_ID + "]";
	}

	public byte[] toBytes() {
		List<byte[]> bytes = new ArrayList<byte[]>();
		bytes.add(DataTransform.StringToBytes(node_ID));
		bytes.add(DataTransform.intToByteGroup(status, 2));

		byte[] backnews = MergerByteList.streamCopy(bytes);
		return backnews;
	}
}
