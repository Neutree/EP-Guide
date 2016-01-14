package com.example.ep_guide;

import com.ep_guide.myadapter.DBAdapter;
import com.ep_guide.thread.Data;
import com.ep_guide.thread.Data_Parking;
import com.ep_guide.thread.OrderParking;



import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class LoginSucActivity extends Activity {
	Button Order;
	Button Query;
	Button Show;
	public DBAdapter dbadapter = new DBAdapter(LoginSucActivity.this);

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_login_suc);
		//Intent intent = getIntent();
		//Bundle bundle = intent.getExtras();
		//String str = bundle.getString("user");
		dbadapter.open();
		String str = dbadapter.queryname();
		String username = "尊敬的"+str+",請選擇您需要的操作:";
		TextView show = (TextView) findViewById(R.id.textView1);
		show.setText(username);
		Order = (Button) findViewById(R.id.Button01);
		Query = (Button) findViewById(R.id.Button02);
		Show = (Button) findViewById(R.id.Button03);
		Order.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(LoginSucActivity.this,
						OrderParkActivity.class);
				new OrderParking().start();
				for (int i = 0; i < 200000000; i++) {
					;
				}
				if (Data_Parking.data_parking == 8004) {
					startActivity(intent);							
				} 
				if(Data_Parking.data_parking == 2004) {
					new AlertDialog.Builder(LoginSucActivity.this)
					.setTitle("错误").setMessage("登录失败")
					.setPositiveButton("确定", null).show();
				}
				if(Data_Parking.data_parking == 3001) {
					new AlertDialog.Builder(LoginSucActivity.this)
					.setTitle("错误").setMessage("非法用户访问")
					.setPositiveButton("确定", null).show();
				}
				if(Data_Parking.data_parking == 6004) {
					new AlertDialog.Builder(LoginSucActivity.this)
					.setTitle("错误").setMessage("没有此车库ID")
					.setPositiveButton("确定", null).show();
				}
				
			}
		});
		
		Query.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent newIntent = new Intent(LoginSucActivity.this,QueryHistoryActivity.class);
				startActivity(newIntent);
			}
		});
		
		Show.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(LoginSucActivity.this,HttpShowActivity.class);
				startActivity(intent);
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_login_suc, menu);
		return true;
	}
	
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub
	  switch(item.getItemId()){
		case R.id.menu_quit:
			dbadapter.deleteAllData();
			LoginSucActivity.this.finish();
			return true;
		default:
			return false;			
		}
	}

	

}
