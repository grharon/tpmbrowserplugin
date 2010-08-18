package com.intel.splat.identityservice.tpm;

import org.opensaml.common.impl.SecureRandomIdentifierGenerator;
import java.security.NoSuchAlgorithmException;
public class GenSecurityID {
	private static SecureRandomIdentifierGenerator _generator = null;
	
	/**
	 * 
	 * @throws NoSuchAlgorithmException
	 */
	public static void Initialize() 
	throws NoSuchAlgorithmException{
		if (_generator == null) {
			_generator = new SecureRandomIdentifierGenerator ();
		}
	}
	/**
	 * 
	 * @return
	 */
	public static String getSecurityID() {
        return _generator.generateIdentifier();
    }
}
