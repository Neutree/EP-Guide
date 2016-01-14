package com.example.ep_guide;

import com.ep_guide.thread.HttpShow;

import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.view.Menu;
import android.webkit.WebView;

public class HttpShowActivity extends Activity {
	private WebView webview;
	private Handler handler = new Handler();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_http_show);
		webview = (WebView) findViewById(R.id.webView1);
		new HttpShow("http://192.168.191.1:8080/EP-Guide/parkAnimation.jsp", webview, handler).start();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_http_show, menu);
		return true;
	}

}
