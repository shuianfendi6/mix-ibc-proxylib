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

// sm9_sw_generate_params()

static char * t_str = "600000000058F98A";

BOOL 
	sm9_sw_generate_params(SM9CurveParams &params)
{
	miracl *mip=&precisionBits;
	ZZn2 X;

	mip->IOBASE = 16;
	mip->TWIST=MR_SEXTIC_M;
	
	Big t= (char *)t_str;  //����t 

	cout<<"t:"<<t<<endl;

	params.q=36*pow(t,4)+36*pow(t,3)+24*t*t+6*t+1; // ��������
	Big tr=6*t*t+1; // ��
	params.N=params.q+1-tr;   // 
	params.cf=1;
	params.a = 0;
	params.b = 0x05;
	params.cid = 0x12;
	params.k = 12;
	params.eid = 0x04; // R-ate ��


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

//// PRE1_keygen()
////
//// Generate a public/secret keypair for the PRE1 scheme.  
//// Secret keys have the form (a1, a2) \in Z*q x Z*q.
//// Public keys have the form (Z^a1, a2*P) \in G_T x G.
////
//// Where P is the public generator of G, and Z = e(P, P).
//
//BOOL 
//PRE1_keygen(SM9CurveParams &params, ProxyPK_PRE1 &publicKey, ProxySK_PRE1 &secretKey)
//{
//  // Pick random secret key (a1, a2) \in Z*q x Z*q, and store in "secretKey"
//  Big a1=rand(params.q);
//  Big a2=rand(params.q);
//  secretKey.set(a1, a2);
//
//  // Compute the value Z^a1 \in G_T by computing:
//  // Zpub1 = e(P, a1 * P)
//  ECn temp = (a1 * params.P);
//  ZZn2 Zpub1;
//  if (ecap(params.P, temp, params.q, params.cube, Zpub1) == FALSE) {
//    PRINT_DEBUG_STRING("Key generation failed due to pairing operation.");
//    return FALSE;
//  }
//  
//  // Compute the value Ppub2 = (a2 * P) \in G
//  ECn Ppub2 = (a2 * params.P);
//  
//  // Store the values (Zpub1, Ppub2) \in G_T x G in "publicKey"
//  publicKey.set(Zpub1, Ppub2);
//  
//  // Success
//  return TRUE;
//}
//
//// PRE1_level1_encrypt()
////
//// Takes a plaintext and a public key and generates a first-level
//// (non-re-encryptable) ciphertext in the values res1 and res2.
////
//// res1 = Z^{(a_i)k}
//// res2 = (plaintext) * Z^k
//
//BOOL 
//PRE1_level1_encrypt(SM9CurveParams &params, Big &plaintext, ProxyPK_PRE1 &publicKey, ProxyCiphertext_PRE1 &ciphertext)
//{
//#ifdef BENCHMARKING
//  gettimeofday(&gTstart, &gTz);
//#endif
//
//  SAFESTATIC Big k;
//  SAFESTATIC ZZn2 temp, c1, c2;
//  SAFESTATIC ZZn2 zPlaintext;
//
//  // Select a random value k \in Z*q, and compute res1 = Zpub1^k
//  k = rand(params.q);
//
//  c1 = pow(publicKey.Zpub1, k);
//  
//  // Compute res2 = plaintext * Z^k
//  temp = pow(params.Z, k);
//  //cout << "encrypt: params.Z = " << params.Z << endl;
//  //cout << "encrypt: temp = " << temp << endl;
//  zPlaintext.set(plaintext, 0);
//
//  //cout << "encrypt: plaintext = " << zPlaintext << endl;
//  c2 = zPlaintext * temp;
//  //cout << "encrypt: c1 = " << c1 << endl;
//  //cout << "encrypt: c2 = " << c2 << endl;
//
//  // Set the ciphertext data structure with (c1, c2)
//  ciphertext.set(CIPH_FIRST_LEVEL, c1, c2);
//
//#ifdef BENCHMARKING
//  gettimeofday(&gTend, &gTz);
//  gBenchmark.CollectTiming(LEVELONEENCTIMING, CalculateUsecs(gTstart, gTend));
//#endif
//
//  return true;
//}
//
//// PRE1_level2_encrypt()
////
//// Takes a plaintext and a public key and generates a second-level
//// (re-encryptable) ciphertext in the values res1 and res2
////
//// res1 = kP, res2 = (plaintext) * Z^{(a1)k}
//
//BOOL PRE1_level2_encrypt(SM9CurveParams &params, Big &plaintext, ProxyPK_PRE1 &publicKey, ProxyCiphertext_PRE1 &ciphertext)
//{
//#ifdef BENCHMARKING
//  gettimeofday(&gTstart, &gTz);
//#endif
//
//  SAFESTATIC Big k;
//  SAFESTATIC ECn c1;
//  SAFESTATIC ZZn2 temp, c2;
//  SAFESTATIC ZZn2 zPlaintext;
//  
//  // Select a random value k \in Z*q and compute res1 = (k * P)  
//  k = rand(params.q);
//  c1 = k * params.P;
//
//  // Compute res2 = plaintext * Zpub1^k
//  zPlaintext.set(plaintext, 0);
//  temp = pow(publicKey.Zpub1, k);
//  c2 = zPlaintext * temp;
//  
//  // Set the ciphertext structure with (c1, c2)
//  ciphertext.set(CIPH_SECOND_LEVEL, c1, c2);
//
//#ifdef BENCHMARKING
//  gettimeofday(&gTend, &gTz);
//  gBenchmark.CollectTiming(LEVELTWOENCTIMING, CalculateUsecs(gTstart, gTend));
//#endif
//
//  return true;
//}
//
//// PRE1_delegate()
////
//// Given a delegate's public key and the original target's secret key,
//// produce a delegation key that can be used to re-encrypt second
//// level ciphertexts.
////
//// reskey = delegator.a1 * (delegatee.b2 * P)
////
//// A security note: It may be possible for an adversary to use the key 
//// delegation process as an oracle for decryption.  It is recommended that
//// delegators verify the correctness of any delegatee public key
//// by e.g., requiring the delegatee to "prove knowledge" of the secret key.
//
//BOOL PRE1_delegate(SM9CurveParams &params, ProxyPK_PRE1 &delegatee, ProxySK_PRE1 &delegator, DelegationKey_PRE1 &reskey)
//{
//#ifdef BENCHMARKING
//  gettimeofday(&gTstart, &gTz);
//#endif
// 
//  // Compute reskey = delegator.a1 * delegatee.Ppub2
//  reskey = delegator.a1 * (delegatee.Ppub2);
//
//#ifdef BENCHMARKING
//  gettimeofday(&gTend, &gTz);
//  gBenchmark.CollectTiming(DELEGATETIMING, CalculateUsecs(gTstart, gTend));
//#endif
//
//  return true;
//}
//
//// PRE1_reencrypt()
////
//// Given a "second-level" ciphertext (c1, c2), along with a 
//// delegation key, produces a re-encrypted ciphertext.
////
//// res1 = e(kP, s1*s2*P) = Z^{k(s1)(s2)}
//// res2 = c2
//
//BOOL 
//PRE1_reencrypt(SM9CurveParams &params, ProxyCiphertext_PRE1 &origCiphertext, 
//	       DelegationKey_PRE1 &delegationKey, 
//	       ProxyCiphertext_PRE1 &newCiphertext)
//{
//#ifdef BENCHMARKING
//  gettimeofday(&gTstart, &gTz);
//#endif
//
//  SAFESTATIC ZZn2 res1;
//
//  // Compute the pairing res1 = e(kP, delegation)
//  if (ecap(origCiphertext.c1a, delegationKey, params.q, params.cube, res1) == FALSE) {
//    // Pairing failed.  Oops.
//    PRINT_DEBUG_STRING("Re-encryption pairing failed."); 
//    return false;
//  }
//
//  // Set the result ciphertext to (res1, c2)
//  newCiphertext.set(CIPH_REENCRYPTED, res1, origCiphertext.c2);
//  
//#ifdef BENCHMARKING
//  gettimeofday(&gTend, &gTz);
//  gBenchmark.CollectTiming(REENCTIMING, CalculateUsecs(gTstart, gTend));
//#endif
//
//  return true;
//}
//
//// PRE1_decrypt()
////
//// Decrypt a ciphertext and return the plaintext.  This routine handles
//// three different types of ciphertext.
////
//// 1. If this is a first-level ciphertext it will have the form:
////    c1 = Z^{(a1)k}, c2 = plaintext * Z^k
//// 2. If this is a re-encrypted ciphertext, it will have the form:
////    c1 = Z^{k(a1)(a2)}, c2 = plaintext * Z^{(a1)k}
//// 3. If this is a second-level ciphertext, it will have the form:
////	  c1 = kP, c2 = plaintext * Z^{(a1)k}
////
//// To decrypt case 1: plaintext = c2 / c1^inv(a1)
//// To decrypt case 2: plaintext = c2 / c1^inv(a2)
//// To decrypt case 3: plaintext = c2 / e(c1, (delegation = a1 * P))
//
//BOOL PRE1_decrypt(SM9CurveParams &params, ProxyCiphertext_PRE1 &ciphertext, ProxySK_PRE1 &secretKey, Big &plaintext)
//{
//#ifdef BENCHMARKING
//  gettimeofday(&gTstart, &gTz);
//#endif
//
//  SAFESTATIC ECn del;
//  SAFESTATIC ZZn2 temp;
//  SAFESTATIC ZZn2 result;
//
// // Handle each type of ciphertext
// switch(ciphertext.type) {
// case CIPH_FIRST_LEVEL:
//   // temp = c1^inv(a1)
//   temp = pow(ciphertext.c1b, inverse(secretKey.a1, params.qsquared));
//   //cout << "decrypt: temp = " << temp << endl;
//   break;
// case CIPH_REENCRYPTED:
//   // temp = c1^inv(a2)
//   temp = pow(ciphertext.c1b, inverse(secretKey.a2, params.qsquared));
//   break;
// case CIPH_SECOND_LEVEL:
//   // temp = e(c1, a1 * P)
//   del = secretKey.a1 * params.P;
//   if (ecap(ciphertext.c1a, del, params.q, params.cube, temp) == FALSE) {
//     PRINT_DEBUG_STRING("Decryption pairing failed.");
//     return FALSE;
//   }
//   break;
// default:
//   PRINT_DEBUG_STRING("Decryption failed: invalid ciphertext type.");
//   break;
// }
// 
// // Compute plaintext = c2 / temp
// result = ciphertext.c2 / temp;
// result.get(plaintext);
//
//#ifdef BENCHMARKING
//  gettimeofday(&gTend, &gTz);
//  gBenchmark.CollectTiming(LEVELONEDECTIMING, CalculateUsecs(gTstart, gTend));
//#endif
//
//  return true;
//}
//
//// SerializeDelegationKey_PRE1()
////
//// Serialize a delegation key into a buffer
//
//int
//SerializeDelegationKey_PRE1(DelegationKey_PRE1 &delKey, SERIALIZE_MODE mode, char *buffer, int maxBuffer)
//{
//  int totSize = 0;
//
//  // Set base-16 ASCII encoding
//  miracl *mip=&precisionBits;
//  mip->IOBASE = 16;
//
//  switch (mode) {
//  case SERIALIZE_BINARY:
//  {
//    int len = ECnTochar (delKey, buffer, maxBuffer);
//    if (len <= 0) return 0;
//    return len;
//    break;
//  }	
//  case SERIALIZE_HEXASCII:
//
//    //string temp;
//    //buffer << delKey;
//    //temp.append(buffer);
//    //temp.append(ASCII_SEPARATOR);
//
//    //strcpy(buffer, temp.c_str());
//    //return strlen(buffer);
//    return 0;
//    break;
//  }
//
//  // Invalid serialization mode
//  return 0;
//}
//
//// DeserializeDelegationKey_PRE1()
////
//// Deserialize a delegation key from a buffer
//
//BOOL
//DeserializeDelegationKey_PRE1(DelegationKey_PRE1 &delKey, SERIALIZE_MODE mode, char *buffer, int bufSize)
//{
//  // Make sure we've been given a real buffer
//  if (buffer == NULL) {
//    return 0;
//  }
//
//  // Set base-16 ASCII encoding
//  miracl *mip=&precisionBits;
//  mip->IOBASE = 16;
//
//  switch (mode) {
//  case SERIALIZE_BINARY:
//    int len;
//
//    //cout << "got one " << len << endl;
//    delKey = charToECn(buffer, &len);
//    if (len <= 0) return FALSE;
//    return TRUE;
//    break;
//
//  case SERIALIZE_HEXASCII:
//    // Serialize to hexadecimal in ASCII 
//    // TBD
//    return FALSE;
//    break;
//  }
//
//  // Invalid serialization mode
//  return 0;
//}
//
////
//// Class members (ProxyPK_PRE1)
////
//
//int
//ProxyPK_PRE1::getSerializedSize(SERIALIZE_MODE mode)
//{
//  switch (mode) {
//  case SERIALIZE_BINARY:
//    //return (PBITS/8 + 10) * 4;
//    return 2*136;
//    break;
//  case SERIALIZE_HEXASCII:
//    break;
//  }
//
//  // Invalid serialization mode
//  return 0;
//}  
//
//int
//ProxyPK_PRE1::serialize(SERIALIZE_MODE mode, char *buffer, int maxBuffer)
//{
//  int totSize = 0;
//  // Make sure we've been given a large enough buffer
//  if (buffer == NULL || maxBuffer < this->getSerializedSize(mode)) {
//    return 0;
//  }
//
//  // Set base-16 ASCII encoding
//  miracl *mip=&precisionBits;
//  mip->IOBASE = 16;
//
//  switch (mode) {
//  case SERIALIZE_BINARY:
//	{
//
//    int size = ZZn2Tochar(this->Zpub1, buffer, maxBuffer);
//    if (size <= 0) return 0;
//    totSize += size;
//    buffer += size;
//    //cout << "Zpub1: " << this->Zpub1 << endl;
//    //cout << "Ppub2: " << this->Ppub2 << endl;
//    //cout << "zzn size: " << size << endl;
//
//    size = ECnTochar(this->Ppub2, buffer, maxBuffer - totSize);
//    if (size <= 0) return 0;
//    totSize += size;
//    buffer += size;
//
//    return totSize;
//    break;
//	}
//  case SERIALIZE_HEXASCII:
//    // Serialize to hexadecimal in ASCII 
//    Big x, y;
//    string temp;
//    this->Zpub1.get(x, y);
//    buffer << x;
//    temp.append(buffer);
//    temp.append(ASCII_SEPARATOR);
//    buffer << y;
//    temp.append(buffer);
//    temp.append(ASCII_SEPARATOR);
//
//    this->Ppub2.get(x, y);
//    buffer << x;
//    temp.append(buffer);
//    temp.append(ASCII_SEPARATOR);
//    buffer << y;
//    temp.append(buffer);
//    temp.append(ASCII_SEPARATOR);
//
//    strcpy(buffer, temp.c_str());
//    return strlen(buffer);
//    break;
//  }
//
//  // Invalid serialization mode
//  return 0;
//}
//
//BOOL
//ProxyPK_PRE1::deserialize(SERIALIZE_MODE mode, char *buffer, int bufSize)
//{
//  // Make sure we've been given a real buffer
//  if (buffer == NULL) {
//    return 0;
//  }
//
//  // Set base-16 ASCII encoding
//  miracl *mip=&precisionBits;
//  mip->IOBASE = 16;
//
//  switch (mode) {
//  case SERIALIZE_BINARY:
//    int len;
//    this->Zpub1 = charToZZn2(buffer, &len);
//    if (len <= 0) return FALSE;
//    buffer += len;
//    //cout << "got one " << len << endl;
//
//    this->Ppub2 = charToECn(buffer, &len);
//    if (len <= 0) return FALSE;
//    //cout << "Zpub1: " << this->Zpub1 << endl;
//    //cout << "Ppub2: " << this->Ppub2 << endl;
//    return TRUE;
//    break;
//
//  case SERIALIZE_HEXASCII:
//    // Serialize to hexadecimal in ASCII 
//    // TBD
//    return FALSE;
//    break;
//  }
//
//  // Invalid serialization mode
//  return 0;
//}
//
////
//// Class members (ProxySK_PRE1)
////
//
//int
//ProxySK_PRE1::getSerializedSize(SERIALIZE_MODE mode)
//{
//  switch (mode) {
//  case SERIALIZE_BINARY:
//    return (QBITS/8 + 10) * 2;
//    break;
//  case SERIALIZE_HEXASCII:
//    break;
//  }
//
//  // Invalid serialization mode
//  return 0;
//}  
//
//int
//ProxySK_PRE1::serialize(SERIALIZE_MODE mode, char *buffer, int maxBuffer)
//{
//  int totSize = 0;
//
//  // Make sure we've been given a large enough buffer
//  if (buffer == NULL || maxBuffer < this->getSerializedSize(mode)) {
//    return 0;
//  }
//
//  // Set base-16 ASCII encoding
//  miracl *mip=&precisionBits;
//  mip->IOBASE = 16;
//
//  switch (mode) {
//  case SERIALIZE_BINARY:
//	{
//    int len = BigTochar (this->a1, buffer, maxBuffer);
//    if (len <= 0) return 0;
//    buffer += len;
//    totSize += len;
//
//    len = BigTochar (this->a2, buffer, maxBuffer);
//    if (len <= 0) return 0;
//    buffer += len;
//    totSize += len;
//
//    //cout << "a1: " << this->a1 << endl;
//    //cout << "a2: " << this->a2 << endl;
//
//    return totSize;
//
//    break;
//	}
//  case SERIALIZE_HEXASCII:
//
//    string temp;
//    buffer << this->a1;
//    temp.append(buffer);
//    temp.append(ASCII_SEPARATOR);
//    buffer << this->a2;
//    temp.append(buffer);
//    temp.append(ASCII_SEPARATOR);
//
//    strcpy(buffer, temp.c_str());
//    return strlen(buffer);
//    break;
//  }
//
//  // Invalid serialization mode
//  return 0;
//}
//
//BOOL
//ProxySK_PRE1::deserialize(SERIALIZE_MODE mode, char *buffer, int bufSize)
//{
//  // Make sure we've been given a real buffer
//  if (buffer == NULL) {
//    return 0;
//  }
//
//  // Set base-16 ASCII encoding
//  miracl *mip=&precisionBits;
//  mip->IOBASE = 16;
//
//  switch (mode) {
//  case SERIALIZE_BINARY:
//    int len;
//
//    //cout << "got one " << len << endl;
//    this->a1 = charToBig(buffer, &len);
//    if (len <= 0) return FALSE;
//    buffer += len;
//
//    this->a2 = charToBig(buffer, &len);
//    if (len <= 0) return FALSE;
//    //cout << "a1: " << this->a1 << endl;
//    //cout << "a2: " << this->a2 << endl;
//    return TRUE;
//    break;
//
//  case SERIALIZE_HEXASCII:
//    // Serialize to hexadecimal in ASCII 
//    // TBD
//    return FALSE;
//    break;
//  }
//
//  // Invalid serialization mode
//  return 0;
//}
//
////
//// Class members (ProxyCiphertext_PRE1)
////
//
//int
//ProxyCiphertext_PRE1::getSerializedSize(SERIALIZE_MODE mode)
//{
//  switch (mode) {
//  case SERIALIZE_BINARY:
//    //return (PBITS/8 + 10) * 3;
//    return (1+8+136*2);
//    break;
//  case SERIALIZE_HEXASCII:
//    break;
//  }
//
//  // Invalid serialization mode
//  return 0;
//}  
//
//int
//ProxyCiphertext_PRE1::serialize(SERIALIZE_MODE mode, char *buffer, int maxBuffer)
//{
//  int totSize = 0;
//
//  // Make sure we've been given a large enough buffer
//  if (buffer == NULL || maxBuffer < this->getSerializedSize(mode)) {
//
//    return 0;
//  }
//
//  // Set base-16 ASCII encoding
//  miracl *mip=&precisionBits;
//  mip->IOBASE = 16;
//
//  switch (mode) {
//  case SERIALIZE_BINARY:
//	{
//    *buffer = (char)this->type;
//    buffer++;
//    totSize++;
//
//    int len = ECnTochar (this->c1a, buffer, maxBuffer - totSize);
//    if (len <= 0) return 0;
//    buffer += len;
//    totSize += len;
//
//    len = ZZn2Tochar (this->c1b, buffer, maxBuffer - totSize);
//    if (len <= 0) return 0;
//    buffer += len;
//    totSize += len;
//
//    len = ZZn2Tochar (this->c2, buffer, maxBuffer - totSize);
//    if (len <= 0) return 0;
//    buffer += len;
//    totSize += len;
//
//    return totSize;
//
//    break;
//	}
//  case SERIALIZE_HEXASCII:
//
//#if 0
//
//    string temp;
//    buffer << this->c1a;
//    temp.append(buffer);
//    temp.append(ASCII_SEPARATOR);
//    buffer << this->c1b;
//    temp.append(buffer);
//    temp.append(ASCII_SEPARATOR);
//    buffer << this->c2;
//    temp.append(buffer);
//    temp.append(ASCII_SEPARATOR);
//
//    strcpy(buffer, temp.c_str());
//    return strlen(buffer);
//#endif
//
//    return 0;
//    break;
//  }
//
//  // Invalid serialization mode
//  return 0;
//}
//
//BOOL
//ProxyCiphertext_PRE1::deserialize(SERIALIZE_MODE mode, char *buffer, int bufSize)
//{
//  // Make sure we've been given a real buffer
//  if (buffer == NULL) {
//    return 0;
//  }
//
//  // Set base-16 ASCII encoding
//  miracl *mip=&precisionBits;
//  mip->IOBASE = 16;
//
//  switch (mode) {
//  case SERIALIZE_BINARY:
//    int len;
//    this->type = (CIPHERTEXT_TYPE)(*buffer);
//    buffer++;
//
//    //cout << "got one " << len << endl;
//    this->c1a = charToECn(buffer, &len);
//    if (len <= 0) return FALSE;
//    buffer += len;
//
//    this->c1b = charToZZn2(buffer, &len);
//    if (len <= 0) return FALSE;
//    buffer += len;
//
//    this->c2 = charToZZn2(buffer, &len);
//    if (len <= 0) return FALSE;
//    buffer += len;
//
//    //cout << "a1: " << this->a1 << endl;
//    //cout << "a2: " << this->a2 << endl;
//    return TRUE;
//    break;
//
//  case SERIALIZE_HEXASCII:
//    // Serialize to hexadecimal in ASCII 
//    // TBD
//    return FALSE;
//    break;
//  }
//
//  // Invalid serialization mode
//  return 0;
//}
