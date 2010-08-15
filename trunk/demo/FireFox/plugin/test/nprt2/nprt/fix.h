#include "npapi.h"
#include "npruntime.h"

#ifndef FIX_H
#define FIX_H
char* m_strdup(char *src);
void m_strFromNP(char **dst, NPString src);
void m_strFromVar(char **dst, NPVariant var);
#endif
