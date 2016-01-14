package com.ep_guide.myadapter;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.database.sqlite.SQLiteOpenHelper;

public class DBOpenHelper extends SQLiteOpenHelper {

	public DBOpenHelper(Context context, String name, CursorFactory factory,
			int version) {
		super(context, name, factory, version);
		// TODO Auto-generated constructor stub
	}
	//创建数据库的SQL语句
	private static final String DB_CREATE="create table userinfo" + "(_id integer primary key autoincrement,username text not null,"+"token text not null);";
	

	@Override
	public void onCreate(SQLiteDatabase _db) {
		// TODO Auto-generated method stub
		_db.execSQL(DB_CREATE);
		
	}

	@Override
	public void onUpgrade(SQLiteDatabase _db, int _oldVersion, int _newVersion) {
		// TODO Auto-generated method stub
		_db.execSQL("DROP TABLE IF EXISTS foruminfo");
		onCreate(_db);
	}

}
