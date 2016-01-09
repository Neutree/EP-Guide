package lower_computer;

import java.net.Socket;

import socket.ServerThead;

public class UptParkSpaceStatus extends ServerThead{

	public UptParkSpaceStatus(Socket socket) {
		super(socket);
		// TODO Auto-generated constructor stub
	}
	
	@Override
	public void deal(){
		backMsg="xixixi";
	}

}
