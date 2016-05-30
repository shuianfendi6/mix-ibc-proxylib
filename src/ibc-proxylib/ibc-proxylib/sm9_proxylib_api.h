#ifndef __SM9__PROXYLIB_API_H__
#define __SM9__PROXYLIB_API_H__

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum {
		SM9_CIPH_FIRST_LEVEL = 0,
		SM9_CIPH_SECOND_LEVEL = 1,
		SM9_CIPH_REENCRYPTED = 2
	} SM9_CIPHERTEXT_TYPE;

	typedef enum {
		SM9_SCHEME_SW = 0,       // 软件加密
		SM9_SCHEME_HW = 1,       // 硬件加密
	} SM9_SCHEME_TYPE;

	typedef enum {
		SM9_SERIALIZE_BINARY = 0,
		SM9_SERIALIZE_HEXASCII = 1
	} SM9_SERIALIZE_MODE;

	typedef enum {
		SM9_ERROR_NONE = 0,
		SM9_ERROR_PLAINTEXT_TOO_LONG = 1,
		SM9_ERROR_OTHER = 100,
	} SM9_ERRORID;

	// C-compatible wrapper routines.  See documentation for usage.
	//
	int sm9_proxylib_generateParams(void **params, SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_serializeParams(void *params, char *buffer, int *bufferSize, 
		int bufferAvailSize, SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_deserializeParams(char *buffer, int bufferSize, void **params,
		SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_destroyParams(void *params);

	int sm9_proxylib_generateMasterKey(void **mpk,void **msk, SM9_SCHEME_TYPE schemeID);



	int sm9_proxylib_generateKeys(void *params, void **pk, void **sk, 
		SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_serializeKeys(void *params, void *pk, void *sk, char *pkBuf, char *skBuf,
		int *pkBufSize, int *skBufSize, int bufferAvailSize, SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_deserializeKeys(void *params, char *pkBuf, char *skBuf,
		int pkBufSize, int skBufSize, void **pk, void **sk, SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_destroyKeys(void *pk, void *sk, SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_encrypt(void *params, void *pk, char *message, int messageLen, 
		char *ciphertext, int *ciphLen, SM9_CIPHERTEXT_TYPE ctype,
		SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_generateDelegationKey(void *params, void *sk1, void *pk2, void** delKey, 
		SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_serializeDelegationKey(void *params, void *delKey, char *delKeyBuf,
		int *delKeyBufSize, int bufferAvailSize, SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_serializeDelegationKey(void *params, void *delKey, char *delKeyBuf,
		int *delKeyBufSize, int bufferAvailSize, SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_destroyDelegationKey(void *delKey, SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_decrypt(void *params, void *sk, char *message, int *messageLen, 
		char *ciphertext, int ciphLen, 
		SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_reencrypt(void *params, void *rk, 
		char *ciphertext, int ciphLen, 
		char *newciphertext, int *newCiphLen, SM9_SCHEME_TYPE schemeID);


#ifdef __cplusplus
}
#endif

#endif // __PROXYLIB_API_H__
