package deal;

import bean.Message;
import constant.ConstantCode;
import decode.DecodeMsg;
import socket.DoWork;
import tool.AndVerify;
import tool.DataTransform;

public class DealMsgCmd implements DoWork {

	Message msgBean;
	byte[] backMsg;
	int recvCmd;

	@Override
	public byte[] doWork(byte[] recvMsg) {
		// -- 数据初始化 --//
		msgBean = DecodeMsg.Decode(recvMsg);
		recvCmd = DataTransform.byteGroupToInt(msgBean.getbMsgCmd());

		// -- 校验字check--//
		if (!AndVerify.checkVrf(recvMsg, msgBean.getbMsgVrf())) {
			System.out.println("校验字有误！");
			return null;// 返回空，丢弃此包
		}

		// -- 根据命令字进行对应操作 --//

		if (recvCmd == ConstantCode.cReqGateWayLogin) {
			backMsg = GtwLogin.login(msgBean);
		}else if (recvCmd == ConstantCode.cReqLinkCheck) {
			backMsg = HeartBeat.link(msgBean);
		}else if(recvCmd == ConstantCode.cReqNodeStatusUpdate) {
			backMsg = NodeStatusUpdate.update(msgBean);
		}else if(recvCmd == ConstantCode.cReqNotNodeInforSync) {
			backMsg = NodeInfoSyncNot.update(msgBean);
		}else if(recvCmd == ConstantCode.cReqAddNode) {
			backMsg = NodeAdd.add(msgBean);
		}else if(recvCmd == ConstantCode.cReqNodeDel) {
			backMsg = NodeDelete.delete(msgBean);
		}

		// -- 返回需响应的消息包--//
		return backMsg;
	}

}
