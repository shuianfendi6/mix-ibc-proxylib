#ifndef __PROXYLIB_PRE_SW_H__
#define __PROXYLIB_PRE_SW_H__


class SM9CurveParams_SW :public SM9Object{
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

	SM9ProxyMPK_SW() { schemeType = SM9_SCHEME_SW; }
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

	SM9ProxyMSK_SW() { this->schemeType = SM9_SCHEME_SW; }
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

	SM9ProxySK_SW() { this->schemeType = SM9_SCHEME_SW; }

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
//
//// Proxy Ciphertext_PRE1: Ciphertext class for PRE1 scheme
////
//// Subclass of ProxyCiphertext
//
//class ProxyCiphertext_PRE1: public ProxyCiphertext {
//public:
//	ECn c1a;
//	ZZn2 c1b;
//	ZZn2 c2;
//
//	ProxyCiphertext_PRE1() { this->schemeType = SCHEME_PRE1; }
//	ProxyCiphertext_PRE1(CIPHERTEXT_TYPE Type, ECn &C1a, ZZn2 &C1b, ZZn2 &C2) {
//		this->schemeType = SCHEME_PRE1;
//		this->set(Type, C1a, C1b, C2);
//	}
//
//	ProxyCiphertext_PRE1(CIPHERTEXT_TYPE Type, ECn &C1a, ZZn2 &C2) { 
//		this->set(Type, C1a, C2);
//	}
//
//	ProxyCiphertext_PRE1(CIPHERTEXT_TYPE Type, ZZn2 &C1b, ZZn2 &C2) {
//		this->set(Type, C1b, C2);
//	}
//
//	void set(CIPHERTEXT_TYPE Type, ECn &C1a, ZZn2 &C1b, ZZn2 &C2) { this->type = Type; this->c1a = C1a;
//	this->c1b = C1b; this->c2 = C2; }
//	void set(CIPHERTEXT_TYPE Type, ECn &C1a, ZZn2 &C2) { this->type = Type; this->c1a = C1a;
//	this->c2 = C2; }
//	void set(CIPHERTEXT_TYPE Type, ZZn2 &C1b, ZZn2 &C2) { this->type = Type; this->c1b = C1b;
//	this->c2 = C2; }
//
//	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode); 
//	virtual int serialize(SM9_SERIALIZE_MODE mode,
//		char *buffer, int maxBuffer);
//	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
//		char *buffer, int maxBuffer);
//
//	BOOL operator==(ProxyCiphertext_PRE1 &second) {
//		return ((this->type == second.type) &&
//			(this->c1a == second.c1a) &&
//			(this->c1b == second.c1b) &&
//			(this->c2 == second.c2));
//	}
//};
//
//typedef ECn DelegationKey_PRE1;
//
//// Cryptographic Routines
BOOL sm9_sw_generate_params(SM9CurveParams_SW &params);
BOOL sm9_sw_generate_masterkey(SM9CurveParams_SW &params,SM9ProxyMPK_SW &mpk,SM9ProxyMSK_SW &msk);
BOOL sm9_sw_calculate_privatekey(SM9CurveParams_SW &params, SM9ProxyMSK_SW &msk, char * userID, int userIDLen, SM9ProxySK_SW &sk);
//BOOL PRE1_keygen(CurveParams &params, ProxyPK_PRE1 &publicKey, ProxySK_PRE1 &secretKey);
//BOOL PRE1_level1_encrypt(CurveParams &params, Big &plaintext, ProxyPK_PRE1 &publicKey, ProxyCiphertext_PRE1 &ciphertext);
//BOOL PRE1_level2_encrypt(CurveParams &params, Big &plaintext, ProxyPK_PRE1 &publicKey, ProxyCiphertext_PRE1 &ciphertext);
//BOOL PRE1_delegate(CurveParams &params, ProxyPK_PRE1 &delegatee, ProxySK_PRE1 &delegator, DelegationKey_PRE1 &reskey);
//BOOL PRE1_reencrypt(CurveParams &params, ProxyCiphertext_PRE1 &origCiphertext, DelegationKey_PRE1 &delegationKey, 
//	ProxyCiphertext_PRE1 &newCiphertext);
//BOOL PRE1_decrypt(CurveParams &params, ProxyCiphertext_PRE1 &ciphertext, ProxySK_PRE1 &secretKey, Big &plaintext);
//
//int SerializeDelegationKey_PRE1(DelegationKey_PRE1 &delKey, SM9_SERIALIZE_MODE mode, char *buffer, int maxBuffer);
//BOOL DeserializeDelegationKey_PRE1(DelegationKey_PRE1 &delKey, SM9_SERIALIZE_MODE mode, char *buffer, int bufSize);

#endif // __PROXYLIB_PRE1_H__
