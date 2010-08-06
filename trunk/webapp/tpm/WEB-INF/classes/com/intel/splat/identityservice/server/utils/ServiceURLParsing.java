package com.intel.splat.identityservice.server.utils;

import javax.servlet.http.HttpServletRequest;

import com.intel.splat.client.IdentityServiceConstants;

public class ServiceURLParsing {
	
	/**
     * Returns idpId embedded in uri.
     * The format is like ".../identityservice/package/idp1/saml2/SSOPost/..."
     * @param uri The uri string.
     * @return the metaAlias embedded in uri or null if not found.
     */
    public static String getIdpID(HttpServletRequest request) {
    	String uri = request.getRequestURI();
        if (uri == null) {
            return null;
        }
        // find the position of PACKAGE
        int length = IdentityServiceConstants.PACKAGE.length();
        int index = uri.indexOf(IdentityServiceConstants.PACKAGE);
        if (index == -1 || 
            index + length == uri.length()) {
            return null;
        }
        // find the next immediate '/' after PACKAGE
        // "index + length" reaches the '/'
        int slashIndex = uri.indexOf("/", index + length + 1);
        if (slashIndex == -1) {
        	return null;
        }
        return uri.substring(index + length + 1, slashIndex);
    }
    
    public static String getProfileType(HttpServletRequest request) {
    	String uri = request.getRequestURI();
        if (uri == null) {
            return null;
        }
        // find the position of PACKAGE
        int length = IdentityServiceConstants.PACKAGE.length();
        int index = uri.indexOf(IdentityServiceConstants.PACKAGE);
        if (index == -1 || 
            index + length == uri.length()) {
            return null;
        }
        // find the slash after idpXX
        int slashAfterIdp = uri.indexOf("/", index + length + 1);
        if (slashAfterIdp == -1) {
        	return null;
        }
        // find the profile type
        int slashIndex = uri.indexOf("/", slashAfterIdp + 1);
        if (slashIndex == -1) {
        	return null;
        }
        return uri.substring(slashAfterIdp + 1, slashIndex);
    }
    
    public static String getBindingType(HttpServletRequest request) {
    	String uri = request.getRequestURI();
        if (uri == null) {
            return null;
        }
        // find the position of PACKAGE
        int length = IdentityServiceConstants.PACKAGE.length();
        int index = uri.indexOf(IdentityServiceConstants.PACKAGE);
        if (index == -1 || 
            index + length == uri.length()) {
            return null;
        }
        // find the slash after idpXX
        int slashAfterIdp = uri.indexOf("/", index + length + 1);
        if (slashAfterIdp == -1) {
        	return null;
        }
        // find the profile type
        int slashIndex = uri.indexOf("/", slashAfterIdp + 1);
        if (slashIndex == -1) {
        	return null;
        }
        return uri.substring(slashIndex + 1, uri.length());
    }
    
    public static StringBuffer getAppRootUrl(HttpServletRequest request) {
    	StringBuffer result = new StringBuffer();
        String scheme = request.getScheme();             // http
        String serverName = request.getServerName();     // hostname.com
        int serverPort = request.getServerPort();        // 80
        String contextPath = request.getContextPath();   // /mywebapp
        
        result.append(scheme)
        	  .append("://")
        	  .append(serverName)
        	  .append(":")
        	  .append(serverPort)
        	  .append(contextPath);
        return result ;
    }
    
    public static StringBuffer getIdentityServiceAppliRootUrl(HttpServletRequest request) {
        StringBuffer result = new StringBuffer();
        String scheme = request.getScheme();             // http
        String serverName = request.getServerName();     // hostname.com
        int serverPort = request.getServerPort();        // 80
        String contextPath = request.getContextPath();   // /mywebapp
        String identityservice = IdentityServiceConstants.DEPLOYMENT_URL;
        result.append(scheme)
        	  .append("://")
        	  .append(serverName)
        	  .append(":")
        	  .append(serverPort)
        	  .append(contextPath)
        	  .append("/")
        	  .append(identityservice);
        return result ;
    }
    
    public static String getIdentityServicePortalUrl(HttpServletRequest request, String idpId) {
    	StringBuffer result = ServiceURLParsing.getIdentityServiceAppliRootUrl(request);
    	result.append("/home?idp=");
    	if(idpId != null && !idpId.isEmpty()) {
    		result.append(idpId);
    	} else {
    		result.append(getIdpID(request));
    	}	
		return result.toString();
    }
    
    public static StringBuffer getInternalSsoProfileBaseUrl(HttpServletRequest request, String idpId) {
    	StringBuffer result = ServiceURLParsing.getIdentityServiceAppliRootUrl(request);
    	result.append("/");
    	result.append(IdentityServiceConstants.PACKAGE);
    	result.append("/");   	
    	if(idpId != null && !idpId.isEmpty()) {
    		result.append(idpId);
    	} else {
    		result.append(getIdpID(request));
    	}
    	result.append("/");
    	result.append(IdentityServiceConstants.INTERNAL_SSO_PROFILE);
		return result;
    }
    
    public static String getAuthnUrl(HttpServletRequest request) {
    	StringBuffer rootUrl = getIdentityServiceAppliRootUrl(request);
    	rootUrl.append("/")
    		   .append(IdentityServiceConstants.AUTHENTICATION)
               .append("/login.jsp");
        return rootUrl.toString();
    }
    
    public static String getConfirmationUrl(HttpServletRequest request) {
    	StringBuffer rootUrl = getIdentityServiceAppliRootUrl(request);
    	rootUrl.append("/")
    		   .append(IdentityServiceConstants.AUTHENTICATION)
               .append("/confirm.jsp");
        return rootUrl.toString();
    }
    
    public static String getCurrentFullUrl(HttpServletRequest request) {
		StringBuffer curURLBuffer = request.getRequestURL();
		if(request.getQueryString() != null && !request.getQueryString().isEmpty()) {
			curURLBuffer.append("?");
			curURLBuffer.append(request.getQueryString());
		}
		return curURLBuffer.toString();
    }
}
