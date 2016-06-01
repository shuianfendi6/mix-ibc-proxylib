#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>

using namespace std;

#ifdef BENCHMARKING
#include "benchmark.h"
#endif
#include "sm9_proxylib_api.h"
#include "sm9_proxylib.h"
#include "sm9_proxylib_sw.h"

#ifdef BENCHMARKING
static struct timeval gTstart, gTend;
static struct timezone gTz;
extern Benchmark gBenchmark;
#endif
extern Miracl precisionBits;

SM9_OBJ_TYPE getSM9ObjectType(char *c, int *totLen)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	return (SM9_OBJ_TYPE)toint(charToBig(c,totLen));
}


int to_binaryZZn12(const ZZn12 &w, int max, char buffer[384])
{
	ZZn4 w1, w2, w3;
	ZZn2 w11, w12, w21, w22,w31,w32;
	ZZn w111,w112, w121,w122, w211,w212,w221,w222,w311,w312,w321,w322;

	w.get(w1,w2,w3);

	w1.get(w11,w12);
	w2.get(w21,w22);
	w3.get(w31,w32);

	w11.get(w111,w112);
	w21.get(w211,w212);
	w31.get(w311,w312);
	w12.get(w121,w122);
	w22.get(w221,w222);
	w32.get(w321,w322);

	int pos = 0;
	{
		char w_item[32] = {0};

		pos = 0;

		int w_item_len = 0;

		w_item_len = 32;
		w_item_len = to_binary(w322,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w321,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w312,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w311,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w222,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w221,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w212,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w211,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w122,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w121,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w112,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w111,w_item_len,w_item);
		pos += 32 - w_item_len;
		memcpy(buffer+pos, w_item, w_item_len);
		pos += w_item_len;
	}

	return pos;
}

static ECn 
	charToECn (char *c, int *totLen)
{
	ECn e;
	Big x,y;
	int len = 0;
	char *orig = c;
	//   format: 4 bytes length, followed by the big

	memcpy (&len, c, sizeof (int));
	c += sizeof (int);
	x = from_binary (len, c);
	c += len;
	//  cout << "Len1 " << len << " x " << x;

	memcpy (&len, c, sizeof (int));
	c += sizeof (int);
	y = from_binary (len, c);
	c += len;
	//  cout << " Len2 " << len << " y " << y << "\n";

	e.set (x, y);

	*totLen = c - orig;
	return e;
}

static Big
	charToBig (char *c, int *totLen)
{
	Big a;
	int len;
	char *orig = c;

	memcpy (&len, c, sizeof (int));
	c += sizeof (int);
	a = from_binary (len, c);
	c += len;

	*totLen = c - orig;
	return a;
}

static int
	BigTochar (Big &x, char *c, int s)
{
	int len = 0;
	int totlen = sizeof (int);

	//   format: 4 bytes length, followed by the big
	if (s <= sizeof (int))
		return -1;
	// Code assumes epoint contains either nulls or bigs > 0
	s -= sizeof (int);
	c += sizeof (int);
	if (x.iszero()) {
		len = 0;
	} else {
		len = to_binary (x, s, c, FALSE);
	}

	if (len < 0)
		return -1;
	memcpy ((char *)(c - sizeof(int)), (void *)&len, sizeof (int));
	totlen += len;
	s -= len;
	c += len;
	//  cout << "Len1 " << len << " x " << x;

	return totlen;
}

static ZZn2 
	charToZZn2 (char *c, int *totLen)
{
	ZZn2 z;
	int len;
	Big a,b;
	char *orig = c;

	memcpy (&len, c, sizeof (int));
	c += sizeof (int);
	a = from_binary (len, c);
	c += len;
	//  cout << "chartozzn2 a: (" << len << ") " 
	//    << a << "\n";

	memcpy (&len, c, sizeof (int));
	c += sizeof (int);
	b = from_binary (len, c);
	//  cout << "chartozzn2 b: (" << len << ") " 
	//   << b << "\n";
	c += len;

	z.set (a, b);

	*totLen = c - orig;
	return z;
}

static int
	ECnTochar (ECn &e, char *c, int s)
{

	Big x, y;
	e.get(x, y);
	int len = 0;
	int totlen = sizeof (int)*2;

	//  cout << "Entering ECnTochar" << endl;
	//   format: 4 bytes length, followed by the big
	if (s <= sizeof (int))
		return -1;
	// Code assumes epoint contains either nulls or bigs > 0
	s -= sizeof (int);
	c += sizeof (int);
	if (x.iszero()) {
		len = 0;
	} else {  
		len = to_binary (x, s, c, FALSE);
	}
	if (len < 0) {
		return -1;
	}

	memcpy ((char *)(c - sizeof(int)), (void *)&len, sizeof (int));

	totlen += len;
	s -= len;
	c += len;
	//    cout << "Len1 " << len << " x " << x;


	if (s <= sizeof (int))
		return -1;
	s -= sizeof (int);
	c += sizeof (int);
	len = to_binary (y, s, c, FALSE);
	if (len < 0)
		return -1;
	memcpy ((char *)(c - sizeof(int)), (void *)&len, sizeof (int));
	totlen += len;
	//  cout << "Len2 " << len << " y " << y;

	return totlen;
}

