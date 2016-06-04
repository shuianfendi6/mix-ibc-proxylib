#ifndef __SM9__PROXYLIB_H__
#define __SM9__PROXYLIB_H__

#include "ecn.h"
#include "zzn2.h"

#include <ctime>
#include "ecn2.h"
#include "zzn12a.h"
#include "sm3.h"
#include "pairing_3.h"
#include <stdio.h>
#include <string.h>

class SM9Data
{
public:
	SM9Data(void)
	{
		m_pValue = 0;
		m_iLen = 0;
	}

	virtual ~SM9Data(void)
	{
		delete [] m_pValue;
		m_pValue = 0;
	}

	void SetValue(char * pValue, int iLen)
	{
		if (m_pValue)
		{
			delete [] m_pValue;
			m_pValue = 0;
		}

		m_iLen = iLen;
		m_pValue = new char[m_iLen];
		memcpy(m_pValue, pValue, iLen);
	}

	void GetValue(char * pValue, int *piLen)
	{
		if (*piLen < m_iLen)
		{
			*piLen = m_iLen;
		}
		else
		{
			*piLen = m_iLen;
			memcpy(pValue, m_pValue, m_iLen);
		}
	}

	int GetSize()
	{
		return GetLength();
	}

	int GetLength()
	{
		return m_iLen;
	}

private:
	char *m_pValue;						//:: 字段数据字节流指针
	int m_iLen;							//:: 字段数据字节流长度	
};

#define DEFAULT_MAX_LEN 1024*1024

class SM9AARData                        //:: 自动分配并释放内存
{
public:
	SM9AARData(int maxLen = DEFAULT_MAX_LEN)
	{
		m_iPos = 0;
		m_iMaxLen = maxLen;
		m_pValue = new char[m_iMaxLen];
	}

	virtual ~SM9AARData(void)
	{
		delete [] m_pValue;
		m_pValue = 0;
	}

	char *m_pValue;						//:: 字段数据字节流指针
	int m_iMaxLen;                      //:: 字段数据字节流最大长度	
	int m_iPos;							//:: 字段数据字节流当前长度
};


class SM9Object {
public:
	SM9_SCHEME_TYPE schemeType;
	SM9_OBJ_TYPE    objectType;

	virtual ~SM9Object(void)
	{

	}

	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode) {return trySerialize(mode,NULL, 0); } 

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) { return 0; } 

	virtual int serialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) {
			int totSize = 0;
			int size = 0;

			// Make sure we've been given a large enough buffer
			if (buffer == NULL || maxBuffer < this->getSerializedSize(mode)) {
				return 0;
			}

			return this->trySerialize(mode, buffer, maxBuffer);
	}
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) {
			return FALSE;
	}

	virtual BOOL operator==(SM9Object &second) {
		BOOL flag = FALSE;
		SM9_SERIALIZE_MODE mode = SM9_SERIALIZE_BINARY;
		int maxBuffer = 1024;
		char buffer_one[1024] = {0};
		char buffer_second[1024] = {0};
		int buffer_one_len = 0;
		int buffer_second_len = 0;

		buffer_one_len = this->trySerialize(mode, buffer_one, maxBuffer);
		buffer_second_len = second.trySerialize(mode, buffer_second, maxBuffer);

		if (buffer_one_len == buffer_second_len && 0 == memcmp(buffer_one,buffer_second,buffer_one_len))
		{
			flag = TRUE;
		}

		return flag;

	}
};

// Utility Routines
BOOL fast_pairing(ECn2& P,ZZn& Qx,ZZn& Qy,Big &x,ZZn2 &X,ZZn12& res);
BOOL ecap(ECn2& P,ECn& Q,Big& x,ZZn2 &X,ZZn12& r);
ECn map_to_point(char *ID);
void strip(char *name);

ECn charToECn (char *c, int *totLen);
ZZn2 charToZZn2 (char *c, int *totLen);

Big charToBig (char *c, int *totLen);
int BigTochar (Big &x, char *c, int s);

int ECnTochar (ECn &e, char *c, int s);
int ZZn2Tochar (ZZn2 &z, char *c, int s);

ECn2 hash_and_map2(char *ID);
void cofactor(ECn2& S,ZZn2 &F,Big& x);
void set_frobenius_constant(ZZn2 &X);
SM9_OBJ_TYPE getSM9ObjectType(char *c, int *totLen);

int SM9_H1(char * pZ,int iZLen, char * pN, int iNLen,char *pH1,int *piH1Len);
int SM9_H2(char * pZ,int iZLen, char * pN, int iNLen,char *pH2,int *piH2Len);
int SM9_MAC(char * pK, int iKLen, char * pZ,int iZLen, char pMac[32]);
int SM9_HV(unsigned int n,unsigned char * src, unsigned char digest[32]);

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);


#define PROJECTIVE
#define HASH_LEN 20

#endif // __SM9__PROXYLIB_H__
