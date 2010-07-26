#include "fix.h"
#include "npapi.h"
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
