package deal;

import bean.Message;
import constant.ConstantCode;
import decode.DecodeMsg;
import socket.DoWork;

public class DealMsgCmd implements DoWork {

	Message msgBean;
	byte[] backMsg;

	@Override
	public byte[] doWork(byte[] recvMsg) {
		// TODO Auto-generated method stub
		msgBean = DecodeMsg.Decode(recvMsg);
		byte[] recvCmd=msgBean.getbMsgCmd();
		backMsg=GtwLogin.GtwLogin();
		if (recvCmd.equals(ConstantCode.cReqGateWayLogin)) {
			
		}
		return backMsg;
	}

}
