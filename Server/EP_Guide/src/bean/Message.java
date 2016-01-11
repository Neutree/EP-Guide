package bean;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import tool.MergerByteList;

public class Message {
	private byte[] gMsgHead;// 消息头：固定={(byte) 0xA4 , 0x02}
	private byte[] gMsgID;// 消息ID：网关维护
	private byte[] gGtwMac;// 网关Mac地址
	private byte[] gMsgLen;// 消息体长度
	private byte[] bMsgCmd;// 命令字
	private byte[] bMsgBody;// 消息体
	private byte[] bMsgVrf;// 校验字

	public Message() {
		super();
	}

	public Message(byte[] gMsgHead, byte[] gMsgID, byte[] gGtwMac, byte[] gMsgLen, byte[] bMsgCmd, byte[] bMsgBody,
			byte[] bMsgVrf) {
		super();
		this.gMsgHead = gMsgHead;
		this.gMsgID = gMsgID;
		this.gGtwMac = gGtwMac;
		this.gMsgLen = gMsgLen;
		this.bMsgCmd = bMsgCmd;
		this.bMsgBody = bMsgBody;
		this.bMsgVrf = bMsgVrf;
	}

	public byte[] getgMsgHead() {
		return gMsgHead;
	}

	public void setgMsgHead(byte[] gMsgHead) {
		this.gMsgHead = gMsgHead;
	}

	public byte[] getgMsgID() {
		return gMsgID;
	}

	public void setgMsgID(byte[] gMsgID) {
		this.gMsgID = gMsgID;
	}

	public byte[] getgGtwMac() {
		return gGtwMac;
	}

	public void setgGtwMac(byte[] gGtwMac) {
		this.gGtwMac = gGtwMac;
	}

	public byte[] getgMsgLen() {
		return gMsgLen;
	}

	public void setgMsgLen(byte[] gMsgLen) {
		this.gMsgLen = gMsgLen;
	}

	public byte[] getbMsgCmd() {
		return bMsgCmd;
	}

	public void setbMsgCmd(byte[] bMsgCmd) {
		this.bMsgCmd = bMsgCmd;
	}

	public byte[] getbMsgBody() {
		return bMsgBody;
	}

	public void setbMsgBody(byte[] bMsgBody) {
		this.bMsgBody = bMsgBody;
	}

	public byte[] getbMsgVrf() {
		return bMsgVrf;
	}

	public void setbMsgVrf(byte[] bMsgVrf) {
		this.bMsgVrf = bMsgVrf;
	}

	@Override
	public String toString() {
		return "Message [gMsgHead=" + Arrays.toString(gMsgHead) + ", gMsgID=" + Arrays.toString(gMsgID) + ", gGtwMac="
				+ Arrays.toString(gGtwMac) + ", gMsgLen=" + Arrays.toString(gMsgLen) + ", bMsgCmd="
				+ Arrays.toString(bMsgCmd) + ", bMsgBody=" + Arrays.toString(bMsgBody) + ", bMsgVrf="
				+ Arrays.toString(bMsgVrf) + "]";
	}
	
	public byte[] toBytesNotCheck() {
		List<byte[]> bytes = new ArrayList<byte[]>();
		bytes.add(gMsgHead);
		bytes.add(gMsgID);
		bytes.add(gGtwMac);
		bytes.add(gMsgLen);
		bytes.add(bMsgCmd);
		bytes.add(bMsgBody);
		byte[] backnews = MergerByteList.streamCopy(bytes);
		return backnews;
	}
	
	public byte[] toBytes() {
		List<byte[]> bytes = new ArrayList<byte[]>();
		bytes.add(gMsgHead);
		bytes.add(gMsgID);
		bytes.add(gGtwMac);
		bytes.add(gMsgLen);
		bytes.add(bMsgCmd);
		bytes.add(bMsgBody);
		bytes.add(bMsgVrf);
		byte[] backnews = MergerByteList.streamCopy(bytes);
		return backnews;
	}
	
	

}
