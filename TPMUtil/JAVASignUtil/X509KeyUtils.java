package com.intel.splat.identityservice.utils;

import org.opensaml.xml.util.Base64;
import org.bouncycastle.jce.provider.BouncyCastleProvider;

//import com.intel.splat.client.model.identityservice.config.general.OCSPConfig;
//import com.intel.splat.identityservice.configuration.ConfigurationManager;
//import com.intel.splat.identityservice.general.CertValidationConfManager;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.StringReader;
import java.security.InvalidAlgorithmParameterException;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.PrivateKey;
import java.security.cert.*;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.X509EncodedKeySpec;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
/**
 * 
 * Get the PublicKey or PrivateKey from key file or key string
 *
 */
public class X509KeyUtils {
	private static final String CRT_KEY_HEAD = "-----BEGIN CERTIFICATE-----";
	private static final String CRT_KEY_TAIL = "-----END CERTIFICATE-----";
	private static final String PUBLIC_KEY_HEAD = "-----BEGIN PUBLIC KEY-----";
	private static final String PUBLIC_KEY_TAIL = "-----END PUBLIC KEY-----";	
	private static final String PRIVATE_KEY_HEAD = "-----BEGIN RSA PRIVATE KEY-----";
	private static final String PRIVATE_KEY_TAIL = "-----END RSA PRIVATE KEY-----";
	private static final String X509KEY_INSTANCE = "X.509";
	private static final String KEY_FACTORY_INSTANCE = "RSA"; 
	
	/**
	 * check whether one cert has been revoked by the CRL
	 * default is not revoked (valid certificate)
	 * @param cert
	 * @return
	 */
	/*public static boolean isCRLRevoked(X509Certificate cert) {
		CRL crl = CertValidationConfManager.getInstance().getCRL();
		if (crl != null) {
			X509CRLEntry crlEntry = ((X509CRL)crl).getRevokedCertificate(cert.getSerialNumber());
			return (crlEntry != null);
		}
		return false;
	}
	
	public static boolean checkOCSP(X509Certificate targetCert) {
		OCSPConfig ocspConf = CertValidationConfManager.getInstance().getOCSPConf();
		if (ocspConf == null || !ocspConf.isOCSPEnabled())
			return true;

		X509Certificate rootCert;
		try {
			String issuerCert = ocspConf.getIssuerCert();
			if (ConfigurationManager.getConfigDirPath() != null)
				issuerCert = ConfigurationManager.getConfigDirPath() + issuerCert;
			rootCert = X509KeyUtils.getCertificateFromFile(issuerCert);
		    List list = Arrays.asList(new X509Certificate[] {
		    		targetCert, rootCert});
		    return X509KeyUtils.checkOSCP(list, rootCert);
		} catch (CertificateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}*/

	/**
	 * certList is a certificate path, with the questioned certificate at the beginning of
	 * this list. The trustedCert is the trustedAnchor in the certList. All the certificates
	 * above the trustedAnchor (includes the trustedAnchor itself) will not be checked, thus 
	 * the trustedCert must exist in the certPath. Please see OCSP rfc for details.
	 * @param certList
	 * @param trustedCert
	 * @return
	 */
	/*public static boolean checkOSCP(List certList, X509Certificate trustedCert) {
		
		OCSPConfig ocspConf = CertValidationConfManager.getInstance().getOCSPConf();
		if (ocspConf == null || !ocspConf.isOCSPEnabled())
			return true;
		
        try {
            CertificateFactory factory = CertificateFactory.getInstance("X.509");
            CertPath certPath = factory.generateCertPath(certList);
            CertPathValidator certPathValidator = CertPathValidator.getInstance("PKIX");
            TrustAnchor anchor = new TrustAnchor(trustedCert, null);
            PKIXParameters params = new PKIXParameters(Collections.singleton(anchor));
            params.setRevocationEnabled(true);
            java.security.Security.setProperty("ocsp.enable", "true");
            if (ocspConf.getOcspResponderURL() != null)
            	java.security.Security.setProperty("ocsp.responderURL", ocspConf.getOcspResponderURL());
            if (ocspConf.getOcspResponderCertSubjectName() != null)
            	java.security.Security.setProperty("ocsp.responderCertSubjectName", ocspConf.getOcspResponderCertSubjectName());
            if (ocspConf.getOcspResponderCertIssuerName() != null)
            	java.security.Security.setProperty("ocsp.responderCertIssuerName", ocspConf.getOcspResponderCertIssuerName());
            if (ocspConf.getOcspResponderCertSerialNumber() != null)
            	java.security.Security.setProperty("ocsp.responderCertSerialNumber", ocspConf.getOcspResponderCertSerialNumber());

            // Validate and obtain results
            try {
                PKIXCertPathValidatorResult result =
                        (PKIXCertPathValidatorResult) certPathValidator.validate(certPath, params);

            } catch (CertPathValidatorException exception) {
            	
            	return false;
            }

        } catch (NoSuchAlgorithmException e) {
        	return false;
        } catch (InvalidAlgorithmParameterException e) {
        	return false;
        } catch (CertificateException e) {
        	return false;
        }
        return true;
    }*/

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
		X509EncodedKeySpec pubSpec;
	    byte [] binaryKey;
	    PublicKey pubKey = null;
	    String pubKeyString = keyString;

