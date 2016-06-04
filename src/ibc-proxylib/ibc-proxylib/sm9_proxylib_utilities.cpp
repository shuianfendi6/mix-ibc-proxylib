#include <iostream>
#include <fstream>
#include <string>
#include "ecn.h"
#include "zzn2.h"

#include <ctime>
#include "ecn2.h"
#include "zzn12a.h"
#include "sm3.h"
#include "pairing_3.h"

using namespace std;

#include "sm9_proxylib_api.h"
#include "sm9_proxylib.h"
#ifdef BENCHMARKING
#include "benchmark.h"
#endif

using namespace std;

Miracl precisionBits(32,0);

#ifdef MR_COUNT_OPS
extern "C"
{
	int fpc=0;
	int fpa=0;
	int fpx=0;
	int fpm2=0;
	int fpi2=0;
	int fpmq=0;
	int fpsq=0;
	int fpaq=0;
}
#endif

#ifdef MR_AFFINE_ONLY
#define AFFINE
#else
#define PROJECTIVE
#endif


SM9_OBJ_TYPE getSM9ObjectType(char *c, int *totLen)
{
	miracl *mip=&precisionBits;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;

	return (SM9_OBJ_TYPE)toint(charToBig(c,totLen));
}


ECn  charToECn (char *c, int *totLen)
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

Big charToBig (char *c, int *totLen)
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

int BigTochar (Big &x, char *c, int s)
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

ZZn2 charToZZn2 (char *c, int *totLen)
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

int ECnTochar (ECn &e, char *c, int s)
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

int ZZn2Tochar (ZZn2 &z, char *c, int s)
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

