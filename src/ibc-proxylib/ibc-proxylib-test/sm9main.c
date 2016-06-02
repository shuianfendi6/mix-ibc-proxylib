
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
	void *sgn = 0;
	void *key = 0;
	void *wrapkey = 0;
	void *cipher = 0;
	void *plain=0;

	char * message = "Chinese IBS standard";
	int messageLen = strlen("Chinese IBS standard");

	//sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);
	//data_len = 2048;
	//sm9_proxylib_getSerializeObjectSize(gParams, SM9_SERIALIZE_BINARY, &data_len);
	//sm9_proxylib_serializeObject(gParams,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	//sm9_proxylib_destroyObject(gParams);
	//sm9_proxylib_deserializeObject(data_value, data_len, &gParams,SM9_SERIALIZE_BINARY);

	//sm9_proxylib_generateMasterKeys(gParams, &mpk,&msk,SM9_SCHEME_SW);
	//data_len = 2048;
	//sm9_proxylib_getSerializeObjectSize(mpk, SM9_SERIALIZE_BINARY, &data_len);
	//sm9_proxylib_serializeObject(mpk,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	//sm9_proxylib_destroyObject(mpk);
	//sm9_proxylib_deserializeObject(data_value, data_len, &mpk,SM9_SERIALIZE_BINARY);
	//data_len = 2048;
	//sm9_proxylib_getSerializeObjectSize(msk, SM9_SERIALIZE_BINARY, &data_len);
	//sm9_proxylib_serializeObject(msk,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	//sm9_proxylib_destroyObject(msk);
	//sm9_proxylib_deserializeObject(data_value, data_len, &msk,SM9_SERIALIZE_BINARY);

	//sm9_proxylib_calculateUserKeys(gParams,msk,"Alice",strlen("Alice"),&sk,SM9_SCHEME_SW);
	//data_len = 2048;
	//sm9_proxylib_getSerializeObjectSize(sk, SM9_SERIALIZE_BINARY, &data_len);
	//sm9_proxylib_serializeObject(sk,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	//sm9_proxylib_destroyObject(sk);
	//sm9_proxylib_deserializeObject(data_value, data_len, &sk,SM9_SERIALIZE_BINARY);

	//sm9_proxylib_sign(gParams,mpk,sk,message,messageLen,&sgn,SM9_SCHEME_SW);
	//data_len = 2048;
	//sm9_proxylib_getSerializeObjectSize(sgn, SM9_SERIALIZE_BINARY, &data_len);
	//sm9_proxylib_serializeObject(sgn,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	//sm9_proxylib_destroyObject(sgn);
	//sm9_proxylib_deserializeObject(data_value, data_len, &sgn,SM9_SERIALIZE_BINARY);

	//sm9_proxylib_verify(gParams,mpk,"Alice",strlen("Alice"),message,messageLen,sgn,SM9_SCHEME_SW);
	//
	//// clear objects
	//sm9_proxylib_destroyObject(gParams);
	//sm9_proxylib_destroyObject(mpk);
	//sm9_proxylib_destroyObject(msk);
	//sm9_proxylib_destroyObject(sk);
	//sm9_proxylib_destroyObject(sgn);

	// sign verify end
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

	sm9_proxylib_encrypt(gParams,mpk,"Bob",strlen("Bob"),message,messageLen,&cipher,SM9_SCHEME_SW);
	data_len = 2048;
	sm9_proxylib_getSerializeObjectSize(cipher, SM9_SERIALIZE_BINARY, &data_len);
	sm9_proxylib_serializeObject(cipher,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);
	sm9_proxylib_destroyObject(cipher);
	sm9_proxylib_deserializeObject(data_value, data_len, &cipher,SM9_SERIALIZE_BINARY);

	sm9_proxylib_decrypt(gParams,mpk,key,"Bob",strlen("Bob"),cipher,&plain,SM9_SCHEME_SW);
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

	return 0;
}
