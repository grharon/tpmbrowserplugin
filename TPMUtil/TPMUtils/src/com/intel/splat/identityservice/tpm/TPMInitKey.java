package com.intel.splat.identityservice.tpm;

import java.io.File;
import java.io.FileOutputStream;
import java.util.Properties;
import java.security.PublicKey;
import java.security.Security;
import java.security.Signature;


import edu.mit.csail.tpmj.TPMConsts;
import edu.mit.csail.tpmj.TPMException;
import edu.mit.csail.tpmj.commands.TPM_ReadPubek;
import edu.mit.csail.tpmj.commands.TPM_ReadPubekOutput;
import edu.mit.csail.tpmj.commands.TPM_TakeOwnership;
import edu.mit.csail.tpmj.commands.TPM_TakeOwnershipOutput;
import edu.mit.csail.tpmj.drivers.TPMDriver;
import edu.mit.csail.tpmj.funcs.TPMOIAPSession;
import edu.mit.csail.tpmj.funcs.TPMStorageFuncs;
import edu.mit.csail.tpmj.funcs.TPMUtilityFuncs;
import edu.mit.csail.tpmj.structs.TPM_KEY;
import edu.mit.csail.tpmj.structs.TPM_PUBKEY;
import edu.mit.csail.tpmj.structs.TPM_SECRET;
import edu.mit.csail.tpmj.structs.TPM_STORE_PUBKEY;
import edu.mit.csail.tpmj.tools.special.TPMSelfTest;
import edu.mit.csail.tpmj.util.ByteArrayUtil;
import edu.mit.csail.tpmj.util.CryptoUtil;
import edu.mit.csail.tpmj.util.FileUtil;
import edu.mit.csail.tpmj.util.TPMToolsUtil;

public class TPMInitKey {
	private static final String TPM_KEYWRAPPER_FILE = "intel-tpm.kw";
	private static final String TPM_PUBLICKEY_FILE = "intel-tpm.pub";
	private static final String TPM_CONFIG_FILE = "tpm.conf";
	
	private static String _tpmKeyWrapperFilePath;
	private static String _tpmPublicKeyFilePath;
	private static String _tpmConfigFilePath;
	
    public static void main(String[] args) {
    	Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider ());
    	
    	if(args.length != 2 || 
    			(!args[0].equalsIgnoreCase("windows") &&
    			!args[0].equalsIgnoreCase("vista") &&
    			!args[0].equalsIgnoreCase("windows7")))  {
    		System.out.println("Usage: TPMInitKey <OS(windows/vista/windows7)> <ownerPwd/keyPwd>\n");
    		return;
    	}
		
		String userHome = System.getProperty("user.home");
		_tpmKeyWrapperFilePath = userHome + "/" + TPM_KEYWRAPPER_FILE;
		_tpmPublicKeyFilePath = userHome + "/" + TPM_PUBLICKEY_FILE;
		_tpmConfigFilePath =  userHome + "/" + TPM_CONFIG_FILE;
		
		String ownerPass = args[1];
		
		String srkPass = args[1];
		
		String keyPass = args[1];
		
