#ifndef __SM9__PROXYLIB_API_H__
#define __SM9__PROXYLIB_API_H__

#define SM9_BYTES_LEN_BIG  32
#define SM9_BYTES_LEN_G1   64
#define SM9_BYTES_LEN_G2  128

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
		SM9_ERROR_BUFERR_LESS,
		SM9_ERROR_DATA_ERR,
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



	// user functions(soft ware implement)

	int sm9_generateMasterKeys(char pMsk[SM9_BYTES_LEN_BIG], int *piMskLen, char pMpkG1[SM9_BYTES_LEN_G1], int *piMpkG1Len, char pMpkG2[SM9_BYTES_LEN_G2], int *piMpkG2Len);
	int sm9_calculateUserKeys(char pMsk[SM9_BYTES_LEN_BIG], int iMskLen,
		char * pUserID, int iUserIDLen,
		char pSkhid01[SM9_BYTES_LEN_G1], int *piSkhid01Len,
		char pSkhid02[SM9_BYTES_LEN_G2], int *piSkhid02Len,
		char pSkhid03[SM9_BYTES_LEN_G2], int *piSkhid03Len);
	int sm9_sign(char pMpk[SM9_BYTES_LEN_G2], int iMpkLen, char pSk[SM9_BYTES_LEN_G1], int iSkLen, char *pMessage, int iMessageLen, 
		char pSgn[SM9_BYTES_LEN_BIG+SM9_BYTES_LEN_G1], int *piSgnLen);
	int sm9_verify(char pMpk[SM9_BYTES_LEN_G2], int iMpkLen, char * pUserID, int iUserIDLen, char *pMessage, int iMessageLen, 
		char pSgn[SM9_BYTES_LEN_BIG+SM9_BYTES_LEN_G1], int iSgnLen);
	int sm9_encrypt(char pMpk[SM9_BYTES_LEN_G1], int iMpkLen, char * pUserID, int iUserIDLen, char *pMessage, int iMessageLen, 
		char *pCipher, int *piCipherLen, SM9_CIPHER_TYPE cipherType
		);
	int sm9_decrypt(char pMpk[SM9_BYTES_LEN_G1], int iMpkLen, char pSk[SM9_BYTES_LEN_G2], int iSkLen, char * pUserID, int iUserIDLen,  
		char *pCipher, int iCipherLen, 
		char *pMessage, int *piMessageLen,  SM9_CIPHER_TYPE cipherType
		);
	int sm9_wrap(char pMpk[SM9_BYTES_LEN_G1], int iMpkLen, char * pUserID, int iUserIDLen, 
		char *pKey, int *piKeyLen, char *pWrapKey, int *piWrapKeyLen
		);
	int sm9_unwrap(char pMpk[SM9_BYTES_LEN_G1], int iMpkLen, char pSk[SM9_BYTES_LEN_G2], int iSkLen, char * pUserID, int iUserIDLen, 
		char *pWrapKey, int iWrapKeyLen, char *pKey, int *piKeyLen
		);


	// inner functions (soft and hard ware implement)

	// C-compatible wrapper routines.  See documentation for usage.
	int sm9_proxylib_getSerializeObjectSize(void *object, SM9_SERIALIZE_MODE mode, int *serialSize);
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
	int sm9_proxylib_keyExchangeB2(void *params, void *mpk, void *sk, char * userIDA, int userIDALen, char * userIDB, int userIDBLen, int key_len,
		void *RA, void **RB, void **SKB, void **SB, void **S2, SM9_KEY_EX_OPTION option,
		SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_keyExchangeA3(void *params, void *mpk, void *sk, char * userIDA, int userIDALen, char * userIDB, int userIDBLen, int key_len,
		void *RA, void *RB, void *SB, void **SKA, void **SA, void *rA, SM9_KEY_EX_OPTION option,
		SM9_SCHEME_TYPE schemeID);

	int sm9_proxylib_keyExchange_pre(void *params, void *mpk, char * userIDOther, int userIDOtherLen,
		void **RSelf,void **rSelf,
		SM9_SCHEME_TYPE schemeID);
	int sm9_proxylib_keyExchange(void *params, void *mpk, void *sk, char * userIDA, int userIDALen, char * userIDB, int userIDBLen, int key_len,
		void*rSelf, void *RA, void *RB, void **SKBorSKA, void **SBorS1, void **S2orSA, int isB, SM9_KEY_EX_OPTION option,
		SM9_SCHEME_TYPE schemeID);


	// 参数转换
	int sm9_proxylib_ObjectToItemsValueCurveParams(void *object);
	int sm9_proxylib_ObjectFromItemsValueCurveParams(void **object);

	// 主公钥转换
	int sm9_proxylib_ObjectToItemsValueMPK(void *object, char g1[SM9_BYTES_LEN_G1], char g2[SM9_BYTES_LEN_G2]);
	int sm9_proxylib_ObjectFromItemsValueMPK(void **object, char g1[SM9_BYTES_LEN_G1], char g2[SM9_BYTES_LEN_G2]);

	// 主私钥转换
	int sm9_proxylib_ObjectToItemsValueMSK(void *object, char msk[SM9_BYTES_LEN_BIG]);
	int sm9_proxylib_ObjectFromItemsValueMSK(void **object, char sk[SM9_BYTES_LEN_BIG]);

	// 用户私钥转换
	int sm9_proxylib_ObjectToItemsValueSK(void *object, char hid01[SM9_BYTES_LEN_G1], char hid02[SM9_BYTES_LEN_G2], char hid03[SM9_BYTES_LEN_G2]);
	int sm9_proxylib_ObjectFromItemsValueSK(void **object, char hid01[SM9_BYTES_LEN_G1], char hid02[SM9_BYTES_LEN_G2], char hid03[SM9_BYTES_LEN_G2]);

	// 签名值转换
	int sm9_proxylib_ObjectToItemsValueSGN(void *object, char h[SM9_BYTES_LEN_BIG], char S[SM9_BYTES_LEN_G1]);
	int sm9_proxylib_ObjectFromItemsValueSGN(void **object, char h[SM9_BYTES_LEN_BIG], char S[SM9_BYTES_LEN_G1]);

	// 封装值转换
	int sm9_proxylib_ObjectToItemsValueWRAP(void *object, char C[SM9_BYTES_LEN_G1]);
	int sm9_proxylib_ObjectFromItemsValueWRAP(void **object, char C[SM9_BYTES_LEN_G1]);

	// 数据转换
	int sm9_proxylib_ObjectToItemsValueDATA(void *object, char *data, int *data_len);
	int sm9_proxylib_ObjectFromItemsValueDATA(void **object, char *data, int data_len);

	// 密文转换
	int sm9_proxylib_ObjectToItemsValueCipher(void *object, char C1[SM9_BYTES_LEN_G1], char C3[SM9_BYTES_LEN_BIG], char *C2, int *C2_len);
	int sm9_proxylib_ObjectFromItemsValueCipher(void **object, char C1[SM9_BYTES_LEN_G1], char C3[SM9_BYTES_LEN_BIG], char *C2, int C2_len);

	// 秘钥交换R值转换
	int sm9_proxylib_ObjectToItemsValueEXR(void *object, char R[SM9_BYTES_LEN_G1]);
	int sm9_proxylib_ObjectFromItemsValueEXR(void **object, char R[SM9_BYTES_LEN_G1]);


#ifdef __cplusplus
}
#endif

#endif // __PROXYLIB_API_H__
