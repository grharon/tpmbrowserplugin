package com.intel.splat.identityservice.tpm;

import java.io.FileInputStream;
import java.io.IOException;
import java.security.cert.CertificateException;
import java.util.Properties;

import edu.mit.csail.tpmj.TPMConsts;
import edu.mit.csail.tpmj.drivers.TPMDriver;
import edu.mit.csail.tpmj.funcs.TPMUtilityFuncs;

public class TPMSign {
	private static final String TPM_KEYWRAPPER_FILE = "intel-tpm.kw";
	private static final String TPM_PUBLICKEY_FILE = "intel-tpm.pub";
	private static final String TPM_CONFIG_FILE = "tpm.conf";	
	
	public static String getPublicKeyContent() {
		String userHome = System.getProperty("user.home");
		String tpmPubKeyFilePath = userHome + "/" + TPM_PUBLICKEY_FILE;
		try {
			String pubKeyContent = RSAKeyUtils.getPublicKeyContentFromFile(tpmPubKeyFilePath);
			return pubKeyContent;
		} catch (Exception e) {
	        throw new RuntimeException(e);
		}
	}
	
	/**
	 * Sign with the specified private key file
	 * @param data - the data needs to be signed
	 * @param keyAuth - tpm password
	 * @return
	 */
	public static byte[] sign(byte[] data, String keyAuth) {	
		String userHome = System.getProperty("user.home");
		String tpmKeyWrapperFilePath = userHome + "/" + TPM_KEYWRAPPER_FILE;
		String tpmConfigFilePath =  userHome + "/" + TPM_CONFIG_FILE;
		
	    try {
	        // Initialize the TPM driver
	    	Properties property = new Properties();
	    	property.load(new FileInputStream(tpmConfigFilePath));
	    	if(!property.containsKey("os.name")) {
	    		throw new RuntimeException("Incorrect TPM platform OS setting.");
	    	}
			System.getProperties().put("os.name", property.get("os.name"));
			
	        TPMDriver tpmDriver = TPMUtilityFuncs.getTPMDriver();
	        
	    	int keyHandle = TPMInitKey.loadKey(TPMConsts.TPM_KH_SRK, keyAuth, tpmKeyWrapperFilePath);
	    	
		    byte[] signature = TPMInitKey.sign(data, keyHandle, keyAuth);
		    
		    TPMInitKey.dropKey(keyHandle);
		    
		    return signature;
	    } catch (Exception e) {
	        throw new RuntimeException(e);
	    } finally {
	        TPMUtilityFuncs.cleanupTPMDriver();
	    }
	}
}
