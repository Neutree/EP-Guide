package com.example.ep_guide;

import com.ep_guide.thread.Data;
import com.ep_guide.thread.HttpThread1;
import com.example.ep_guide.LoginSucActivity;
import com.example.ep_guide.RegistActivity;
import com.example.ep_guide.R;
import com.ep_guide.myadapter.*;

import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.text.TextUtils;
import android.view.Menu;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.CompoundButton.OnCheckedChangeListener;

public class MainActivity extends Activity {
	private Button signIn;
	private String account, password,getuser,getpassword;
	private TextView signUp;
	private SharedPreferences sp_aap;
	private CheckBox checkbox1;
	private EditText USER, PASSWORD;
	private ProgressDialog progressDialog;
	public DBAdapter dbadapter = new DBAdapter(MainActivity.this);
	
	public static int MODE = MODE_PRIVATE;
	public static final String AAP = "userInfo";
	public static final String CB = "ifChecked";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		signIn = (Button) findViewById(R.id.signIn);
		signUp = (TextView) findViewById(R.id.signUp);
		checkbox1 = (CheckBox) findViewById(R.id.checkbox_save);
		
		sp_aap = this.getSharedPreferences(AAP, MODE);
		USER = (EditText) findViewById(R.id.Account);
		PASSWORD = (EditText) findViewById(R.id.Password);
		getuser = sp_aap.getString("Account", "dandan");
		getpassword = sp_aap.getString("Password", "gg");
		USER.setText(getuser);
		PASSWORD.setText(getpassword);
		
		//checkbox选中
		checkbox1.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView,
					boolean isChecked) {
				// TODO Auto-generated method stub
				if (isChecked) {
					Toast.makeText(MainActivity.this, "Checked",
							Toast.LENGTH_SHORT).show();
				} else {

					Toast.makeText(MainActivity.this, "unChecked",
							Toast.LENGTH_SHORT).show();

				}

			}
		});
		
		//注册入口
		signUp.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent intentsignUp = new Intent(MainActivity.this,
						RegistActivity.class);
				startActivity(intentsignUp);


			}
		});
		
		//登录按钮响应
		signIn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				account = ((EditText) findViewById(R.id.Account)).getText()
						.toString();
				password = ((EditText) findViewById(R.id.Password)).getText()
						.toString();
				Intent intent = new Intent(MainActivity.this,
						LoginSucActivity.class);
						Bundle bundle = new Bundle();
						bundle.putCharSequence("user", account);
						intent.putExtras(bundle);
						if (TextUtils.isEmpty(account) || TextUtils.isEmpty(password)) {
						new AlertDialog.Builder(MainActivity.this)
						.setTitle("请补充以下内容").setMessage("用户名或密码为空，请完善")
								.setPositiveButton("确定", null).show();

				} else {
					//show();
					new HttpThread1(account, password).start();
					for (int i = 0; i < 200000000; i++) {
						;
					}

					if (Data.data == 8001) {
						startActivity(intent);
						dbadapter.open();
						dbadapter.insert(account,Data.token);				
					} 
					if(Data.data == 2001) {
						new AlertDialog.Builder(MainActivity.this)
						.setTitle("错误").setMessage("登录失败")
						.setPositiveButton("确定", null).show();
					}
					if(Data.data == 4002) {
						new AlertDialog.Builder(MainActivity.this)
						.setTitle("错误").setMessage("密码错误")
						.setPositiveButton("确定", null).show();
					}
					if(Data.data == 6002) {
						new AlertDialog.Builder(MainActivity.this)
						.setTitle("错误").setMessage("账号未注册")
						.setPositiveButton("确定", null).show();
					}
					if (checkbox1.isChecked() == true) {
						SharedPreferences.Editor editor_aap = sp_aap.edit();
						editor_aap.putString("Account", account);
						editor_aap.putString("Password", password);
						editor_aap.commit();
					}
				
				}
			}
		});

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}
	
	public void show(){
		progressDialog = new ProgressDialog(MainActivity.this);
		progressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
		progressDialog.setTitle("登录中");	
		progressDialog.setMessage("欢迎支持EP-Guide");
		progressDialog.show();
	}

}
