#ifndef __SM9__PROXYLIB_H__
#define __SM9__PROXYLIB_H__



#include "ecn.h"
#include "zzn2.h"

#include <ctime>
#include "ecn2.h"
#include "zzn12a.h"
#include "sm3.h"
#include "pairing_3.h"

class SM9CurveParams {
public:

	int bits_local;

	Big cid;  // 曲线的识别符
	Big q;  
	Big a,b;
	Big beta; // 不使用当cid=12
	Big cf,N;
	Big k;    // 嵌入次数k
	ECn P1;
	ECn2 P2;
	Big eid;

	ZZn2 X;
	
	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode); 
	virtual int serialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual int maxPlaintextSize() {
		Big temp;
		X.get(temp);
		return bits(temp);
	}

	BOOL operator==(SM9CurveParams &second) {
		return ((this->bits_local == second.bits_local) && 
			(this->q == second.q) &&
			(this->N == second.N) &&
			(this->P1 == second.P1) &&
			(this->P2 == second.P2) &&
			(this->k == second.k)
			);
	}
};

// ProxyPK: Public Key class
//
// This is a top-level class; specific proxy re-encryption schemes
// implement subclasses of this structure.

class SM9ProxyMPK {
public:
	SM9_SCHEME_TYPE schemeType;

	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode) { return 0; } 
	virtual int serialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) {
			return 0;
	}
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) {
			return FALSE;
	}
};

// ProxySK: Secret Key class
//
// This is a top-level class; specific proxy re-encryption schemes
// implement subclasses of this structure.

class SM9ProxyMSK {
public:
	SM9_SCHEME_TYPE schemeType;

	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode) { return 0; } 
	virtual int serialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) {
			return 0;
	}
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) {
			return FALSE;
	}
};

// ProxyPK: Public Key class
//
// This is a top-level class; specific proxy re-encryption schemes
// implement subclasses of this structure.

class SM9ProxyPK {
public:
	SM9_SCHEME_TYPE schemeType;

	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode) { return 0; } 
	virtual int serialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) {
			return 0;
	}
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) {
			return FALSE;
	}
};

// ProxySK: Secret Key class
//
// This is a top-level class; specific proxy re-encryption schemes
// implement subclasses of this structure.

class SM9ProxySK {
public:
	SM9_SCHEME_TYPE schemeType;

	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode) { return 0; } 
	virtual int serialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) {
			return 0;
	}
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) {
			return FALSE;
	}
};

// ProxyCiphertext: Ciphertext class
//
// This is a top-level class; specific re-encryption schemes
// implement subclasses of this structure.

class SM9ProxyCiphertext {
public:
	SM9_CIPHERTEXT_TYPE type;
	SM9_SCHEME_TYPE schemeType;

	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode) { return 0; } 
	virtual int serialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer) {
			return 0;
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

// Core Routines
//BOOL proxy_level1_encrypt(SM9CurveParams &params, Big &plaintext, SM9ProxyPK &publicKey, ZZn2 &res1, ZZn2 &res2);
//BOOL proxy_level2_encrypt(CurveParams &params, ZZn2 zPlaintext, ProxyPK &publicKey, ECn &res1, ZZn2 &res2);
//BOOL proxy_delegate(CurveParams &params, ProxyPK &delegate, ProxySK &delegator, ECn &reskey);
//BOOL proxy_reencrypt(CurveParams &params, ECn &c1, ECn &delegation, ZZn2 &res1);
//BOOL proxy_decrypt_level1(CurveParams &params, ZZn2 &c1, ZZn2 &c2, ProxySK &secretKey, Big &plaintext);
//BOOL proxy_decrypt_level2(CurveParams &params, ECn &c1, ZZn2 &c2, ProxySK &secretKey, Big &plaintext);
//BOOL proxy_decrypt_reencrypted(CurveParams &params, ZZn2 &c1, ZZn2 &c2, ProxySK &secretKey, Big &plaintext);

// Utility routines
//BOOL encodePlaintextAsBig(CurveParams &params,
//		     char *message, int messageLen, Big &msg);
//BOOL decodePlaintextFromBig(CurveParams &params,
//			  char *message, int maxMessage, 
//			  int *messageLen, Big &msg);
//
//* Given a char array of bytes and its length, convert to a Big */
//ECn charToECn (char *c, int *totLen);
//ZZn2 charToZZn2 (char *c, int *totLen);
//
//Big charToBig (char *c, int *totLen);
//int BigTochar (Big &x, char *c, int s);
//
//* Given a big, encode as a byte string in the char, assuming its
//   size is less than s
//   Return length of bytes written to c, or -1 if error
//*/
//int ECnTochar (ECn &e, char *c, int s);
//int ZZn2Tochar (ZZn2 &z, char *c, int s);
//
//void bufrand(char* seedbuf, int seedsize);
//BOOL entropyCollect(char *entropyBuf, int entropyBytes);

// Debug output routine
void printDebugString(string debugString);

ECn2 hash_and_map2(char *ID);
void cofactor(ECn2& S,ZZn2 &F,Big& x);
void set_frobenius_constant(ZZn2 &X);

//
// Set parameter sizes. For example change PBITS to 1024
//

#define PBITS 512
#define QBITS 160

#define HASH_LEN 20

#define PROJECTIVE

// use static temp variables in crypto routines-- faster, but not
// thread safe
#define SAFESTATIC static
  
//
// Benchmarking
//

//#define BENCHMARKING 1

#define NUMBENCHMARKS 7
#define LEVELONEENCTIMING 0
#define LEVELTWOENCTIMING 1
#define DELEGATETIMING 2
#define REENCTIMING 3
#define LEVELONEDECTIMING 4
#define LEVELTWODECTIMING 5
#define REENCDECTIMING 6

#define LEVELONEENCDESC "Level-1 Encryption"
#define LEVELTWOENCDESC "Level-2 Encryption"
#define DELEGATEDESC "Proxy Delegation"
#define REENCDESC "Proxy Re-encryption"
#define LEVELONEDECDESC "Level-1 Decryption"
#define LEVELTWODECDESC "Level-2 Decryption"
#define REENCDECDESC "Re-encrypted Decryption"

#endif // __SM9__PROXYLIB_H__
