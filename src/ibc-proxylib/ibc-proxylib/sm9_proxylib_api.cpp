
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>

using namespace std;

#include "sm9_proxylib_api.h"
#include "sm9_proxylib.h"
#include "sm9_proxylib_sw.h"
#include "sm9_proxylib_hw.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>

int sm9_proxylib_generateParams(void **params, SM9_SCHEME_TYPE schemeID) 
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *curveParams = new SM9CurveParams_SW();
			curveParams->objectType = SM9_OBJ_SW_PARAM;
			if (sm9_sw_generate_params(*curveParams) == TRUE) {
				error = SM9_ERROR_NONE;
				*params = (void*) curveParams;

				return error;
			}
			else
			{
				delete curveParams;
				curveParams = NULL;
			}
		}
		break;
	case SM9_SCHEME_HW:
		// add hardware here
		break;
	}

	
	return error;
}

int sm9_proxylib_getSerializeObjectSize(void *object, SM9_SERIALIZE_MODE mode, int *serialSize)
{
	int error = SM9_ERROR_OTHER;
	SM9Object *cp = (SM9Object*) object;

	if (serialSize)
	{
		*serialSize = cp->getSerializedSize(mode);

		if (*serialSize) {
			error = SM9_ERROR_NONE;
		}
	}

	return error;
}

int sm9_proxylib_serializeObject(void *params, char *buffer, int *bufferSize,
	int bufferAvailSize, SM9_SERIALIZE_MODE mode) 
{
	int error = SM9_ERROR_OTHER;
	SM9Object *cp = (SM9Object*) params;

	if (cp->getSerializedSize(mode) <= bufferAvailSize) {
		*bufferSize = cp->serialize(mode, buffer,
			bufferAvailSize);
		if (*bufferSize > 0) {
			error = SM9_ERROR_NONE;
		}
	}
	return error;
}

int sm9_proxylib_deserializeObject(char *buffer, int bufferSize, void **params,
	SM9_SERIALIZE_MODE mode)
{
	int error = SM9_ERROR_OTHER;
	SM9Object *cp = NULL;

	SM9AARData bufferBin(bufferSize);

	switch(mode)
	{
	case SM9_SERIALIZE_BINARY:
		{
			memcpy(bufferBin.m_pValue, buffer,bufferSize);
			bufferBin.m_iPos = bufferSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			Hex2Bin(buffer,bufferSize,(unsigned char *)bufferBin.m_pValue,&bufferBin.m_iPos);
			Hex2Bin(buffer,bufferSize,(unsigned char *)bufferBin.m_pValue,&bufferBin.m_iPos);
		}
		break;
	}

	int totLen = 0; 

	SM9_OBJ_TYPE type = getSM9ObjectType(bufferBin.m_pValue,&totLen);

	switch(type)
	{
	case SM9_OBJ_SW_PARAM:
		{
			cp = new SM9CurveParams_SW;
		}
		break;
	case SM9_OBJ_SW_MSK:
		{
			cp = new SM9ProxyMSK_SW;
		}
		break;
	case SM9_OBJ_SW_MPK:
		{
			cp = new SM9ProxyMPK_SW;
		}
		break;
	case SM9_OBJ_SW_SK:
		{
			cp = new SM9ProxySK_SW;
		}
		break;
	case SM9_OBJ_SW_SGN:
		{
			cp = new SM9ProxySGN_SW;
		}
		break;
	case SM9_OBJ_SW_WRAP:
		{
			cp = new SM9ProxyWRAP_SW;
		}
		break;
	case SM9_OBJ_SW_DATA:
		{
			cp = new SM9ProxyDATA_SW;
		}
		break;
	case SM9_OBJ_SW_CIPHER:
		{
			cp = new SM9ProxyCipher_SW;
		}
		break;

	case SM9_OBJ_SW_EX_R:
		{
			cp = new SM9ProxyEXR_SW;
		}
		break;

	default:
		return error;
		break;
	}

	if (cp->deserialize(SM9_SERIALIZE_BINARY, bufferBin.m_pValue, bufferBin.m_iPos) == FALSE) {
		delete cp;
		*params = NULL;
	} else {
		error = SM9_ERROR_NONE;
		*params = cp;
	}

	return error;
}