		try {
	    	Properties properties = new Properties(); 
	    	properties.setProperty("os.name", args[0]);
			properties.save(new FileOutputStream(_tpmConfigFilePath), "TPM configurations");
		
	        // Initialize the TPM driver
			System.getProperties().put("os.name", args[0]);
	        TPMDriver tpmDriver = TPMUtilityFuncs.getTPMDriver();
		
		    takeOwnership(ownerPass, srkPass);
			TPM_KEY keyWrapper = createKey( 
					TPMConsts.TPM_KH_SRK,
		    		srkPass,
		    		keyPass);
		    
		    saveKeyWrapper(keyWrapper);
		    
		    System.out.println("Generate TPM Signing Key index at: " + _tpmKeyWrapperFilePath);
		    
		    savePublicKeyFile(keyWrapper.getPubKey());
		    
		    System.out.println("Corresponding TPM Public Key index at: " + _tpmPublicKeyFilePath);
		    
		    System.out.println("Start sign and verify test for generated key...");
		    
            System.out.println( "\nLoading the key into the TPM ..." );
		    
		    int keyHandle = loadKey(
		    		TPMConsts.TPM_KH_SRK,
		    		srkPass, 
		    		_tpmKeyWrapperFilePath);
		    
            System.out.println( "keyHandle = 0x" + Integer.toHexString( keyHandle ) );
            
            System.out.println( "\nTrying to sign 'Hello World' ... " );

		    byte[] signature = sign("Hello World".getBytes(), keyHandle, keyPass);
		    
            System.out.println( "Signature returned (" + signature.length
                    + " bytes): " + ByteArrayUtil.toHexString( signature ) );
		    
            System.out.println( "\nTrying to verify the signature with PubKey ... " );
            
		    if(verify(signature, "Hello World".getBytes(), _tpmPublicKeyFilePath)) {
		    	System.out.println("\nTest Pass!");
		    } else {
		    	System.out.println("\nTest Fail!");
		    }
		    
		    dropKey(keyHandle);
		    
	        TPMToolsUtil.cleanupTPMDriver();
		} catch (Exception e) {
		    e.printStackTrace();
		}
    }
    
    public static void takeOwnership(
    		String ownerKey, 
    		String srkKey) {
        TPMDriver tpmDriver = TPMUtilityFuncs.getTPMDriver();
        
        TPM_SECRET ownerAuth = TPMToolsUtil.createTPM_SECRETFromPrefixedString( ownerKey ); 
        TPM_SECRET srkAuth = TPMToolsUtil.createTPM_SECRETFromPrefixedString( srkKey );
        
        int keyUsageFlags = 0;
        short keyUsageType = TPMConsts.TPM_KEY_STORAGE;
        
        try {
            TPMSelfTest.doSelfTest();
            
            // create SRK key info

            // if srkAuth is all-zeros or null, then set no authorization needed
            byte authDataUsage = ((srkAuth == null) || srkAuth.equals( TPM_SECRET.NULL ))
                ? (byte) 0
                : (byte) 1;
            TPM_KEY keyInfo = TPMStorageFuncs.createKeyInfo( keyUsageType,
                keyUsageFlags, authDataUsage );

            // encrypt auth data
            TPM_ReadPubek readPubekCmd = new TPM_ReadPubek( TPM_SECRET.NULL );
            TPM_ReadPubekOutput pubekOut = readPubekCmd.execute( TPMUtilityFuncs.getTPMDriver() );
            TPM_PUBKEY pubKey = pubekOut.getPubKey();

            if ( ownerAuth == null )
            {
                ownerAuth = TPM_SECRET.NULL;
            }
            byte[] encOwnerAuth = CryptoUtil.encryptTPM_ES_RSAOAEP_SHA1_MGF1(
                pubKey.getPubKey().getKeyBytes(), ownerAuth.toBytes() );

            if ( srkAuth == null )
            {
                srkAuth = TPM_SECRET.NULL;
            }
            byte[] encSrkAuth = CryptoUtil.encryptTPM_ES_RSAOAEP_SHA1_MGF1(
                pubKey.getPubKey().getKeyBytes(), srkAuth.toBytes() );

            // Take ownership
            TPM_TakeOwnership cmd = new TPM_TakeOwnership( encOwnerAuth,
                encSrkAuth, keyInfo );

            System.out.println( "Executing take ownership command ... " );
            TPM_TakeOwnershipOutput output = (TPM_TakeOwnershipOutput) TPMOIAPSession.executeOIAPSession(
            		TPMUtilityFuncs.getTPMDriver(), cmd, ownerAuth );
            System.out.println( "TakeOwnership finish!" );
        } catch ( TPMException e ) {
            TPMToolsUtil.handleTPMException( e );
            throw new RuntimeException(e);
        } catch ( Exception e ) {
	    	throw new RuntimeException(e);
        }

    }
  
	public static TPM_KEY createKey(int parentKeyHandle, 
			String parentKey, 
			String newKey) {        
        TPM_SECRET keyAuth = TPMToolsUtil.createTPM_SECRETFromPrefixedString( newKey );
        
        TPM_SECRET parentAuth = TPMToolsUtil.createTPM_SECRETFromPrefixedString( parentKey );
        
	    try {
	    	TPM_KEY myKey = TPMStorageFuncs.TPM_CreateWrapKey( parentKeyHandle,
                parentAuth, keyAuth, null, TPMConsts.TPM_KEY_LEGACY, 0 );
	        return myKey;
        } catch ( TPMException e ) {
            TPMToolsUtil.handleTPMException( e );
            throw new RuntimeException(e);
        } catch ( Exception e ) {
	    	throw new RuntimeException(e);
        }
	}
	
	public static int loadKey(int parentKeyHandle, 
			String parentAuth, 
			String myKeyFilePath) {
	    try {
            int keyHandle = TPMStorageFuncs.TPM_LoadKey( parentKeyHandle, 
            		FileUtil.readIntoByteArray(myKeyFilePath),
            		TPMToolsUtil.createTPM_SECRETFromPrefixedString( parentAuth ) );
            return keyHandle;            
        } catch ( TPMException e ) {
            TPMToolsUtil.handleTPMException( e );
            throw new RuntimeException(e);
        } catch ( Exception e ) {
	    	throw new RuntimeException(e);
        }
	}
	
	public static void dropKey(int keyId) {
	    try {
	    	TPMStorageFuncs.TPM_EvictKey( keyId );
	    } catch (Exception e) {
	        throw new RuntimeException(e);
	    }
	}
	
	public static byte[] sign(byte[] data, 
			int keyHandle, 
			String keyAuth) {
	    try {
            byte[] sig = TPMStorageFuncs.TPM_SignSHA1OfData( keyHandle,
                    data, 
                    TPMToolsUtil.createTPM_SECRETFromPrefixedString( keyAuth ));
	        
	        return sig;
        } catch ( TPMException e ) {
            TPMToolsUtil.handleTPMException( e );
            throw new RuntimeException(e);
        } catch ( Exception e ) {
	    	throw new RuntimeException(e);
        }
	}
	
	public static boolean verify(byte[] signature, byte data[], String pubKeyPath) {
	    try {
	    	PublicKey pubKey = RSAKeyUtils.getPublicKeyFromFile(pubKeyPath);
	        Signature sig_v = Signature.getInstance("SHA1withRSA");
	        sig_v.initVerify(pubKey);
	        sig_v.update(data);
	        return sig_v.verify(signature);
	    } catch (Exception e) {
	        throw new RuntimeException(e);
	    }
	}
	
	public static void savePublicKeyFile(TPM_STORE_PUBKEY pubKey) {
		File pubKeyFile = new File(_tpmPublicKeyFilePath);
		try {
			FileOutputStream outStream = new FileOutputStream(pubKeyFile);
			outStream.write("-----BEGIN PUBLIC KEY-----\n".getBytes());
			outStream.write(Base64.encode(pubKey.getKeyBytes()).getBytes());
			outStream.write("\n-----END PUBLIC KEY-----".getBytes());
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
	}
	
	public static void saveKeyWrapper(TPM_KEY keyWrapper) {
		File keyWrapperFile = new File(_tpmKeyWrapperFilePath);
		try {
			FileOutputStream outStream = new FileOutputStream(keyWrapperFile);
			outStream.write(keyWrapper.toBytes());
			outStream.close();
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
	}
}
