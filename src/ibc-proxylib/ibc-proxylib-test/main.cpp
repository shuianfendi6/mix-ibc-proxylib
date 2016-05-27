#include <iostream>
#include <fstream>
#include <string.h>
#include "ecn.h"
#include <ctime>
#include "ecn2.h"
#include "zzn12a.h"
#include "sm3.h"
#include "pairing_3.h"

using namespace std;

Miracl precisionBits(2048*10,0);

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


int SM9_HV(unsigned int n,unsigned char * src, unsigned char * digest)
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

	Big Z = from_binary(iZLen,pZ);
	Big n = from_binary(iNLen,pN);

	// step 2
	Big hlen=8*((5*((bits(n))) + 31)/32);
	int v = 256;
	//step 3
	Big h1 = 0;

	int ihlen = toint(hlen);

	HaItem = new unsigned char[ihlen+32];

	for(int i = 0; i < ihlen/v + 1; i++)
	{
		char buffer[1024] = {0};
		Big one = 0x01;
		int pos = 0;
		char ct_str[4] = {0};

		pos += to_binary(one,1024,buffer + pos);

		pos += to_binary(Z,1024,buffer + pos);

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

	Ha = from_binary(ihlen/8,(char *)HaItem);

	cout << "Ha:"<<Ha << endl;
	h1 = pow(Ha,1,n-1) + 1;
	cout << "h1:"<<h1 << endl;

	delete HaItem;

	*piH1Len = to_binary(h1,*piH1Len, pH1);

	return 0;
}