static int
	ZZn2Tochar (ZZn2 &z, char *c, int s)
{
	int len = 0;
	int totlen = 2*sizeof(int);
	Big a,b;
	z.get (a, b);

	s -= sizeof (int);
	c += sizeof (int);
	if (a.iszero()) {

		len = 0;
	} else {
		len = to_binary(a, s, c, FALSE);
	}
	if (len < 0)
		return -1;
	*(c - sizeof(int));
	memcpy ((char *)(c - sizeof (int)), (void *)&len, sizeof (int));
	totlen += len;
	s -= len;
	c += len;


	s -= sizeof (int);
	c += sizeof (int);
	if (b.iszero()) {
		len = 0;
	} else {
		len = to_binary(b, s, c, FALSE);
	}
	if (len < 0)
		return -1;
	memcpy ((char *)(c - sizeof (int)), (void *)&len, sizeof (int));
	totlen += len;

	return totlen;
}

static char * t_str = "600000000058F98A";

BOOL 
	sm9_sw_generate_params(SM9CurveParams_SW &params)
{
	miracl *mip=&precisionBits;
	ZZn2 X;

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
	char * xp1_str =       "93DE051D62BF718FF5ED0704487D01D6E1E4086909DC3280E8C4E4817C66DDDD";
	char * yp1_str =       "21FE8DDA4F21E607631065125C395BBC1C1C00CBFA6024350C464CD70A3EA616";
	char * xp2_part1_str = "85AEF3D078640C98597B6027B441A01FF1DD2C190F5E93C454806C11D8806141";
	char * yp2_part1_str = "17509B092E845C1266BA0D262CBEE6ED0736A96FA347C8BD856DC76B84EBEB96";
	char * xp2_part2_str = "3722755292130B08D2AAB97FD34EC120EE265948D19C17ABF9B7213BAF82D65B";
	char * yp2_part2_str = "A7CF28D519BE3DA65F3170153D278FF247EFBA98A71A08116215BBA5C999A7C7";

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

	//cacl ds_hid01
	{
		Big t1,t2, hid01 = 0x01;

		Big id = from_binary(userIDLen, userID);

		Big id_union_hid;

		char buffer[1024];
		int pos = 0;

		pos += to_binary(id,1024,buffer+pos);
		pos += to_binary(hid01,1024,buffer+pos);

		id_union_hid = from_binary(pos, buffer);

		char h1_str[1024] = {0};
		int h1_len = 1024;

		char n_str[1024];
		int n_len = 1024;

		n_len = to_binary(params.N,n_len, n_str);

		SM9_H1(buffer, pos,n_str,n_len, h1_str,&h1_len);

		Big h1 = from_binary(h1_len,h1_str);

		t1 = h1 + msk.master;

		t2 =  pow(msk.master * inverse(t1,params.N),1,params.N);

		sk.ds_hid01 = t2 * params.P1;
	}

	//cacl de_hid02
	{
		Big t1,t2, hid01 = 0x02;

		Big id = from_binary(userIDLen, userID);

		Big id_union_hid;

		char buffer[1024];
		int pos = 0;

		pos += to_binary(id,1024,buffer+pos);
		pos += to_binary(hid01,1024,buffer+pos);

		id_union_hid = from_binary(pos, buffer);

		char h1_str[1024] = {0};
		int h1_len = 1024;

		char n_str[1024];
		int n_len = 1024;

		n_len = to_binary(params.N,n_len, n_str);

		SM9_H1(buffer, pos,n_str,n_len, h1_str,&h1_len);

		Big h1 = from_binary(h1_len,h1_str);

		t1 = h1 + msk.master;

		t2 =  pow(msk.master * inverse(t1,params.N),1,params.N);

		sk.de_hid02 = t2 * params.P2;
	}
	
	{
		Big t1,t2, hid01 = 0x03;

		Big id = from_binary(userIDLen, userID);

		Big id_union_hid;

		char buffer[1024];
		int pos = 0;

		pos += to_binary(id,1024,buffer+pos);
		pos += to_binary(hid01,1024,buffer+pos);

		id_union_hid = from_binary(pos, buffer);

		char h1_str[1024] = {0};
		int h1_len = 1024;

		char n_str[1024];
		int n_len = 1024;

		n_len = to_binary(params.N,n_len, n_str);

		SM9_H1(buffer, pos,n_str,n_len, h1_str,&h1_len);

		Big h1 = from_binary(h1_len,h1_str);

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

	int pos = 0;

	pos = 0;

	char buffer[1024] = {0};

	Big M;

	M = from_binary(messageLen, message);

	pos += to_binary(M,1024,buffer + pos);
	pos += to_binaryZZn12(w,1024,buffer + pos);

	Big m_and_w = from_binary(pos,buffer);

	cout <<"m_and_w:"<<m_and_w<<endl;

	char h2_str[1024] = {0};
	int h2_len = 1024;

	char n_str[1024];
	int n_len = 1024;

	n_len = to_binary(params.N,n_len, n_str);

	SM9_H2(buffer, pos,n_str,n_len, h2_str,&h2_len);

	Big h = from_binary(h2_len,h2_str);

	cout <<"h:"<<h<<endl;

	Big l = (r-h + params.N)%params.N;

	cout <<"l:"<<l<<endl;

	ECn S = l * sk.ds_hid01;

	cout <<"S:"<<S<<endl;

	sgn.h = h;
	sgn.S = S;

	return TRUE;
}


BOOL sm9_sw_verify(SM9CurveParams_SW &params, SM9ProxyMPK_SW &mpk, char *message, int messageLen, char * userID, int userIDLen, SM9ProxySGN_SW &sgn)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	ZZn12 g;
	ZZn2 X;

	set_frobenius_constant(X);

	cout <<"X:"<<X<<endl;

	ecap(mpk.Ppubs,params.P1, params.t, X, g);

	cout <<"g:"<<g<<endl;

	Big t1,t2, hid01 = 0x01;

	Big id = from_binary(userIDLen, userID);

	Big id_union_hid;

	char buffer[1024];
	int pos = 0;

	pos += to_binary(id,1024,buffer+pos);
	pos += to_binary(hid01,1024,buffer+pos);

	id_union_hid = from_binary(pos, buffer);

	char h1_str[1024] = {0};
	int h1_len = 1024;

	char n_str[1024];
	int n_len = 1024;

	n_len = to_binary(params.N,n_len, n_str);

	SM9_H1(buffer, pos,n_str,n_len, h1_str,&h1_len);

	Big h1 = from_binary(h1_len,h1_str);


	cout <<"h1:"<<h1<<endl;
	cout <<"g"<<g<<endl;

	ECn2 P = h1 * params.P2 + mpk.Ppubs;

	cout <<"P:"<<P<<endl;

	ZZn12 tv = pow(g,sgn.h);

	cout <<"tv"<<tv<<endl;

	ZZn12 u;

	ecap(P,sgn.S,params.t,X,u);

	cout <<"u:"<<u<<endl;

	ZZn12 w_ = u * tv;

	cout <<"w_:"<<w_<<endl;

	pos = 0;

	Big M;

	M = from_binary(messageLen, message);

	pos += to_binary(M,1024,buffer + pos);
	pos += to_binaryZZn12(w_,1024,buffer + pos);

	Big m_and_w = from_binary(pos,buffer);

	cout <<"m_and_w:"<<m_and_w<<endl;

	char h2_str[1024] = {0};
	int h2_len = 1024;

	SM9_H2(buffer, pos,n_str,n_len, h2_str,&h2_len);

	Big h2 = from_binary(h2_len,h2_str);

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

int SM9CurveParams_SW::trySerialize(SM9_SERIALIZE_MODE mode,
	char *buffer, int maxBuffer) { 
		int totSize = 0;
		int size = 0;

		char bufferLocal[1024] = {0};
		int bufferLocalLen = 1024;

		if (buffer == NULL)
		{
			buffer = bufferLocal;
			maxBuffer = bufferLocalLen;
		}

		// Set base-16 ASCII encoding
		miracl *mip=&precisionBits;
		mip->IOBASE = 16;

		switch (mode) {
		case SM9_SERIALIZE_BINARY:
			{
				size = BigTochar(Big(this->objectType), buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				size = BigTochar(this->t, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				size = BigTochar(this->cid, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				size = BigTochar(this->q, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				size = BigTochar(this->a, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				size = BigTochar(this->b, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				size = BigTochar(this->cf, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				size = BigTochar(this->N, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				size = BigTochar(this->k, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				size = ECnTochar(this->P1, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				ZZn2 a,b;

				this->P2.get(a,b);

				size = ZZn2Tochar(a, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				size = ZZn2Tochar(b, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				size = BigTochar(this->eid, buffer, maxBuffer - totSize);
				if (size <= 0) return 0;
				totSize += size;
				buffer += size;

				return totSize;
			}
			break;
		case SM9_SERIALIZE_HEXASCII:
			{
				bufferLocalLen = this->trySerialize(SM9_SERIALIZE_BINARY,bufferLocal,maxBuffer);

				Bin2Hex((unsigned char *)bufferLocal,bufferLocalLen,buffer,&maxBuffer);

				return maxBuffer;
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

			this->objectType = (SM9_OBJ_TYPE)toint(charToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			this->t = charToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->cid = charToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->q = charToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->a = charToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->b = charToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->cf = charToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->N = charToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->k = charToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			// Set up the elliptic curve 
#ifdef AFFINE
			ecurve(this->a,this->b,this.q,MR_AFFINE);   
#endif
#ifdef PROJECTIVE
			ecurve(this->a,this->b,this->q,MR_PROJECTIVE);
#endif

			this->P1 = charToECn(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			ZZn2 a,b;

			a = charToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			b = charToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->P2.set(a,b);

			this->eid = charToBig(buffer, &len);
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

int
	SM9ProxyMPK_SW::trySerialize(SM9_SERIALIZE_MODE mode, char *buffer, int maxBuffer)
{
	int totSize = 0;
	int size = 0;

	char bufferLocal[1024] = {0};
	int bufferLocalLen = 1024;

	if (buffer == NULL)
	{
		buffer = bufferLocal;
		maxBuffer = bufferLocalLen;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			size = BigTochar(Big(this->objectType), buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			// real value

			size = ECnTochar(this->Ppube, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			ZZn2 a,b;

			this->Ppubs.get(a,b);

			size = ZZn2Tochar(a, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ZZn2Tochar(b, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			bufferLocalLen = this->trySerialize(SM9_SERIALIZE_BINARY,bufferLocal,maxBuffer);

			Bin2Hex((unsigned char *)bufferLocal,bufferLocalLen,buffer,&maxBuffer);

			return maxBuffer;
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

			this->objectType = (SM9_OBJ_TYPE)toint(charToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			// real value

			this->Ppube = charToECn(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			ZZn2 a,b;

			a = charToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			b = charToZZn2(buffer, &len);
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

	char bufferLocal[1024] = {0};
	int bufferLocalLen = 1024;

	if (buffer == NULL)
	{
		buffer = bufferLocal;
		maxBuffer = bufferLocalLen;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			size = BigTochar(Big(this->objectType), buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochar(this->master, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			bufferLocalLen = this->trySerialize(SM9_SERIALIZE_BINARY,bufferLocal,maxBuffer);

			Bin2Hex((unsigned char *)bufferLocal,bufferLocalLen,buffer,&maxBuffer);

			return maxBuffer;
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

			this->objectType = (SM9_OBJ_TYPE)toint(charToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			this->master = charToBig(buffer, &len);
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

	char bufferLocal[1024] = {0};
	int bufferLocalLen = 1024;

	if (buffer == NULL)
	{
		buffer = bufferLocal;
		maxBuffer = bufferLocalLen;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			size = BigTochar(Big(this->objectType), buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ECnTochar(this->ds_hid01, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			ZZn2 a,b;

			this->de_hid02.get(a,b);

			size = ZZn2Tochar(a, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ZZn2Tochar(b, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			this->de_hid03.get(a,b);

			size = ZZn2Tochar(a, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ZZn2Tochar(b, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;


			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			bufferLocalLen = this->trySerialize(SM9_SERIALIZE_BINARY,bufferLocal,maxBuffer);

			Bin2Hex((unsigned char *)bufferLocal,bufferLocalLen,buffer,&maxBuffer);

			return maxBuffer;
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

			this->objectType = (SM9_OBJ_TYPE)toint(charToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			this->ds_hid01 = charToECn(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			ZZn2 a,b;

			a = charToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			b = charToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->de_hid02.set(a,b);

			a = charToZZn2(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			b = charToZZn2(buffer, &len);
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

	char bufferLocal[1024] = {0};
	int bufferLocalLen = 1024;

	if (buffer == NULL)
	{
		buffer = bufferLocal;
		maxBuffer = bufferLocalLen;
	}

	// Set base-16 ASCII encoding
	miracl *mip=&precisionBits;
	mip->IOBASE = 16;

	switch (mode) {
	case SM9_SERIALIZE_BINARY:
		{
			size = BigTochar(Big(this->objectType), buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = BigTochar(this->h, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			size = ECnTochar(this->S, buffer, maxBuffer - totSize);
			if (size <= 0) return 0;
			totSize += size;
			buffer += size;

			return totSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			bufferLocalLen = this->trySerialize(SM9_SERIALIZE_BINARY,bufferLocal,maxBuffer);

			Bin2Hex((unsigned char *)bufferLocal,bufferLocalLen,buffer,&maxBuffer);

			return maxBuffer;
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

			this->objectType = (SM9_OBJ_TYPE)toint(charToBig(buffer, &len));
			if (len <= 0) return FALSE;
			buffer += len;

			this->h = charToBig(buffer, &len);
			if (len <= 0) return FALSE;
			buffer += len;

			this->S = charToECn(buffer, &len);
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

