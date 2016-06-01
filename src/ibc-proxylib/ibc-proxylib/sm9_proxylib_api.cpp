
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

int sm9_proxylib_getSerializeObjectSize(void *params, SM9_SERIALIZE_MODE mode, int *serialSize)
{
	int error = SM9_ERROR_OTHER;
	SM9Object *cp = (SM9Object*) params;

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
		(*(int *)buffer) = cp->objectType; 

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

	char bufferBin[1024];
	int bufferBinLen = 1024;

	switch(mode)
	{
	case SM9_SERIALIZE_BINARY:
		{
			memcpy(bufferBin, buffer,bufferSize);
			bufferBinLen = bufferSize;
		}
		break;
	case SM9_SERIALIZE_HEXASCII:
		{
			Hex2Bin(buffer,bufferSize,(unsigned char *)bufferBin,&bufferBinLen);
		}
		break;
	}

	int totLen = 0; 

	SM9_OBJ_TYPE type = getSM9ObjectType(bufferBin,&totLen);

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

	default:
		return error;
		break;
	}

	if (cp->deserialize(SM9_SERIALIZE_BINARY, bufferBin, bufferBinLen) == FALSE) {
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
		free(cp);
	}

	return SM9_ERROR_NONE;
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
	void **cipher,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9CurveParams_SW *pparams = (SM9CurveParams_SW *)params;
			SM9ProxyMPK_SW *pmpk = (SM9ProxyMPK_SW*)mpk;

			SM9ProxyCipher_SW *pcipher = new SM9ProxyCipher_SW;

			if (sm9_sw_encrypt(*pparams, *pmpk, userID, userIDLen, message, messageLen, *pcipher) == TRUE) {
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
	void *cipher, void **plain,
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

			if (sm9_sw_decrypt(*pparams, *pmpk, *psk, userID, userIDLen, *pcipher,*pplain) == TRUE) {
				*plain = (void*) pplain;
				error = SM9_ERROR_NONE;
			}
			else
			{
				delete plain;
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
