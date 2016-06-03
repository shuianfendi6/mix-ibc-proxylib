#ifndef __SM9__PROXYLIB_API_H__
#define __SM9__PROXYLIB_API_H__

#ifdef __cplusplus
extern "C" {
#endif
	typedef enum {
		SM9_SCHEME_SW = 0,       // 软件加密
		SM9_SCHEME_HW = 1,       // 硬件加密
	} SM9_SCHEME_TYPE;

	typedef enum {
		SM9_CIPHER_KDF_BASE = 0,        // 基于KDF
		SM9_CIPHER_KDF_UNION = 1,       // 结合KDF 
	} SM9_CIPHER_TYPE;

	typedef enum {
		SM9_SERIALIZE_BINARY = 0,
		SM9_SERIALIZE_HEXASCII = 1
	} SM9_SERIALIZE_MODE;

	typedef enum {
		SM9_ERROR_NONE = 0,
		SM9_ERROR_OTHER = -1,
	} SM9_ERRORID;

	typedef enum {
		SM9_KEY_EX_OPTION_NO = 0,
		SM9_KEY_EX_OPTION_YES,
	} SM9_KEY_EX_OPTION;

	typedef enum _SM9_OBJ_TYPE{
		SM9_OBJ_NONE = 0,

		SM9_OBJ_SW_PARAM,
		SM9_OBJ_SW_MSK,
		SM9_OBJ_SW_MPK,
		SM9_OBJ_SW_SK,
		SM9_OBJ_SW_PK,
		SM9_OBJ_SW_SGN,
		SM9_OBJ_SW_WRAP,
		SM9_OBJ_SW_DATA,
		SM9_OBJ_SW_CIPHER,
		SM9_OBJ_SW_EX_R,

		SM9_OBJ_HW_PARAM,
		SM9_OBJ_HW_MSK,
		SM9_OBJ_HW_MPK,
		SM9_OBJ_HW_SK,
		SM9_OBJ_HW_PK,

	}SM9_OBJ_TYPE;

	// C-compatible wrapper routines.  See documentation for usage.
	int sm9_proxylib_getSerializeObjectSize(void *params, SM9_SERIALIZE_MODE mode, int *serialSize);
	int sm9_proxylib_serializeObject(void *params, char *buffer, int *bufferSize, 
		int bufferAvailSize, SM9_SERIALIZE_MODE mode);
	int sm9_proxylib_deserializeObject(char *buffer, int bufferSize, void **params,
		SM9_SERIALIZE_MODE mode);
	int sm9_proxylib_destroyObject(void *params);
	int sm9_proxylib_cmpObject(void *first, void *second);

	int sm9_proxylib_generateParams(void **params, SM9_SCHEME_TYPE schemeID);

	int sm9_proxylib_generateMasterKeys(void *params, void **mpk,void **msk, SM9_SCHEME_TYPE schemeID);

	int sm9_proxylib_calculateUserKeys(void *params, void *msk, char * userID, int userIDLen, void **sk, 
		SM9_SCHEME_TYPE schemeID);

	// sign
	int sm9_proxylib_sign(void *params, void *mpk, void *sk, char *message, int messageLen, 
		void **sgn,
		SM9_SCHEME_TYPE schemeID);
	// verify
	int sm9_proxylib_verify(void *params, void *mpk, char * userID, int userIDLen, char *message, int messageLen, 
		void *sgn,
		SM9_SCHEME_TYPE schemeID);

	// wrap
	int sm9_proxylib_wrap(void *params, void *mpk, char * userID, int userIDLen, char *seed, int seedLen, void **key, void **wrapkey, 
		SM9_SCHEME_TYPE schemeID);
	// unwrap
	int sm9_proxylib_unwrap(void *params, void *mpk, void *sk,  char * userID, int userIDLen, void *wrapkey, void **key,
		SM9_SCHEME_TYPE schemeID);

	// encrypt
	int sm9_proxylib_encrypt(void *params, void *mpk, char * userID, int userIDLen, char *message, int messageLen, 
		void **cipher, SM9_CIPHER_TYPE cipherType,
		SM9_SCHEME_TYPE schemeID);
	// decrypt
	int sm9_proxylib_decrypt(void *params,void *mpk, void *sk,  char * userID, int userIDLen, 
		void *cipher, void **plain, SM9_CIPHER_TYPE cipherType,
		SM9_SCHEME_TYPE schemeID);

	// key exchange 
	int sm9_proxylib_keyExchangeA1(void *params, void *mpk, char * userIDB, int userIDBLen,
		void **RA,void **rA,
		SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_keyExchangeB2B4(void *params, void *mpk, void *sk, char * userIDA, int userIDALen, char * userIDB, int userIDBLen, int key_len,
		void *RA, void **RB, void **SKB, void **SB, void **S2, SM9_KEY_EX_OPTION option,
		SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_keyExchangeA3(void *params, void *mpk, void *sk, char * userIDA, int userIDALen, char * userIDB, int userIDBLen, int key_len,
		void *RA, void *RB, void *SB, void **SKA, void **SA, void *rA, SM9_KEY_EX_OPTION option,
		SM9_SCHEME_TYPE schemeID);


#ifdef __cplusplus
}
#endif

#endif // __PROXYLIB_API_H__
