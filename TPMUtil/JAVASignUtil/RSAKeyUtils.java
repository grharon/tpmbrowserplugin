package com.intel.splat.identityservice.utils;

import org.opensaml.xml.util.Base64;
import org.bouncycastle.jce.provider.BouncyCastleProvider;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;
import java.math.BigInteger;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.PrivateKey;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.security.spec.KeySpec;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.RSAPublicKeySpec;
import java.security.spec.X509EncodedKeySpec;
/**
 * 
 * Get the PublicKey or PrivateKey from key file or key string
 *
 */
public class RSAKeyUtils {
	private static final String CRT_KEY_HEAD = "-----BEGIN CERTIFICATE-----";
	private static final String CRT_KEY_TAIL = "-----END CERTIFICATE-----";
	private static final String PUBLIC_KEY_HEAD = "-----BEGIN PUBLIC KEY-----";
	private static final String PUBLIC_KEY_TAIL = "-----END PUBLIC KEY-----";	
	private static final String PRIVATE_KEY_HEAD = "-----BEGIN RSA PRIVATE KEY-----";
	private static final String PRIVATE_KEY_TAIL = "-----END RSA PRIVATE KEY-----";
	private static final String X509KEY_INSTANCE = "X.509";
	private static final String KEY_FACTORY_INSTANCE = "RSA"; 
	
	/**
	 * The certificate file must contain CRT_KEY_HEAD and CRT_KEY_TAIL
	 * @param certFile
	 * @return
	 * @throws CertificateException
	 * @throws IOException
	 */
	public static X509Certificate getCertificateFromFile(String certFile) 
	throws CertificateException, IOException{
		CertificateFactory cf;
	    FileInputStream fin = new FileInputStream(certFile);
	    cf = CertificateFactory.getInstance(X509KEY_INSTANCE);
	    X509Certificate x509Cert = (X509Certificate) cf.generateCertificate(fin);
	    fin.close();
	    return x509Cert;
	}
	/**
	 * The certificate content must contain CRT_KEY_HEAD and CRT_KEY_TAIL
	 * @param keyString
	 * @return
	 * @throws CertificateException
	 * @throws IOException
	 */
	public static X509Certificate getCertificateFromContent(String keyString) 
	throws CertificateException, IOException{
		if (!keyString.contains(CRT_KEY_HEAD) || !keyString.contains(CRT_KEY_TAIL)) {
			return null;
		}
		ByteArrayInputStream bin = 
			new ByteArrayInputStream(keyString.getBytes());
		CertificateFactory cf;
	    
	    cf = CertificateFactory.getInstance(X509KEY_INSTANCE);
	    X509Certificate x509Cert = 
	    	(X509Certificate) cf.generateCertificate(bin);
	    bin.close();
	    return x509Cert;
	}
	/**
	 * The public key file may contain PUBLIC_KEY_HEAD and PUBLIC_KEY_TAIL
	 * It does not matter if it does not contain them.
	 * @param pubKeyFile
	 * @return
	 * @throws CertificateException
	 * @throws IOException
	 */
	public static PublicKey getPublicKeyFromFile(String pubKeyFile) 
	throws CertificateException, IOException{	    
	    return getPublicKeyFromContent(getPublicKeyContentFromFile(pubKeyFile));
	}
	
	public static String getPublicKeyContentFromFile(String pubKeyFile) 
	throws CertificateException, IOException{
		BufferedReader in = new BufferedReader(new FileReader(pubKeyFile));
		String line;
		StringBuffer pubKeyCont = new StringBuffer("");
	    
	    line = in.readLine();
	    while (line != null) {
	    	pubKeyCont.append(line).append("\r\n");
	    	line = in.readLine();
	    }
	    in.close();
	    
	    String pubKeyString = pubKeyCont.toString();
	    
	    pubKeyString = pubKeyString.replace(PUBLIC_KEY_HEAD, "");
	    pubKeyString = pubKeyString.replace(PUBLIC_KEY_TAIL, "");
	    pubKeyString = pubKeyString.trim();
	    
	    return pubKeyString;
	}
	
	public static PublicKey getPublicKeyFromContent(String keyString) {
		KeyFactory keyFactory;
		KeySpec pubSpec;
	    byte [] binaryKey;
	    PublicKey pubKey = null;
	    String pubKeyString = keyString;

		try {
	        keyFactory = KeyFactory.getInstance(KEY_FACTORY_INSTANCE);
	        binaryKey = Base64.decode(pubKeyString);
		    pubSpec = new RSAPublicKeySpec(createUnsignedBigInt(binaryKey), new BigInteger("65537"));
		    pubKey = keyFactory.generatePublic(pubSpec);
	        
	        pubKey = keyFactory.generatePublic(pubSpec);
	    } catch (NoSuchAlgorithmException nsae) {
	    	nsae.printStackTrace();
	    } catch (InvalidKeySpecException ikse) {
	    	ikse.printStackTrace();
	    }
	    return pubKey;
	}
	
    public static BigInteger createUnsignedBigInt( byte[] bytes ) {
        if ( (bytes[0] & 0x80) != 0 )
        {
            byte[] singleZero =
                { 0 };
            bytes = ByteArrayConcat( singleZero, bytes );
        }
        BigInteger bi = new BigInteger( bytes );
        return bi;
    }
    
    public static byte[] ByteArrayConcat( byte[]... arrays ) {
        int totalLength = 0;
        for ( byte[] a : arrays )
        {
            if ( a != null )
            {
                totalLength += a.length;
            }
        }
        byte[] ret = new byte[totalLength];
        int cur = 0;
        for ( byte[] a : arrays )
        {
            if ( a != null )
            {
                System.arraycopy( a, 0, ret, cur, a.length );
                cur += a.length;
            }
        }
        return ret;
    }
}
