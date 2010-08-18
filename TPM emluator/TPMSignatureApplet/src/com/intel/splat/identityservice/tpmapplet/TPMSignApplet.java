// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 
// Source File Name:   TPMSignApplet.java

package com.intel.splat.identityservice.tpmapplet;

import java.applet.Applet;
import java.io.*;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
import javax.swing.JOptionPane;
import javax.swing.JTextField;

// Referenced classes of package com.intel.splat.identityservice.tpmapplet:
//			Base64

public class TPMSignApplet
{

	private static final String SIGNCLASS = "com.intel.splat.identityservice.tpm.TPMSign";
	private static final String SIGNLIBRARY = "TPMUtils.jar";
	private static final String SIGNMETHOD = "sign";
	private static final String PRIVATEKEYFILE = "intel-tpm.kw";
	private static final String PUBLICKEYFILE = "intel-tpm.pub";
	private static final String CPNAME = "java.class.path";
	private static final String LIBPATH = "java.library.path";
	private static final String SEPARATOR = "path.separator";
	private static final String HOME = "user.home";
	private static final String JAVADLL = "javaddl";
	private static final String PUBKEY_HEADER = "-----BEGIN PUBLIC KEY-----";
	private static final String PUBKEY_TAIL = "-----END PUBLIC KEY-----";
	private static Class _sign = null;
	private String _random;
	private String _tpmPass;
	private String _signedValue;
	private JTextField _randomStringField;
	private JTextField _signedValueField;

	public TPMSignApplet()
	{
		_randomStringField = new JTextField(20);
		_signedValueField = new JTextField(20);
	}

	/*public void init()
	{
		_random = getParameter("randomStr");
		_tpmPass = null;
		System.out.println((new StringBuilder("###")).append(_random).toString());
	}有错*/

	private String getUserHome()
	{
		String userHome = null;
		try
		{
			System.getProperties().setProperty("user.home", "d:\\TPM emluator");//设定一个user.home
			userHome = System.getProperty("user.home");
			System.out.println((new StringBuilder("===")).append(userHome).toString());
		}
		catch (SecurityException se)
		{
			JOptionPane.showMessageDialog(null, (new StringBuilder("can not get user.home property:")).append(se.getMessage()).toString());
			System.out.println((new StringBuilder("can not get user.home property: ")).append(se.getMessage()).toString());
			return null;
		}
		return userHome;
	}

	public String getPublicKeyContent()//publickey存放位置？
	{
		String publicKeyContent = null;
		System.out.println("getPublicKeyContent---");
		String userHome = getUserHome();
		if (!userHome.endsWith("/") && !userHome.endsWith("\\"))
			userHome = (new StringBuilder(String.valueOf(userHome))).append("/").toString();
		try
		{
			BufferedReader in = new BufferedReader(new FileReader((new StringBuilder(String.valueOf(userHome))).append("intel-tpm.pub").toString()));
			StringBuffer keyBuffer = new StringBuffer("");
			for (String line = in.readLine(); line != null; line = in.readLine())
				keyBuffer.append(line).append("\r\n");

			in.close();
			publicKeyContent = keyBuffer.toString();
			publicKeyContent = publicKeyContent.replace("-----BEGIN PUBLIC KEY-----", "");
			publicKeyContent = publicKeyContent.replace("-----END PUBLIC KEY-----", "");
			publicKeyContent = publicKeyContent.trim();
			System.out.println((new StringBuilder("public key content: ")).append(publicKeyContent).toString());
		}
		catch (Exception e)
		{
			System.out.println((new StringBuilder("Can not get the public key content: ")).append(e.getMessage()).toString());
		}
		return publicKeyContent;
	}

	public String sign(String random, String tpmPass)//重点
	{
		if (random != null)
		{
			System.out.println((new StringBuilder("***")).append(random).toString());
			_random = random;
			_tpmPass = tpmPass;
		}
		String result = null;
		String userHome = getUserHome();
		byte signature[] = (byte[])null;
		if (userHome != null)
			try
			{
				String cp = System.getProperty("java.class.path");
				if (!cp.contains(userHome))
				{
					cp = (new StringBuilder(String.valueOf(cp))).append(System.getProperty("path.separator")).append(System.getProperty("user.home")).append(System.getProperty("path.separator")).append(".").toString();
					System.setProperty("java.class.path", cp);
				}
				System.out.println((new StringBuilder("ClassPath: ")).append(System.getProperty("java.class.path")).toString());
				String signLibJarPath = (new StringBuilder(String.valueOf(userHome))).append("/").append("TPMUtils.jar").toString();
				if (_sign == null)
				{
					File file = new File(signLibJarPath);
					URL url = file.toURL();
					URLClassLoader loader = new URLClassLoader(new URL[] {
						url
					});
					_sign = loader.loadClass("com.intel.splat.identityservice.tpm.TPMSign");
				}
				Class paramTypes[] = new Class[2];
				paramTypes[0] = Class.forName("[B");
				paramTypes[1] = Class.forName("java.lang.String");
				//paramTypes[1] = java/lang/String;
				Method method = _sign.getMethod("sign", paramTypes);
				Object args[] = new Object[2];
				args[0] = _random.getBytes("UTF-8");
				args[1] = _tpmPass;
				signature = (byte[])method.invoke(_sign.newInstance(), args);
				if (signature != null)
				{
					System.out.println("Sign Successful!");
					result = Base64.encode(signature);
					System.out.println((new StringBuilder("Sign result: ")).append(result).toString());
				} else
				{
					System.out.println("Sign Failed!");
				}
			}
			catch (Exception e)
			{
				e.printStackTrace();
				System.out.println((new StringBuilder("Exception: ")).append(e.getMessage()).toString());
				return null;
			}
		else
			System.out.println("can not find user.home property");
		return result;
	}

	public String doSignature(String randomString, String tpmPass)
	{
		System.out.println((new StringBuilder("RandomString: ")).append(randomString).toString());
		System.out.println((new StringBuilder("TPMPass: ")).append(tpmPass).toString());
		String signedValue = null;
		String randomStringLabel = "Received string:";
		String signedValueLabel = "Signed string:";
		_randomStringField.setText(randomString);
		_tpmPass = tpmPass;
		_signedValueField.setEditable(false);
		_signedValueField.setText("");
		_signedValue = sign(_randomStringField.getText(), _tpmPass);//调用sign函数
		_signedValueField.setText(_signedValue);
		if (_signedValue != null)
		{
			Object options[] = {
				"Continue", "Cancel"
			};
			int response = JOptionPane.showOptionDialog(null, ((Object) (new Object[] {
				randomStringLabel, _randomStringField, signedValueLabel, _signedValueField
			})), "TPM Signature", 2, 1, null, options, options[0]);
			if (response == 0)
				signedValue = _signedValueField.getText();
		} else
		{
			JOptionPane.showMessageDialog(null, "TPM signature fails, please check the TPM setting in your machine", "Error", 0);
		}
		return signedValue;
	}

	public static void main(String args[])
	{
		TPMSignApplet tpm = new TPMSignApplet();
		tpm.doSignature("1234567", "password");
	}
}

