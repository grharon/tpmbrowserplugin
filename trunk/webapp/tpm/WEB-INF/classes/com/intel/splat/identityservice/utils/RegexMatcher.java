package com.intel.splat.identityservice.utils;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

public class RegexMatcher {
	/**
	 * Find the matched strings from @input with 
	 * specified regular expression
	 * @param regexPattern - Java regular expression
	 * @param input - instance string
	 * @return - the matched string list
	 * @throws PatternSyntaxException
	 */
	public static List<String> getMatchedStringList(String regexPattern, 
			List<String> input) throws PatternSyntaxException{
		List<String> result = null;
		
		Pattern pattern = Pattern.compile(regexPattern);
		result = new ArrayList<String>();
		for (int j = 0; j < input.size(); j++) {
			Matcher matcher = pattern.matcher(input.get(j));
			if (matcher.find()) {
				result.add(matcher.group(1));
			}
		}
		
		return result;
	}
}
