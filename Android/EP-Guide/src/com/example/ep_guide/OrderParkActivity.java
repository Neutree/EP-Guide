package com.example.ep_guide;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import com.ep_guide.myadapter.DBAdapter;
import com.ep_guide.thread.Data_Order;

public class OrderParkActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_order_park);
		if(Data_Order.Status[0].equals("1")){
			Data_Order.Status[0] = "被占用";
		}
		else{
			Data_Order.Status[0] = "可使用";
		}
		if(Data_Order.Status[1].equals("1")){
			Data_Order.Status[1] = "被占用";
		}
		else{
			Data_Order.Status[1] = "可使用";
		}

		//text1
		TextView textView1 = (TextView) findViewById(R.id.textView1);
		TextView textView2 = (TextView) findViewById(R.id.textView2); 
		TextView textView3 = (TextView) findViewById(R.id.textView3); 
		TextView textView4 = (TextView) findViewById(R.id.textView4); 
		String id1 = "车位号："+Data_Order.Park_ID[0].toString();
		String parkspaceName1 = "车主姓名："+Data_Order.ParkspaceName[0].toString();
		String status1 = "车位状态："+Data_Order.Status[0].toString();
		String start_time1 = "停车时间："+Data_Order.Start_Time[0];
		textView1.setText(id1);
		textView2.setText(parkspaceName1);
		textView3.setText(status1);
		textView4.setText(start_time1);
		//text2
		TextView textView5 = (TextView) findViewById(R.id.textView5);
		TextView textView6 = (TextView) findViewById(R.id.textView6);
		TextView textView7 = (TextView) findViewById(R.id.textView7);
		TextView textView8 = (TextView) findViewById(R.id.textView8);
		String id2 = "车位号："+Data_Order.Park_ID[1].toString();
		String parkspaceName2 = "车主姓名："+Data_Order.ParkspaceName[1].toString();
		String status2 = "车位状态："+Data_Order.Status[1].toString();
		String start_time2 = "停车时间："+Data_Order.Start_Time[1];
		textView5.setText(id2);
		textView6.setText(parkspaceName2);
		textView7.setText(status2);
		textView8.setText(start_time2);
		Button button = (Button) findViewById(R.id.button1);
		button.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(OrderParkActivity.this,CarportActivity.class);
				startActivity(intent);
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_order_park, menu);
		return true;
	}
	

}
