#ifndef __SM9__PROXYLIB_H__
#define __SM9__PROXYLIB_H__

#include "ecn.h"
#include "zzn2.h"

#include <ctime>
#include "ecn2.h"
#include "zzn12a.h"
#include "sm3.h"
#include "pairing_3.h"

class SM9Object {
public:
	SM9_SCHEME_TYPE schemeType;
	SM9_OBJ_TYPE    objectType;

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

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);


#define PROJECTIVE
#define HASH_LEN 20

#endif // __SM9__PROXYLIB_H__
