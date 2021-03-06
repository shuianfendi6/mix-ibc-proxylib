#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>

using namespace std;

#include "sm9_proxylib_api.h"
#include "sm9_proxylib.h"
#include "sm9_proxylib_sw.h"

extern Miracl precisionBits;

#include "sm4.h"

static char * t_str = (char *)"600000000058F98A";

#ifndef GET_ULONG_BE
#define GET_ULONG_BE(n,b,i)                             \
{                                                       \
	(n) = ( (unsigned int) (b)[(i)    ] << 24 )        \
	| ( (unsigned int) (b)[(i) + 1] << 16 )        \
	| ( (unsigned int) (b)[(i) + 2] <<  8 )        \
	| ( (unsigned int) (b)[(i) + 3]       );       \
}
#endif

#ifndef PUT_ULONG_BE
#define PUT_ULONG_BE(n,b,i)                             \
{                                                       \
	(b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
	(b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
	(b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
	(b)[(i) + 3] = (unsigned char) ( (n)       );       \
}
#endif

BOOL 
	sm9_sw_generate_params(SM9CurveParams_SW &params)
{
	miracl *mip=&precisionBits;
	ZZn2 X;

	srand(time(NULL)); // 设定随机数种子
	irand(rand());

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	Big t= (char *)t_str;  //参数t 

	cout<<"t:"<<t<<endl;

	params.q=36*pow(t,4)+36*pow(t,3)+24*t*t+6*t+1; // 基域特征
	Big tr=6*t*t+1; // 迹
	params.N=params.q+1-tr;   // 
	params.cf=1;
	params.a = 0;
	params.b = 0x05;
	params.cid = 0x12;
	params.k = 12;
	params.eid = 0x04; // R-ate 对
	params.t = t;


#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif



#if defined(MIX_BUILD_FOR_SYSTEM_PARAM_GEN)
	forever {
		while (!params.P1.set(randn()));
		params.P1 *= params.cf;
		if (!params.P1.iszero()) 
		{
			break;
		}
	}

	forever
	{
		X.set((ZZn)1,(ZZn)randn());
		if (!params.P2.set(X))
		{
			continue;
		}
		else
		{
			if(!params.P2.iszero())
			{
				break;
			}
			else
			{
				continue;
			}
		}
	}

#else
	char * xp1_str =       (char *)"93DE051D62BF718FF5ED0704487D01D6E1E4086909DC3280E8C4E4817C66DDDD";
	char * yp1_str =       (char *)"21FE8DDA4F21E607631065125C395BBC1C1C00CBFA6024350C464CD70A3EA616";
	char * xp2_part1_str = (char *)"85AEF3D078640C98597B6027B441A01FF1DD2C190F5E93C454806C11D8806141";
	char * yp2_part1_str = (char *)"17509B092E845C1266BA0D262CBEE6ED0736A96FA347C8BD856DC76B84EBEB96";
	char * xp2_part2_str = (char *)"3722755292130B08D2AAB97FD34EC120EE265948D19C17ABF9B7213BAF82D65B";
	char * yp2_part2_str = (char *)"A7CF28D519BE3DA65F3170153D278FF247EFBA98A71A08116215BBA5C999A7C7";

	params.P1.set(xp1_str,yp1_str);

	ZZn2 xp2,yp2;

	xp2.set((Big)xp2_part2_str, xp2_part1_str);
	yp2.set((Big)yp2_part2_str, yp2_part1_str);
	params.P2.set(xp2,yp2);
#endif

	set_frobenius_constant(X);

	cout<<"q:"<<params.q<<endl;
	cout<<"N:"<<params.N<<endl;
	cout<<"cf:"<<params.cf<<endl;
	cout<<"k:"<<params.k<<endl;
	cout<<"P1:"<<params.P1<<endl;
	cout<<"P2:"<<params.P2<<endl;

	ZZn12 g;

	if (ecap(params.P2,params.P1,t,X,g) == FALSE)
	{
		cout<<"err P1 P2.";
		return FALSE;
	}
	else
	{
		ZZn12 tmp = pow(g,params.N);

		ZZn12 one;

		one.set((Big)1);

		cout<<"g:"<<g<<endl;
		cout<<"tmp:"<<tmp<<endl;
		cout<<"one:"<<one<<endl;

		if (tmp != one)
		{
			cout<<"err P1 P2.";
			return FALSE;
		}
	}

	// Success
	return TRUE;
}

BOOL sm9_sw_generate_masterkey(SM9CurveParams_SW &params,SM9ProxyMPK_SW &mpk,SM9ProxyMSK_SW &msk)
{
	Big imsk;

	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;
	ZZn2 X;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	set_frobenius_constant(X);

#if defined(MIX_BUILD_FOR_SYSTEM_MASTER_KEY_SIGN) 
	imsk="0130E78459D78545CB54C587E02CF480CE0B66340F319F348A1D5B1F2DC5F4";
#elif defined(MIX_BUILD_FOR_SYSTEM_MASTER_KEY_EX) 
	imsk="02E65B0762D042F51F0D23542B13ED8CFA2E9A0E7206361E013A283905E31F";
#elif defined(MIX_BUILD_FOR_SYSTEM_MASTER_KEY_WRAP) || defined(MIX_BUILD_FOR_SYSTEM_MASTER_KEY_EN)
	imsk="01EDEE3778F441F8DEA3D9FA0ACC4E07EE36C93F9A08618AF4AD85CEDE1C22";
#else
	while(0 == (imsk=rand(params.N)))
	{

	};
#endif

	msk.master = imsk;

	mpk.Ppubs = imsk * params.P2;
	mpk.Ppube = imsk * params.P1;

	cout<<"Ppubs:"<<mpk.Ppubs<<endl;
	cout<<"Ppube:"<<mpk.Ppube<<endl;

	return TRUE;
}


BOOL sm9_sw_calculate_privatekey(SM9CurveParams_SW &params, SM9ProxyMSK_SW &msk, char * userID, int userIDLen, SM9ProxySK_SW &sk)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	ZZn2 X;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	set_frobenius_constant(X);

	//cacl ds_hid01
	{
		Big t1,t2;
		char hid = 0x01;

		SM9AARData buffer(userIDLen + 1);

		memcpy(buffer.m_pValue + buffer.m_iPos,userID,userIDLen);
		buffer.m_iPos += userIDLen;
		buffer.m_iPos += to_binaryChar(hid,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue+buffer.m_iPos);

		SM9AARData h1_data(32);
		SM9AARData n_data(32);

		n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

		SM9_H1_V2(buffer.m_pValue, buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h1_data.m_pValue, &h1_data.m_iPos);

		Big h1 = from_binary(h1_data.m_iPos, h1_data.m_pValue);

		t1 = h1 + msk.master;

		t2 =  pow(msk.master * inverse(t1,params.N),1,params.N);

		sk.ds_hid01 = t2 * params.P1;
	}

	//cacl de_hid02
	{
		Big t1,t2;
		char hid = 0x02;

		SM9AARData buffer(userIDLen + 1);

		memcpy(buffer.m_pValue + buffer.m_iPos,userID,userIDLen);
		buffer.m_iPos += userIDLen;
		buffer.m_iPos += to_binaryChar(hid,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue+buffer.m_iPos);

		SM9AARData h1_data(32);
		SM9AARData n_data(32);

		n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

		SM9_H1_V2(buffer.m_pValue, buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h1_data.m_pValue, &h1_data.m_iPos);

		Big h1 = from_binary(h1_data.m_iPos, h1_data.m_pValue);

		t1 = h1 + msk.master;

		t2 =  pow(msk.master * inverse(t1,params.N),1,params.N);

		sk.de_hid02 = t2 * params.P2;
	}

	{
		Big t1,t2;
		char hid = 0x03;

		SM9AARData buffer(userIDLen + 1);

		memcpy(buffer.m_pValue + buffer.m_iPos,userID,userIDLen);
		buffer.m_iPos += userIDLen;
		buffer.m_iPos += to_binaryChar(hid,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue+buffer.m_iPos);

		SM9AARData h1_data(32);
		SM9AARData n_data(32);

		n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

		SM9_H1_V2(buffer.m_pValue, buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h1_data.m_pValue, &h1_data.m_iPos);

		Big h1 = from_binary(h1_data.m_iPos, h1_data.m_pValue);

		t1 = h1 + msk.master;

		t2 =  pow(msk.master * inverse(t1,params.N),1,params.N);

		sk.de_hid03 = t2 * params.P2;
	}

	cout<<"ds_hid01:"<<sk.ds_hid01<<endl;
	cout<<"de_hid02:"<<sk.de_hid02<<endl;
	cout<<"de_hid03:"<<sk.de_hid03<<endl;

	return TRUE;
}

BOOL sm9_sw_sign(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, char *message, int messageLen, SM9ProxySK_SW &sk, SM9ProxySGN_SW &sgn)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	ZZn12 g;
	ZZn2 X;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	set_frobenius_constant(X);

	cout <<"X:"<<X<<endl;

	ecap(mpk.Ppubs,params.P1, params.t, X, g);

	cout <<"g:"<<g<<endl;

	Big r;

#if defined(MIX_BUILD_FOR_SYSTEM_MASTER_KEY_SIGN) 
	r = "033C8616B06704813203DFD00965022ED15975C662337AED648835DC4B1CBE";
#else
	while(0 == (r=rand(params.N)))
	{

	};
#endif

	ZZn12 w = pow(g,r);

	SM9AARData buffer(12 * 32); // zzn12 == 12 * 32

	buffer.m_iPos += to_binaryZZn12(w,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	SM9AARData h2_data(32);
	SM9AARData n_data(32);

	n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

	SM9_H2_V2(message, messageLen, buffer.m_pValue,buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h2_data.m_pValue, &h2_data.m_iPos);

	Big h = from_binary(h2_data.m_iPos, h2_data.m_pValue);

	//cout <<"h:"<<h<<endl;

	Big l = (r-h + params.N)%params.N;

	//cout <<"l:"<<l<<endl;

	ECn S = l * sk.ds_hid01;

	//cout <<"S:"<<S<<endl;

	sgn.h = h;
	sgn.S = S;

	return TRUE;
}


BOOL sm9_sw_verify(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, char *message, int messageLen, char * userID, int userIDLen, SM9ProxySGN_SW &sgn)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	ZZn2 X;
	ZZn12 g;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	set_frobenius_constant(X);
	cout <<"X:"<<X<<endl;

	ecap(mpk.Ppubs,params.P1, params.t, X, g);

	cout <<"g:"<<g<<endl;

	Big t1,t2;
	char hid = 0x01;

	SM9AARData buffer(userIDLen + 1 > 12 * 32 ? userIDLen + 1: 12 * 32);

	memcpy(buffer.m_pValue + buffer.m_iPos,userID,userIDLen);
	buffer.m_iPos += userIDLen;
	buffer.m_iPos += to_binaryChar(hid,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue+buffer.m_iPos);

	SM9AARData h1_data(32);
	SM9AARData n_data(32);

	n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

	SM9_H1_V2(buffer.m_pValue, buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h1_data.m_pValue, &h1_data.m_iPos);

	Big h1 = from_binary(h1_data.m_iPos, h1_data.m_pValue);

	//cout <<"h1:"<<h1<<endl;
	//cout <<"g"<<g<<endl;

	ECn2 P = h1 * params.P2 + mpk.Ppubs;

	cout <<"P:"<<P<<endl;

	ZZn12 tv = pow(g,sgn.h);

	cout <<"tv"<<tv<<endl;

	ZZn12 u;

	ecap(P,sgn.S,params.t,X,u);

	cout <<"u:"<<u<<endl;

	ZZn12 w = u * tv;

	cout <<"w_:"<<w<<endl;

	buffer.m_iPos = 0;
	buffer.m_iPos += to_binaryZZn12(w,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	char h2_str[32] = {0};
	int h2_len = 32;

	SM9AARData h2_data(32);

	n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

	SM9_H2_V2(message,messageLen, buffer.m_pValue, buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h2_data.m_pValue, &h2_data.m_iPos);

	Big h2 = from_binary(h2_data.m_iPos, h2_data.m_pValue);

	cout <<"h2:"<<h2<<endl;
	cout <<"h:"<<sgn.h<<endl;

	cout <<"h2-h:"<< h2-sgn.h <<endl;
	cout <<"h-h2:"<< sgn.h-h2 <<endl;

	if (h2 == sgn.h)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL sm9_sw_wrap(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk,char * userID, int userIDLen, char *seed, int seedLen, SM9ProxyDATA_SW &key, SM9ProxyWRAP_SW &wrapkey)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	char hid = 0x03;
	ZZn12 g;
	ZZn2 X;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	SM9AARData buffer(32 * 2 + 12 * 32 + userIDLen);

	memcpy(buffer.m_pValue + buffer.m_iPos,userID,userIDLen);
	buffer.m_iPos += userIDLen;
	buffer.m_iPos += to_binaryChar(hid,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue+buffer.m_iPos);

	Big t1 = 0;
	//calc t1

	SM9AARData h1_data(32);
	SM9AARData n_data(32);

	n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

	SM9_H1_V2(buffer.m_pValue, buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h1_data.m_pValue, &h1_data.m_iPos);

	Big h1 = from_binary(h1_data.m_iPos, h1_data.m_pValue);

	SM9AARData key_wrap(0x0100/8); //

	ECn QB = h1 * params.P1;

	QB += mpk.Ppube;

	cout <<"QB:"<<QB<<endl;

	Big r;

#if defined(MIX_BUILD_FOR_SYSTEM_MASTER_KEY_WRAP) 
	r = "74015F8489C01EF4270456F9E6475BFB602BDE7F33FD482AB4E3684A6722";
#else
	if (NULL == seed || 0 == seedLen)
	{
		while(0 == (r=rand(params.N)))
		{

		};
	}
	else
	{
		r = from_binary(seedLen,seed);
	}
#endif

	ECn C= r*QB;

	cout <<"r:"<<r<<endl;

	cout <<"C:"<<C<<endl;

	set_frobenius_constant(X);

	ecap(params.P2,mpk.Ppube,params.t,X,g);

	ZZn12 w = pow(g,r);

	Big cx,cy;

	C.get(cx,cy);

	buffer.m_iPos = 0;

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(w,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	memcpy(buffer.m_pValue + buffer.m_iPos,userID,userIDLen);
	buffer.m_iPos += userIDLen;

	key_wrap.m_iPos = key_wrap.m_iMaxLen;
	tcm_kdf((unsigned char *)key_wrap.m_pValue,key_wrap.m_iPos,(unsigned char *)buffer.m_pValue, buffer.m_iPos);

	Big K = from_binary(key_wrap.m_iPos, key_wrap.m_pValue);

	key.data.SetValue(key_wrap.m_pValue, key_wrap.m_iPos);

	cout <<"K:"<<K<<endl;

	//key.data = K;
	wrapkey.C = C;

	return TRUE;
}

BOOL sm9_sw_unwrap(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, SM9ProxySK_SW &sk, char * userID, int userIDLen, SM9ProxyWRAP_SW &wrapkey, SM9ProxyDATA_SW &key)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	char hid = 0x03;
	ZZn2 X;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	set_frobenius_constant(X);

	ZZn12 w;

	ecap(sk.de_hid03, wrapkey.C,params.t,X,w);

	cout <<"w_:"<<w<<endl;

	Big cx,cy;

	wrapkey.C.get(cx,cy);

	SM9AARData buffer(32 * 2 + 12 * 32 + userIDLen);

	buffer.m_iPos = 0;

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(w,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	memcpy(buffer.m_pValue + buffer.m_iPos,userID,userIDLen);
	buffer.m_iPos += userIDLen;

	SM9AARData key_wrap(0x0100/8); //

	key_wrap.m_iPos = key_wrap.m_iMaxLen;
	tcm_kdf((unsigned char *)key_wrap.m_pValue,key_wrap.m_iPos,(unsigned char *)buffer.m_pValue, buffer.m_iPos);

	Big K = from_binary(key_wrap.m_iPos, key_wrap.m_pValue);

	//key.data = K_;

	key.data.SetValue(key_wrap.m_pValue, key_wrap.m_iPos);

	cout <<"K:"<<K<<endl;

	return TRUE;
}


BOOL sm9_sw_encrypt(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk,char * userID, int userIDLen, char *message, int messageLen, SM9ProxyCipher_SW &cipher, SM9_CIPHER_TYPE cipherType)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	char hid = 0x03;
	ZZn12 g;
	ZZn2 X;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	int blockLen = 128/8;

	SM9AARData buffer(((messageLen + blockLen)/(blockLen) * blockLen) > 32 * 2 + 12 * 32 + userIDLen ? ((messageLen + blockLen)/(blockLen) * blockLen) : 32 * 2 + 12 * 32 + userIDLen);

	memcpy(buffer.m_pValue + buffer.m_iPos,userID,userIDLen);
	buffer.m_iPos += userIDLen;
	buffer.m_iPos += to_binaryChar(hid,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue+buffer.m_iPos);

	Big t1 = 0;
	//calc t1

	SM9AARData h1_data(32);
	SM9AARData n_data(32);

	n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

	SM9_H1_V2(buffer.m_pValue, buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h1_data.m_pValue, &h1_data.m_iPos);

	Big h1 = from_binary(h1_data.m_iPos, h1_data.m_pValue);

	ECn QB = h1 * params.P1;

	QB += mpk.Ppube;

	cout <<"QB:"<<QB<<endl;

	Big r;

#if defined(MIX_BUILD_FOR_SYSTEM_MASTER_KEY_WRAP) 
	r = "AAC0541779C8FC45E3E2CB25C12B5D2576B2129AE8BB5EE2CBE5EC9E785C";
#else
	while(0 == (r=rand(params.N)))
	{

	};
#endif

	ECn C1= r*QB;

	cout <<"r:"<<r<<endl;

	cout <<"C1:"<<C1<<endl;

	set_frobenius_constant(X);

	ecap(params.P2,mpk.Ppube,params.t,X,g);

	ZZn12 w = pow(g,r);

	int K1_len = 0x80/8;
	int K2_len = 0x0100/8;
	int klen = 0;

	if(SM9_CIPHER_KDF_BASE == cipherType)
	{
		K1_len = messageLen;
	}

	klen = K1_len + K2_len;

	Big cx,cy;

	C1.get(cx,cy);

	buffer.m_iPos = 0;

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(w,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	memcpy(buffer.m_pValue + buffer.m_iPos,userID,userIDLen);
	buffer.m_iPos += userIDLen;

	SM9AARData kdata(klen);

	kdata.m_iPos = kdata.m_iMaxLen;

	tcm_kdf((unsigned char *)kdata.m_pValue,kdata.m_iPos,(unsigned char *)buffer.m_pValue,buffer.m_iPos);

	Big C3;

	if(SM9_CIPHER_KDF_BASE == cipherType)
	{
		for(buffer.m_iPos = 0; buffer.m_iPos < messageLen; buffer.m_iPos++)
		{
			buffer.m_pValue[buffer.m_iPos] = message[buffer.m_iPos] ^ kdata.m_pValue[buffer.m_iPos];
		}

		cipher.C2.SetValue(buffer.m_pValue,buffer.m_iPos);
		//C2 = from_binary(messageLen,buffer);

		char C3_str[32] = {0};

		SM9_MAC(kdata.m_pValue + K1_len,K2_len,buffer.m_pValue,buffer.m_iPos,C3_str);

		C3 = from_binary(32,C3_str);
	}
	else
	{
		sm4_context ctx;

		sm4_setkey_enc(&ctx,(unsigned char *)kdata.m_pValue);

		int plainLen = (messageLen + blockLen)/(blockLen) * blockLen;
		SM9AARData plain (messageLen+blockLen);
		char pad = plainLen - messageLen;

		memcpy(plain.m_pValue,message,messageLen);
		memset(plain.m_pValue + messageLen,pad,pad);

		sm4_crypt_ecb(&ctx,0,plainLen,(unsigned char *)plain.m_pValue,(unsigned char *)buffer.m_pValue);

		cipher.C2.SetValue(buffer.m_pValue,plainLen);
		//C2 = from_binary(plainLen,buffer);

		char C3_str[32] = {0};

		SM9_MAC(kdata.m_pValue + K1_len,K2_len,buffer.m_pValue,plainLen,C3_str);

		C3 = from_binary(32,C3_str);
	}

	cipher.C1 = C1;
	cipher.C3 = C3;

	return TRUE;
}

BOOL sm9_sw_decrypt(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, SM9ProxySK_SW &sk, char * userID, int userIDLen, SM9ProxyCipher_SW &cipher, SM9ProxyDATA_SW &plain, SM9_CIPHER_TYPE cipherType)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	char hid = 0x03;
	ZZn2 X;
	ZZn12 w;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	set_frobenius_constant(X);

	ecap(sk.de_hid03,cipher.C1,params.t,X,w);

	cout <<"w:"<<w<<endl;

	int K1_len = 0x80/8;
	int K2_len = 0x0100/8;
	int klen = 0;

	SM9AARData buffer(32 * 2 + 12 * 32 + userIDLen > cipher.C2.GetLength()? 32 * 2 + 12 * 32 + userIDLen : cipher.C2.GetLength());

	int iLen = cipher.C2.GetLength();

	buffer.m_iPos = 0;
	cipher.C2.GetValue(buffer.m_pValue + buffer.m_iPos,&iLen);
	buffer.m_iPos += cipher.C2.GetLength();

	if(SM9_CIPHER_KDF_BASE == cipherType)
	{
		K1_len = buffer.m_iPos;
	}

	klen = K1_len + K2_len;

	buffer.m_iPos = 0;

	Big cx,cy;

	buffer.m_iPos = 0;

	cipher.C1.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(w,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	memcpy(buffer.m_pValue + buffer.m_iPos,userID,userIDLen);
	buffer.m_iPos += userIDLen;

	SM9AARData kdata(klen);

	kdata.m_iPos = kdata.m_iMaxLen;
	tcm_kdf((unsigned char *)kdata.m_pValue,kdata.m_iPos,(unsigned char *)buffer.m_pValue,buffer.m_iPos);

	Big u;
	Big M;

	char U_str[32] = {0};

	buffer.m_iPos = 0;
	cipher.C2.GetValue(buffer.m_pValue + buffer.m_iPos,&iLen);
	buffer.m_iPos += cipher.C2.GetLength();

	if(SM9_CIPHER_KDF_BASE == cipherType)
	{
		SM9_MAC(kdata.m_pValue + K1_len,K2_len,buffer.m_pValue,K1_len,U_str);
	}
	else
	{
		SM9_MAC(kdata.m_pValue + K1_len,K2_len,buffer.m_pValue,buffer.m_iPos,U_str);
	}

	u = from_binary(32,U_str);

	cout <<"u:"<<u<<endl;

	if(0 == u-cipher.C3)
	{
		if(SM9_CIPHER_KDF_BASE == cipherType)
		{
			SM9AARData M_(K1_len);

			for(buffer.m_iPos = 0; buffer.m_iPos < K1_len; buffer.m_iPos++)
			{
				M_.m_pValue[buffer.m_iPos] = buffer.m_pValue[buffer.m_iPos] ^ kdata.m_pValue[buffer.m_iPos];
			}

			plain.data.SetValue(M_.m_pValue, buffer.m_iPos);
		}
		else
		{
			sm4_context ctx;

			sm4_setkey_dec(&ctx,(unsigned char *)kdata.m_pValue);

			int blockLen = 128/8;

			int iLen = cipher.C2.GetLength();

			buffer.m_iPos = 0;
			cipher.C2.GetValue(buffer.m_pValue + buffer.m_iPos,&iLen);
			buffer.m_iPos += cipher.C2.GetLength();

			SM9AARData pplain(buffer.m_iPos);

			sm4_crypt_ecb(&ctx,0,buffer.m_iPos,(unsigned char *)buffer.m_pValue,(unsigned char *)pplain.m_pValue);

			plain.data.SetValue(pplain.m_pValue,buffer.m_iPos-pplain.m_pValue[buffer.m_iPos-1]);
		}
	}

	if(0 == u-cipher.C3)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

BOOL sm9_sw_keyexchangeA1(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk,  char * userIDB, int userIDBLen, SM9ProxyEXR_SW &RA, SM9ProxyDATA_SW &rA)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	char hid = 0x02;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	SM9AARData buffer(userIDBLen + 1>32?userIDBLen+1:32);

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDB,userIDBLen);
	buffer.m_iPos += userIDBLen;
	buffer.m_iPos += to_binaryChar(hid,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue+buffer.m_iPos);

	SM9AARData h1_data(32);
	SM9AARData n_data(32);

	n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

	SM9_H1_V2(buffer.m_pValue, buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h1_data.m_pValue, &h1_data.m_iPos);

	Big h1 = from_binary(h1_data.m_iPos, h1_data.m_pValue);

	ECn Q = h1*params.P1;

	Q += mpk.Ppube;

	cout <<"Q:"<<Q<<endl;

	Big r;

#if defined(MIX_BUILD_FOR_SYSTEM_MASTER_KEY_EX) 
	r = "5879DD1D51E175946F23B1B41E93BA31C584AE59A426EC1046A4D03B06C8";
#else
	while(0 == (r=rand(params.N)))
	{

	};
#endif

	ECn R = r*Q;

	RA.R = R;

	//rA.data = r;
	{
		buffer.m_iPos = 0;
		buffer.m_iPos += to_binaryBig(r,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue);

		rA.data.SetValue(buffer.m_pValue,buffer.m_iPos);
	}

	cout <<"R:"<<R<<endl;

	return TRUE;
}


BOOL sm9_sw_keyexchangeCalcRr(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk,  char * userIDOther, int userIDOtherLen, SM9ProxyEXR_SW &RSelf, SM9ProxyDATA_SW &rSelf)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	char hid = 0x02;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	SM9AARData buffer(userIDOtherLen + 1>32?userIDOtherLen+1:32);

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDOther,userIDOtherLen);
	buffer.m_iPos += userIDOtherLen;
	buffer.m_iPos += to_binaryChar(hid,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue+buffer.m_iPos);

	SM9AARData h1_data(32);
	SM9AARData n_data(32);

	n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

	SM9_H1_V2(buffer.m_pValue, buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h1_data.m_pValue, &h1_data.m_iPos);

	Big h1 = from_binary(h1_data.m_iPos, h1_data.m_pValue);

	ECn Q = h1*params.P1;

	Q += mpk.Ppube;

	cout <<"Q:"<<Q<<endl;

	Big r;

#if defined(MIX_BUILD_FOR_SYSTEM_MASTER_KEY_EX) 
	r = "5879DD1D51E175946F23B1B41E93BA31C584AE59A426EC1046A4D03B06C8";
#else
	while(0 == (r=rand(params.N)))
	{

	};
#endif

	ECn R = r*Q;

	RSelf.R = R;

	//rA.data = r;
	{
		buffer.m_iPos = 0;
		buffer.m_iPos += to_binaryBig(r,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue);

		rSelf.data.SetValue(buffer.m_pValue,buffer.m_iPos);
	}

	cout <<"R:"<<R<<endl;

	return TRUE;
}