Big SM9_H2(char * pZ,int iZLen, char * pN, int iNLen,char *pH2,int *piH2Len)
{
	miracl *mip=&precisionBits;
	mip->IOBASE=16;

	// step 1
	int ct=0x00000001;

	unsigned char * HaItem = NULL;
	Big Ha = 0;

	Big Z = from_binary(iZLen,pZ);
	Big n = from_binary(iNLen,pN);

	// step 2
	Big hlen=8*((5*((bits(n))) + 31)/32);
	int v = 256;
	//step 3
	Big h2 = 0;

	int ihlen = toint(hlen);

	HaItem = new unsigned char[ihlen+32];

	for(int i = 0; i < ihlen/v + 1; i++)
	{
		char buffer[1024] = {0};
		Big two = 0x02;
		int pos = 0;
		char ct_str[4] = {0};

		pos += to_binary(two,1024,buffer + pos);

		pos += to_binary(Z,1024,buffer + pos);

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

	Ha = from_binary(ihlen/8,(char *)HaItem);

	cout << "Ha:"<<Ha << endl;
	h2 = pow(Ha,1,n-1) + 1;
	cout << "h2:"<<h2 << endl;

	delete HaItem;

	*piH2Len = to_binary(h2,*piH2Len, pH2);

	return 0;
}


Big SM9_H3()
{
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

int main()
{
	for(int bits = 8; bits < 65536; bits++)
	{
		miracl* mip=&precisionBits;

		ECn P1;
		ECn2 Ppub_s,P2;
		ZZn2 X;
		Big q,N;
		// SM9
		Big t,tr;
		Big b = 5;      //方程参数
		Big cf = 1;     //余因子
		Big k = 12;     //嵌入次数
		Big _b;         //扭曲线的参数 sqrt(-2)
		Big d1 = 1;     //k 的因子 d1=1, d2=2 
		Big d2 = 2;     //k 的因子 d1=1, d2=2 
		Big cid = 0x12; //曲线识别符
		Big ks;
		Big xp1,yp1;
		ZZn2 xp2,yp2;
		Big eid = 0x04; //双线性对的识别符

		char * b_str = "05";
		char * xp1_str =       "93DE051D62BF718FF5ED0704487D01D6E1E4086909DC3280E8C4E4817C66DDDD";
		char * yp1_str =       "21FE8DDA4F21E607631065125C395BBC1C1C00CBFA6024350C464CD70A3EA616";
		char * xp2_part1_str = "85AEF3D078640C98597B6027B441A01FF1DD2C190F5E93C454806C11D8806141";
		char * yp2_part1_str = "17509B092E845C1266BA0D262CBEE6ED0736A96FA347C8BD856DC76B84EBEB96";
		char * xp2_part2_str = "3722755292130B08D2AAB97FD34EC120EE265948D19C17ABF9B7213BAF82D65B";
		char * yp2_part2_str = "A7CF28D519BE3DA65F3170153D278FF247EFBA98A71A08116215BBA5C999A7C7";
		char * ks_str =          "0130E78459D78545CB54C587E02CF480CE0B66340F319F348A1D5B1F2DC5F4";
		char * ke_str =          "01EDEE3778F441F8DEA3D9FA0ACC4E07EE36C93F9A08618AF4AD85CEDE1C22";

		Big x0,y0;
		Big hid = 0x01;

		Big IDA;

		ZZn4 w1, w2, w3;
		ZZn2 w11, w12, w21, w22,w31,w32;
		ZZn w111,w112, w121,w122, w211,w212,w221,w222,w311,w312,w321,w322;


		mip->IOBASE=16;
		mip->TWIST=MR_SEXTIC_M;

		//  Curve 2.
		t= (char *)"600000000058F98A";  //参数t

		q=36*pow(t,4)+36*pow(t,3)+24*t*t+6*t+1; // 基域特征
		tr=6*t*t+1; // 迹
		N=q+1-tr;   // 
		cf=1;

		ks = ks_str;


		cout<<"t:"<<t<<endl;
		cout<<"q:"<<q<<endl;
		cout<<"N:"<<N<<endl;
		cout<<"cf:"<<cf<<endl;
		cout<<"k:"<<k<<endl;
		cout<<"ks:"<<ks<<endl;

		IDA = from_binary(strlen("Alice"),"Alice");
		mip->IOBASE=16;

		cout<<"IDA:"<<IDA<<endl;

#ifdef AFFINE
		ecurve((Big)0,(Big)b,q,MR_AFFINE);
#endif
#ifdef PROJECTIVE
		ecurve((Big)0,(Big)b,q,MR_PROJECTIVE);
#endif

		xp1 = xp1_str;
		yp1 = yp1_str;

		P1.set(xp1,yp1);

		cout<<"P1:"<<P1<<endl;

		{
			bool bFlag = false;

			xp2.set((Big)xp2_part2_str, xp2_part1_str);
			yp2.set((Big)yp2_part2_str, yp2_part1_str);
			bFlag = P2.set(xp2,yp2);

			cout<<"bFlag:"<<bFlag<<endl;
			cout<<"P2:"<<P2<<endl;
			if (bFlag)
			{
				cout<<"success:bits=="<<bits<<endl;
				Ppub_s = ks*P2;
				cout<<"Ppub_s:"<<Ppub_s<<endl;
			}
		}

#if 0

		Big IDA_union_hid;
		char buffer[1024];
		int pos = 0;

		pos += to_binary(IDA,1024,buffer+pos);

		pos += to_binary(hid,1024,buffer+pos);

		IDA_union_hid = from_binary(pos, buffer);

		// SM9_H1 param calc success

		cout<<"IDA_union_hid:"<<IDA_union_hid<<endl;

		Big t1 = 0;

		//calc t1

		char h1_str[1024] = {0};
		int h1_len = 1024;

		char n_str[1024];
		int n_len = 1024;

		n_len = to_binary(N,n_len, n_str);

		SM9_H1(buffer, pos,n_str,n_len, h1_str,&h1_len);

		Big h1 = from_binary(h1_len,h1_str);

		t1 = h1 + ks;

		// SM9_H1 calc success

		cout<<"t1:"<<t1<<endl;

		// t1 calc success

		Big t2 =  pow(ks * inverse(t1,N),1,N);

		cout <<"t2:"<<t2<<endl;

		// t2 calc success

		cout <<"xp1:"<<xp1<<endl;
		cout <<"yp1:"<<yp1<<endl;

		//P1.set(yp1,xp1);

		cout <<"P1:"<<P1<<endl;


		ECn dsa = t2*P1;

		cout <<"dsa:"<<dsa<<endl;

		// dsa calc success

		Big M;

		M = from_binary(strlen("Chinese IBS standard"),"Chinese IBS standard");
		cout <<"M:"<<M<<endl;
		// M calc success

		ZZn12 g;

		// R-Ate 对

		set_frobenius_constant(X);

		cout <<"X:"<<X<<endl;

		ecap(Ppub_s,P1,t,X,g);

		cout <<"g:"<<g<<endl;

		Big ww = 0;//"81377B8FDBC2839B4FA2D0E0F8AA6853BBBE9E9C4099608F8612C6078ACD7563815AEBA217AD502DA0F48704CC73CABB3C06209BD87142E14CBD99E8BCA1680F30DADC5CD9E207AEE32209F6C3CA3EC0D800A1A42D33C73153DED47C70A39D2E8EAF5D179A1836B359A9D1D9BFC19F2EFCDB829328620962BD3FDF15F2567F58A543D25609AE943920679194ED30328BB33FD15660BDE485C6B79A7B32B013983F012DB04BA59FE88DB889321CC2373D4C0C35E84F7AB1FF33679BCA575D67654F8624EB435B838CCA77B2D0347E65D5E46964412A096F4150D8C5EDE5440DDF0656FCB663D24731E80292188A2471B8B68AA993899268499D23C89755A1A89744643CEAD40F0965F28E1CD2895C3D118E4F65C9A0E3E741B6DD52C0EE2D25F5898D60848026B7EFB8FCC1B2442ECF0795F8A81CEE99A6248F294C82C90D26BD6A814AAF475F128AEF43A128E37F80154AE6CB92CAD7D1501BAE30F750B3A9BD1F96B08E97997363911314705BFB9A9DBB97F75553EC90FBB2DDAE53C8F68E42";

		cout <<"ww:"<<ww<<endl;

		Big r = "033C8616B06704813203DFD00965022ED15975C662337AED648835DC4B1CBE";

		ZZn12 w = pow(g,r);

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

		cout <<"w:"<<w<<endl;

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

			ww = from_binary(pos,buffer);

			cout <<"ww:"<<ww<<endl;
		}

		pos = 0;

		char buffer2[1024] = {0};

		pos += to_binary(M,1024,buffer2 + pos);

		memcpy(buffer2+pos, buffer, 12 * 32);

		pos += 12 * 32;

		Big m_and_w = from_binary(pos,buffer2);

		cout <<"m_and_w:"<<m_and_w<<endl;

		char h2_str[1024] = {0};
		int h2_len = 1024;

		SM9_H2(buffer2, pos,n_str,n_len, h2_str,&h2_len);

		Big h = from_binary(h2_len,h2_str);

		cout <<"h:"<<h<<endl;

		Big l = (r-h + N)%N;

		cout <<"l:"<<l<<endl;


		ECn S = l * dsa;

		cout <<"S:"<<S<<endl;

		// from here verify sign 
		{
			cout <<"h1:"<<h1<<endl;
			cout <<"g"<<g<<endl;

			ECn2 P = h1 * P2 + Ppub_s;

			cout <<"P:"<<P<<endl;

			ZZn12 tv = pow(g,h);

			cout <<"tv"<<tv<<endl;

			w = tv;
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

				Big tv2 = from_binary(pos,buffer);

				cout <<"tv2:"<<tv2<<endl;
			}

			ZZn12 u;

			ecap(P,S,t,X,u);

			cout <<"u:"<<u<<endl;

			w = u;
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

				Big uu = from_binary(pos,buffer);

				cout <<"uu:"<<uu<<endl;
			}

			ZZn12 w_ = u * tv;

			cout <<"w_:"<<w_<<endl;

			w = w_; // w_ to string
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

				Big ww_ = from_binary(pos,buffer);

				cout <<"ww_:"<<ww_<<endl;
			}

			pos = 0;

			char buffer3[1024] = {0};

			pos += to_binary(M,1024,buffer3 + pos);

			memcpy(buffer3+pos, buffer, 12 * 32);

			pos += 12 * 32;

			Big m_and_w = from_binary(pos,buffer3);

			cout <<"m_and_w:"<<m_and_w<<endl;

			SM9_H2(buffer3, pos,n_str,n_len, h2_str,&h2_len);

			Big h2 = from_binary(h2_len,h2_str);

			cout <<"h2:"<<h2<<endl;
			cout <<"h:"<<h<<endl;

			cout <<"h2-h:"<< h2-h <<endl;
			cout <<"h-h2:"<< h-h2 <<endl;

		}



		// success for verfify
