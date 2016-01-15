package com.ep_guide.thread;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import android.os.Handler;
import android.webkit.WebView;

public class HttpShow extends Thread {
	private String url;
	private WebView webview;
	private Handler handler;
	
	public HttpShow(String url,WebView webview,Handler handler){
		this.url = url;
		this.webview = webview;
		this.handler = handler;
	}
	
	@Override
	public void run() {
		try {
			URL httpUrl = new URL(url);
			HttpURLConnection conn = (HttpURLConnection)httpUrl.openConnection();
			conn.setConnectTimeout(5000);
			conn.setRequestMethod("GET");
			final StringBuffer sb = new StringBuffer();
			BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
			String str;
			while((str = reader.readLine())!=null){
				sb.append(str);
			}
			
			handler.post(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					webview.loadData(sb.toString(), "text/html;charset=utf-8", null);
				}
			});
				
		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
