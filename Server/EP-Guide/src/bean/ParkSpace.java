package bean;

public class ParkSpace {
	private String ID;
	private String park_ID;
	private String park_spaceName;
	private String garage_ID;
	private String status;
	private String start_time;
	
	public ParkSpace(){
		
	}
	
	public ParkSpace(String ID,String park_ID, String park_spaceName, String garage_ID, String status, String start_time) {
		super();
		this.ID=ID;
		this.park_ID = park_ID;
		this.park_spaceName = park_spaceName;
		this.garage_ID = garage_ID;
		this.status = status;
		this.start_time = start_time;
	}

	public String getPark_ID() {
		return park_ID;
	}

	public void setPark_ID(String park_ID) {
		this.park_ID = park_ID;
	}

	public String getPark_spaceName() {
		return park_spaceName;
	}

	public void setPark_spaceName(String park_spaceName) {
		this.park_spaceName = park_spaceName;
	}

	public String getGarage_ID() {
		return garage_ID;
	}

	public void setGarage_ID(String garage_ID) {
		this.garage_ID = garage_ID;
	}

	public String getStatus() {
		return status;
	}

	public void setStatus(String status) {
		this.status = status;
	}

	public String getStart_time() {
		return start_time;
	}

	public void setStart_time(String start_time) {
		this.start_time = start_time;
	}

	public String getID() {
		return ID;
	}

	public void setID(String iD) {
		ID = iD;
	}

	@Override
	public String toString() {
		return "ParkSpace [ID=" + ID + ", park_ID=" + park_ID + ", park_spaceName=" + park_spaceName + ", garage_ID="
				+ garage_ID + ", status=" + status + ", start_time=" + start_time + "]";
	}
	
	
}
