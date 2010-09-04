package com.intel.splat.identityservice.tpm;

import java.io.UnsupportedEncodingException;

import javax.swing.JOptionPane;
import javax.swing.JTextField;

public class TPMSign {
	private static final String TPM_KEYWRAPPER_FILE = "intel_demo_key.key";
	// private static final String TPM_KEYWRAPPER_FILE = "intel-tpm.kw";
	private static final String TPM_PUBLICKEY_FILE = "intel_demo_cert.crt";

	// private static final String TPM_PUBLICKEY_FILE = "intel-tpm.pub";
	// private static final String TPM_CONFIG_FILE = "tpm.conf";

	public static String getPublicKeyContent() {
		// System.getProperties().setProperty("user.home",
		// "d:/TPM/TPM emluator");// �趨һ��user.home
		String userHome = System.getProperty("user.home");
		String tpmPubKeyFilePath = userHome + "/" + TPM_PUBLICKEY_FILE;
		try {
			String pubKeyContent = X509KeyUtils
					.getPublicKeyContentFromFile(tpmPubKeyFilePath);
			System.out.println("public key content: " + pubKeyContent);
			return pubKeyContent;
		} catch (Exception e) {
			System.err.println("Can not get the public key content!");
			throw new RuntimeException(e);
		}
	}

	/**
	 * Sign with the specified private key file
	 * 
	 * @param data
	 *            - the data needs to be signed
	 * @param keyAuth
	 *            - tpm password
	 * @return
	 */
	public static byte[] sign(byte[] data, String keyAuth) {
		String userHome = System.getProperty("user.home");
		String tpmKeyWrapperFilePath = userHome + "/" + TPM_KEYWRAPPER_FILE;
		// String tpmConfigFilePath = userHome + "/" + TPM_CONFIG_FILE;
		// String keystorepath = userHome;
		// String storepass = "pass";

		/*
		 * try {
		 * // Initialize the TPM driver
		 * Properties property = new Properties();
		 * property.load(new FileInputStream(tpmConfigFilePath));
		 * if(!property.containsKey("os.name")) {
		 * throw new RuntimeException("Incorrect TPM platform OS setting.");
		 * }
		 * System.getProperties().put("os.name", property.get("os.name"));
		 * 
		 * TPMDriver tpmDriver = TPMUtilityFuncs.getTPMDriver();
		 * 
		 * int keyHandle = TPMInitKey.loadKey(TPMConsts.TPM_KH_SRK, keyAuth,
		 * tpmKeyWrapperFilePath);
		 * byte[] signature = TPMInitKey.sign(data, keyHandle, keyAuth);
		 */
		byte[] signature = DigitalSignatureUtils.sign(data,
				tpmKeyWrapperFilePath);

		// TPMInitKey.dropKey(keyHandle);

		return signature;
		/*
		 * } catch (Exception e) {
		 * throw new RuntimeException(e);
		 * } finally {
		 * TPMUtilityFuncs.cleanupTPMDriver();
		 * }
		 */
	}

	private static String sign(String random, String tpmPass) {
		String result = null;
		byte[] input = null;
		try {
			input = random.getBytes("UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return null;
		}
		byte[] signature = sign(input, tpmPass);
		if (signature != null) {
			System.out.println("Sign Successful!");
			result = Base64.encode(signature);
			System.out.println("Sign successful with result: " + result);
		} else {
			System.out.println("Sign failed!");
		}
		return result;
	}

	public static String doSignature(String randomString, String tpmPass) {
		String signedValue = "";
		String randomStringLabel = "Received string:";
		String signedValueLabel = "Signed string:";
		JTextField randomStringField = new JTextField(20);
		JTextField signedValueField = new JTextField(20);

		randomStringField.setText(randomString);
		signedValueField.setEditable(false);
		signedValue = sign(randomString, tpmPass);
		signedValueField.setText(signedValue);

		if (signedValue != null) {
			Object[] options = { "Continue" };
			JOptionPane.showOptionDialog(null, new Object[] {
					randomStringLabel, randomStringField, signedValueLabel,
					signedValueField }, "TPM Signature", JOptionPane.OK_OPTION,
					JOptionPane.INFORMATION_MESSAGE, null, options, options[0]);
		} else {
			JOptionPane
					.showMessageDialog(
							null,
							"TPM signature fails, please check the TPM setting in your machine",
							"Error", JOptionPane.ERROR_MESSAGE);
		}

		return signedValue;
	}

	public static void main(String args[]) {
		getPublicKeyContent();
		doSignature("1234567", "password");
	}
}
