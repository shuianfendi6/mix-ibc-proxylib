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
	virtual int maxPlaintextSize() {
		Big temp;
		X.get(temp);
		return bits(temp);
	}

	BOOL operator==(SM9CurveParams_SW &second) {
		return ((this->bits_local == second.bits_local) && 
			(this->q == second.q) &&
			(this->N == second.N) &&
			(this->P1 == second.P1) &&
			(this->P2 == second.P2) &&
			(this->k == second.k)
			);
	}
};

class SM9ProxyMPK_SW: public SM9Object {
public:
	ECn2 Ppubs;
	ECn  Ppube;

	SM9ProxyMPK_SW() { schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_MPK;}
	SM9ProxyMPK_SW(ECn2 &Pp2, ECn &Pp1) { this->schemeType = SM9_SCHEME_SW;
	this->Ppubs = Pp2; this->Ppube = Pp1; }
	void set(ECn2 &Pp2, ECn &Pp1) { this->Ppube = Pp1; this->Ppubs = Pp2; }

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);

	BOOL operator==(SM9ProxyMPK_SW &second) {
		return ((this->Ppubs == second.Ppubs) && 
			(this->Ppube == second.Ppube));
	}
};

class SM9ProxyMSK_SW: public SM9Object {
public:
	Big master;

	SM9ProxyMSK_SW() { this->schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_MSK;}
	SM9ProxyMSK_SW(Big &sk) { this->schemeType = SM9_SCHEME_SW; 
	this->set(sk); }
	void set(Big &sk) { this->master = sk; }

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);

	BOOL operator==(SM9ProxyMSK_SW &second) {
		return ((this->master == second.master));
	}
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

	BOOL operator==(SM9ProxySK_SW &second) {
		return ((this->ds_hid01 == second.ds_hid01) && 
			(this->de_hid02 == second.de_hid02) && 
			(this->de_hid03 == second.de_hid03)
			);
	}
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

	BOOL operator==(SM9ProxySGN_SW &second) {
		return ((this->h == second.h) && 
			(this->S == second.S)
			);
	}
};

class SM9ProxyWRAP_SW: public SM9Object {
public:
	ECn C;

	SM9ProxyWRAP_SW() { this->schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_WRAP;}

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);

	BOOL operator==(SM9ProxyWRAP_SW &second) {
		return ((this->C == second.C)
			);
	}
};

class SM9ProxyDATA_SW: public SM9Object {
public:
	Big data;

	SM9ProxyDATA_SW() { this->schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_DATA;}

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);

	BOOL operator==(SM9ProxyDATA_SW &second) {
		return ((this->data == second.data)
			);
	}
};

class SM9ProxyCipher_SW: public SM9Object {
public:
	Big C1,C2,C3;

	SM9ProxyCipher_SW() { this->schemeType = SM9_SCHEME_SW; this->objectType = SM9_OBJ_SW_DATA;}

	virtual int trySerialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);

	BOOL operator==(SM9ProxyCipher_SW &second) {
		return ((this->C1 == second.C1)&&
			(this->C2 == second.C2)&&
			(this->C3 == second.C3)
			);
	}
};

BOOL sm9_sw_generate_params(SM9CurveParams_SW &params);
BOOL sm9_sw_generate_masterkey(SM9CurveParams_SW &params,SM9ProxyMPK_SW &mpk,SM9ProxyMSK_SW &msk);
BOOL sm9_sw_calculate_privatekey(SM9CurveParams_SW &params, SM9ProxyMSK_SW &msk, char * userID, int userIDLen, SM9ProxySK_SW &sk);

BOOL sm9_sw_sign(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, char *message, int messageLen, SM9ProxySK_SW &sk, SM9ProxySGN_SW &sgn);
BOOL sm9_sw_verify(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, char *message, int messageLen, char * userID, int userIDLen, SM9ProxySGN_SW &sgn);

BOOL sm9_sw_wrap(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk,char * userID, int userIDLen, char *seed, int seedLen, SM9ProxyDATA_SW &key, SM9ProxyWRAP_SW &wrapkey);
BOOL sm9_sw_unwrap(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, SM9ProxySK_SW &sk, char * userID, int userIDLen, SM9ProxyWRAP_SW &wrapkey, SM9ProxyDATA_SW &key);



#endif // __PROXYLIB_PRE_SW_H__
