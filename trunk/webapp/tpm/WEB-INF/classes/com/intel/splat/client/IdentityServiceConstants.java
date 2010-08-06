package com.intel.splat.client;

public class IdentityServiceConstants {
	///////////////////////////////// UI Constants /////////////////////////////////
	public static final String SERVICE = "identityconfig";
	public static final String TARGETATTRIBUTESPACCOUNT = "SPAccountAttribute";
	////////// upload and download file ////////
	public static final String TRANSFER_FILE_CATEGORY = "category";
	public static final String TRANSFER_FILE_PUBKEY = "PubKeyFile";		// X509 public key file
	public static final String TRANSFER_FILE_X509KEY = "X509KeyFile";	// X509 public/private key file
	public static final String TRANSFER_FILE_KEYSTORE = "KeyStoreFile";
	public static final String TRANSFER_FILE_PDPPOLICY = "PDPPolicy";
	public static final String TRANSFER_FILE_AD = "ActiveDirectory";
	public static final String TRANSFER_FILE_JAR = "JarFile";
	public static final String TRANSFER_FILE_NAME = "TransferFileName";
	public static final String TRANSFER_DESTINATION = "IdpId";
	public static final String INTERNAL_DEMO_PUBKEY = "intel_demo_cert.crt";
	public static final String INTERNAL_DEMO_PRIVATEKEY = "intel_demo_key.key";
	
	public static final String DEFAULT_SUCCESS_RETURN = "Upload Success";
	public static final String DEFAULT_FAIL_RETURN = "Upload Fail";
	public static final String UPLOAD_FILE_PARAM = "param";
	public static final String SEND_UPLOAD_FILE_CONTENT_BACK = "SendContentBack";
	public static final String RESPONSE_START_TAG = "<pre>";
	public static final String RESPONSE_END_TAG = "</pre>";
	
	public static final String DOWNLOAD_SERVLET = "downloadFile";
	public static final String UPLOAD_SERVLET = "uploadFile";
	public static final String DOWNLOAD_ID = "__download";
	
	///////////// widget ////////////////////
	public static final String VIEWPORT = "viewPort";
	public static final String DATASTORE_SAVE_BUTTON = "DataStoreSaveButton";
	public static final String DATASTORE_REMOVE_BUTTON = "DataStoreRemoveButton";
	
	public static final String IDP_SAVE_BUTTON = "IDPSaveButton";
	public static final String PROVISIONING_PLUGIN_SAVE_BUTTON = "ProvisioningPluginSaveButton"; 
	public static final String SP_ENTITY_SAVE_BUTTON = "SPEntitySaveButton";
	public static final String PDP_SAVE_BUTTON = "PDPSaveButton";
	
	public static final String USER_PROVISIONING_PLUGIN_FOR_FEDERATEDSP = 
									"UserProvisioningPluginInFederatedSP";
	public static final String POLICYFILELIST_FOR_PDP = "PolicyFileListInPDP";
	///////////// Model ID which is the key of the Registry /////
	public static final String IDPSUMMARY = "IdpSummary";
	public static final String IDPMAP = "IdpMap";
	
	public static final String DATASTORECONFIG = "DataStoreConfig";
	public static final String SELECTED_TREEFOLDER = "SelectedFolder";
	
	public static final String PROVISIONINGPLUGIN = "ProvisioningPluginConfig";
	
	public static final String XACMLPDPCONFIG = "XACMLPDPConfig";
	public static final String PDP_POLICY_FILELIST = "PDPPolicyFileList";
	public static final String PROVISION_JAR_FILELIST = "ProvisionJarFileList";
	
	public static final String WAITING_OBJECTS_NUM = "WaitingObjectsNum";
	public static final String NEXT_EVENT_AFTER_FINISHED = "NextEventAfterFinished";

	public static final String XACML_POLICY_CONFIG = "XACMLPolicyConfig";
	///////////////////////////////// Server Constants /////////////////////////
	public static final String LOGIN_GOTO_ATTR = "goto";
	public static final String PREVIOUS_ATTR = "previous";
	public static final String DEPLOYMENT_URL = "identityservice";
	public static final String LOGIN_SUCCESS = "LoginSuccess";
	public static final String IDP_NAME = "idp";
	public static final String FORWARD = "forward";
	public static final String SPENTITY_NAME = "SpEntity";
	public static final String RANDOMSTR = "RandomString";
	public static final String SIGNEDVALUE = "SignedValue";
	public static final String USETPM = "useTPM";
	public static final String OPENID_LOGIN = "openIdLogin";
	public static final String OPENID_REALM = "openIdRealm";
	public static final String UNAUTH = "unAuth";
	public static final String CLIENTTPMPUBKEY = "ClientTPMPubKeyFile";
	public static final String AUTHENTICATION = "authn";
	public static final String TPMPUBKEYCONTENT = "TPMPubKeyContent";
    public static final String AUTHN_ACS = "acs";
    public static final String LOGOUT_ACS = "lacs";
    public static final String DATA_CCS="dcs";
	
