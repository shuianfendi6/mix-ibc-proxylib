#ifndef __PROXYLIB_PRE_SW_H__
#define __PROXYLIB_PRE_SW_H__

class SM9ProxyMPK_SW: public SM9ProxyMPK {
public:
	ZZn2 Zpub1;
	ECn Ppub2;

	SM9ProxyMPK_SW() { schemeType = SM9_SCHEME_SW; }
	SM9ProxyMPK_SW(ZZn2 &Zp1, ECn &Pp2) { this->schemeType = SM9_SCHEME_SW;
	this->Zpub1 = Zp1; this->Ppub2 = Pp2; }
	void set(ZZn2 &Zp1, ECn &Pp2) { this->Zpub1 = Zp1; this->Ppub2 = Pp2; }

	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode); 
	virtual int serialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);

	BOOL operator==(SM9ProxyMPK_SW &second) {
		return ((this->Zpub1 == second.Zpub1) && 
			(this->Ppub2 == second.Ppub2));
	}
};

class SM9ProxyMSK_SW: public SM9ProxyMSK {
public:
	Big a1;
	Big a2;

	SM9ProxyMSK_SW() { this->schemeType = SM9_SCHEME_SW; }
	SM9ProxyMSK_SW(Big &sk1, Big &sk2) { this->schemeType = SM9_SCHEME_SW; 
	this->set(sk1, sk2); }
	void set(Big &sk1, Big &sk2) { this->a1 = sk1; this->a2 = sk2; }

	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode); 
	virtual int serialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);

	BOOL operator==(SM9ProxyMSK_SW &second) {
		return ((this->a1 == second.a1) && 
			(this->a2 == second.a2));
	}
};

class SM9ProxyPK_SW: public SM9ProxyPK {
public:
	ZZn2 Zpub1;
	ECn Ppub2;

	SM9ProxyPK_SW() { schemeType = SM9_SCHEME_SW; }
	SM9ProxyPK_SW(ZZn2 &Zp1, ECn &Pp2) { this->schemeType = SM9_SCHEME_SW;
	this->Zpub1 = Zp1; this->Ppub2 = Pp2; }
	void set(ZZn2 &Zp1, ECn &Pp2) { this->Zpub1 = Zp1; this->Ppub2 = Pp2; }

	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode); 
	virtual int serialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);

	BOOL operator==(SM9ProxyPK_SW &second) {
		return ((this->Zpub1 == second.Zpub1) && 
			(this->Ppub2 == second.Ppub2));
	}
};

class SM9ProxySK_SW: public SM9ProxySK {
public:
	Big a1;
	Big a2;

	SM9ProxySK_SW() { this->schemeType = SM9_SCHEME_SW; }
	SM9ProxySK_SW(Big &sk1, Big &sk2) { this->schemeType = SM9_SCHEME_SW; 
	this->set(sk1, sk2); }
	void set(Big &sk1, Big &sk2) { this->a1 = sk1; this->a2 = sk2; }

	virtual int getSerializedSize(SM9_SERIALIZE_MODE mode); 
	virtual int serialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);
	virtual BOOL deserialize(SM9_SERIALIZE_MODE mode,
		char *buffer, int maxBuffer);

	BOOL operator==(SM9ProxySK_SW &second) {
		return ((this->a1 == second.a1) && 
			(this->a2 == second.a2));
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
BOOL sm9_sw_generate_params(SM9CurveParams &params);
BOOL sm9_sw_generate_master(SM9CurveParams &params,SM9ProxyMPK_SW &mpk,SM9ProxyMPK_SW &msk);
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
