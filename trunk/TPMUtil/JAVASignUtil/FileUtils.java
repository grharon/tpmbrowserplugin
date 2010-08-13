package com.intel.splat.identityservice.utils;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class FileUtils {
	public static String fileToString(String filePath) throws IOException {
		StringBuffer stringBuffer = new StringBuffer();
		char[] charBuffer = new char[4096];
		int count = -1;
		
		BufferedReader in = new BufferedReader(new FileReader(filePath));
		while ((count = in.read(charBuffer)) > 0) {
			stringBuffer.append(charBuffer, 0, count);
		}
	    
		return stringBuffer.toString();
	}
}
