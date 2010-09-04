#include <npapi.h>
#include <npruntime.h>

#ifndef FIX_H
#define FIX_H

char * m_strdup(const char *src);
char * m_strndup(const char *src, int len);
void m_strFromNP(char **dst, NPString src);
void m_strFromVar(char **dst, NPVariant var);
char * getFileContent(const char* filename);

#endif
