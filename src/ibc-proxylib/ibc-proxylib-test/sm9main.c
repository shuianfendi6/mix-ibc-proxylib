
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "sm9_proxylib_api.h"

#define NUMENCRYPTIONS 100

int main()
{
	char buffer[1000];

	void *sk1 = NULL;
	void *pk1 = NULL;
	void *sk2 = NULL;
	void *pk2 = NULL;
	void* delKey = NULL;
	int serTestResult = 1;
	int bufsize1 = 1024;
	int bufsize2 = 1024;
	char buffer1[1024] = {0};
	char buffer2[1024] = {0};
	int bufferSize = 1024;

	char data_value[2048] = {0};
	int data_len = 2048;

	void *gParams = 0;
	void *msk = 0;
	void *mpk = 0;
	void *sk = 0;
	void *skb = 0;
	void *sgn = 0;
	void *key = 0;
	void *wrapkey = 0;
	void *cipher = 0;
	void *plain=0;
	void *RA = 0;
	void *RB = 0;
	void *SKB = 0;
	void *SB = 0;

	char * message = "Chinese IBS standard";
	int messageLen = strlen("Chinese IBS standard");
#if 0
	// sign verify start

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(gParams, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(gParams,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(gParams);
	sm9_proxylib_deserializeObject(data_value, data_len, &gParams,SM9_SERIALIZE_BINARY);

	sm9_proxylib_generateMasterKeys(gParams, &mpk,&msk,SM9_SCHEME_SW);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(mpk, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(mpk,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(mpk);
	sm9_proxylib_deserializeObject(data_value, data_len, &mpk,SM9_SERIALIZE_BINARY);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(msk, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(msk,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(msk);
	sm9_proxylib_deserializeObject(data_value, data_len, &msk,SM9_SERIALIZE_BINARY);

	sm9_proxylib_calculateUserKeys(gParams,msk,"Alice",strlen("Alice"),&sk,SM9_SCHEME_SW);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(sk, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(sk,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(sk);
	sm9_proxylib_deserializeObject(data_value, data_len, &sk,SM9_SERIALIZE_BINARY);

	sm9_proxylib_sign(gParams,mpk,sk,message,messageLen,&sgn,SM9_SCHEME_SW);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(sgn, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(sgn,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(sgn);
	sm9_proxylib_deserializeObject(data_value, data_len, &sgn,SM9_SERIALIZE_BINARY);

	sm9_proxylib_verify(gParams,mpk,"Alice",strlen("Alice"),message,messageLen,sgn,SM9_SCHEME_SW);
	
	// clear objects
	sm9_proxylib_destroyObject(gParams);
	sm9_proxylib_destroyObject(mpk);
	sm9_proxylib_destroyObject(msk);
	sm9_proxylib_destroyObject(sk);
	sm9_proxylib_destroyObject(sgn);

	// sign verify end

#elif 0

	// crypto and wrap start
	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);
	sm9_proxylib_generateMasterKeys(gParams, &mpk,&msk,SM9_SCHEME_SW);
	sm9_proxylib_calculateUserKeys(gParams,msk,"Bob",strlen("Bob"),&sk,SM9_SCHEME_SW);

	sm9_proxylib_wrap(gParams,mpk,"Bob",strlen("Bob"),NULL,0,&key,&wrapkey,SM9_SCHEME_SW);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(key, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(key,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(key);
	sm9_proxylib_deserializeObject(data_value, data_len, &key,SM9_SERIALIZE_BINARY);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(wrapkey, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(wrapkey,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(wrapkey);
	sm9_proxylib_deserializeObject(data_value, data_len, &wrapkey,SM9_SERIALIZE_BINARY);

	sm9_proxylib_destroyObject(key);
	
	sm9_proxylib_unwrap(gParams,mpk,sk,"Bob",strlen("Bob"),wrapkey,&key,SM9_SCHEME_SW);

	message = "Chinese IBE standard";
	messageLen = strlen("Chinese IBE standard");

	sm9_proxylib_encrypt(gParams,mpk,"Bob",strlen("Bob"),message,messageLen,&cipher,SM9_CIPHER_KDF_UNION,SM9_SCHEME_SW);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(cipher, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(cipher,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(cipher);
	sm9_proxylib_deserializeObject(data_value, data_len, &cipher,SM9_SERIALIZE_BINARY);

	sm9_proxylib_decrypt(gParams,mpk,sk,"Bob",strlen("Bob"),cipher,&plain,SM9_CIPHER_KDF_UNION, SM9_SCHEME_SW);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(plain, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(plain,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(plain);
	sm9_proxylib_deserializeObject(data_value, data_len, &plain,SM9_SERIALIZE_BINARY);

	sm9_proxylib_destroyObject(cipher);
	sm9_proxylib_destroyObject(plain);
	sm9_proxylib_destroyObject(key);
	sm9_proxylib_destroyObject(wrapkey);

	//clear objects
	sm9_proxylib_destroyObject(gParams);
	sm9_proxylib_destroyObject(mpk);
	sm9_proxylib_destroyObject(msk);
	sm9_proxylib_destroyObject(sk);

	//// crypto and wrap end

#elif 1

	// key ex start
	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);
	sm9_proxylib_generateMasterKeys(gParams, &mpk,&msk,SM9_SCHEME_SW);
	sm9_proxylib_calculateUserKeys(gParams,msk,"Alice",strlen("Alice"),&sk,SM9_SCHEME_SW);
	sm9_proxylib_calculateUserKeys(gParams,msk,"Bob",strlen("Bob"),&skb,SM9_SCHEME_SW);

	sm9_proxylib_keyExchangeA1(gParams,mpk,"Bob",strlen("Bob"),&RA,SM9_SCHEME_SW);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(RA, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(RA,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(RA);
	RA = NULL;
	sm9_proxylib_deserializeObject(data_value, data_len, &RA,SM9_SERIALIZE_BINARY);

	sm9_proxylib_keyExchangeB2(gParams,mpk,skb,"Alice",strlen("Alice"),"Bob",strlen("Bob"), 0x80/8, RA,&RB,&SKB,&SB,SM9_SCHEME_SW);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(RB, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(RB,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(RB);
	sm9_proxylib_deserializeObject(data_value, data_len, &RB,SM9_SERIALIZE_BINARY);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(SKB, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(SKB,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(SKB);
	sm9_proxylib_deserializeObject(data_value, data_len, &SKB,SM9_SERIALIZE_BINARY);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(SB, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(SB,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(SB);
	sm9_proxylib_deserializeObject(data_value, data_len, &SB,SM9_SERIALIZE_BINARY);






	//sm9_proxylib_keyExchangeResponder_step2(gParams,mpk,"Bob",strlen("Bob"),NULL,0,&key,&wrapkey,SM9_SCHEME_SW);
	//sm9_proxylib_keyExchangeInitiator_step3(gParams,mpk,"Bob",strlen("Bob"),NULL,0,&key,&wrapkey,SM9_SCHEME_SW);
	//sm9_proxylib_keyExchangeInitiator_step4(gParams,mpk,"Bob",strlen("Bob"),NULL,0,&key,&wrapkey,SM9_SCHEME_SW);



	//clear objects
	sm9_proxylib_destroyObject(gParams);
	sm9_proxylib_destroyObject(mpk);
	sm9_proxylib_destroyObject(msk);
	sm9_proxylib_destroyObject(sk);

#else
	
#endif

	//key ex end

	return 0;
}
