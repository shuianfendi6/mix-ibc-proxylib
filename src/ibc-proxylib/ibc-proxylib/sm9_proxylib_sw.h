#ifndef __PROXYLIB_PRE_SW_H__
#define __PROXYLIB_PRE_SW_H__

class SM9CurveParams_SW :public SM9Object{
public:

	int bits_local;

	Big t;

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

	SM9CurveParams_SW()
	{
		this->objectType = SM9_OBJ_SW_PARAM;
	}

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
};

class SM9ProxyMPK_SW: public SM9Object {
public:
	ECn2 Ppubs;
	ECn  Ppube;

	SM9ProxyMPK_SW() { schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_MPK;}
	
	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
};

class SM9ProxyMSK_SW: public SM9Object {
public:
	Big master;

	SM9ProxyMSK_SW() { this->schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_MSK;}

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
};

class SM9ProxySK_SW: public SM9Object {
public:
	ECn2 de_hid02;
	ECn2 de_hid03;
	ECn  ds_hid01;

	SM9ProxySK_SW() { this->schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_SK;}

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
};

class SM9ProxySGN_SW: public SM9Object {
public:
	Big h;
	ECn S;

	SM9ProxySGN_SW() { this->schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_SGN;}

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
};

class SM9ProxyWRAP_SW: public SM9Object {
public:
	ECn C;

	SM9ProxyWRAP_SW() { this->schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_WRAP;}

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
};

class SM9ProxyDATA_SW: public SM9Object {
public:
	SM9Data data;

	SM9ProxyDATA_SW() { this->schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_DATA;}

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
};

class SM9ProxyCipher_SW: public SM9Object {
public:
	ECn C1;
	Big C3;
	SM9Data C2;

	SM9ProxyCipher_SW() { this->schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_CIPHER;}

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
};

class SM9ProxyEXR_SW: public SM9Object {
public:
	ECn R;

	SM9ProxyEXR_SW() { this->schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_EX_R;}

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
};


BOOL sm9_sw_generate_params(SM9CurveParams_SW &params);
BOOL sm9_sw_generate_masterkey(SM9CurveParams_SW &params,SM9ProxyMPK_SW &mpk,SM9ProxyMSK_SW &msk);
BOOL sm9_sw_calculate_privatekey(SM9CurveParams_SW &params, SM9ProxyMSK_SW &msk, char * userID, int userIDLen, SM9ProxySK_SW &sk);

BOOL sm9_sw_sign(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, char *message, int messageLen, SM9ProxySK_SW &sk, SM9ProxySGN_SW &sgn);
BOOL sm9_sw_verify(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, char *message, int messageLen, char * userID, int userIDLen, SM9ProxySGN_SW &sgn);

BOOL sm9_sw_wrap(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk,char * userID, int userIDLen, char *seed, int seedLen, SM9ProxyDATA_SW &key, SM9ProxyWRAP_SW &wrapkey);
BOOL sm9_sw_unwrap(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, SM9ProxySK_SW &sk, char * userID, int userIDLen, SM9ProxyWRAP_SW &wrapkey, SM9ProxyDATA_SW &key);

BOOL sm9_sw_encrypt(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk,char * userID, int userIDLen, char *message, int messageLen, SM9ProxyCipher_SW &cipher, SM9_CIPHER_TYPE cipherType);
BOOL sm9_sw_decrypt(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, SM9ProxySK_SW &sk, char * userID, int userIDLen, SM9ProxyCipher_SW &cipher, SM9ProxyDATA_SW &plain, SM9_CIPHER_TYPE cipherType);

BOOL sm9_sw_keyexchangeA1(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, char * userIDB, int userIDBLen, SM9ProxyEXR_SW &RA, SM9ProxyDATA_SW &rA);
BOOL sm9_sw_keyexchangeB2B4(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, SM9ProxySK_SW &sk, 
	char * userIDA, int userIDALen, char * userIDB, int userIDBLen, int key_len, 
	SM9ProxyEXR_SW &RA, SM9ProxyEXR_SW &RB, SM9ProxyDATA_SW &SKB, SM9ProxyDATA_SW &SB, SM9ProxyDATA_SW &S2, SM9_KEY_EX_OPTION option);
BOOL sm9_sw_keyexchangeA3(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, SM9ProxySK_SW &sk, 
	char * userIDA, int userIDALen, char * userIDB, int userIDBLen, int key_len, 
	SM9ProxyEXR_SW &RA, SM9ProxyEXR_SW &RB, SM9ProxyDATA_SW &SB, SM9ProxyDATA_SW &SKA, SM9ProxyDATA_SW &SA, SM9ProxyDATA_SW &rA, SM9_KEY_EX_OPTION option);


BOOL sm9_sw_keyexchangeCalcRr(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk,  char * userIDOther, int userIDOtherLen, SM9ProxyEXR_SW &RSelf, SM9ProxyDATA_SW &rRSelf);
BOOL sm9_sw_keyexchange(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, SM9ProxySK_SW &sk, char * userIDA, int userIDALen, char * userIDB, int userIDBLen,int key_len,
	SM9ProxyDATA_SW &rSelf, SM9ProxyEXR_SW &RA, SM9ProxyEXR_SW &RB, SM9ProxyDATA_SW &SKBorSKA, SM9ProxyDATA_SW &SBorS1,  SM9ProxyDATA_SW &S2orSA, BOOL isB,SM9_KEY_EX_OPTION option);


#endif // __PROXYLIB_PRE_SW_H__
