package com.intel.splat.identityservice.tpmapplet;

import java.applet.Applet;
import java.io.File;

import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import javax.swing.*; 

public class TPMSignApplet extends Applet {
	private static final String SIGNCLASS = "com.intel.splat.identityservice.tpm.TPMSign";
    private static final String SIGNLIBRARY = "TPMUtils.jar";
    private static final String SIGNMETHOD = "sign";
    private static final String GETPUBKEYMETHOD = "getPublicKeyContent";
    private static final String HOME = "user.home";
	
	private static Class _signClass = null;
	private static String _userHome = null;
    
	public void init() {
    }  
	
	private static String getUserHome() {
		if(_userHome == null) {
	        try {
	        	_userHome = System.getProperty(HOME);
	            return _userHome;
	        } catch(SecurityException e)  {
	        	JOptionPane.showMessageDialog(null, "can not get user.home property:" + e.getMessage());
	            throw new RuntimeException(e);
	        }
		}
		return _userHome;
	}
	
	private static Class getTPMSignClass() {
        if(_signClass == null) {
			try {
	        	String signLibJarPath = getUserHome() + "/" + SIGNLIBRARY;
	        	File file = new File(signLibJarPath); 
	            URL url = file.toURL();;
	            URLClassLoader loader = new URLClassLoader(new  URL[]  {url});
				_signClass = loader.loadClass(SIGNCLASS);
			} catch (MalformedURLException me) {
				throw new RuntimeException(me);
			} catch (ClassNotFoundException ce) {
				throw new RuntimeException(ce);
			}
        }
        return _signClass;
	}
	
	/**
	 * get the default public key content
	 * @return
	 */
	public String getPublicKeyContent() {
		String pubKeyContent = null;
        try {                
            // create the getPublicKeyContent method
            Method  method = getTPMSignClass().getMethod(GETPUBKEYMETHOD);
            pubKeyContent = (String)method.invoke(getTPMSignClass().newInstance());
            if (pubKeyContent != null) {
            	System.out.println("public key content: " + pubKeyContent);
            } else {
                System.out.println("Can not get the public key content!");
            }
        } catch (Exception e) {
        	e.printStackTrace();
            return null;
        }
        return pubKeyContent;
	}
	
    public String sign(String random, String tpmPass) {     
        String result = null;
        byte[] signature = null;
        try {                
            // create the invoke method and parameters
            Class[] paramTypes = new Class[2];
            paramTypes[0] = byte[].class;
            paramTypes[1] = String.class;
            Method  method = getTPMSignClass().getMethod(SIGNMETHOD, paramTypes);
            
            Object[] args = new Object[2];   
            args[0] = random.getBytes("UTF-8");
            args[1] = tpmPass;
            signature = (byte[])method.invoke(getTPMSignClass().newInstance(), args);
            
            if (signature != null) {
                System.out.println("Sign Successful!");
                result = Base64.encode(signature);
                System.out.println("Sign successful with result: " + result);
            } else {
                System.out.println("Sign failed!");
            }
        } catch (Exception e) {
        	e.printStackTrace();
            return null;
        }
        return result;
    }
    
    public String doSignature(String randomString, String tpmPass) {
		String signedValue = null;
		String randomStringLabel = "Received string:";
		String signedValueLabel = "Signed string:";
		JTextField randomStringField = new JTextField(20);
	    JTextField signedValueField = new JTextField(20);
	    
		randomStringField.setText(randomString);
		signedValueField.setEditable(false);
	    signedValueField.setText("");
	    signedValue = sign(randomString, tpmPass);
	    signedValueField.setText(signedValue);
	    
	    if (signedValue != null) {
	    	Object[] options = { "Continue", "Cancel" };
		    JOptionPane.showOptionDialog(null, new Object[] {
		    		randomStringLabel, randomStringField,
		    		signedValueLabel, signedValueField}, "TPM Signature", 
		    		JOptionPane.OK_CANCEL_OPTION,
		    		JOptionPane.INFORMATION_MESSAGE,
		    		null, options, options[0]);
	    } else {
	    	JOptionPane.showMessageDialog(null, 
	    			"TPM signature fails, please check the TPM setting in your machine", 
	    			"Error", JOptionPane.ERROR_MESSAGE);
	    }
	    
	    return signedValue;
	}
	
    public static void main(String args[]) {
    	TPMSignApplet tpm = new TPMSignApplet();
    	tpm.getPublicKeyContent();
    	tpm.doSignature("1234567", "password");
    }
}
