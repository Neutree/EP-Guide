package com.example.ep_guide;

import com.ep_guide.myadapter.DBAdapter;
import com.ep_guide.thread.Data;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Window;
import android.view.WindowManager;

public class StartActivity extends Activity {
	private final int SPLASH_DISPLAY_LENGHT = 3000;
	private String token = null;
	public DBAdapter dbadapter = new DBAdapter(StartActivity.this);
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// 设置无标题
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		// 设置全屏显示
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		// 加载布局文件
		setContentView(R.layout.activity_start);
		// 线程控制3S后跳转主界面
		dbadapter.open();
		token = dbadapter.query();
		if(token.equals("0")){
		new Handler().postDelayed(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				Intent mainIntent = new Intent(StartActivity.this,
						MainActivity.class);
				StartActivity.this.startActivity(mainIntent);
				StartActivity.this.finish();

			}
		}, SPLASH_DISPLAY_LENGHT);
		}
		
		else {
			Data.token = token;
			new Handler().postDelayed(new Runnable() {
				@Override
				public void run() {
					
					Intent secondIntent = new Intent(StartActivity.this,
							LoginSucActivity.class);
					StartActivity.this.startActivity(secondIntent);
					StartActivity.this.finish();
				}
			}, SPLASH_DISPLAY_LENGHT);
		}
	}

}