int sm9_proxylib_destroyObject(void *params) {

	if (params)
	{
		SM9Object *cp = (SM9Object*) params;
		delete(cp);
	}

	return SM9_ERROR_NONE;
}

int sm9_proxylib_cmpObject(void *first, void *second)
{
	int error = SM9_ERROR_OTHER;
	SM9Object *pfirst = (SM9Object*) first;
	SM9Object *psecond = (SM9Object*) second;

	if (TRUE == ((*pfirst)==(*psecond)))
	{
		error = SM9_ERROR_NONE;
	}
	
	return error;
}

int sm9_proxylib_generateMasterKeys(void *params, void **mpk,void **msk, SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;

			SM9ProxyMPK_SW *pmpk = new SM9ProxyMPK_SW;
			SM9ProxyMSK_SW *pmsk = new SM9ProxyMSK_SW;

			if (sm9_sw_generate_masterkey(*pparams, *pmpk,*pmsk) == TRUE) {
				error = SM9_ERROR_NONE;
				*mpk = (void*) pmpk;
				*msk = (void*) pmsk;

				return error;
			}
			else
			{
				delete pmsk;
				delete pmpk;
			}
			
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}

int sm9_proxylib_calculateUserKeys(void *params, void *msk, char * userID, int userIDLen, void **sk,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxyMSK_SW *pmsk = (SM9ProxyMSK_SW *)msk;

			SM9ProxySK_SW *psk = new SM9ProxySK_SW;

			if (sm9_sw_calculate_privatekey(*pparams, *pmsk, userID, userIDLen, *psk) == TRUE) {
				error = SM9_ERROR_NONE;
				*sk = (void*) psk;
			}
			else
			{
				delete psk;
				return error;
			}


		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}


// sign
int sm9_proxylib_sign(void *params, void *mpk, void *sk, char *message, int messageLen, 
	void **sgn,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW *)mpk;
			SM9ProxySK_SW *psk = (SM9ProxySK_SW *) sk;

			SM9ProxySGN_SW *psgn = new SM9ProxySGN_SW;

			if (sm9_sw_sign(*pparams, *pmpk, message, messageLen, *psk, *psgn) == TRUE) {
				error = SM9_ERROR_NONE;
				*sgn = (void*) psgn;
			}
			else
			{
				delete psgn;
			}

			return error;
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}

// verify
int sm9_proxylib_verify(void *params,void *mpk, char *userID, int userIDLen, char *message, int messageLen, 
	void *sgn,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxySGN_SW *psgn = (SM9ProxySGN_SW*)sgn;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW*)mpk;

			if (sm9_sw_verify(*pparams, *pmpk, message, messageLen, userID,userIDLen, *psgn) == TRUE) {
				error = SM9_ERROR_NONE;
			}

			return error;
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}

// wrap
int sm9_proxylib_wrap(void *params, void *mpk, char * userID, int userIDLen, char *seed, int seedLen, void **key, void **wrapkey, 
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW*)mpk;

			SM9ProxyDATA_SW *pkey = new SM9ProxyDATA_SW;
			SM9ProxyWRAP_SW *pwrapkey = new SM9ProxyWRAP_SW;

			if (sm9_sw_wrap(*pparams, *pmpk, userID, userIDLen, seed, seedLen, *pkey,*pwrapkey) == TRUE) {

				*key = (void*) pkey;
				*wrapkey = (void*) pwrapkey;

				error = SM9_ERROR_NONE;
			}
			else
			{
				delete pkey;
				delete pwrapkey;
			}

			return error;
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}

