#include "fix.h"
#include <stdio.h>

char * m_strdup(char *src) {
	char *ret = NULL;
	if (src) {
		int len = strlen(src);
		if (len >= 0) {
			ret = (char*) NPN_MemAlloc(len+1);
			strncpy(ret,src,len);
			ret[len] = '\0';
		}
	}
	return ret;
}

char * m_strndup(char *src, int len) {
	char *ret = NULL;
	if ((src) && (len >= 0)) {
		ret = (char*) NPN_MemAlloc(len+1);
		strncpy(ret,src,len);
		ret[len] = '\0';
	}
	return ret;
}

void m_strFromNP(char **dst, NPString src) {
	// dst should not be a pointer without initailization
	if (dst == NULL)
		return;
	// free old pointer values
	if (*dst) {
		NPN_MemFree(*dst);
		*dst = NULL;
	}
	int len = src.UTF8Length;
	*dst = (char*) NPN_MemAlloc(len+1);
	strncpy(*dst, src.UTF8Characters, len);
	(*dst)[len] = '\0';
}

void m_strFromVar(char **dst, NPVariant var) {
	if (!dst)
		return;
	if (!((NPVARIANT_IS_STRING(var)) || (NPVARIANT_IS_VOID(var))))
		return;
	if (*dst) {
		NPN_MemFree(*dst);
		*dst = NULL;
	}
	if (NPVARIANT_IS_STRING(var)) {
		NPString src = NPVARIANT_TO_STRING(var);
		int len = src.UTF8Length;
		*dst = (char*) NPN_MemAlloc(len+1);
		strncpy(*dst, src.UTF8Characters, len);
		(*dst)[len] = '\0';
	}
	else {
		*dst = NULL;
	}
}