	// URL segment definitions
	public static String PACKAGE = "package";
	// profile
    public static String SAML2_SSO_PROFILE = "saml2";
    public static String INTERNAL_SSO_PROFILE = "internal";
    public static String OPENID_AUTH2_PROTOCOL = "openid2";
    
    public static String SAML_PROFILE = "SAML2";
    public static String OPENID_PROFILE = "OPENID";
    
    public static String OPENID_SP_NAME = "OpenID-RP";
    
    // binding
    public static String SPSSOREDIRECT = "SpSSORedirect";
    public static String IDPSSOREDIRECT = "IdpSSORedirect";
    public static String SSOPOST = "SSOPost";
    public static String SSOSOAP = "SSOSoap";
    public static String SLOREDIRECT = "SLORedirect";
    //////////////////////// Context Listener parameters /////////////
    public static String WEB_INF_DIR = "WEB_INF_DIR";
    
    //////////////////////// Provisioning plugin dir    //////////////
    public static final String PROVISIONING_PLUGIN_DIR = "provisioning-plugin";
    
    /////////////////////// XACML ////////////////////////////
    public static final String SUBJECT_ID = "urn:oasis:names:tc:xacml:1.0:subject:subject-id";
    public static final String SUBJECT_GROUP = "urn:oasis:names:tc:xacml:1.0:subject:subject-group";
    public static final String SUBJECT_ROLE = "urn:oasis:names:tc:xacml:1.0:subject:subject-role";
    
    public static final String RESOURCE_ID = "urn:oasis:names:tc:xacml:1.0:resource:resource-id";
    public static final String ACTION_ID = "urn:oasis:names:tc:xacml:1.0:action:action-id";
    
    public static final String PDP_POLICY_DIR = "policy";
    public static final String PDP_LOG_DIR = "log";
    
    public static final String FILE_DATA_STORE_ENGINE = "an.xacml.adapter.file.XMLFileDataStore";
    public static final String POLICY_DIRECTORY = "@IDENTITY_SERVICE_CONFIG_DIR@/pdp1/policy";
    public static final String POLICY_FILE_PATTERN = ".*xml";
    
    public static final String DEFAULT_CONTEXT_FACTORY = 
    	"an.xacml.engine.DefaultContextFactory";
    public static final String DEFAULT_DATASTORE_POLICY_RESOLVER = 
    	"an.xacml.engine.DefaultDataStorePolicyResolver";
    
    public static final String[] MULTI_POLICY_COMBINE_ALG = {
    	"an:multiple-policies-deny-overrides",
		"an:multiple-policies-permit-overrides", 
		"an:multiple-policies-first-applicable", 
		"an:multiple-policies-only-one-applicable"
	};
    
    ////////////////////// XACML Policy //////////////////
    public static final String XACML_POLICY_NAMESPACE_PREFIX = 
    	"urn:oasis:names:tc:xacml:2.0:pdp1:";
    public static final String XACML_EFFECT_DENY = "Deny";
    public static final String XACML_EFFECT_PERMIT = "Permit";
    
    public static final String XACML_ACTION_POST = "POST";
    public static final String XACML_ACTION_GET = "GET";
    public static final String XACML_ACTION_PUT = "PUT";
    public static final String XACML_ACTION_DELETE = "DELETE";
    
    // Equal function IDs
    public static final String XACML_MATCH_FUNCTION_ANYURL_EQUAL 
    	= "urn:oasis:names:tc:xacml:1.0:function:anyURI-equal";
    public static final String XACML_MATCH_FUNCTION_STRING_EQUAL
    	= "urn:oasis:names:tc:xacml:1.0:function:string-equal";
    
    // Comparison function IDs
    public static final String XACML_MATCH_FUNCTION_DATE_GREATER_THAN
    	= "urn:oasis:names:tc:xacml:1.0:function:date-greater-than";
    public static final String XACML_MATCH_FUNCTION_DATE_GREATER_THAN_OR_EQUAL
    	= "urn:oasis:names:tc:xacml:1.0:function:date-greater-than-or-equal";
    public static final String XACML_MATCH_FUNCTION_DATE_LESS_THAN
    	= "urn:oasis:names:tc:xacml:1.0:function:date-less-than";
    public static final String XACML_MATCH_FUNCTION_DATE_LESS_THAN_OR_EQUAL
    	= "urn:oasis:names:tc:xacml:1.0:function:date-less-than-or-equal";
    
    // Match function IDs
    public static final String XACML_MATCH_FUNCTION_REGEX 
    	= "urn:oasis:names:tc:xacml:1.0:function:regexp-string-match"; 
    
    ///////////////////// Salesforce attribute name //////////
    public static final String SALESFORCE_SAML2_ATTR_FORMAT_UNSPECIFIED =
    	"urnasis:names:tc:SAML:2.0:attrname-format:unspecified";
    public static final String SALESFORCE_SAML2_ATTR_FORMAT_URI =
    	"urn:smileysurprised:asis:names:tc:SAML:2.0:attrname-format:uri";
}