// unwrap
int sm9_proxylib_unwrap(void *params, void *mpk, void *sk,  char * userID, int userIDLen, void *wrapkey, void **key,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxySK_SW *psk = (SM9ProxySK_SW*)sk;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW*)mpk;
			SM9ProxyWRAP_SW *pwrapkey = (SM9ProxyWRAP_SW*)wrapkey;

			SM9ProxyDATA_SW *pkey = new SM9ProxyDATA_SW;

			if (sm9_sw_unwrap(*pparams, *pmpk, *psk, userID, userIDLen, *pwrapkey,*pkey) == TRUE) {
				*key = (void*) pkey;
				error = SM9_ERROR_NONE;
			}
			else
			{
				delete pkey;
			}

			return error;
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}


// encrypt
int sm9_proxylib_encrypt(void *params, void *mpk, char * userID, int userIDLen, char *message, int messageLen, 
	void **cipher, SM9_CIPHER_TYPE cipherType,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW*)mpk;

			SM9ProxyCipher_SW *pcipher = new SM9ProxyCipher_SW;

			if (sm9_sw_encrypt(*pparams, *pmpk, userID, userIDLen, message, messageLen, *pcipher, cipherType) == TRUE) {
				*cipher = (void*) pcipher;
				error = SM9_ERROR_NONE;
			}
			else
			{
				delete pcipher;
			}

			return error;
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}

// decrypt
int sm9_proxylib_decrypt(void *params,void *mpk, void *sk,  char * userID, int userIDLen, 
	void *cipher, void **plain, SM9_CIPHER_TYPE cipherType,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxySK_SW *psk = (SM9ProxySK_SW*)sk;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW*)mpk;
			SM9ProxyCipher_SW *pcipher = (SM9ProxyCipher_SW*)cipher;

			SM9ProxyDATA_SW *pplain = new SM9ProxyDATA_SW;

			if (sm9_sw_decrypt(*pparams, *pmpk, *psk, userID, userIDLen, *pcipher,*pplain, cipherType) == TRUE) {
				*plain = (void*) pplain;
				error = SM9_ERROR_NONE;
			}
			else
			{
				delete pplain;
			}

			return error;
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}

int sm9_proxylib_keyExchangeA1(void *params, void *mpk, char * userIDB, int userIDBLen,
	void **RA,void **rA,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW*)mpk;
			SM9ProxyEXR_SW *pRA = new SM9ProxyEXR_SW;
			SM9ProxyDATA_SW *prA = new SM9ProxyDATA_SW;

			if (sm9_sw_keyexchangeA1(*pparams, *pmpk, userIDB, userIDBLen, *pRA, *prA) == TRUE) {
				*RA = (void*) pRA;
				*rA = (void*) prA;
				error = SM9_ERROR_NONE;
			}
			else
			{
				delete pRA;
				delete prA;
			}

			return error;
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}