BOOL sm9_sw_keyexchange(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, SM9ProxySK_SW &sk, char * userIDA, int userIDALen, char * userIDB, int userIDBLen,int key_len,
	SM9ProxyDATA_SW &rSelf, SM9ProxyEXR_SW &RA, SM9ProxyEXR_SW &RB, SM9ProxyDATA_SW &SKBorSKA, SM9ProxyDATA_SW &SBorS1,  SM9ProxyDATA_SW &S2orSA, BOOL isB,SM9_KEY_EX_OPTION option)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	char hid = 0x02;
	ZZn2 X;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	set_frobenius_constant(X);

	SM9AARData buffer(32 * 2 * 2 + 12 * 32 * 3 + userIDALen + userIDBLen);

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDA,userIDALen);
	buffer.m_iPos += userIDALen;
	buffer.m_iPos += to_binaryChar(hid,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue+buffer.m_iPos);

	SM9AARData h1_data(32);
	SM9AARData n_data(32);

	n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

	SM9_H1_V2(buffer.m_pValue, buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h1_data.m_pValue, &h1_data.m_iPos);

	Big h1 = from_binary(h1_data.m_iPos, h1_data.m_pValue);

	ECn Q = h1*params.P1;

	Q += mpk.Ppube;

	cout <<"Q:"<<Q<<endl;

	Big r;

	int iLen = rSelf.data.GetLength();

	rSelf.data.GetValue(buffer.m_pValue, &iLen);

	r = from_binary(iLen, buffer.m_pValue);

	ZZn12 g1,g2,g3;

	//ECn R = r*Q;

	//RB.R = R;

	//cout <<"R:"<<R<<endl;

	cout <<"RA.R:"<<RA.R<<endl;
	cout <<"RB.R:"<<RB.R<<endl;

	if(isB)
	{
		ecap(sk.de_hid02,RA.R,params.t,X,g1);

		ecap(params.P2, mpk.Ppube,params.t,X,g2);

		g2 = pow(g2,r);

		g3 = pow(g1,r);
	}
	else
	{
		ecap(params.P2,mpk.Ppube,params.t,X,g1);

		g1 = pow(g1,r);

		ecap(sk.de_hid02,RB.R,params.t,X,g2);

		//g3 = pow(g2,rA.data);
		g3 = pow(g2,r);
	}

	buffer.m_iPos = 0;

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDA,userIDALen);
	buffer.m_iPos += userIDALen;

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDB,userIDBLen);
	buffer.m_iPos += userIDBLen;

	Big cx,cy;
	RA.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	RB.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	buffer.m_iPos += to_binaryZZn12(g1,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(g2,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(g3,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	SM9AARData key_str(key_len);
	key_str.m_iPos = key_str.m_iMaxLen;
	tcm_kdf((unsigned char *)key_str.m_pValue, key_str.m_iPos,(unsigned char *)buffer.m_pValue,buffer.m_iPos);

	//SKB.data = from_binary(key_len,key_str);
	SKBorSKA.data.SetValue(key_str.m_pValue,key_str.m_iPos);

	if (option == SM9_KEY_EX_OPTION_YES)
	{
		char digest[32] = {0};

		buffer.m_iPos = 0;

		buffer.m_iPos += to_binaryZZn12(g2,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		buffer.m_iPos += to_binaryZZn12(g3,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		memcpy(buffer.m_pValue + buffer.m_iPos,userIDA,userIDALen);
		buffer.m_iPos += userIDALen;

		memcpy(buffer.m_pValue + buffer.m_iPos,userIDB,userIDBLen);
		buffer.m_iPos += userIDBLen;

		RA.R.get(cx,cy);

		buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
		buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		RB.R.get(cx,cy);

		buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
		buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

		buffer.m_iPos = 0;

		buffer.m_iPos += to_binaryChar(0x82,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
		buffer.m_iPos += to_binaryZZn12(g1,buffer.m_iMaxLen - buffer.m_iPos, buffer.m_pValue + buffer.m_iPos);
		memcpy(buffer.m_pValue + buffer.m_iPos, digest, 32);
		buffer.m_iPos += 32;

		SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

		//SB.data = from_binary(32,digest);
		SBorS1.data.SetValue(digest,32);

		//cout <<"SB:"<<SB.data<<endl;

		buffer.m_iPos = 0;

		buffer.m_iPos += to_binaryZZn12(g2,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		buffer.m_iPos += to_binaryZZn12(g3,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		memcpy(buffer.m_pValue + buffer.m_iPos,userIDA,userIDALen);
		buffer.m_iPos += userIDALen;

		memcpy(buffer.m_pValue + buffer.m_iPos,userIDB,userIDBLen);
		buffer.m_iPos += userIDBLen;

		RA.R.get(cx,cy);

		buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
		buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		RB.R.get(cx,cy);

		buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
		buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

		buffer.m_iPos = 0;

		buffer.m_iPos += to_binaryChar(0x83,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
		buffer.m_iPos += to_binaryZZn12(g1,buffer.m_iMaxLen - buffer.m_iPos, buffer.m_pValue + buffer.m_iPos);
		memcpy(buffer.m_pValue + buffer.m_iPos, digest, 32);
		buffer.m_iPos += 32;

		SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

		//S2.data = from_binary(32,digest);
		S2orSA.data.SetValue(digest,32);
		//cout <<"S2:"<<S2.data<<endl;
	}

	return TRUE;
}



BOOL sm9_sw_keyexchangeB2B4(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, SM9ProxySK_SW &sk, char * userIDA, int userIDALen, char * userIDB, int userIDBLen,int key_len,
	SM9ProxyEXR_SW &RA, SM9ProxyEXR_SW &RB, SM9ProxyDATA_SW &SKB, SM9ProxyDATA_SW &SB,  SM9ProxyDATA_SW &S2, SM9_KEY_EX_OPTION option)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	char hid = 0x02;
	ZZn2 X;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	set_frobenius_constant(X);

	SM9AARData buffer(32 * 2 * 2 + 12 * 32 * 3 + userIDALen + userIDBLen);

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDA,userIDALen);
	buffer.m_iPos += userIDALen;
	buffer.m_iPos += to_binaryChar(hid,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue+buffer.m_iPos);

	SM9AARData h1_data(32);
	SM9AARData n_data(32);

	n_data.m_iPos = to_binaryBig(params.N,n_data.m_iMaxLen - n_data.m_iPos, n_data.m_pValue);

	SM9_H1_V2(buffer.m_pValue, buffer.m_iPos, n_data.m_pValue,n_data.m_iPos, h1_data.m_pValue, &h1_data.m_iPos);

	Big h1 = from_binary(h1_data.m_iPos, h1_data.m_pValue);

	ECn Q = h1*params.P1;

	Q += mpk.Ppube;

	cout <<"Q:"<<Q<<endl;

	Big r;

#if defined(MIX_BUILD_FOR_SYSTEM_MASTER_KEY_EX) 
	r = "018B98C44BEF9F8537FB7D071B2C928B3BC65BD3D69E1EEE213564905634FE";
#else
	while(0 == (r=rand(params.N)))
	{

	};
#endif

	ZZn12 g1,g2,g3;

	ECn R = r*Q;

	RB.R = R;

	cout <<"R:"<<R<<endl;

	cout <<"RA.R:"<<RA.R<<endl;

	ecap(sk.de_hid02,RA.R,params.t,X,g1);

	ecap(params.P2, mpk.Ppube,params.t,X,g2);

	g2 = pow(g2,r);

	g3 = pow(g1,r);

	buffer.m_iPos = 0;

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDA,userIDALen);
	buffer.m_iPos += userIDALen;

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDB,userIDBLen);
	buffer.m_iPos += userIDBLen;

	Big cx,cy;
	RA.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	RB.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	buffer.m_iPos += to_binaryZZn12(g1,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(g2,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(g3,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	SM9AARData key_str(key_len);
	key_str.m_iPos = key_str.m_iMaxLen;
	tcm_kdf((unsigned char *)key_str.m_pValue, key_str.m_iPos,(unsigned char *)buffer.m_pValue,buffer.m_iPos);

	//SKB.data = from_binary(key_len,key_str);
	SKB.data.SetValue(key_str.m_pValue,key_str.m_iPos);

	char digest[32] = {0};

	buffer.m_iPos = 0;

	buffer.m_iPos += to_binaryZZn12(g2,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	buffer.m_iPos += to_binaryZZn12(g3,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDA,userIDALen);
	buffer.m_iPos += userIDALen;

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDB,userIDBLen);
	buffer.m_iPos += userIDBLen;

	RA.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	RB.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

	buffer.m_iPos = 0;

	buffer.m_iPos += to_binaryChar(0x82,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(g1,buffer.m_iMaxLen - buffer.m_iPos, buffer.m_pValue + buffer.m_iPos);
	memcpy(buffer.m_pValue + buffer.m_iPos, digest, 32);
	buffer.m_iPos += 32;

	SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

	//SB.data = from_binary(32,digest);
	SB.data.SetValue(digest,32);

	//cout <<"SB:"<<SB.data<<endl;

	buffer.m_iPos = 0;

	buffer.m_iPos += to_binaryZZn12(g2,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	buffer.m_iPos += to_binaryZZn12(g3,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDA,userIDALen);
	buffer.m_iPos += userIDALen;

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDB,userIDBLen);
	buffer.m_iPos += userIDBLen;

	RA.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	RB.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

	buffer.m_iPos = 0;

	buffer.m_iPos += to_binaryChar(0x83,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(g1,buffer.m_iMaxLen - buffer.m_iPos, buffer.m_pValue + buffer.m_iPos);
	memcpy(buffer.m_pValue + buffer.m_iPos, digest, 32);
	buffer.m_iPos += 32;

	SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

	//S2.data = from_binary(32,digest);
	S2.data.SetValue(digest,32);
	//cout <<"S2:"<<S2.data<<endl;

	return TRUE;
}

BOOL sm9_sw_keyexchangeA3(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, SM9ProxySK_SW &sk, 
	char * userIDA, int userIDALen, char * userIDB, int userIDBLen, int key_len, 
	SM9ProxyEXR_SW &RA, SM9ProxyEXR_SW &RB,SM9ProxyDATA_SW &SB, SM9ProxyDATA_SW &SKA, SM9ProxyDATA_SW &SA, SM9ProxyDATA_SW &rA, SM9_KEY_EX_OPTION option)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	char hid = 0x02;
	ZZn2 X;

#ifdef AFFINE
	ecurve(params.a,params.b,params.q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
	ecurve(params.a,params.b,params.q,MR_PROJECTIVE);
#endif

	set_frobenius_constant(X);

	if (RB.R.iszero())
	{
		return FALSE;
	}

	ZZn12 g1,g2,g3;
	Big cx,cy;

	ecap(params.P2,mpk.Ppube,params.t,X,g1);

	SM9AARData buffer(32 * 2 * 2 + 12 * 32 * 3 + userIDALen + userIDBLen);

	int iLen = rA.data.GetLength();

	rA.data.GetValue(buffer.m_pValue, &iLen);

	Big r = from_binary(iLen, buffer.m_pValue);

	//g1 = pow(g1,rA.data);

	g1 = pow(g1,r);

	ecap(sk.de_hid02,RB.R,params.t,X,g2);

	//g3 = pow(g2,rA.data);
	g3 = pow(g2,r);

	char digest[32] = {0};

	buffer.m_iPos = 0;

	buffer.m_iPos += to_binaryZZn12(g2,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	buffer.m_iPos += to_binaryZZn12(g3,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDA,userIDALen);
	buffer.m_iPos += userIDALen;

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDB,userIDBLen);
	buffer.m_iPos += userIDBLen;


	RA.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	RB.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

	buffer.m_iPos = 0;

	buffer.m_iPos += to_binaryChar(0x82,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(g1,buffer.m_iMaxLen - buffer.m_iPos, buffer.m_pValue + buffer.m_iPos);
	memcpy(buffer.m_pValue + buffer.m_iPos, digest, 32);
	buffer.m_iPos += 32;

	SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

	Big S1 = from_binary(32,digest);

	if (option == SM9_KEY_EX_OPTION_YES)
	{
		iLen = SB.data.GetLength();
		SB.data.GetValue(buffer.m_pValue, &iLen);
		//if(S1!=SB.data)
		if (0 != memcmp(digest,buffer.m_pValue,32))
		{
			return FALSE;
		}
	}

	buffer.m_iPos = 0;

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDA,userIDALen);
	buffer.m_iPos += userIDALen;

	memcpy(buffer.m_pValue + buffer.m_iPos,userIDB,userIDBLen);
	buffer.m_iPos += userIDBLen;

	RA.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	RB.R.get(cx,cy);

	buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	buffer.m_iPos += to_binaryZZn12(g1,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(g2,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
	buffer.m_iPos += to_binaryZZn12(g3,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

	SM9AARData key_str(key_len);

	key_str.m_iPos = key_str.m_iMaxLen;
	tcm_kdf((unsigned char *)key_str.m_pValue, key_str.m_iPos,(unsigned char *)buffer.m_pValue,buffer.m_iPos);
	SKA.data.SetValue(key_str.m_pValue,key_str.m_iPos);

	buffer.m_iPos = 0;

	if (option == SM9_KEY_EX_OPTION_YES)
	{
		buffer.m_iPos += to_binaryZZn12(g2,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		buffer.m_iPos += to_binaryZZn12(g3,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		memcpy(buffer.m_pValue + buffer.m_iPos,userIDA,userIDALen);
		buffer.m_iPos += userIDALen;

		memcpy(buffer.m_pValue + buffer.m_iPos,userIDB,userIDBLen);
		buffer.m_iPos += userIDBLen;

		RA.R.get(cx,cy);

		buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
		buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		RB.R.get(cx,cy);

		buffer.m_iPos += to_binaryBig(cx,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
		buffer.m_iPos += to_binaryBig(cy,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);

		SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

		buffer.m_iPos = 0;

		buffer.m_iPos += to_binaryChar(0x83,buffer.m_iMaxLen - buffer.m_iPos,buffer.m_pValue + buffer.m_iPos);
		buffer.m_iPos += to_binaryZZn12(g1,buffer.m_iMaxLen - buffer.m_iPos, buffer.m_pValue + buffer.m_iPos);
		memcpy(buffer.m_pValue + buffer.m_iPos, digest, 32);
		buffer.m_iPos += 32;

		SM9_HV(buffer.m_iPos,(unsigned char*)buffer.m_pValue,(unsigned char *)digest);

		//SA.data = from_binary(32,digest);
		SA.data.SetValue(digest,32);
	}

	return TRUE;
}

int SM9CurveParams_SW::trySerialize(SM9_SERIALIZE_MODE mode, char *buffer, int maxBuffer) 
{ 
	int totSize = 0;
	int size = 0;

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			Big objectType(this->objectType);

			size = BigTochars(objectType, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->t, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->cid, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->q, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->a, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->b, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->cf, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->N, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->k, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ECnTochars(this->P1, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			ZZn2 a,b;

			this->P2.get(a,b);

			size = ZZn2Tochars(a, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ZZn2Tochars(b, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->eid, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			int bin_len = this->trySerialize(SM9_SERIALIZE_BINARY,NULL,0);

			SM9AARData serial(bin_len);

			if (maxBuffer>=bin_len * 2)
			{
				memset(buffer, 0, bin_len * 2);
				serial.m_iPos = this->trySerialize(SM9_SERIALIZE_BINARY,serial.m_pValue,serial.m_iMaxLen);
				Bin2Hex((unsigned char *)serial.m_pValue,serial.m_iPos,buffer,&maxBuffer);
			}

			return bin_len * 2;
		}
		break;
	}

	// Invalid serialization mode
	return 0;
}

BOOL
	SM9CurveParams_SW::deserialize(SM9_SERIALIZE_MODE mode, char *buffer, int bufSize)
{
	// Make sure we've been given a real buffer
	if (buffer == NULL) {
		return 0;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			int len;

			this->objectType = (SM9_OBJ_TYPE)toint(charsToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			this->t = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->cid = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->q = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->a = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->b = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->cf = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->N = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->k = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			// Set up the elliptic curve 
#ifdef AFFINE
			ecurve(this->a,this->b,this.q,MR_AFFINE);   
#endif
#ifdef PROJECTIVE
			ecurve(this->a,this->b,this->q,MR_PROJECTIVE);
#endif

			this->P1 = charsToECn(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			ZZn2 a,b;

			a = charsToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			b = charsToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->P2.set(a,b);

			this->eid = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			cout<<"q:"<<(this->q)<<endl;
			cout<<"N:"<<(this->N)<<endl;
			cout<<"cf:"<<(this->cf)<<endl;
			cout<<"k:"<<(this->k)<<endl;
			cout<<"P1:"<<(this->P1)<<endl;
			cout<<"P2:"<<(this->P2)<<endl;
			cout<<"a:"<<(this->a)<<endl;
			cout<<"b:"<<(this->b)<<endl;

			return TRUE;
		}

		break;
	case SM9_SERIALIZE_HEXASCII:
		// Serialize to hexadecimal in ASCII 
		// TBD
		return FALSE;
		break;
	}

	// Invalid serialization mode
	return 0;
}

int SM9ProxyMPK_SW::trySerialize(SM9_SERIALIZE_MODE mode, char *buffer, int maxBuffer)
{
	int totSize = 0;
	int size = 0;

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			Big objectType(this->objectType);

			size = BigTochars(objectType, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			// real value

			size = ECnTochars(this->Ppube, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			ZZn2 a,b;

			this->Ppubs.get(a,b);

			size = ZZn2Tochars(a, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ZZn2Tochars(b, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			int bin_len = this->trySerialize(SM9_SERIALIZE_BINARY,NULL,0);

			SM9AARData serial(bin_len);

			if (maxBuffer>=bin_len * 2)
			{
				memset(buffer, 0, bin_len * 2);
				serial.m_iPos = this->trySerialize(SM9_SERIALIZE_BINARY,serial.m_pValue,serial.m_iMaxLen);
				Bin2Hex((unsigned char *)serial.m_pValue,serial.m_iPos,buffer,&maxBuffer);
			}

			return bin_len * 2;
		}
		break;
	}

	// Invalid serialization mode
	return 0;

}



BOOL
	SM9ProxyMPK_SW::deserialize(SM9_SERIALIZE_MODE mode, char *buffer, int bufSize)
{
	// Make sure we've been given a real buffer
	if (buffer == NULL) {
		return 0;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			int len;

			this->objectType = (SM9_OBJ_TYPE)toint(charsToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			// real value

			this->Ppube = charsToECn(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			ZZn2 a,b;

			a = charsToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			b = charsToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->Ppubs.set(a,b);

			return TRUE;
		}

		break;
	case SM9_SERIALIZE_HEXASCII:
		// Serialize to hexadecimal in ASCII 
		// TBD
		return FALSE;
		break;
	}

	// Invalid serialization mode
	return 0;
}

int SM9ProxyMSK_SW::trySerialize(SM9_SERIALIZE_MODE mode, char *buffer, int maxBuffer)
{
	int totSize = 0;
	int size = 0;

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			Big objectType(this->objectType);

			size = BigTochars(objectType, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->master, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			int bin_len = this->trySerialize(SM9_SERIALIZE_BINARY,NULL,0);

			SM9AARData serial(bin_len);

			if (maxBuffer>=bin_len * 2)
			{
				memset(buffer, 0, bin_len * 2);
				serial.m_iPos = this->trySerialize(SM9_SERIALIZE_BINARY,serial.m_pValue,serial.m_iMaxLen);
				Bin2Hex((unsigned char *)serial.m_pValue,serial.m_iPos,buffer,&maxBuffer);
			}

			return bin_len * 2;
		}
		break;
	}

	// Invalid serialization mode
	return 0;
}

BOOL SM9ProxyMSK_SW::deserialize(SM9_SERIALIZE_MODE mode, char *buffer, int bufSize)
{
	// Make sure we've been given a real buffer
	if (buffer == NULL) {
		return 0;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			int len;

			this->objectType = (SM9_OBJ_TYPE)toint(charsToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			this->master = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			return TRUE;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		// Serialize to hexadecimal in ASCII 
		// TBD
		return FALSE;
		break;
	}

	// Invalid serialization mode
	return 0;
}



int SM9ProxySK_SW::trySerialize(SM9_SERIALIZE_MODE mode, char *buffer, int maxBuffer)
{
	int totSize = 0;
	int size = 0;

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			Big objectType(this->objectType);

			size = BigTochars(objectType, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ECnTochars(this->ds_hid01, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			ZZn2 a,b;

			this->de_hid02.get(a,b);

			size = ZZn2Tochars(a, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ZZn2Tochars(b, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			this->de_hid03.get(a,b);

			size = ZZn2Tochars(a, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ZZn2Tochars(b, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;


			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			int bin_len = this->trySerialize(SM9_SERIALIZE_BINARY,NULL,0);

			SM9AARData serial(bin_len);

			if (maxBuffer>=bin_len * 2)
			{
				memset(buffer, 0, bin_len * 2);
				serial.m_iPos = this->trySerialize(SM9_SERIALIZE_BINARY,serial.m_pValue,serial.m_iMaxLen);
				Bin2Hex((unsigned char *)serial.m_pValue,serial.m_iPos,buffer,&maxBuffer);
			}

			return bin_len * 2;
		}
		break;
	}

	// Invalid serialization mode
	return 0;
}

BOOL SM9ProxySK_SW::deserialize(SM9_SERIALIZE_MODE mode, char *buffer, int bufSize)
{
	// Make sure we've been given a real buffer
	if (buffer == NULL) {
		return 0;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			int len;

			this->objectType = (SM9_OBJ_TYPE)toint(charsToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			this->ds_hid01 = charsToECn(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			ZZn2 a,b;

			a = charsToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			b = charsToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->de_hid02.set(a,b);

			a = charsToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			b = charsToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->de_hid03.set(a,b);

			return TRUE;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		// Serialize to hexadecimal in ASCII 
		// TBD
		return FALSE;
		break;
	}

	// Invalid serialization mode
	return 0;
}



int SM9ProxySGN_SW::trySerialize(SM9_SERIALIZE_MODE mode, char *buffer, int maxBuffer)
{
	int totSize = 0;
	int size = 0;

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			Big objectType(this->objectType);

			size = BigTochars(objectType, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->h, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ECnTochars(this->S, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			int bin_len = this->trySerialize(SM9_SERIALIZE_BINARY,NULL,0);

			SM9AARData serial(bin_len);

			if (maxBuffer>=bin_len * 2)
			{
				memset(buffer, 0, bin_len * 2);
				serial.m_iPos = this->trySerialize(SM9_SERIALIZE_BINARY,serial.m_pValue,serial.m_iMaxLen);
				Bin2Hex((unsigned char *)serial.m_pValue,serial.m_iPos,buffer,&maxBuffer);
			}

			return bin_len * 2;
		}
		break;
	}

	// Invalid serialization mode
	return 0;
}

BOOL SM9ProxySGN_SW::deserialize(SM9_SERIALIZE_MODE mode, char *buffer, int bufSize)
{
	// Make sure we've been given a real buffer
	if (buffer == NULL) {
		return 0;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			int len;

			this->objectType = (SM9_OBJ_TYPE)toint(charsToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			this->h = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->S = charsToECn(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			return TRUE;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		// Serialize to hexadecimal in ASCII 
		// TBD
		return FALSE;
		break;
	}

	// Invalid serialization mode
	return 0;
}



int SM9ProxyWRAP_SW::trySerialize(SM9_SERIALIZE_MODE mode, char *buffer, int maxBuffer)
{
	int totSize = 0;
	int size = 0;

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			Big objectType(this->objectType);

			size = BigTochars(objectType, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ECnTochars(this->C, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			int bin_len = this->trySerialize(SM9_SERIALIZE_BINARY,NULL,0);

			SM9AARData serial(bin_len);

			if (maxBuffer>=bin_len * 2)
			{
				memset(buffer, 0, bin_len * 2);
				serial.m_iPos = this->trySerialize(SM9_SERIALIZE_BINARY,serial.m_pValue,serial.m_iMaxLen);
				Bin2Hex((unsigned char *)serial.m_pValue,serial.m_iPos,buffer,&maxBuffer);
			}

			return bin_len * 2;
		}
		break;
	}

	// Invalid serialization mode
	return 0;
}

BOOL SM9ProxyWRAP_SW::deserialize(SM9_SERIALIZE_MODE mode, char *buffer, int bufSize)
{
	// Make sure we've been given a real buffer
	if (buffer == NULL) {
		return 0;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			int len;

			this->objectType = (SM9_OBJ_TYPE)toint(charsToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			this->C = charsToECn(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			return TRUE;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		// Serialize to hexadecimal in ASCII 
		// TBD
		return FALSE;
		break;
	}

	// Invalid serialization mode
	return 0;
}

int SM9ProxyDATA_SW::trySerialize(SM9_SERIALIZE_MODE mode, char *buffer, int maxBuffer)
{
	int totSize = 0;
	int size = 0;

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			Big objectType(this->objectType);

			size = BigTochars(objectType, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			//size = BigTochars(this->data, buffer, maxBuffer - totSize);
			//if (size <= 0) return 0;
			//totSize += size;
			//buffer += size;

			int iLen = 0;
			iLen = this->data.GetLength();
			if (buffer && maxBuffer - totSize>=4)
			{
				PUT_ULONG_BE(iLen, buffer, 0);
			}
			totSize += 4;
			buffer += 4;
			if (buffer && maxBuffer - totSize>=iLen)
			{
				this->data.GetValue(buffer,&iLen);
			}
			totSize += iLen;
			buffer += iLen;

			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			int bin_len = this->trySerialize(SM9_SERIALIZE_BINARY,NULL,0);

			SM9AARData serial(bin_len);

			if (maxBuffer>=bin_len * 2)
			{
				memset(buffer, 0, bin_len * 2);
				serial.m_iPos = this->trySerialize(SM9_SERIALIZE_BINARY,serial.m_pValue,serial.m_iMaxLen);
				Bin2Hex((unsigned char *)serial.m_pValue,serial.m_iPos,buffer,&maxBuffer);
			}

			return bin_len * 2;
		}
		break;
	}

	// Invalid serialization mode
	return 0;
}

BOOL SM9ProxyDATA_SW::deserialize(SM9_SERIALIZE_MODE mode, char *buffer, int bufSize)
{
	// Make sure we've been given a real buffer
	if (buffer == NULL) {
		return 0;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			int len;

			this->objectType = (SM9_OBJ_TYPE)toint(charsToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			//this->data = charsToBig(buffer, &len);
			//if (len <= 0) return FALSE;
			//buffer += len;

			GET_ULONG_BE(len,buffer,0);
			if (len <= 0) return FALSE;
			buffer+=4;
			this->data.SetValue(buffer,len);
			buffer += len;


			return TRUE;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		// Serialize to hexadecimal in ASCII 
		// TBD
		return FALSE;
		break;
	}

	// Invalid serialization mode
	return 0;
}


int SM9ProxyCipher_SW::trySerialize(SM9_SERIALIZE_MODE mode, char *buffer, int maxBuffer)
{
	int totSize = 0;
	int size = 0;

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			Big objectType(this->objectType);

			size = BigTochars(objectType, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ECnTochars(this->C1, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochars(this->C3, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			//size = BigTochars(this->C2, buffer, maxBuffer - totSize);
			//if (size <= 0) return 0;
			//totSize += size;
			//buffer += size;

			int iLen = 0;
			iLen = this->C2.GetLength();
			if (buffer && maxBuffer - totSize>=4)
			{
				PUT_ULONG_BE(iLen, buffer, 0);
			}
			totSize += 4;
			buffer += 4;
			if (buffer && maxBuffer - totSize>=iLen)
			{
				this->C2.GetValue(buffer,&iLen);
			}
			totSize += iLen;
			buffer += iLen;

			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			int bin_len = this->trySerialize(SM9_SERIALIZE_BINARY,NULL,0);

			SM9AARData serial(bin_len);

			if (maxBuffer>=bin_len * 2)
			{
				memset(buffer, 0, bin_len * 2);
				serial.m_iPos = this->trySerialize(SM9_SERIALIZE_BINARY,serial.m_pValue,serial.m_iMaxLen);
				Bin2Hex((unsigned char *)serial.m_pValue,serial.m_iPos,buffer,&maxBuffer);
			}

			return bin_len * 2;
		}
		break;
	}

	// Invalid serialization mode
	return 0;
}

BOOL SM9ProxyCipher_SW::deserialize(SM9_SERIALIZE_MODE mode, char *buffer, int bufSize)
{
	// Make sure we've been given a real buffer
	if (buffer == NULL) {
		return 0;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			int len;

			this->objectType = (SM9_OBJ_TYPE)toint(charsToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			this->C1 = charsToECn(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->C3 = charsToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			//this->C2 = charsToBig(buffer, &len);
			//if (len <= 0) return FALSE;
			//buffer += len;

			GET_ULONG_BE(len,buffer,0);
			if (len <= 0) return FALSE;
			buffer+=4;
			this->C2.SetValue(buffer,len);
			buffer += len;

			return TRUE;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		// Serialize to hexadecimal in ASCII 
		// TBD
		return FALSE;
		break;
	}

	// Invalid serialization mode
	return 0;
}

int SM9ProxyEXR_SW::trySerialize(SM9_SERIALIZE_MODE mode, char *buffer, int maxBuffer)
{
	int totSize = 0;
	int size = 0;

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			Big objectType(this->objectType);

			size = BigTochars(objectType, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ECnTochars(this->R, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			int bin_len = this->trySerialize(SM9_SERIALIZE_BINARY,NULL,0);

			SM9AARData serial(bin_len);

			if (maxBuffer>=bin_len * 2)
			{
				memset(buffer, 0, bin_len * 2);
				serial.m_iPos = this->trySerialize(SM9_SERIALIZE_BINARY,serial.m_pValue,serial.m_iMaxLen);
				Bin2Hex((unsigned char *)serial.m_pValue,serial.m_iPos,buffer,&maxBuffer);
			}

			return bin_len * 2;
		}
		break;
	}

	// Invalid serialization mode
	return 0;
}

BOOL SM9ProxyEXR_SW::deserialize(SM9_SERIALIZE_MODE mode, char *buffer, int bufSize)
{
	// Make sure we've been given a real buffer
	if (buffer == NULL) {
		return 0;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			int len;

			this->objectType = (SM9_OBJ_TYPE)toint(charsToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			this->R = charsToECn(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			return TRUE;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		// Serialize to hexadecimal in ASCII 
		// TBD
		return FALSE;
		break;
	}

	// Invalid serialization mode
	return 0;
}


// 参数转换
int sm9_proxylib_ObjectToItemsValueCurveParams(void *object)
{
	return SM9_ERROR_OTHER;
}

int sm9_proxylib_ObjectFromItemsValueCurveParams(void **object)
{
	return SM9_ERROR_OTHER;
}

// 主公钥转换
int sm9_proxylib_ObjectToItemsValueMPK(void *object, char g1[SM9_BYTES_LEN_G1], char g2[SM9_BYTES_LEN_G2])
{
	SM9Object *pobject = (SM9Object *)object;
	int pos = 0;

	if (!pobject)
	{
		return SM9_ERROR_DATA_ERR;
	}

	if (SM9_OBJ_SW_MPK  != pobject->objectType)
	{
		return SM9_ERROR_DATA_ERR;
	}

	SM9ProxyMPK_SW *ppobject = (SM9ProxyMPK_SW *)object;

	if (g1)
	{
		to_binaryECn(ppobject->Ppube,SM9_BYTES_LEN_G1,g1);
	}

	if (g2)
	{
		to_binaryECn2(ppobject->Ppubs,SM9_BYTES_LEN_G2,g2);
	}

	return SM9_ERROR_NONE;
}

int sm9_proxylib_ObjectFromItemsValueMPK(void **object, char g1[SM9_BYTES_LEN_G1], char g2[SM9_BYTES_LEN_G2])
{
	SM9ProxyMPK_SW *ppobject = new SM9ProxyMPK_SW();

	if (g1)
	{
		from_binaryECn(ppobject->Ppube,g1);
	}
	
	if (g2)
	{
		from_binaryECn2(ppobject->Ppubs, g2);
	}

	*object = (void *)ppobject;
	
	return SM9_ERROR_NONE;
}

// 主私钥转换
int sm9_proxylib_ObjectToItemsValueMSK(void *object, char msk[SM9_BYTES_LEN_BIG])
{
	SM9Object *pobject = (SM9Object *)object;
	int pos = 0;

	if (!pobject)
	{
		return SM9_ERROR_DATA_ERR;
	}

	if (SM9_OBJ_SW_MSK  != pobject->objectType)
	{
		return SM9_ERROR_DATA_ERR;
	}

	SM9ProxyMSK_SW *ppobject = (SM9ProxyMSK_SW *)object;

	if (msk)
	{
		to_binaryBig(ppobject->master,SM9_BYTES_LEN_BIG,msk);
	}


	return SM9_ERROR_NONE;
}

int sm9_proxylib_ObjectFromItemsValueMSK(void **object, char msk[SM9_BYTES_LEN_BIG])
{
	SM9ProxyMSK_SW *ppobject = new SM9ProxyMSK_SW();

	if (msk)
	{
		from_binaryBig(ppobject->master,msk);
	}
	
	*object = (void *)ppobject;

	return SM9_ERROR_NONE;
}

// 用户私钥转换
int sm9_proxylib_ObjectToItemsValueSK(void *object, char hid01[SM9_BYTES_LEN_G1], char hid02[SM9_BYTES_LEN_G2], char hid03[SM9_BYTES_LEN_G2])
{
	SM9Object *pobject = (SM9Object *)object;
	int pos = 0;

	if (!pobject)
	{
		return SM9_ERROR_DATA_ERR;
	}

	if (SM9_OBJ_SW_SK  != pobject->objectType)
	{
		return SM9_ERROR_DATA_ERR;
	}

	SM9ProxySK_SW *ppobject = (SM9ProxySK_SW *)object;

	if (hid01)
	{
		to_binaryECn(ppobject->ds_hid01,SM9_BYTES_LEN_G1,hid01);
	}

	if (hid02)
	{
		to_binaryECn2(ppobject->de_hid02,SM9_BYTES_LEN_G2,hid02);
	}

	if (hid03)
	{
		to_binaryECn2(ppobject->de_hid03,SM9_BYTES_LEN_G2,hid03);
	}


	return SM9_ERROR_NONE;
}

int sm9_proxylib_ObjectFromItemsValueSK(void **object, char hid01[SM9_BYTES_LEN_G1], char hid02[SM9_BYTES_LEN_G2], char hid03[SM9_BYTES_LEN_G2])
{
	SM9ProxySK_SW *ppobject = new SM9ProxySK_SW();

	if (hid01)
	{
		from_binaryECn(ppobject->ds_hid01,hid01);
	}

	if (hid02)
	{
		from_binaryECn2(ppobject->de_hid02,hid02);
	}

	if (hid03)
	{
		from_binaryECn2(ppobject->de_hid03,hid03);
	}

	*object = (void *)ppobject;

	return SM9_ERROR_NONE;
}

// 签名值转换
int sm9_proxylib_ObjectToItemsValueSGN(void *object, char h[SM9_BYTES_LEN_BIG], char S[SM9_BYTES_LEN_G1])
{
	SM9Object *pobject = (SM9Object *)object;
	int pos = 0;

	if (!pobject)
	{
		return SM9_ERROR_DATA_ERR;
	}

	if (SM9_OBJ_SW_SGN  != pobject->objectType)
	{
		return SM9_ERROR_DATA_ERR;
	}

	SM9ProxySGN_SW *ppobject = (SM9ProxySGN_SW *)object;

	if (h)
	{
		to_binaryBig(ppobject->h,SM9_BYTES_LEN_BIG,h);
	}

	if (S)
	{
		to_binaryECn(ppobject->S,SM9_BYTES_LEN_G1,S);
	}

	return SM9_ERROR_NONE;
}

int sm9_proxylib_ObjectFromItemsValueSGN(void **object, char h[SM9_BYTES_LEN_BIG], char S[SM9_BYTES_LEN_G1])
{
	SM9ProxySGN_SW *ppobject = new SM9ProxySGN_SW();

	if (h)
	{
		from_binaryBig(ppobject->h,h);
	}

	if (S)
	{
		from_binaryECn(ppobject->S,S);
	}

	*object = (void *)ppobject;

	return SM9_ERROR_NONE;
}

// 封装值转换
int sm9_proxylib_ObjectToItemsValueWRAP(void *object, char C[SM9_BYTES_LEN_G1])
{
	SM9Object *pobject = (SM9Object *)object;
	int pos = 0;

	if (!pobject)
	{
		return SM9_ERROR_DATA_ERR;
	}

	if (SM9_OBJ_SW_WRAP  != pobject->objectType)
	{
		return SM9_ERROR_DATA_ERR;
	}

	SM9ProxyWRAP_SW *ppobject = (SM9ProxyWRAP_SW *)object;

	if (C)
	{
		to_binaryECn(ppobject->C,SM9_BYTES_LEN_G1,C);
	}

	return SM9_ERROR_NONE;
}

int sm9_proxylib_ObjectFromItemsValueWRAP(void **object, char C[SM9_BYTES_LEN_G1])
{
	SM9ProxyWRAP_SW *ppobject = new SM9ProxyWRAP_SW();

	if (C)
	{
		from_binaryECn(ppobject->C,C);
	}

	*object = (void *)ppobject;

	return SM9_ERROR_NONE;
}

// 数据转换
int sm9_proxylib_ObjectToItemsValueDATA(void *object, char *data, int *data_len)
{
	SM9Object *pobject = (SM9Object *)object;
	int pos = 0;

	if (!pobject)
	{
		return SM9_ERROR_DATA_ERR;
	}

	if (SM9_OBJ_SW_DATA  != pobject->objectType)
	{
		return SM9_ERROR_DATA_ERR;
	}

	SM9ProxyDATA_SW *ppobject = (SM9ProxyDATA_SW *)object;

	if (data)
	{
		if(ppobject->data.GetSize() > *data_len)
		{
			*data_len = ppobject->data.GetSize();
			return SM9_ERROR_BUFERR_LESS;
		}
		else
		{
			ppobject->data.GetValue(data,data_len);
		}
	}
	else
	{
		if (NULL != data_len)
		{
			*data_len = ppobject->data.GetSize();
		}
	}

	return SM9_ERROR_NONE;
}

int sm9_proxylib_ObjectFromItemsValueDATA(void **object, char *data, int data_len)
{
	SM9ProxyDATA_SW *ppobject = new SM9ProxyDATA_SW();

	if (data)
	{
		ppobject->data.SetValue(data,data_len);
	}

	*object = (void *)ppobject;

	return SM9_ERROR_NONE;
}

// 密文转换
int sm9_proxylib_ObjectToItemsValueCipher(void *object, char C1[SM9_BYTES_LEN_G1], char C3[SM9_BYTES_LEN_BIG], char *C2, int *C2_len)
{
	SM9Object *pobject = (SM9Object *)object;
	int pos = 0;

	if (!pobject)
	{
		return SM9_ERROR_DATA_ERR;
	}

	if (SM9_OBJ_SW_CIPHER  != pobject->objectType)
	{
		return SM9_ERROR_DATA_ERR;
	}

	SM9ProxyCipher_SW *ppobject = (SM9ProxyCipher_SW *)object;

	if (C1)
	{
		to_binaryECn(ppobject->C1,SM9_BYTES_LEN_G1,C1);
	}

	if (C3)
	{
		to_binaryBig(ppobject->C3,SM9_BYTES_LEN_BIG,C3);
	}

	if (C2)
	{
		if(ppobject->C2.GetSize() > *C2_len)
		{
			*C2_len = ppobject->C2.GetSize();
			return SM9_ERROR_BUFERR_LESS;
		}
		else
		{
			ppobject->C2.GetValue(C2,C2_len);
		}
	}
	else
	{
		if (NULL != C2_len)
		{
			*C2_len = ppobject->C2.GetSize();
		}
	}

	return SM9_ERROR_NONE;
}

int sm9_proxylib_ObjectFromItemsValueCipher(void **object, char C1[SM9_BYTES_LEN_G1], char C3[SM9_BYTES_LEN_BIG], char *C2, int C2_len)
{
	SM9ProxyCipher_SW *ppobject = new SM9ProxyCipher_SW();

	if (C1)
	{
		from_binaryECn(ppobject->C1,C1);
	}

	if (C3)
	{
		from_binaryBig(ppobject->C3,C3);
	}

	if (C2)
	{
		ppobject->C2.SetValue(C2,C2_len);
	}

	*object = (void *)ppobject;

	return SM9_ERROR_NONE;
}

// 秘钥交换R值转换
int sm9_proxylib_ObjectToItemsValueEXR(void *object, char R[SM9_BYTES_LEN_G1])
{
	SM9Object *pobject = (SM9Object *)object;
	int pos = 0;

	if (!pobject)
	{
		return SM9_ERROR_DATA_ERR;
	}

	if (SM9_OBJ_SW_EX_R  != pobject->objectType)
	{
		return SM9_ERROR_DATA_ERR;
	}

	SM9ProxyEXR_SW *ppobject = (SM9ProxyEXR_SW *)object;

	if (R)
	{
		to_binaryECn(ppobject->R,SM9_BYTES_LEN_G1,R);
	}

	return SM9_ERROR_NONE;
}

int sm9_proxylib_ObjectFromItemsValueEXR(void **object, char R[SM9_BYTES_LEN_G1])
{
	SM9ProxyEXR_SW *ppobject = new SM9ProxyEXR_SW();

	if (R)
	{
		from_binaryECn(ppobject->R,R);
	}

	*object = (void *)ppobject;

	return SM9_ERROR_NONE;
}