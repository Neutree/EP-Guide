package bean;

public class ParkHistory {
	private  String car_ID;
	private  String pSpace_ID;
	private  String garage_ID;
	private  String getIn_time;
	private  String park_time;
	
	public ParkHistory() {
		super();
	}

	public ParkHistory(String car_ID, String pSpace_ID, String garage_ID, String getIn_time, String park_time) {
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

	public String getGetIn_time() {
		return getIn_time;
	}

	public void setGetIn_time(String getIn_time) {
		this.getIn_time = getIn_time;
	}

	public String getPark_time() {
		return park_time;
	}

	public void setPark_time(String park_time) {
		this.park_time = park_time;
	}

	@Override
	public String toString() {
		return "ParkHistory [car_ID=" + car_ID + ", pSpace_ID=" + pSpace_ID + ", garage_ID=" + garage_ID
				+ ", getIn_time=" + getIn_time + ", park_time=" + park_time + "]";
	}
	
	
	
}
