<!DOCTYPE html >
<!-- saved from url=(0014)about:internet -->
<html>
<head>
<title>Intel SplitPoint User Confirmation</title>
<jsp:useBean id="HelperBeans" scope="page" class="com.intel.splat.identityservice.server.utils.HelperBeans" />
<jsp:useBean id="LoginConst" scope="page" class="com.intel.splat.client.IdentityServiceConstants" />
<% 
	String openidRealm = request.getParameter("openid.realm");
%>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

<link type="text/css" rel="stylesheet" href="css/login.css">

<script type="text/javascript" language="javascript">

function formSubmit(str) {
	
}
</script>
</head>
<body>

<!-- OPTIONAL: include this if you want history support -->
<iframe src="javascript:''" id="__gwt_historyFrame" tabIndex='-1'
	style="position: absolute; width: 0; height: 0; border: 0"></iframe>

<!-- RECOMMENDED if your web app will not function without JavaScript enabled -->
<noscript>
<div
	style="width: 22em; position: absolute; left: 50%; margin-left: -11em; color: red; background-color: white; border: 1px solid red; padding: 4px; font-family: sans-serif">
Your web browser must have JavaScript enabled in order for this
application to display correctly.</div>
</noscript>

<div><img src="img/splitpoint_logo.jpg" alt="logo" style="margin-left: 210px;"><img
	src="img/splitpoint_text.png"></img></div>

<div align="center" style="margin-top: 30px">Send your following personal information to <b>
		<%
		if(openidRealm != null) {
			out.println(openidRealm);
		} else { 
			out.println("Unknown service");
		}
		%>
	</b>?</div>
<table width="500px" align="center" cellspacing="0" cellpadding="0"
	style="margin-top: 20px; border: #cccccc solid 2px;" align="center"
	bgcolor="#ffffcc" RULES="all">
	<% 
	java.util.Enumeration paramKeys = request.getParameterNames();
    for ( ; paramKeys.hasMoreElements() ;) {
    	String paramKey = (String)paramKeys.nextElement();
    	if(paramKey.startsWith("openid-ax.")) {
    %>
	<tr> 
		<td align="left" width="100px">
		<% out.println(paramKey.substring("openid-ax.".length())); %>
		</td>
		<td align="left" width="400px">
		<% out.println(new String(org.opensaml.xml.util.Base64.decode(request.getParameter(paramKey)))); %>
		</td>
	</tr>
	<%
    	}
    }
	%>
</table>
<div style="margin-top: 20px" align="center">
<button id="allow" name="allow"  onclick="formSubmit(true)">Allow</button>
<button style="margin-left: 30px" id="deny" name="deny" onclick="formSubmit(false)">Deny</button>
</div>
<hr style="margin-left: 210px; margin-right: 210px; margin-top: 30px;">
<div align="center">Copyright 2010 Intel(R). All rights
reserved.</div>
</body>
</html>