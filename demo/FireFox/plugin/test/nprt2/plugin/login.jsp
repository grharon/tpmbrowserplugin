<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<!-- saved from url=(0014)about:internet -->
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Intel SSO Demo Login</title>
<jsp:useBean id="HelperBeans" scope="page" class="com.intel.splat.identityservice.server.utils.HelperBeans" />
<jsp:useBean id="LoginConst" scope="page" class="com.intel.splat.client.IdentityServiceConstants" />
<%
String gotoURL = request.getParameter(LoginConst.LOGIN_GOTO_ATTR);
String serviceURL = HelperBeans.getServiceURL(request);
String acsURL = HelperBeans.getAuthnConsumerServiceURL(request);
String idpID = request.getParameter(LoginConst.IDP_NAME);
boolean useTPM = false;
if(request.getParameter(LoginConst.USETPM) != null &&
		request.getParameter(LoginConst.USETPM).equals("true")) {
	useTPM = true;
}
boolean openidLogin = false;
if(request.getParameter(LoginConst.OPENID_LOGIN) != null &&
		request.getParameter(LoginConst.OPENID_LOGIN).equals("true")) {
	openidLogin = true;
}
String openidRealm = request.getParameter(LoginConst.OPENID_REALM);
boolean unAuth = false;
if(request.getParameter(LoginConst.UNAUTH) !=null &&
		request.getParameter(LoginConst.UNAUTH).equals("true")) {
	unAuth = true;
}
String randomString = null;
if(useTPM) {
	randomString = HelperBeans.getRandom();
}
%>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link type="text/css" rel="stylesheet" href="css/login.css">
<script type="text/javascript">
function documentLoaded() {
	document.InputForm.j_username.focus()
}

function doSignature() {
	document.forms[0].SignedValue.value = 
		document.Sign.doSignature(document.forms[0].RandomString.value,
					document.forms[0].TPMPassword.value);
	document.forms[0].TPMPubKeyContent.value =
		document.Sign.getPublicKeyContent();
}

function formSubmit(needTPM) {
	if (needTPM) {
		doSignature();
		if (checkSignature()) {
			document.forms[0].submit();
		} else {
			alert("The string received from sever must be signed before submitting.");
		}
	} else {
		document.forms[0].submit();
	}
}

function checkSignature() {
	if (document.forms[0].SignedValue.value == "" ||
		document.forms[0].TPMPubKeyContent.value == "") {
		return false;
	}
	return true;
}
</script>
</head>

<body onload="document.InputForm.j_username.focus()">
    <% if (useTPM) { %>
		<embed name="Sign" type="application/mozilla-npruntime-scriptable-plugin" width="0" height="0"
		code="<%= randomString%>">
    <% } %>
<!--
		<Applet code="com.intel.splat.identityservice.tpmapplet.TPMSignApplet.class" 
            name="Sign" codebase="<%= HelperBeans.getAppletJarAbsolutePath(request)%>" 
            ARCHIVE="DigitalSignApplet.jar" WIDTH="0" HEIGHT="0">
            <param name=randomStr value="<%= randomString%>">
        </Applet>
-->
<!-- RECOMMENDED if your web app will not function without JavaScript enabled -->
<noscript>
<div
	style="width: 22em; position: absolute; left: 50%; margin-left: -11em; color: red; background-color: white; border: 1px solid red; padding: 4px; font-family: sans-serif">
Your web browser must have JavaScript enabled in order for this
application to display correctly.</div>
</noscript>

<div>
	<img src="img/splitpoint_logo.jpg" alt="logo of splitpoint" style="margin-left: 210px;">
	<img src="img/splitpoint_text.png"></img>
</div>

<table width="800px" align="center">
	<tr>
		<td>
		<table width="400px">
			<tr height="100px">
				<td>
					<div>
    					<% if (openidLogin) { %>
							<b>
								<%
									if(openidRealm != null) {
										out.println(openidRealm);
									} else { 
										out.println("Unknown service");
									}
								%>
							</b> want to get access to some of your information. If you approve this action, please sign in.
						<% } else { %>
							Secure your information with SplitPoint powered by Intel. 
						<% } %>
					</div>
				</td>
			</tr>
			<tr height="100px">
				<td>
					<div id="alertmsg"><font size="3" color="red"><b>
					<% 
					if(unAuth) {
						out.println("Wrong user name or Password!");
						out.println("Please sign in before any operation.");
					}
					%>
			 		</b></font></div>
				</td>
			</tr>
		</table>
		</td>
		<td>
		<form name="InputForm" method="post" action="test.jsp">
		<table border="0px" cellspacing="0" cellpadding="0"
			style="margin-top: 20px; margin-right: 60px; border: #cccccc solid 3px;"
			align="right" bgcolor="#ffffcc">
			<tr>
				<%
					if(openidLogin){
						out.println("<td><div style=\"margin-top: 25px;margin-right: 10px;\" align=\"right\"><img src=\"img/openidlogo.png\" alt=\"openid\"></div></td>");
					} else {
						out.println("<td>&nbsp;</td>");
					}
				%>
				<td>
				<div style="margin-top: 20px"><font size="4">SplitPoint Login</font></div>
				</td>
			</tr>
			<tbody>
				<tr>
					<td>&nbsp;</td>
				</tr>
				<TR>
					<TD><div style="margin-left: 50px;font-weight: bold">User name:&nbsp;&nbsp;</div></TD>
					<td><div style="margin-right: 50px"><INPUT TYPE="TEXT" ID="j_username" NAME="j_username"></div></td>
				</TR>
				<tr>
					<td>&nbsp;</td>
				</tr>
				<TR>
					<TD><div style="margin-left: 50px;font-weight: bold">Password:&nbsp;&nbsp;</div></TD>
					<td><div style="margin-right: 50px"><INPUT TYPE="PASSWORD" ID="j_password" NAME="j_password"></div>
					</td>
				</TR>
				<% if (useTPM) { %>
				<TR>
					<TD align="left"><div style="margin-left: 50px;font-weight: bold">TPM key password:&nbsp;&nbsp;</div></TD>
					<td><INPUT TYPE="password" NAME="TPMPassword"></td>
					<td>
					<input type="hidden" name="RandomString" value="<%= randomString %>"/>
					<input type="hidden" name="TPMPubKeyContent" value=""/>
					<input type="hidden" name="SignedValue" value=""/>
					</td>
				</TR>
				<% } %>
				<tr>
					<td>&nbsp;</td>
				</tr>
				<tr>
					<td>&nbsp;</td>
					<td>
					<div style="margin-bottom: 20px">
					<button id="submitBtn" onclick="formSubmit(<%= useTPM %>)"><font color="black" face="verdana" size="2">Sign In</font></button>
					</div>
					</td>
					<td>
					<% if (gotoURL != null) { %>
					<input type="hidden" name="goto" value="<%= gotoURL %>"/>
					<% } %>
					<% if (idpID != null) { %>
					<input type="hidden" name="idp" value="<%= idpID %>"/>
					<% } %>
					</td>
				</tr>
			</tbody>
		</table>
		</form>
		</td>
	</tr>
</table>
<hr style="margin-left: 210px; margin-right: 210px; margin-top: 50px;">
<div align="center">Copyright 2010 Intel(R). All rights
reserved.</div>
</body>
</html>