int sm9_proxylib_keyExchange_pre(void *params, void *mpk, char * userIDOther, int userIDOtherLen,
	void **RSelf,void **rSelf,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW*)mpk;
			SM9ProxyEXR_SW *pRSelf = new SM9ProxyEXR_SW;
			SM9ProxyDATA_SW *prSelf = new SM9ProxyDATA_SW;

			if (sm9_sw_keyexchangeA1(*pparams, *pmpk, userIDOther, userIDOtherLen, *pRSelf, *prSelf) == TRUE) {
				*RSelf = (void*) pRSelf;
				*rSelf = (void*) prSelf;
				error = SM9_ERROR_NONE;
			}
			else
			{
				delete pRSelf;
				delete prSelf;
			}

			return error;
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}
int sm9_proxylib_keyExchange(void *params, void *mpk, void *sk, char * userIDA, int userIDALen, char * userIDB, int userIDBLen, int key_len,
		void*rSelf, void *RA, void *RB, void **SKBorSKA, void **SBorS1, void **S2orSA, int isB, SM9_KEY_EX_OPTION option,
		SM9_SCHEME_TYPE schemeID)
{
		int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW*)mpk;
			SM9ProxyEXR_SW *pRA = (SM9ProxyEXR_SW*)RA;
			SM9ProxyEXR_SW *pRB = (SM9ProxyEXR_SW*)RB;
			SM9ProxySK_SW *psk = (SM9ProxySK_SW*)sk;
			SM9ProxyDATA_SW *prSelf = (SM9ProxyDATA_SW*)rSelf;

			SM9ProxyDATA_SW *pSKBorSKA = new SM9ProxyDATA_SW;
			SM9ProxyDATA_SW *pSBorS1 = new SM9ProxyDATA_SW;
			SM9ProxyDATA_SW *pS2orSA = new SM9ProxyDATA_SW;

			if (sm9_sw_keyexchange(*pparams, *pmpk, *psk, userIDA, userIDALen, userIDB, userIDBLen,key_len,*prSelf,*pRA, *pRB, *pSKBorSKA, *pSBorS1, *pS2orSA, isB, option) == TRUE) {
				*SKBorSKA = (void*) pSKBorSKA;
				if (option == SM9_KEY_EX_OPTION_YES)
				{
					*SBorS1 = (void*) pSBorS1;
					*S2orSA = (void*) pS2orSA;
				}
				else
				{
					delete pSBorS1;
					delete pS2orSA;
				}

				error = SM9_ERROR_NONE;
			}
			else
			{
				delete pSKBorSKA;
				delete pSBorS1;
				delete pS2orSA;
			}

			return error;
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}


int sm9_proxylib_keyExchangeB2(void *params, void *mpk, void *sk, char * userIDA, int userIDALen, char * userIDB, int userIDBLen, int key_len,
	void *RA, void **RB, void **SKB, void **SB, void **S2, SM9_KEY_EX_OPTION option,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW*)mpk;
			SM9ProxyEXR_SW *pRA = (SM9ProxyEXR_SW*)RA;
			SM9ProxySK_SW *psk = (SM9ProxySK_SW*)sk;

			SM9ProxyEXR_SW *pRB = new SM9ProxyEXR_SW;
			SM9ProxyDATA_SW *pSKB = new SM9ProxyDATA_SW;
			SM9ProxyDATA_SW *pSB = new SM9ProxyDATA_SW;
			SM9ProxyDATA_SW *pS2 = new SM9ProxyDATA_SW;

			if (sm9_sw_keyexchangeB2B4(*pparams, *pmpk, *psk, userIDA, userIDALen, userIDB, userIDBLen,key_len, *pRA, *pRB, *pSKB, *pSB, *pS2, option) == TRUE) {
				*RB = (void*) pRB;
				*SKB = (void*) pSKB;
				if (option == SM9_KEY_EX_OPTION_YES)
				{
					*SB = (void*) pSB;
					*S2 = (void*) pS2;
				}
				else
				{
					delete pSB;
					delete pS2;
				}

				error = SM9_ERROR_NONE;
			}
			else
			{
				delete pSKB;
				delete pSB;
				delete pRB;
				delete pS2;
			}

			return error;
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}

