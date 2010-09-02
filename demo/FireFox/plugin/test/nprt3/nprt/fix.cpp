#include "fix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char * m_strdup(const char *src) {
	char *ret = NULL;
	if (src) {
		int len = strlen(src);
		if (len >= 0) {
			ret = (char*) NPN_MemAlloc(len+1);
			memcpy(ret, src, len);
			ret[len] = '\0';
		}
	}
	return ret;
}

char * m_strndup(const char *src, int len) {
	char *ret = NULL;
	if ((src) && (len >= 0)) {
		ret = (char*) NPN_MemAlloc(len+1);
		memcpy(ret, src, len);
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
	memcpy(*dst, src.UTF8Characters, len);
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
		memcpy(*dst, src.UTF8Characters, len);
		(*dst)[len] = '\0';
	}
	else {
		*dst = NULL;
	}
}

// manually NPN_Free it
char * getFileContent(const char* filename) {
	FILE *f = fopen(filename, "rb");
	if (f == NULL)
		return NULL;
	fseek(f,0,SEEK_END);
	int buflen = ftell(f);
	fseek(f,0,SEEK_SET);
	fprintf(stderr,"file size = %d\n",buflen);
	char *buf = (char*) NPN_MemAlloc(buflen);
	int count = fread(buf,sizeof(char),buflen,f);
	if (count == -1)
		return NULL;
	fprintf(stderr,"read = %d\n",count);
	if (count < buflen)
		buflen = count;
	buf[buflen] = '\0';
	fclose(f);
	fprintf(stderr,"buf = %s\n",buf);
	return buf;
}