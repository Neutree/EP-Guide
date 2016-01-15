package com.example.ep_guide;


import com.ep_guide.thread.Data;
import com.ep_guide.thread.Data_Regist;
import com.ep_guide.thread.HttpRegist;
import com.ep_guide.thread.HttpThread1;

import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.text.TextUtils;
import android.view.Menu;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class RegistActivity extends Activity {
	private String username,password,car_ID,user_ID;
	private Button signUp;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_regist);
		signUp = (Button) findViewById(R.id.signUp);
		signUp.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				username = ((EditText) findViewById(R.id.nickName)).getText()
						.toString();
				password = ((EditText) findViewById(R.id.newPassword))
						.getText().toString();
				System.out.println("username:"+username);
				System.out.println("pass:"+password);
				car_ID = ((EditText) findViewById(R.id.newCarID)).getText().toString();
				System.out.println("car:"+car_ID);
				user_ID = ((EditText)findViewById(R.id.UserID))
						.getText().toString();
				if (TextUtils.isEmpty(username) || TextUtils.isEmpty(password) || TextUtils.isEmpty(car_ID)){
					new AlertDialog.Builder(RegistActivity.this)
					.setTitle("请补充以下内容").setMessage("用户名或密码为空，请完善")
							.setPositiveButton("确定", null).show();
					}
				else {
					new HttpRegist(username, password,car_ID,user_ID).start();
					for (int i = 0; i < 200000000; i++) {
						;
					}
					System.out.println(Data_Regist.data_regist);
					if (Data_Regist.data_regist == 8002) {
						new AlertDialog.Builder(RegistActivity.this)
						.setTitle("恭喜").setMessage("注册成功")
						.setPositiveButton("确定", null).show();		
						
					} 
					if(Data_Regist.data_regist == 7002) {
						new AlertDialog.Builder(RegistActivity.this)
						.setTitle("遗憾").setMessage("该用户名已注册")
						.setPositiveButton("确定", null).show();
					}
					if(Data_Regist.data_regist == 2002) {
						new AlertDialog.Builder(RegistActivity.this)
						.setTitle("错误").setMessage("注册失败")
						.setPositiveButton("确定", null).show();
					}
				}
				
			}
			});
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_regist, menu);
		return true;
	}

}
