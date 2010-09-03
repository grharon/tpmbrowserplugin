package com.intel.splat.identityservice.tpm;

import java.io.FileInputStream;
import java.io.UnsupportedEncodingException;
import java.util.Properties;

import javax.swing.JOptionPane;
import javax.swing.JTextField;

import edu.mit.csail.tpmj.TPMConsts;
import edu.mit.csail.tpmj.drivers.TPMDriver;
import edu.mit.csail.tpmj.funcs.TPMUtilityFuncs;

public class TPMSign {
	private static final String TPM_KEYWRAPPER_FILE = "intel_demo_key.key";
	private static final String TPM_PUBLICKEY_FILE = "intel_demo_cert.crt";
	private static final String TPM_CONFIG_FILE = "tpm.conf";

	public static String getPublicKeyContent() {
		String userHome = System.getProperty("user.home");
		String tpmPubKeyFilePath = userHome + "/" + TPM_PUBLICKEY_FILE;
		try {
			String pubKeyContent = RSAKeyUtils
					.getPublicKeyContentFromFile(tpmPubKeyFilePath);
			System.out.println("public key content: " + pubKeyContent);
			return pubKeyContent;
		} catch (Exception e) {
			System.out.println("Can not get the public key content!");
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
		String tpmConfigFilePath = userHome + "/" + TPM_CONFIG_FILE;

		try {
			// Initialize the TPM driver
			Properties property = new Properties();
			property.load(new FileInputStream(tpmConfigFilePath));
			if (!property.containsKey("os.name")) {
				throw new RuntimeException("Incorrect TPM platform OS setting.");
			}
			System.getProperties().put("os.name", property.get("os.name"));

			TPMDriver tpmDriver = TPMUtilityFuncs.getTPMDriver();

			int keyHandle = TPMInitKey.loadKey(TPMConsts.TPM_KH_SRK, keyAuth,
					tpmKeyWrapperFilePath);

			byte[] signature = TPMInitKey.sign(data, keyHandle, keyAuth);

			TPMInitKey.dropKey(keyHandle);

			return signature;
		} catch (Exception e) {
			throw new RuntimeException(e);
		} finally {
			TPMUtilityFuncs.cleanupTPMDriver();
		}
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
					randomStringLabel, randomStringField, signedValueLabel,
					signedValueField }, "TPM Signature",
					JOptionPane.OK_CANCEL_OPTION,
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
		TPMSign.getPublicKeyContent();
		TPMSign.doSignature("1234567", "password");
	}
}
