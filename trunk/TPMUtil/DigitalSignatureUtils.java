package com.intel.splat.identityservice.utils;

import java.io.IOException;
import java.security.InvalidKeyException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.Signature;
import java.security.SignatureException;
import java.security.UnrecoverableEntryException;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;

public class DigitalSignatureUtils {
	public static byte[] sign(byte[] plainText, String privateKeyFile) {
		try {
			Signature sig = Signature.getInstance("SHA1WithRSA");
			sig.initSign(X509KeyUtils.getPrivateKeyFromFile(privateKeyFile));
			sig.update(plainText);     
			return sig.sign();
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		} catch (InvalidKeyException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (SignatureException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public static byte[] sign(byte[] plainText, 
			String keyStorePath, 
			String storePass,
			String entryAlias,
			String entryPass) {
		try {
			Signature sig = Signature.getInstance("SHA1WithRSA");           
            KeyStore keyStore = KeyStoreUtils.getKeyStore (keyStorePath, storePass); 
            KeyStore.PrivateKeyEntry entry = 
            	(KeyStore.PrivateKeyEntry)keyStore.getEntry (entryAlias, 
            			new KeyStore.PasswordProtection(entryPass.toCharArray()));
			sig.initSign(entry.getPrivateKey());
			sig.update(plainText);     
			return sig.sign();
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		} catch (InvalidKeyException e) {
			e.printStackTrace();
		} catch (SignatureException e) {
			e.printStackTrace();
		} catch (KeyStoreException e) {
			e.printStackTrace();
		} catch (UnrecoverableEntryException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public static boolean verify(byte[] plainText, 
			byte[] signature, 
			PublicKey pubKey) {
		try {
			Signature sig = Signature.getInstance("SHA1WithRSA");
			sig.initVerify(pubKey); 
			sig.update(plainText);
			return sig.verify(signature);
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		} catch (InvalidKeyException e) {
			e.printStackTrace();
		}catch (SignatureException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	public static boolean verify(byte[] plainText, 
			byte[] signature,
			String keyStorePath, 
			String storePass,
			String entryAlias,
			String entryPass) {
		try {
			Signature sig = Signature.getInstance("SHA1WithRSA");           
            KeyStore keyStore = KeyStoreUtils.getKeyStore (keyStorePath, storePass); 
            KeyStore.PrivateKeyEntry entry = 
            	(KeyStore.PrivateKeyEntry)keyStore.getEntry (entryAlias, 
            			new KeyStore.PasswordProtection(entryPass.toCharArray()));
			sig.initVerify(entry.getCertificate ().getPublicKey ());
			sig.update(plainText);
			return sig.verify(signature);
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		} catch (InvalidKeyException e) {
			e.printStackTrace();
		} catch (SignatureException e) {
			e.printStackTrace();
		} catch (KeyStoreException e) {
			e.printStackTrace();
		} catch (UnrecoverableEntryException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	public static void main(String[] args) throws Exception{     
		// Sign and verify with certificate file
		byte[] signature = DigitalSignatureUtils.sign("1263952065375".getBytes(), 
					"war/WEB-INF/conf/identityservice/idp1/intel_demo_key.key");
		if(signature != null) {
			X509Certificate cert = X509KeyUtils.getCertificateFromFile("war/WEB-INF/conf/identityservice/idp1/intel_demo_cert.crt");
			if(cert != null && DigitalSignatureUtils.verify("1263952065375".getBytes(), 
					signature, 
					cert.getPublicKey())) {
				System.out.println("Sign and verify with certificate file successfully!");
			} else {
				System.out.println("Fail to verify with certificate file!");
			}
		} else {
			System.out.println("Fail to sign certificate file!");
		}
		
		// Sign and verify with keystore
		signature = DigitalSignatureUtils.sign("plainText".getBytes(), 
				"war/WEB-INF/conf/keystore.jks",
				"changeit",
				"test",
				"changeit");
		if(signature != null) {
			if(DigitalSignatureUtils.verify("plainText".getBytes(), 
					signature, 
					"war/WEB-INF/conf/keystore.jks",
					"changeit",
					"test",
					"changeit")) {
				System.out.println("Sign and verify with keystore successfully!");
			} else {
				System.out.println("Fail to verify with keystore!");
			}
		} else {
			System.out.println("Fail to sign with keystore!");
		}
	}     
}