int sm9_proxylib_keyExchangeA3(void *params, void *mpk,void *sk, char * userIDA, int userIDALen, char * userIDB, int userIDBLen, int key_len,
	void *RA, void *RB, void *SB, void **SKA, void **SA, void *rA, SM9_KEY_EX_OPTION option,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW*)mpk;
			SM9ProxyEXR_SW *pRA = (SM9ProxyEXR_SW*)RA;
			SM9ProxySK_SW *psk = (SM9ProxySK_SW*)sk;
			SM9ProxyEXR_SW *pRB = (SM9ProxyEXR_SW*)RB;
			SM9ProxyDATA_SW *prA = (SM9ProxyDATA_SW*)rA;
			SM9ProxyDATA_SW *pSB = (SM9ProxyDATA_SW*)SB;

			SM9ProxyDATA_SW *pSKA = new SM9ProxyDATA_SW;
			SM9ProxyDATA_SW *pSA = new SM9ProxyDATA_SW;

			if (sm9_sw_keyexchangeA3(*pparams, *pmpk, *psk, userIDA, userIDALen, userIDB, userIDBLen,key_len, *pRA, *pRB,*pSB,*pSKA, *pSA, *prA, option) == TRUE) {
				*SKA = (void*) pSKA;
				if (option == SM9_KEY_EX_OPTION_YES)
				{
					*SA = (void*) pSA;
				}
				else
				{
					delete pSA;
				}
				
				error = SM9_ERROR_NONE;
			}
			else
			{
				delete pSKA;
				delete pSA;
			}

			return error;
		}
		break;
	case SM9_SCHEME_HW:
		{
			return error;
		}
		break;
	}

	return error;
}


