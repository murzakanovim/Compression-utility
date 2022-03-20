#ifndef _MD5_H_
#define _MD5_H_

#include "../defs.h"

#ifndef _WIN_MD5

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>

typedef unsigned int UINT4; 

using namespace std;

typedef struct { 
	UINT4 i[2]; /* number of _bits_ handled mod 2^64 */ 
	UINT4 buf[4]; /* scratch buffer */ 
	unsigned char in[64]; /* input buffer */ 
	unsigned char digest[16]; /* actual digest after MD5Final call */
} MD5_CTX; 

void MD5Init(MD5_CTX *); 
void MD5Update(MD5_CTX *,unsigned char *,unsigned int); 
void MD5Final(MD5_CTX *); 
void Transform(UINT4 *,UINT4 *);
void md5(char *data, unsigned char *out);
void md5(unsigned char *data, int dlen, unsigned char *out);
void md5(unsigned char *data, int dlen, char *out);
void md5(wchar_t *data, unsigned char *out);
void md5(char *data, char *out);
void md5(string data, string &out, bool up = false);
void md5(wstring data, wstring &out);
void md5(char *data, wchar_t *out);
void md5(wchar_t *data, wchar_t *out);
void md5(wchar_t *data, char *out);

#endif

#endif