
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "sm9_proxylib_api.h"


#define NUMENCRYPTIONS 100

void *gParams, *gParamsDS;
int testNum = 0, testsSuccess = 0;

//
// Main routine for tests
//

int main()
{
	int err;
	char plaintext1[20] = "0123456789ABCDEF";
	char plaintext2[20] = "0000000000000000";
	char ciphertext1[1024] = "";
	char ciphertext2[1024] = "";
	int ptextLen1 = 16;
	int ptextLen2 = 16;
	int ctextLen1 = 20;
	int ctextLen2 = 20;
	char buffer[1000];
	void *newParams = NULL;
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

	void *msk, *mpk;
	void *sk;
	void *sgn;

	char * message = "Chinese IBS standard";
	int messageLen = strlen("Chinese IBS standard");


	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	sm9_proxylib_getSerializeObjectSize(gParams, SM9_SERIALIZE_BINARY, &data_len);

	sm9_proxylib_serializeObject(gParams,data_value, &data_len, data_len, SM9_SERIALIZE_BINARY);

	sm9_proxylib_deserializeObject(data_value, data_len, &gParamsDS,SM9_SERIALIZE_BINARY);

	sm9_proxylib_destroyObject(gParamsDS);

	sm9_proxylib_generateMasterKeys(gParams, &mpk,&msk,SM9_SCHEME_SW);

	sm9_proxylib_calculateUserKeys(gParams,msk,"Alice",strlen("Alice"),&sk,SM9_SCHEME_SW);

	sm9_proxylib_sign(gParams,mpk,sk,message,messageLen,&sgn,SM9_SCHEME_SW);

	sm9_proxylib_verify(gParams,mpk,"Alice",strlen("Alice"),message,messageLen,sgn,SM9_SCHEME_SW);

	return 0;

	//printf("Proxy Re-encryption Library\nDiagnostic Test Routines (C language)\n\n");

	///*
	//* Initialize library test
	//*/
	//printf("%d. Initializing library", ++testNum);
	//err = proxylib_initLibrary("FFFF", 4);
	//if (err != ERROR_NONE) {
	//	printf(" ... FAILED (error %d)\n", err);
	//} else {
	//	printf(" ... OK\n");
	//	testsSuccess++;
	//}

	///****************/
	///* PRE1 Tests   */
	///****************/

	//printf("\nTESTING PRE1 ROUTINES\n\n");
	///*
	//* Parameter generation test
	//*/
	//printf("%d. Generating curve parameters ", ++testNum);
	//err = proxylib_generateParams(&gParams, SCHEME_PRE1);
	//if (err != ERROR_NONE) {
	//	printf(" ... FAILED (error %d)\n", err);
	//} else {
	//	printf(" ... OK\n");
	//	testsSuccess++;
	//}

	///*
	//* Key generation tests
	//*/
	//printf("%d. Generating keypair 1 ", ++testNum);

	//err = proxylib_generateKeys(gParams, &pk1, &sk1, SCHEME_PRE1);
	//if (err != ERROR_NONE) {
	//	printf(" ... FAILED (error %d)\n", err);
	//} else {
	//	printf(" ... OK\n");
	//	testsSuccess++;
	//}

	//printf("%d. Generating keypair 2 ", ++testNum);
	//err = proxylib_generateKeys(gParams, &pk2, &sk2, SCHEME_PRE1);
	//if (err != ERROR_NONE) {
	//	printf(" ... FAILED (error %d)\n", err);
	//} else {
	//	printf(" ... OK\n");
	//	testsSuccess++;
	//}

	///*
	//* Re-encryption key generation test
	//*/
	//printf("%d. Re-encryption key generation test ", ++testNum);

	///* Generate a delegation key from user1->user2 */
	//err = proxylib_generateDelegationKey(gParams, sk1, pk2, &delKey, 
	//	SCHEME_PRE1);
	//if (err != ERROR_NONE) {
	//	printf(" ... FAILED (error %d)\n", err);
	//} else {
	//	printf(" ... OK\n");
	//	testsSuccess++;
	//}

	///*
	//* First-level encryption/decryption test
	//*/
	//printf("%d. First-level encryption/decryption test ", ++testNum);
	//err = proxylib_encrypt(gParams, pk1, (char*)plaintext1, ptextLen1, 
	//	ciphertext1, &ctextLen1, CIPH_FIRST_LEVEL,
	//	SCHEME_PRE1);
	//if (err != ERROR_NONE) {
	//	printf(" ... FAILED (error %d)\n", err);
	//} else {
	//	// Decrypt the ciphertext
	//	err = proxylib_decrypt(gParams, sk1, plaintext2, &ptextLen2, 
	//		ciphertext1, ctextLen1, SCHEME_PRE1);
	//	if (err != ERROR_NONE) {
	//		printf(" ... FAILED (error %d)\n", err);
	//	} else {
	//		if ((ptextLen1 != ptextLen2) || (memcmp(plaintext1, plaintext2, ptextLen1) != 0)) {
	//			printf(" ... FAILED (plaintexts don't match)\n", err);
	//		} else {
	//			printf(" ... OK\n");
	//			testsSuccess++;
	//		}
	//	}
	//}

	///*
	//* Second-level encryption/decryption test
	//*/
	//strcpy(plaintext1, "0123456789ABCDEF");
	//strcpy(plaintext2, "0000000000000000");
	//ptextLen1 = ptextLen2 = 16;
	//printf("%d. Second-level encryption/decryption test ", ++testNum);
	//err = proxylib_encrypt(gParams, pk1, (char*)plaintext1, ptextLen1, 
	//	ciphertext1, &ctextLen1, CIPH_SECOND_LEVEL,
	//	SCHEME_PRE1);
	//if (err != ERROR_NONE) {
	//	printf(" ... FAILED (error %d)\n", err);
	//} else {
	//	// Decrypt the ciphertext
	//	err = proxylib_decrypt(gParams, sk1, plaintext2, &ptextLen2, 
	//		ciphertext1, ctextLen1, SCHEME_PRE1);
	//	if (err != ERROR_NONE) {
	//		printf(" ... FAILED (error %d)\n", err);
	//	} else {
	//		if ((ptextLen1 != ptextLen2) || (memcmp(plaintext1, plaintext2, ptextLen1) != 0)) {
	//			printf(" ... FAILED (plaintexts don't match)\n", err);
	//		} else {
	//			printf(" ... OK\n");
	//			testsSuccess++;
	//		}
	//	}
	//}

	///*
	//* Re-encryption/decryption test
	//*/
	//printf("%d. Re-encryption/decryption test ", ++testNum);
	///* Re-encrypt ciphertext from user1->user2 using delKey 
	//* We make use of the ciphertext generated in the previous test. */
	//err = proxylib_reencrypt(gParams, delKey, ciphertext1, ctextLen1, ciphertext2, 
	//	&ctextLen2, SCHEME_PRE1);
	//if (err != ERROR_NONE) {
	//	printf(" ... FAILED (error %d)\n", err);
	//} else {
	//	/* Decrypt the ciphertext using sk2 */
	//	err = proxylib_decrypt(gParams, sk2, plaintext2, &ptextLen2, 
	//		ciphertext2, ctextLen2, SCHEME_PRE1);
	//	if (err = ERROR_NONE) {
	//		printf(" ... FAILED (error %d)\n", err);
	//	} else {
	//		if ((ptextLen1 != ptextLen2) || (memcmp(plaintext1, plaintext2, ptextLen1) != 0)) {
	//			printf(" ... FAILED (plaintexts don't match)\n", err);
	//		} else {
	//			printf(" ... OK\n");
	//			testsSuccess++;
	//		}
	//	}
	//}

	///*
	//* Serialization/Deserialization test
	//*/
	//
	//printf("%d. Serialization/deserialization tests\n", ++testNum);

	///* Serialize a public/secret keypair */
	//err = proxylib_serializeKeys(gParams, pk1, sk1, buffer1, buffer2,
	//	&bufsize1, &bufsize2, 1000, SCHEME_PRE1);
	//if (err != ERROR_NONE) {
	//	printf(" ... FAILED (serialize keys, error %d)\n", err);
	//}

	//err = proxylib_deserializeKeys(gParams, buffer1, buffer2,
	//	bufsize1, bufsize2, &pk2, &pk2, SCHEME_PRE1);
	//if (err != ERROR_NONE) {
	//	printf(" ... FAILED (deserialize keys, error %d)\n", err);
	//}

	///* Serialize a delegation key */
	///* TODO */

	///* Serialize curve parameters */
	//err = proxylib_serializeParams(gParams, buffer, &bufferSize, 1000, SCHEME_PRE1);
	//if (err != ERROR_NONE) {
	//	printf(" ... FAILED (error %d)\n", err);
	//} else {
	//	err = proxylib_deserializeParams(buffer, bufferSize, &gParams2, SCHEME_PRE1);
	//	if (err != ERROR_NONE) {
	//		printf(" ... FAILED (error %d)\n", err);
	//	}
	//}
	//serTestResult = serTestResult && (newParams == gParams);

	//if (serTestResult == 1) {
	//	printf(" ... FAILED\n", err);
	//} else {
	//	printf(" ... OK\n");
	//	testsSuccess++;
	//}

	///* cout << endl << "All tests complete." << endl;
	//cout << testsSuccess << " succeeded out of " <<
	//testNum << " total." << endl;*/
}
