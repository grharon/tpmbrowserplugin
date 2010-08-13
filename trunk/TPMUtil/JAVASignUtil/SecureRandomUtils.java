package com.intel.splat.identityservice.utils;

import java.math.BigInteger;
import java.security.SecureRandom;

public class SecureRandomUtils {
	private static SecureRandom _secureRandom = new SecureRandom();
	
	public static String generateRandomHexString(int size) {
		/*
		byte[] sid = new byte[_randomStringSize];
        _secureRandom.nextBytes(sid);
        String result = new String(sid);
        */
		String result = new BigInteger(130, _secureRandom).toString(size);
        return result;
	}
}