int to_binaryZZn12(const ZZn12 &w, int max, char output[384])
{
	ZZn4 w1, w2, w3;
	ZZn2 w11, w12, w21, w22,w31,w32;
	ZZn w111,w112, w121,w122, w211,w212,w221,w222,w311,w312,w321,w322;

	memset(output, 0, 384);

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

	int pos_tmp = 0;
	{
		char w_item[32] = {0};

		pos_tmp = 0;

		int w_item_len = 0;

		w_item_len = 32;
		w_item_len = to_binary(w322,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w321,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w312,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w311,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w222,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w221,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w212,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w211,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w122,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w121,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w112,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w111,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;
	}

	return pos_tmp;
}

int to_binaryBig(const Big &w, int max, char output[32])
{
	memset(output, 0, 32);

	int pos_tmp = 0;
	{
		char w_item[32] = {0};

		pos_tmp = 0;

		int w_item_len = 0;

		w_item_len = 32;
		w_item_len = to_binary(w,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;
	}

	return pos_tmp;
}


int to_binaryChar(const char &w, int max, char output[1])
{
	memset(output, 0, 1);

	int pos_tmp = 1;

	output[0] = w;

	return pos_tmp;
}

int to_binaryZZn2(const ZZn2 &w, int max, char output[64])
{
	ZZn w1,w2;

	memset(output, 0, 64);

	w.get(w1,w2);

	int pos_tmp = 0;
	{
		char w_item[32] = {0};

		pos_tmp = 0;

		int w_item_len = 0;

		w_item_len = 32;
		w_item_len = to_binary(w2,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;

		w_item_len = 32;
		w_item_len = to_binary(w1,w_item_len,w_item);
		pos_tmp += 32 - w_item_len;
		memcpy(output + pos_tmp, w_item, w_item_len);
		pos_tmp += w_item_len;
	}

	return pos_tmp;
}

int SM9_HV(unsigned int n,unsigned char * src, unsigned char digest[32])
{
	return tcm_sch_hash(n,src,digest);
}

int SM9_H1(char * pZ,int iZLen, char * pN, int iNLen,char *pH1,int *piH1Len)
{
	miracl *mip=&precisionBits;
	mip->IOBASE=16;

	// step 1
	int ct=0x00000001;

	unsigned char * HaItem = NULL;
	Big Ha = 0;

	Big n = from_binary(iNLen,pN);

	// step 2
	Big hlen=8*((5*((bits(n))) + 31)/32);
	int v = 256;
	//step 3
	Big h1 = 0;

	int ihlen = toint(hlen);

	HaItem = new unsigned char[ihlen+32];

	int bufferLen = 1+iZLen+4;
	char * buffer = new char[1+iZLen+4];

	for(int i = 0; i < ihlen/v + 1; i++)
	{
		Big one = 0x01;
		int pos = 0;
		char ct_str[4] = {0};

		memset(buffer,0,bufferLen);

		pos += to_binary(one,1024,buffer + pos);

		memcpy(buffer+pos,pZ,iZLen);
		pos += iZLen;

		for (int j = 0; j < 4; j++)
		{
			ct_str[j] = ((char*)&ct)[4- 1 - j];
		}

		memcpy(buffer + pos,ct_str,4);

		pos += 4;

		//pos += to_binary(ct,1024,buffer + pos);

		SM9_HV(pos, (unsigned char *)buffer, HaItem+SM3_DIGEST_LEN*i);

		ct++;
	}

	delete buffer;

	Ha = from_binary(ihlen/8,(char *)HaItem);

	cout << "Ha:"<<Ha << endl;
	h1 = pow(Ha,1,n-1) + 1;
	cout << "h1:"<<h1 << endl;

	delete HaItem;

	*piH1Len = to_binary(h1,*piH1Len, pH1);

	return 0;
}


int SM9_H2(char * pZ,int iZLen, char * pN, int iNLen,char *pH2,int *piH2Len)
{
	miracl *mip=&precisionBits;
	mip->IOBASE=16;

	// step 1
	int ct=0x00000001;

	unsigned char * HaItem = NULL;
	Big Ha = 0;

	Big n = from_binary(iNLen,pN);

	// step 2
	Big hlen=8*((5*((bits(n))) + 31)/32);
	int v = 256;
	//step 3
	Big h2 = 0;

	int ihlen = toint(hlen);

	HaItem = new unsigned char[ihlen+32];

	int bufferLen = 1+iZLen+4;
	char * buffer = new char[1+iZLen+4];

	for(int i = 0; i < ihlen/v + 1; i++)
	{
		Big two = 0x02;
		int pos = 0;
		char ct_str[4] = {0};

		memset(buffer,0,bufferLen);

		pos += to_binary(two,1024,buffer + pos);

		memcpy(buffer+pos,pZ,iZLen);
		pos += iZLen;

		for (int j = 0; j < 4; j++)
		{
			ct_str[j] = ((char*)&ct)[4- 1 - j];
		}

		memcpy(buffer + pos,ct_str,4);

		pos += 4;

		SM9_HV(pos, (unsigned char *)buffer, HaItem+SM3_DIGEST_LEN*i);

		ct++;
	}

	delete buffer;

	Ha = from_binary(ihlen/8,(char *)HaItem);

	cout << "Ha:"<<Ha << endl;
	h2 = pow(Ha,1,n-1) + 1;
	cout << "h2:"<<h2 << endl;

	delete HaItem;

	*piH2Len = to_binary(h2,*piH2Len, pH2);

	return 0;
}


int SM9_MAC(char * pK, int iKLen, char * pZ,int iZLen, char pMac[32])
{
	char * buffer = new char[iKLen + iZLen];

	memcpy(buffer,pZ,iZLen);
	memcpy(buffer+iZLen,pK,iKLen);

	SM9_HV(iKLen + iZLen,(unsigned char *)buffer,(unsigned char *)pMac);

	if(buffer)
	{
		delete buffer;
	}

	return 0;
}


#define PROJECTIVE


void set_frobenius_constant(ZZn2 &X)
{
	Big p=get_modulus();
	switch (get_mip()->pmod8)
	{
	case 5:
		X.set((Big)0,(Big)1); // = (sqrt(-2)^(p-1)/2     
		break;
	case 3:                    // = (1+sqrt(-1))^(p-1)/2                                
		X.set((Big)1,(Big)1);      
		break;
	case 7: 
		X.set((Big)2,(Big)1); // = (2+sqrt(-1))^(p-1)/2
	default: break;
	}
	X=pow(X,(p-1)/6);
}

//
// Line from A to destination C. Let A=(x,y)
// Line Y-slope.X-c=0, through A, so intercept c=y-slope.x
// Line Y-slope.X-y+slope.x = (Y-y)-slope.(X-x) = 0
// Now evaluate at Q -> return (Qy-y)-slope.(Qx-x)
//

ZZn12 line(ECn2& A,ECn2& C,ECn2& B,ZZn2& slope,ZZn2& extra,BOOL Doubling,ZZn& Qx,ZZn& Qy)
{
	ZZn12 w;
	ZZn4 nn,dd,cc;
	ZZn2 X,Y;

#ifdef AFFINE
	A.get(X,Y);

	if (get_mip()->TWIST==MR_SEXTIC_M)
	{
		nn.set(txx((ZZn2)-Qy),Y-slope*X);
		cc.seth(slope*Qx);
	}
	if (get_mip()->TWIST==MR_SEXTIC_D)
	{
		nn.set((ZZn2)-Qy,Y-slope*X);
		dd.set(slope*Qx);
	}
	w.set(nn,dd,cc);

#endif
#ifdef PROJECTIVE
	ZZn2 Z3;

	C.getZ(Z3);

	// Thanks to A. Menezes for pointing out this optimization...
	if (Doubling)
	{
		ZZn2 Z,ZZ;
		A.get(X,Y,Z);
		ZZ=Z; ZZ*=ZZ;
		if (get_mip()->TWIST==MR_SEXTIC_M)
		{ // "multiplied across" by i to simplify
			nn.set((Z3*ZZ)*txx((ZZn2)Qy),slope*X-extra);
			cc.seth(-(ZZ*slope)*Qx);
		}
		if (get_mip()->TWIST==MR_SEXTIC_D)
		{
			nn.set((Z3*ZZ)*Qy,slope*X-extra);
			dd.set(-(ZZ*slope)*Qx);
		}
	}
	else
	{
		ZZn2 X2,Y2;
		B.get(X2,Y2);
		if (get_mip()->TWIST==MR_SEXTIC_M)
		{
			nn.set(Z3*txx((ZZn2)Qy),slope*X2-Y2*Z3);
			cc.seth(-slope*Qx);
		}
		if (get_mip()->TWIST==MR_SEXTIC_D)
		{
			nn.set(Z3*Qy,slope*X2-Y2*Z3);
			dd.set(-slope*Qx);
		}
	}
	w.set(nn,dd,cc);
#endif

	return w;
}

void endomorph(ECn &A,ZZn &Beta)
{ // apply endomorphism (x,y) = (Beta*x,y) where Beta is cube root of unity
	ZZn x;
	x=(A.get_point())->X;
	x*=Beta;
	copy(getbig(x),(A.get_point())->X);
}

void q_power_frobenius(ECn2 &A,ZZn2 &F)
{ 
	// Fast multiplication of A by q (for Trace-Zero group members only)
	ZZn2 x,y,z,w,r;

#ifdef AFFINE
	A.get(x,y);
#else
	A.get(x,y,z);
#endif

	w=F*F;
	r=F;

	if (get_mip()->TWIST==MR_SEXTIC_M) r=inverse(F);  // could be precalculated
	if (get_mip()->TWIST==MR_SEXTIC_D) r=F;

	w=r*r;
	x=w*conj(x);
	y=r*w*conj(y);

#ifdef AFFINE
	A.set(x,y);
#else
	z.conj();
	A.set(x,y,z);

#endif
}

//
// Faster Hashing to G2 - Fuentes-Castaneda, Knapp and Rodriguez-Henriquez
//

void cofactor(ECn2& S,ZZn2 &F,Big& x)
{
	ECn2 T,K;
	T=S;
	T*=x;
	T.norm();
	K=(T+T)+T;
	K.norm();
	q_power_frobenius(K,F);
	q_power_frobenius(S,F); q_power_frobenius(S,F); q_power_frobenius(S,F); 
	S+=T; S+=K;
	q_power_frobenius(T,F); q_power_frobenius(T,F);
	S+=T;
	S.norm();
}

//
// Add A=A+B  (or A=A+A) 
// Return line function value
//

ZZn12 g(ECn2& A,ECn2& B,ZZn& Qx,ZZn& Qy)
{
	ZZn2 lam,extra;
	ZZn12 r;
	ECn2 P=A;
	BOOL Doubling;

	// Evaluate line from A
	Doubling=A.add(B,lam,extra);

	if (A.iszero())   return (ZZn12)1; 
	r=line(P,A,B,lam,extra,Doubling,Qx,Qy);

	return r;
}

//
// R-ate Pairing G2 x G1 -> GT
//
// P is a point of order q in G1. Q(x,y) is a point of order q in G2. 
// Note that P is a point on the sextic twist of the curve over Fp^2, Q(x,y) is a point on the 
// curve over the base field Fp
//

BOOL fast_pairing(ECn2& P,ZZn& Qx,ZZn& Qy,Big &x,ZZn2 &X,ZZn12& res)
{ 
	ECn2 A,KA;
	ZZn2 AX,AY;
	int i,nb;
	Big n;
	ZZn12 r;
	ZZn12 t0,t1;
	ZZn12 x0,x1,x2,x3,x4,x5;

#ifdef MR_COUNT_OPS
	fpc=fpa=fpx=fpmq=fpsq=fpaq=0;
#endif

	if (x<0) n=-(6*x+2);
	else n=6*x+2;
	A=P;
	nb=bits(n);
	r=1;
	// Short Miller loop
	r.mark_as_miller();

	for (i=nb-2;i>=0;i--)
	{
		r*=r;
		r*=g(A,A,Qx,Qy);
		if (bit(n,i))
			r*=g(A,P,Qx,Qy);
	}
	// Combining ideas due to Longa, Aranha et al. and Naehrig
	KA=P;
	q_power_frobenius(KA,X);
	if (x<0) {A=-A; r.conj();}
	r*=g(A,KA,Qx,Qy);
	q_power_frobenius(KA,X); KA=-KA;
	r*=g(A,KA,Qx,Qy);

#ifdef MR_COUNT_OPS
	cout << "Miller fpc= " << fpc << endl;
	cout << "Miller fpa= " << fpa << endl;
	cout << "Miller fpx= " << fpx << endl;
	cout << "Miller fpmq= " << fpmq << endl;
	cout << "Miller fpsq= " << fpsq << endl;
	cout << "Miller fpaq= " << fpaq << endl;

	fpa=fpc=fpx=fpmq=fpsq=fpaq=0;
#endif
	if (r.iszero()) return FALSE;

	// The final exponentiation

	t0=r;

	r.conj();

	r/=t0;    // r^(p^6-1)
	r.mark_as_regular();  // no longer "miller"

	t0=r;
	r.powq(X); r.powq(X);
	r*=t0;    // r^[(p^6-1)*(p^2+1)]

	r.mark_as_unitary();  // from now on all inverses are just conjugates !! (and squarings are faster)

	res=r;
	// Newer new idea...
	// See "On the final exponentiation for calculating pairings on ordinary elliptic curves" 
	// Michael Scott and Naomi Benger and Manuel Charlemagne and Luis J. Dominguez Perez and Ezekiel J. Kachisa 


	t0=res;    t0.powq(X);
	x0=t0;   x0.powq(X);

	x0*=(res*t0);
	x0.powq(X);

	x1=inverse(res);  // just a conjugation!

	x4=pow(res,-x);  // x is sparse..
	x3=x4; x3.powq(X);

	x2=pow(x4,-x);
	x5=inverse(x2);
	t0=pow(x2,-x);

	x2.powq(X); 
	x4/=x2;

	x2.powq(X);

	res=t0; res.powq(X); t0*=res;

	t0*=t0;
	t0*=x4;
	t0*=x5;
	res=x3*x5;
	res*=t0;
	t0*=x2;
	res*=res;
	res*=t0;
	res*=res;
	t0=res*x1;
	res*=x0;
	t0*=t0;
	t0*=res;

#ifdef MR_COUNT_OPS
	cout << "FE fpc= " << fpc << endl;
	cout << "FE fpa= " << fpa << endl;
	cout << "FE fpx= " << fpx << endl;
	cout << "FE fpmq= " << fpmq << endl;
	cout << "FE fpsq= " << fpsq << endl;
	cout << "FE fpaq= " << fpaq << endl;
	fpa=fpc=fpx=fpmq=fpsq=fpaq=0;
#endif

	res= t0; 
	return TRUE;
}

BOOL ecap(ECn2& P,ECn& Q,Big& x,ZZn2 &X,ZZn12& r)
{
	BOOL Ok;
	Big xx,yy;
	ZZn Qx,Qy;

	P.norm();

	cout<<"P:"<<P<<endl;

	Q.get(xx,yy); Qx=xx; Qy=yy;

	Ok=fast_pairing(P,Qx,Qy,x,X,r);

	if (Ok) return TRUE;
	return FALSE;
}

BOOL member(ZZn12 r,Big &x,ZZn2 &X)
{
	ZZn12 w=r;
	w.powq(X);
	r=pow(r,x); r=pow(r,x); r=pow(r,(Big)6); // t-1=6x^2
	if (w==r) return TRUE;
	return FALSE;
}

// Use Galbraith & Scott Homomorphism idea ...

void galscott(Big &e,Big &r,Big WB[4],Big B[4][4],Big u[4])
{
	int i,j;
	Big v[4],w;

	for (i=0;i<4;i++)
	{
		v[i]=mad(WB[i],e,(Big)0,r,w);
		u[i]=0;
	}

	u[0]=e;
	for (i=0;i<4;i++)
		for (j=0;j<4;j++)
			u[i]-=v[j]*B[j][i];
	return;
}

// GLV method

void glv(Big &e,Big &r,Big W[2],Big B[2][2],Big u[2])
{
	int i,j;
	Big v[2],w;
	for (i=0;i<2;i++)
	{
		v[i]=mad(W[i],e,(Big)0,r,w);
		u[i]=0;
	}
	u[0]=e;
	for (i=0;i<2;i++)
		for (j=0;j<2;j++)
			u[i]-=v[j]*B[j][i];
	return;
}

// Use GLV endomorphism idea for multiplication in G1


ECn G1_mult(ECn &P,Big &e,ZZn &Beta,Big &r,Big W[2],Big B[2][2])
{
	//	return e*P;
	int i;
	ECn Q;
	Big u[2];
	glv(e,r,W,B,u);

	Q=P;
	endomorph(Q,Beta);

	Q=mul(u[0],P,u[1],Q);

	return Q;
}

//.. for multiplication in G2

ECn2 G2_mult(ECn2 &P,Big &e,ZZn2 &X,Big &r,Big WB[4],Big B[4][4])
{
	//	return e*P;
	int i;
	ECn2 Q[4];
	Big u[4];
	galscott(e,r,WB,B,u);

	Q[0]=P;
	for (i=1;i<4;i++)
	{
		Q[i]=Q[i-1]; 
		q_power_frobenius(Q[i],X);
	}

	// deal with -ve multipliers
	for (i=0;i<4;i++)
	{
		if (u[i]<0)
		{u[i]=-u[i];Q[i]=-Q[i];}
	}

	// simple multi-addition
	return mul4(Q,u);
}

//.. and for exponentiation in GT

ZZn12 GT_pow(ZZn12 &res,Big &e,ZZn2 &X,Big &r,Big WB[4],Big B[4][4])
{
	//	return pow(res,e);
	int i,j;
	ZZn12 Y[4];
	Big u[4];

	galscott(e,r,WB,B,u);

	Y[0]=res;
	for (i=1;i<4;i++)
	{Y[i]=Y[i-1]; Y[i].powq(X);}

	// deal with -ve exponents
	for (i=0;i<4;i++)
	{
		if (u[i]<0)
		{u[i]=-u[i];Y[i].conj();}
	}

	// simple multi-exponentiation
	return pow(4,Y,u);
}

#define CHAR_TO_16(achar) ((achar)>='0'&&(achar)<='9'?((achar)-'0'):((((((achar)>='A'  )&&( (achar)<='Z' )  ))? ((achar)-'A'): ((achar)-'a')) + 10))

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen)
{
	long i;
	unsigned long sn_len = ulINLen / 2;

	if(sn_len > *pulOUTLen)
	{
		*pulOUTLen = sn_len;
		return -1;
	}

	*pulOUTLen = sn_len;

	if(0 == pbOUT)
	{

	}
	else
	{
		memset(pbOUT, 0,sn_len);
		for (i = 0; i < sn_len; i++) {
			pbOUT[i] += CHAR_TO_16(*(pbIN + i * 2)) * 16;
			pbOUT[i] += CHAR_TO_16(*(pbIN + i * 2 + 1));
		}
	}

	return 0;
}

extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen)
{
	long i;
	unsigned long sn_len = ulINLen * 2;

	if(sn_len > *pulOUTLen)
	{
		*pulOUTLen = sn_len;
		return -1;
	}

	*pulOUTLen = sn_len;

	if(0 == pbOUT)
	{

	}
	else
	{
		for (i = 0; i < ulINLen; i++) {
			sprintf(pbOUT + 2 * i, "%02X", pbIN[i]);
		}
	}

	return 0;
}