int sm9_generateMasterKeys(char pMsk[SM9_BYTES_LEN_BIG], int *piMskLen, char pMpkG1[SM9_BYTES_LEN_G1], int *piMpkG1Len, char pMpkG2[SM9_BYTES_LEN_G2], int *piMpkG2Len)
{
	int error = SM9_ERROR_OTHER;

	void *gParams = 0;
	void *msk = 0;
	void *mpk = 0;

	int mpkG1Len = SM9_BYTES_LEN_G1;
	int mpkG2Len = SM9_BYTES_LEN_G2;
	int mskLen = SM9_BYTES_LEN_BIG;

	error = sm9_proxylib_generateParams(&gParams, SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	error = sm9_proxylib_generateMasterKeys(gParams, &mpk,&msk,SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	if (mskLen>*piMskLen)
	{
		error = SM9_ERROR_BUFERR_LESS;
		*piMskLen = mskLen;
		goto err;
	}
	else
	{
		*piMskLen = mskLen;
		sm9_proxylib_ObjectToItemsValueMSK(msk,pMsk);
	}

	if(mpkG1Len>*piMpkG1Len)
	{
		*piMpkG1Len = mpkG1Len;
	}
	else
	{
		*piMpkG1Len = mpkG1Len;
		sm9_proxylib_ObjectToItemsValueMPK(mpk,pMpkG1,NULL);
	}

	if(mpkG2Len>*piMpkG2Len)
	{
		*piMpkG2Len = mpkG2Len;
	}
	else
	{
		*piMpkG2Len = mpkG2Len;
		sm9_proxylib_ObjectToItemsValueMPK(mpk,NULL,pMpkG2);
	}

	error = SM9_ERROR_NONE;

err:
	if (gParams)
	{
		sm9_proxylib_destroyObject(gParams);
	}
	if (msk)
	{
		sm9_proxylib_destroyObject(msk);
	}
	if (mpk)
	{
		sm9_proxylib_destroyObject(mpk);
	}

	return error;
}

int sm9_calculateUserKeys(char pMsk[SM9_BYTES_LEN_BIG], int iMskLen,
	char * pUserID, int iUserIDLen,
	char pSkhid01[SM9_BYTES_LEN_G1], int *piSkhid01Len,
	char pSkhid02[SM9_BYTES_LEN_G2], int *piSkhid02Len,
	char pSkhid03[SM9_BYTES_LEN_G2], int *piSkhid03Len)
{
	int error = SM9_ERROR_OTHER;

	void *gParams = 0;
	void *msk = 0;
	void *sk = 0;

	int skhid01Len = SM9_BYTES_LEN_G1;
	int skhid02Len = SM9_BYTES_LEN_G2;
	int skhid03Len = SM9_BYTES_LEN_G2;


	error = sm9_proxylib_generateParams(&gParams, SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueMSK(&msk,pMsk);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_calculateUserKeys(gParams,msk,pUserID,iUserIDLen,&sk,SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	if (skhid01Len>*piSkhid01Len)
	{
		*piSkhid01Len = skhid01Len;
	}
	else
	{
		*piSkhid01Len = skhid01Len;
		sm9_proxylib_ObjectToItemsValueSK(sk,pSkhid01,NULL,NULL);
	}

	if (skhid02Len>*piSkhid02Len)
	{
		*piSkhid02Len = skhid02Len;
	}
	else
	{
		*piSkhid02Len = skhid02Len;
		sm9_proxylib_ObjectToItemsValueSK(sk,NULL,pSkhid02,NULL);
	}

	if (skhid03Len>*piSkhid03Len)
	{
		*piSkhid03Len = skhid03Len;
	}
	else
	{
		*piSkhid03Len = skhid03Len;
		sm9_proxylib_ObjectToItemsValueSK(sk,NULL,NULL,pSkhid03);
	}

	error = SM9_ERROR_NONE;

err:
	if (gParams)
	{
		sm9_proxylib_destroyObject(gParams);
	}
	if (msk)
	{
		sm9_proxylib_destroyObject(msk);
	}
	if (sk)
	{
		sm9_proxylib_destroyObject(sk);
	}

	return error;
}

int sm9_sign(char pMpk[SM9_BYTES_LEN_G2], int iMpkLen, char pSk[SM9_BYTES_LEN_G1], int iSkLen, char *pMessage, int iMessageLen, 
	char pSgn[SM9_BYTES_LEN_BIG+SM9_BYTES_LEN_G1], int *piSgnLen)
{
	int error = SM9_ERROR_OTHER;

	void *gParams = 0;
	void *mpk = 0;
	void *sk = 0;
	void *sgn = 0;
	int sgnLen = SM9_BYTES_LEN_G1+SM9_BYTES_LEN_BIG;

	error = sm9_proxylib_generateParams(&gParams, SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueMPK(&mpk,NULL,pMpk);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueSK(&sk,pSk,NULL,NULL);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_sign(gParams,mpk,sk,pMessage,iMessageLen,&sgn,SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	if (sgnLen>*piSgnLen)
	{
		error = SM9_ERROR_BUFERR_LESS;
		*piSgnLen = sgnLen;
		goto err;
	}
	else
	{
		*piSgnLen = sgnLen;
		sm9_proxylib_ObjectToItemsValueSGN(sgn,pSgn,pSgn+SM9_BYTES_LEN_BIG);
		
	}

	error = SM9_ERROR_NONE;

err:
	if (gParams)
	{
		sm9_proxylib_destroyObject(gParams);
	}
	if (mpk)
	{
		sm9_proxylib_destroyObject(mpk);
	}
	if (sk)
	{
		sm9_proxylib_destroyObject(sk);
	}

	return error;
}

int sm9_verify(char pMpk[SM9_BYTES_LEN_G2], int iMpkLen, char * pUserID, int iUserIDLen, char *pMessage, int iMessageLen, 
	char pSgn[SM9_BYTES_LEN_BIG+SM9_BYTES_LEN_G1], int iSgnLen)
{
	int error = SM9_ERROR_OTHER;

	void *gParams = 0;
	void *mpk = 0;
	void *sgn = 0;
	int sgnLen = 0;

	error = sm9_proxylib_generateParams(&gParams, SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueMPK(&mpk,NULL,pMpk);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueSGN(&sgn,pSgn,pSgn+SM9_BYTES_LEN_BIG);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_verify(gParams,mpk,pUserID,iUserIDLen,pMessage,iMessageLen,sgn,SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}
	else
	{
		error = SM9_ERROR_NONE;
	}

err:
	if (gParams)
	{
		sm9_proxylib_destroyObject(gParams);
	}
	if (mpk)
	{
		sm9_proxylib_destroyObject(mpk);
	}
	if (sgn)
	{
		sm9_proxylib_destroyObject(sgn);
	}

	return error;
}

int sm9_encrypt(char pMpk[SM9_BYTES_LEN_G1], int iMpkLen, char * pUserID, int iUserIDLen, char *pMessage, int iMessageLen, 
	char *pCipher, int *piCipherLen, SM9_CIPHER_TYPE cipherType
	)
{
	int error = SM9_ERROR_OTHER;

	void *gParams = 0;
	void *mpk = 0;
	void *cipher = 0;
	int cipherLen = 0;
	int C2Len = 0;

	if (SM9_CIPHER_KDF_UNION == cipherType)
	{
		C2Len = (iMessageLen+16)/16*16;
	}
	else
	{
		C2Len = iMessageLen;
	}
	cipherLen = SM9_BYTES_LEN_G1+SM9_BYTES_LEN_BIG+C2Len;

	error = sm9_proxylib_generateParams(&gParams, SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueMPK(&mpk,pMpk,NULL);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_encrypt(gParams,mpk,pUserID,iUserIDLen,pMessage,iMessageLen,&cipher,cipherType,SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	if (cipherLen>*piCipherLen)
	{
		error = SM9_ERROR_BUFERR_LESS;
		*piCipherLen = cipherLen;
		goto err;
	}
	else
	{
		*piCipherLen = cipherLen;
		sm9_proxylib_ObjectToItemsValueCipher(cipher,pCipher,pCipher+SM9_BYTES_LEN_G1,pCipher+SM9_BYTES_LEN_G1+SM9_BYTES_LEN_BIG,&C2Len);
	}

	error = SM9_ERROR_NONE;

err:
	if (gParams)
	{
		sm9_proxylib_destroyObject(gParams);
	}
	if (mpk)
	{
		sm9_proxylib_destroyObject(mpk);
	}
	if (cipher)
	{
		sm9_proxylib_destroyObject(cipher);
	}

	return error;
}

int sm9_decrypt(char pMpk[SM9_BYTES_LEN_G1], int iMpkLen, char pSk[SM9_BYTES_LEN_G2], int iSkLen, char * pUserID, int iUserIDLen,  
	char *pCipher, int iCipherLen, 
	char *pMessage, int *piMessageLen,  SM9_CIPHER_TYPE cipherType
	)
{
	int error = SM9_ERROR_OTHER;

	void *gParams = 0;
	void *mpk = 0;
	void *sk = 0;
	void *cipher = 0;
	void *plain = 0;
	int plainLen = 0;

	error = sm9_proxylib_generateParams(&gParams, SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueMPK(&mpk,pMpk,NULL);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueCipher(&cipher, pCipher, pCipher+SM9_BYTES_LEN_G1,pCipher+SM9_BYTES_LEN_G1+SM9_BYTES_LEN_BIG,iCipherLen-SM9_BYTES_LEN_G1-SM9_BYTES_LEN_BIG);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueSK(&sk,NULL,NULL,pSk);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_decrypt(gParams,mpk,sk, pUserID,iUserIDLen,cipher,&plain,cipherType, SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	sm9_proxylib_ObjectToItemsValueDATA(plain,NULL,&plainLen);

	if (plainLen > *piMessageLen)
	{
		error = SM9_ERROR_BUFERR_LESS;
		*piMessageLen = plainLen;
		goto err;
	}
	else
	{
		*piMessageLen = plainLen;
		sm9_proxylib_ObjectToItemsValueDATA(plain,pMessage,piMessageLen);
	}

	error = SM9_ERROR_NONE;

err:
	if (gParams)
	{
		sm9_proxylib_destroyObject(gParams);
	}
	if (mpk)
	{
		sm9_proxylib_destroyObject(mpk);
	}
	if (sk)
	{
		sm9_proxylib_destroyObject(sk);
	}
	if (cipher)
	{
		sm9_proxylib_destroyObject(cipher);
	}
	if (plain)
	{
		sm9_proxylib_destroyObject(plain);
	}

	return error;
}

int sm9_wrap(char pMpk[SM9_BYTES_LEN_G1], int iMpkLen, char * pUserID, int iUserIDLen, 
	char *pKey, int *piKeyLen, char *pWrapKey, int *piWrapKeyLen
	)
{
	int error = SM9_ERROR_OTHER;

	void *gParams = 0;
	void *mpk = 0;
	void *key = 0;
	void *wrapkey = 0;

	int keyLen = 0;
	int wrapKeyLen = SM9_BYTES_LEN_G1;

	error = sm9_proxylib_generateParams(&gParams, SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueMPK(&mpk,pMpk,NULL);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_wrap(gParams,mpk,pUserID,iUserIDLen,NULL,0,&key,&wrapkey,SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	sm9_proxylib_ObjectToItemsValueDATA(key,NULL,&keyLen);

	if (keyLen>*piKeyLen || wrapKeyLen>*piWrapKeyLen)
	{
		error = SM9_ERROR_BUFERR_LESS;
		*piKeyLen = keyLen;
		*piWrapKeyLen = wrapKeyLen;
	}
	else
	{
		*piKeyLen = keyLen;
		*piWrapKeyLen = wrapKeyLen;
		sm9_proxylib_ObjectToItemsValueDATA(key,pKey,&keyLen);
		sm9_proxylib_ObjectToItemsValueWRAP(wrapkey,pWrapKey);
		error = SM9_ERROR_NONE;
	}

err:
	if (gParams)
	{
		sm9_proxylib_destroyObject(gParams);
	}
	if (mpk)
	{
		sm9_proxylib_destroyObject(mpk);
	}
	if (key)
	{
		sm9_proxylib_destroyObject(key);
	}
	if (wrapkey)
	{
		sm9_proxylib_destroyObject(wrapkey);
	}

	return error;
}

int sm9_unwrap(char pMpk[SM9_BYTES_LEN_G1], int iMpkLen, char pSk[SM9_BYTES_LEN_G2], int iSkLen, char * pUserID, int iUserIDLen, 
	char *pWrapKey, int iWrapKeyLen, char *pKey, int *piKeyLen
	)
{
	int error = SM9_ERROR_OTHER;

	void *gParams = 0;
	void *mpk = 0;
	void *sk = 0;
	void *key = 0;
	void *wrapkey = 0;

	int keyLen = 0;

	error = sm9_proxylib_generateParams(&gParams, SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueMPK(&mpk,pMpk,NULL);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueWRAP(&wrapkey, pWrapKey);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_ObjectFromItemsValueSK(&sk,NULL,NULL,pSk);
	if (error)
	{
		error = SM9_ERROR_DATA_ERR;
		goto err;
	}

	error = sm9_proxylib_unwrap(gParams,mpk,sk,pUserID,iUserIDLen,wrapkey,&key,SM9_SCHEME_SW);
	if (error)
	{
		goto err;
	}

	sm9_proxylib_ObjectToItemsValueDATA(key,NULL,&keyLen);

	if (keyLen>*piKeyLen)
	{
		error = SM9_ERROR_BUFERR_LESS;
		*piKeyLen = keyLen;
	}
	else
	{
		*piKeyLen = keyLen;
		sm9_proxylib_ObjectToItemsValueDATA(key,pKey,&keyLen);
		error = SM9_ERROR_NONE;
	}

err:
	if (gParams)
	{
		sm9_proxylib_destroyObject(gParams);
	}
	if (mpk)
	{
		sm9_proxylib_destroyObject(mpk);
	}
	if (sk)
	{
		sm9_proxylib_destroyObject(sk);
	}

	if (wrapkey)
	{
		sm9_proxylib_destroyObject(wrapkey);
	}

	if (key)
	{
		sm9_proxylib_destroyObject(key);
	}

	return error;
}
