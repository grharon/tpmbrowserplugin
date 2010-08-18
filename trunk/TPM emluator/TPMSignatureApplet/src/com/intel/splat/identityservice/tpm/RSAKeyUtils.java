package com.intel.splat.identityservice.tpm;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.math.BigInteger;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.cert.CertificateException;
import java.security.spec.KeySpec;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.RSAPublicKeySpec;

/**
 * 
 * Get the PublicKey or PrivateKey from key file or key string
 *
 */
public class RSAKeyUtils {
	private static final String PUBLIC_KEY_HEAD = "-----BEGIN PUBLIC KEY-----";
	private static final String PUBLIC_KEY_TAIL = "-----END PUBLIC KEY-----";
	private static final String KEY_FACTORY_INSTANCE = "RSA"; 
	
	/**
	 * The public key file may contain PUBLIC_KEY_HEAD and PUBLIC_KEY_TAIL
	 * It does not matter if it does not contain them.
	 * @param pubKeyFile
	 * @return
	 * @throws CertificateException
	 * @throws IOException
	 * @throws Base64DecoderException 
	 * @throws InvalidKeySpecException 
	 * @throws NoSuchAlgorithmException 
	 */
	public static PublicKey getPublicKeyFromFile(String pubKeyFile) 
	throws CertificateException, IOException, NoSuchAlgorithmException, InvalidKeySpecException, Base64DecoderException{	    
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
	
	public static PublicKey getPublicKeyFromContent(String keyString) throws NoSuchAlgorithmException, Base64DecoderException, InvalidKeySpecException {
	    String pubKeyString = keyString;

	    KeyFactory keyFactory = KeyFactory.getInstance(KEY_FACTORY_INSTANCE);
	    byte [] binaryKey = Base64.decode(pubKeyString);

	    KeySpec pubSpec = new RSAPublicKeySpec(createUnsignedBigInt(binaryKey), new BigInteger("65537"));
	    
	    PublicKey pubKey = keyFactory.generatePublic(pubSpec);
	    return pubKey;
	}
	
    public static BigInteger createUnsignedBigInt( byte[] bytes )
    {
        if ( (bytes[0] & 0x80) != 0 )
        {
            byte[] singleZero =
                { 0 };
            //bytes = ByteArrayUtil.concat( singleZero, bytes );
        }
        BigInteger bi = new BigInteger( bytes );
        return bi;
    }
}