#else
		Big ke = ke_str;

		cout <<"ke:"<<ke<<endl;

		ECn Ppub_e;

		Ppub_e = ke * P1;

		cout <<"Ppub_e:"<<Ppub_e<<endl;

		// Ppub_e ok

		hid = 0x03;

		Big IDB;

		IDB = from_binary(strlen("Bob"),"Bob");

		cout <<"IDB:"<<IDB<<endl;

		Big IDB_union_hid;
		char buffer[1024];
		int pos = 0;

		pos += to_binary(IDB,1024,buffer+pos);

		pos += to_binary(hid,1024,buffer+pos);

		IDB_union_hid = from_binary(pos, buffer);

		// SM9_H1 param calc success

		cout<<"IDB_union_hid:"<<IDB_union_hid<<endl;

		Big t1 = 0;

		//calc t1

		char h1_str[1024] = {0};
		int h1_len = 1024;

		char n_str[1024];
		int n_len = 1024;

		n_len = to_binary(N,n_len, n_str);

		SM9_H1(buffer, pos,n_str,n_len, h1_str,&h1_len);

		Big h1 = from_binary(h1_len,h1_str);

		t1 = h1 + ke;

		// SM9_H1 calc success

		cout<<"t1:"<<t1<<endl;

		Big t2 =  pow(ke * inverse(t1,N),1,N);
		cout<<"t2:"<<t2<<endl;

		ECn2 deB;

		deB = t2 * P2;

		cout<<"deB:"<<deB<<endl;

		int key_wrap_len = 0x0100;

		ECn QB = h1 * P1;

		QB += Ppub_e;

		cout <<"QB:"<<QB<<endl;



		// wrapper
		Big r = "74015F8489C01EF4270456F9E6475BFB602BDE7F33FD482AB4E3684A6722";

		ECn C= r*QB;

		cout <<"C:"<<C<<endl;

		set_frobenius_constant(X);

		ZZn12 g ;

		ecap(P2,Ppub_e,t,X,g);

		cout <<"g:"<<g<<endl;

		ZZn12 w = pow(g,r);

		cout <<"w:"<<w<<endl;

		w = w;
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

			Big ww = from_binary(pos,buffer);

			cout <<"ww:"<<ww<<endl;
		}

		pos = 0;

		char buffer2[1024] = {0};

		Big cx,cy;

		C.get(cx,cy);

		pos += to_binary(cx,1024,buffer2 + pos);

		pos += to_binary(cy,1024,buffer2 + pos);

		memcpy(buffer2+pos, buffer, 12 * 32);

		pos += 12 * 32;

		pos += to_binary(IDB,1024,buffer2 + pos);

		Big value_union = from_binary(pos,buffer2);

		cout <<"value_union:"<<value_union<<endl;

		char key_wrap_data[0x100];

		tcm_kdf((unsigned char *)key_wrap_data,key_wrap_len,(unsigned char *)buffer2,pos);

		Big K = from_binary(key_wrap_len/8, key_wrap_data);

		cout <<"K:"<<K<<endl;

		// unwrapper

		ZZn12 w_;

		ecap(deB, C,t,X,w_);

		cout <<"w_:"<<w_<<endl;

		Big ww;

		w = w_;
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

			Big ww_ = from_binary(pos,buffer);

			cout <<"ww_:"<<ww_<<endl;
		}

		pos = 0;

		pos += to_binary(cx,1024,buffer2 + pos);

		pos += to_binary(cy,1024,buffer2 + pos);

		memcpy(buffer2+pos, buffer, 12 * 32);

		pos += 12 * 32;

		pos += to_binary(IDB,1024,buffer2 + pos);

		value_union = from_binary(pos,buffer2);

		cout <<"value_union:"<<value_union<<endl;

		tcm_kdf((unsigned char *)key_wrap_data,key_wrap_len,(unsigned char *)buffer2,pos);

		Big K_ = from_binary(key_wrap_len/8, key_wrap_data);

		cout <<"K_:"<<K_<<endl;

		cout <<"K_-K:"<<K_-K<<endl;
		cout <<"K-K_:"<<K-K_<<endl;
#endif

		cout <<"success:"<<bits<<endl;

	}

	return 0;
}