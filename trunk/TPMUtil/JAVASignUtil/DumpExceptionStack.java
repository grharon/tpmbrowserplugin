package com.intel.splat.identityservice.utils;

import java.io.PrintWriter;
import java.io.StringWriter;

public class DumpExceptionStack {
	public static String getStackTraceAsString(Exception e) {
        StringWriter sw = new StringWriter();
        e.printStackTrace(new PrintWriter(sw));
        return sw.toString();
    }
}
