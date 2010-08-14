<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<!-- saved from url=(0014)about:internet -->
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Intel SSO Demo Login - Test</title>
<jsp:useBean id="HelperBeans" scope="page" class="com.intel.splat.identityservice.server.utils.HelperBeans" />
<jsp:useBean id="LoginConst" scope="page" class="com.intel.splat.client.IdentityServiceConstants" />
<%
String gotoURL = request.getParameter("goto");
String idpID = request.getParameter("idp");
String signedValue = request.getParameter("SignedValue");
String tpmPubKeyContent = request.getParameter("TPMPubKeyContent");
String randomString = request.getParameter("RandomString");
String tpmPassword = request.getParameter("TPMPassword");
String j_password = request.getParameter("j_password");
String j_username = request.getParameter("j_username");
%>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
</head>
<body>
<!-- RECOMMENDED if your web app will not function without JavaScript enabled -->
<noscript>
<div
	style="width: 22em; position: absolute; left: 50%; margin-left: -11em; color: red; background-color: white; border: 1px solid red; padding: 4px; font-family: sans-serif">
Your web browser must have JavaScript enabled in order for this
application to display correctly.</div>
</noscript>

<table width="800px" align="center">
	<tr>
		<td>
			<div style="margin-left: 30px">
				<%out.println("goto = " + gotoURL);%>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div style="margin-left: 30px">
				<%out.println("signedvalue = " + signedValue);%>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div style="margin-left: 30px">
				<%out.println("tpmpubkeycontent = " + tpmPubKeyContent);%>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div style="margin-left: 30px">
				<%out.println("randomstring = " + randomString);%>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div style="margin-left: 30px">
				<%out.println("tpmpassword = " + tpmPassword);%>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div style="margin-left: 30px">
				<%out.println("j_password = " + j_password);%>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div style="margin-left: 30px">
				<%out.println("j_username = " + j_username);%>
			</div>
		</td>
	</tr>
</table>
</body>
</html>
