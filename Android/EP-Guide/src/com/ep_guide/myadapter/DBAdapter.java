package com.ep_guide.myadapter;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteException;

import com.ep_guide.thread.*;


public class DBAdapter {

	private Context context;
	private SQLiteDatabase db;
	private DBOpenHelper dbOpenHelper;

	public DBAdapter(Context context1) {
		context = context1;

	}

	public void open() {
		dbOpenHelper = new DBOpenHelper(context, "user.db", null, 1);
		try {
			db = dbOpenHelper.getWritableDatabase();
		} catch (SQLiteException ex) {
			db = dbOpenHelper.getReadableDatabase();
		}

	}

	
	//--用户信息操作	
	
	//插入
		public long insert(String username,String token) {
		ContentValues newValues = new ContentValues();
		newValues.put("username", username);
		newValues.put("token", token);
		return db.insert("userinfo", null, newValues);
	}
		
	//查询	
		public String query(){
			String token = null;
			Cursor pw = db.query("userinfo", new String[] { "username",
					"token"}, null, null, null, null, null);
			if (pw.getCount()==0) {
				return  "0";
			}
			for (pw.moveToFirst(); !pw.isAfterLast(); pw.moveToNext()) {
				int pwColumn = pw.getColumnIndex("token");
				token = pw.getString(pwColumn);
			}
			pw.close();
			return token;
		}
		
		public String queryname(){
			String username = null;
			Cursor pw = db.query("userinfo", new String[] { "username",
					"token"}, null, null, null, null, null);
			for (pw.moveToFirst(); !pw.isAfterLast(); pw.moveToNext()) {
				int pwColumn = pw.getColumnIndex("username");
				username = pw.getString(pwColumn);
			}
			return username;
		}

	public long deleteAllData() {
		return db.delete("userinfo", null, null);
	}

	public long deleteOneData(String name) {
		return db.delete("userinfo", "username=?", new String[] {name});
	}
	


	public void close() {
		if (db != null) {
			db.close();
			db = null;
		}

	}

}
