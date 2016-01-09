package test;

import tools.Encrypt;

public class testMd5 {
	public static void main(String[] args) {
		String pwd="gg";
		pwd=Encrypt.md5(pwd);
		System.out.println("md5:"+pwd);
		pwd=Encrypt.encode_sha1(pwd+"&A-402");
		System.out.println("sha1:"+pwd);
	}
}
