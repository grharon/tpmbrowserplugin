package com.intel.splat.identityservice.server.utils;

import javax.servlet.http.*;
import java.util.Enumeration;
import java.io.UnsupportedEncodingException;
import org.opensaml.xml.util.Base64;

import com.intel.splat.client.IdentityServiceConstants;
import com.intel.splat.identityservice.utils.URLEncDec;

/**
 * Java Beans which helps jsp.
 * @author hzhan38
 *
 */
public class HelperBeans {
	 
	private static String[] ignoreList = {
        "goto", "splashParameters"
    };
	
	/** 
     * Returns the validated and Base64 ecoded query params value.
     * @param request from which query parameters have to be extracted.
     * @return a String the validated and Base64 ecoded query params String
     */
    public String getEncodedQueryParams(HttpServletRequest request)
    {
        String returnQueryParams = "";
        StringBuffer queryParams = new StringBuffer();
        queryParams.append("");
        Enumeration parameters = request.getParameterNames();
        for ( ; parameters.hasMoreElements() ;) {
            boolean ignore = false;
            String parameter = (String)parameters.nextElement();
            for (int i = 0; i < ignoreList.length; i++) {
                if (parameter.equalsIgnoreCase(ignoreList[i])) {
                    ignore = true;
                    break;
                }
            }
            if(!ignore) {
                queryParams.append(parameter);
                String value = request.getParameter(parameter);
                queryParams.append('=');
             
                queryParams.append(value);
                if (parameters.hasMoreElements()) {
                    queryParams.append('&');
                }
            }
        }
        String queryParamsString = queryParams.toString();
        
        if ((queryParamsString != null) &&
            (queryParamsString.length() != 0))
        {
            returnQueryParams = getEncodedInputValue(queryParamsString);
        }

        return returnQueryParams;
    }
    
    /** 
     * Returns the Base64 ecoded URL value.
     * @param inputValue input string value
     * @return a String the Base64 ecoded URL value
     */
    public String getEncodedInputValue(String inputValue) {
        String returnValue = "";
        
        if ((inputValue != null) && (inputValue.length() != 0) && 
            (!inputValue.equalsIgnoreCase("null"))) {        
            try {                
                returnValue = Base64.encodeBytes(inputValue.getBytes("UTF-8"));                            
            } catch (UnsupportedEncodingException ueex) {
                           
            }
        }

        return returnValue;
    }
    
    public String getServiceURL(HttpServletRequest request) {
        StringBuffer serviceURL = new StringBuffer();
        String contextPath = request.getContextPath();
        if ("true".equals(request.getParameter("forward"))) {
            String previousURL = request.getParameter("previous");
            if (previousURL != null) {
                String pURL = URLEncDec.decode(previousURL);
                System.out.println("previous URL: " + pURL);
                
                if (contextPath.length() == 0) {
                	serviceURL = createSlashURL(pURL, 
                		"/" + IdentityServiceConstants.DEPLOYMENT_URL + "/");
                } else {
                	serviceURL = createSlashURL(pURL, 
                    		contextPath + "/" + IdentityServiceConstants.DEPLOYMENT_URL + "/");
                }
                serviceURL.append(IdentityServiceConstants.AUTHENTICATION).append("/");
            }
        }
        return serviceURL.toString();
    }
    
    public String getAuthnConsumerServiceURL(HttpServletRequest request) {
    	StringBuffer result = ServiceURLParsing.getIdentityServiceAppliRootUrl(request);
    	result.append("/");
    	result.append(IdentityServiceConstants.PACKAGE);
    	result.append("/");
    	result.append(request.getParameter(IdentityServiceConstants.IDP_NAME));
    	result.append("/");    	
    	result.append(IdentityServiceConstants.AUTHN_ACS);
    	return result.toString();
    }
    
    public String getDataConfirmationServiceURL(HttpServletRequest request) {
    	StringBuffer result = ServiceURLParsing.getIdentityServiceAppliRootUrl(request);
    	result.append("/");
    	result.append(IdentityServiceConstants.PACKAGE);
    	result.append("/");
    	result.append(request.getParameter(IdentityServiceConstants.IDP_NAME));
    	result.append("/");    	
    	result.append(IdentityServiceConstants.DATA_CCS);
    	return result.toString();
    }
    
    public String getRandom() {
    	long currTime = System.currentTimeMillis();
        return Long.toString(currTime);
    }
    
    public String getAppletJarAbsolutePath(HttpServletRequest request) {
    	StringBuffer result = ServiceURLParsing.getIdentityServiceAppliRootUrl(request);
    	result.append("/")
	          .append(IdentityServiceConstants.AUTHENTICATION);
    	System.out.println("AppletJar Path: " + result.toString());
    	return result.toString();
    }

    private StringBuffer createSlashURL(String pattern, String startFrom) {
        StringBuffer result = new StringBuffer("");
        if (pattern.startsWith(startFrom)) {
            int firstslash = pattern.indexOf("/", startFrom.length());
            while (firstslash != -1) {
                result.append("../");
                firstslash = pattern.indexOf("/", firstslash + 1);
            }
        }
        return result;
    }
}