		try {
	        keyFactory = KeyFactory.getInstance(KEY_FACTORY_INSTANCE);
	        binaryKey = Base64.decode(pubKeyString);
	        pubSpec = new X509EncodedKeySpec(binaryKey);
	        pubKey = keyFactory.generatePublic(pubSpec);
	    } catch (NoSuchAlgorithmException nsae) {
	    	nsae.printStackTrace();
	    } catch (InvalidKeySpecException ikse) {
	    	ikse.printStackTrace();
	    }
	    return pubKey;
	}
	
	public static PrivateKey getPrivateKeyFromFile(String privateKeyFile) 
	throws IOException{	    
	    return getPrivateKeyFromContent(getPrivateKeyContentFromFile(privateKeyFile));
	}
	/**
	 * The private key file must contain PRIVATE_KEY_HEAD and PRIVATE_KEY_TAIL
	 * @param privateKeyFile
	 * @return
	 * @throws IOException
	 */
	private static String getPrivateKeyContentFromFile(String privateKeyFile) 
	throws IOException {
		BufferedReader in = new BufferedReader(new FileReader(privateKeyFile));
		String privateKeyString = parsePrivateKey(in);
	    in.close();
	    
	    return privateKeyString;
	}
	/**
	 * The private key string must contain PRIVATE_KEY_HEAD and PRIVATE_KEY_TAIL
	 * @param privateKeyMem
	 * @return
	 * @throws IOException
	 */
	public static PrivateKey getPrivateKeyContentFromMemory(String privateKeyMem) 
	throws IOException {
		BufferedReader in = new BufferedReader(new StringReader(privateKeyMem));
		String privateKeyString = parsePrivateKey(in);
	    in.close();
	    PrivateKey privateKey = getPrivateKeyFromContent(privateKeyString);
	    return privateKey;
	}
	
	private static String parsePrivateKey(BufferedReader in) 
	throws IOException{
		String line;
		StringBuffer privateKeyCont = new StringBuffer("");
	    
	    line = in.readLine();
	    while (line != null) {
	    	privateKeyCont.append(line).append("\r\n");
	    	line = in.readLine();
	    }
	    
	    String privateKeyString = privateKeyCont.toString();
	    privateKeyString = privateKeyString.replace(PRIVATE_KEY_HEAD, "");
	    privateKeyString = privateKeyString.replace(PRIVATE_KEY_TAIL, "");
	    privateKeyString = privateKeyString.trim();
	    
	    return privateKeyString;
	}
	/**
	 * The private key string must not contain PRIVATE_KEY_HEAD and PRIVATE_KEY_TAIL
	 * @param keyString
	 * @return
	 */
	public static PrivateKey getPrivateKeyFromContent(String keyString) {
		KeyFactory keyFactory;
	    PKCS8EncodedKeySpec privSpec;
	    byte [] binaryKey;
	    PrivateKey privateKey = null;
	    String privateKeyContent = keyString;

		try {
	    	BouncyCastleProvider provider = new BouncyCastleProvider();
	        keyFactory = KeyFactory.getInstance(KEY_FACTORY_INSTANCE, provider);
	        binaryKey = Base64.decode(privateKeyContent);
	        privSpec = new PKCS8EncodedKeySpec(binaryKey);
	        privateKey = keyFactory.generatePrivate(privSpec);
	    } catch (NoSuchAlgorithmException nsae) {
	    	nsae.printStackTrace();
	    } catch (InvalidKeySpecException ikse) {
	    	ikse.printStackTrace();
	    }
	    return privateKey;
	}
}
