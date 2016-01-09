package bean;

public class ParkHistory {
	private  String car_ID;
	private  String pSpace_ID;
	private  String garage_ID;
	private  long getIn_time;
	private  long park_time;
	
	public ParkHistory() {
		super();
	}

	public ParkHistory(String car_ID, String pSpace_ID, String garage_ID, long getIn_time, long park_time) {
		super();
		this.car_ID = car_ID;
		this.pSpace_ID = pSpace_ID;
		this.garage_ID = garage_ID;
		this.getIn_time = getIn_time;
		this.park_time = park_time;
	}

	public String getCar_ID() {
		return car_ID;
	}

	public void setCar_ID(String car_ID) {
		this.car_ID = car_ID;
	}

	public String getpSpace_ID() {
		return pSpace_ID;
	}

	public void setpSpace_ID(String pSpace_ID) {
		this.pSpace_ID = pSpace_ID;
	}

	public String getGarage_ID() {
		return garage_ID;
	}

	public void setGarage_ID(String garage_ID) {
		this.garage_ID = garage_ID;
	}

	public long getGetIn_time() {
		return getIn_time;
	}

	public void setGetIn_time(long getIn_time) {
		this.getIn_time = getIn_time;
	}

	public long getPark_time() {
		return park_time;
	}

	public void setPark_time(long park_time) {
		this.park_time = park_time;
	}

	@Override
	public String toString() {
		return "ParkHistory [car_ID=" + car_ID + ", pSpace_ID=" + pSpace_ID + ", garage_ID=" + garage_ID
				+ ", getIn_time=" + getIn_time + ", park_time=" + park_time + "]";
	}
	
	
	
}
