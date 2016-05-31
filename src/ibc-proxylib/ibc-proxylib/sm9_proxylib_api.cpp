
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

	SM9CurveParams *curveParams = new SM9CurveParams();
	switch (schemeID) {
	case SM9_SCHEME_SW:
		if (sm9_sw_generate_params(*curveParams) == TRUE) {
			error = SM9_ERROR_NONE;
		}
		break;
	case SM9_SCHEME_HW:
		//if (PRE2_generate_params(*curveParams) == TRUE) {
		//	error = SM9_ERROR_NONE;
		//}
		break;
	}

	*params = (void*) curveParams;
	return error;
}

int sm9_proxylib_serializeParams(void *params, char *buffer, int *bufferSize,
	int bufferAvailSize, SM9_SCHEME_TYPE schemeID) 
{
	int error = SM9_ERROR_OTHER;
	int serialSize = 0;

	SM9CurveParams *cp = (SM9CurveParams*) params;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		if (cp->getSerializedSize(SM9_SERIALIZE_BINARY) <= bufferAvailSize) {
			*bufferSize = cp->serialize(SM9_SERIALIZE_BINARY, buffer,
				bufferAvailSize);
			if (*bufferSize > 0) {
				error = SM9_ERROR_NONE;
			}
		}
		break;
	case SM9_SCHEME_HW:
		if (cp->getSerializedSize(SM9_SERIALIZE_BINARY) <= bufferAvailSize) {
			*bufferSize = cp->serialize(SM9_SERIALIZE_BINARY, buffer,
				bufferAvailSize);
			if (*bufferSize > 0) {
				error = SM9_ERROR_NONE;
			}
		}
		break;
	}

	return error;
}

int sm9_proxylib_deserializeParams(char *buffer, int bufferSize, void **params,
	SM9_SCHEME_TYPE schemeID)
{
	int error = SM9_ERROR_OTHER;

	SM9CurveParams *cp = new SM9CurveParams;
	if (cp->deserialize(SM9_SERIALIZE_BINARY, buffer, bufferSize) == FALSE) {
		delete cp;
		*params = NULL;
	} else {
		error = SM9_ERROR_NONE;
		*params = cp;
	}

	return error;
}

int sm9_proxylib_destroyParams(void *params) {
	if (params != NULL) {
		SM9CurveParams *cp = (SM9CurveParams*) params;
		free(cp);
	}

	return SM9_ERROR_NONE;
}


int sm9_proxylib_generateMasterKey(void *params, void **mpk,void **msk, SM9_SCHEME_TYPE schemeID)
{
	SM9CurveParams *pparams = (SM9CurveParams *)params;
	int error = SM9_ERROR_OTHER;

	switch (schemeID) {
	case SM9_SCHEME_SW:
		{
			SM9ProxyMPK_SW *pmpk = new SM9ProxyMPK_SW;
			SM9ProxyMSK_SW *pmsk = new SM9ProxyMSK_SW;

			if (sm9_sw_generate_masterkey(*pparams, *pmpk,*pmsk) == TRUE) {
				error = SM9_ERROR_NONE;
			}

			*mpk = (void*) pmpk;
			*msk = (void*) pmsk;

			return error;

			break;
		}
	case SM9_SCHEME_HW:
		{
			return error;
			break;
		}
	}
}

//
//// proxylib_generateKeys()
////
//// Generate a public/private keypair.  Allocates memory.
//
//int proxylib_generateKeys(void *params, void **pk, void **sk,
//		SCHEME_TYPE schemeID) {
//	CurveParams *cp = (CurveParams*) params;
//	int error = SM9_ERROR_OTHER;
//
//	switch (schemeID) {
//	case SCHEME_PRE1: {
//		ProxyPK_PRE1 *pubKey = new ProxyPK_PRE1();
//		ProxySK_PRE1 *secKey = new ProxySK_PRE1();
//		if (PRE1_keygen(*cp, *pubKey, *secKey) == TRUE) {
//			error = SM9_ERROR_NONE;
//		}
//		*pk = (void*) pubKey;
//		*sk = (void*) secKey;
//	}
//		break;
//	case SCHEME_PRE2: {
//		ProxyPK_PRE2 *pubKey = new ProxyPK_PRE2();
//		ProxySK_PRE2 *secKey = new ProxySK_PRE2();
//		if (PRE2_keygen(*cp, *pubKey, *secKey) == TRUE) {
//			error = SM9_ERROR_NONE;
//		}
//		*pk = (void*) pubKey;
//		*sk = (void*) secKey;
//	}
//		break;
//	}
//
//	return error;
//}
//
//// proxylib_serializeKeys()
////
//// Serializes a pair of keys into a buffer.
//
//int proxylib_serializeKeys(void *params, void *pk, void *sk, char *pkBuf,
//		char *skBuf, int *pkBufSize, int *skBufSize, int bufferAvailSize,
//		SCHEME_TYPE schemeID) {
//	int error = SM9_ERROR_OTHER;
//
//	CurveParams *cp = (CurveParams*) params;
//	switch (schemeID) {
//	case SCHEME_PRE1: {
//		ProxyPK_PRE1 *pubkey = (ProxyPK_PRE1*) pk;
//		*pkBufSize
//				= pubkey->serialize(SM9_SERIALIZE_BINARY, pkBuf, bufferAvailSize);
//		if (*pkBufSize > 0) {
//			error = SM9_ERROR_NONE;
//		}
//
//		ProxySK_PRE1 *seckey = (ProxySK_PRE1*) sk;
//		*skBufSize
//				= seckey->serialize(SM9_SERIALIZE_BINARY, skBuf, bufferAvailSize);
//		if (*skBufSize > 0) {
//			error = SM9_ERROR_NONE;
//		}
//		break;
//	}
//	case SCHEME_PRE2: {
//		ProxyPK_PRE2 *pubkey = (ProxyPK_PRE2*) pk;
//		*pkBufSize
//				= pubkey->serialize(SM9_SERIALIZE_BINARY, pkBuf, bufferAvailSize);
//		if (*pkBufSize > 0) {
//			error = SM9_ERROR_NONE;
//		}
//
//		ProxySK_PRE2 *seckey = (ProxySK_PRE2*) sk;
//		*skBufSize
//				= seckey->serialize(SM9_SERIALIZE_BINARY, skBuf, bufferAvailSize);
//		if (*skBufSize > 0) {
//			error = SM9_ERROR_NONE;
//		}
//		break;
//	}
//	}
//	return error;
//}
//
//// proxylib_serializeKeys()
////
//// Serializes a pair of keys into a buffer.
//
//int proxylib_deserializeKeys(void *params, char *pkBuf, char *skBuf,
//		int pkBufSize, int skBufSize, void **pk, void **sk,
//		SCHEME_TYPE schemeID) {
//	int error = SM9_ERROR_OTHER;
//
//	CurveParams *cp = (CurveParams*) params;
//	switch (schemeID) {
//	case SCHEME_PRE1: {
//		ProxyPK_PRE1 *pubkey = new ProxyPK_PRE1;
//		if (pubkey->deserialize(SM9_SERIALIZE_BINARY, pkBuf, pkBufSize) == TRUE) {
//			error = SM9_ERROR_NONE;
//		}
//		*pk = (void*) pubkey;
//
//		ProxySK_PRE1 *seckey = new ProxySK_PRE1;
//		if (seckey->deserialize(SM9_SERIALIZE_BINARY, skBuf, skBufSize) == TRUE) {
//			error = SM9_ERROR_NONE;
//		}
//		*sk = (void*) seckey;
//		break;
//	}
//	case SCHEME_PRE2: {
//		ProxyPK_PRE2 *pubkey = new ProxyPK_PRE2;
//		if (pubkey->deserialize(SM9_SERIALIZE_BINARY, pkBuf, pkBufSize) == TRUE) {
//			error = SM9_ERROR_NONE;
//		}
//		*pk = (void*) pubkey;
//
//		ProxySK_PRE2 *seckey = new ProxySK_PRE2;
//		if (seckey->deserialize(SM9_SERIALIZE_BINARY, skBuf, skBufSize) == TRUE) {
//			error = SM9_ERROR_NONE;
//		}
//		*sk = (void*) seckey;
//		break;
//	}
//	}
//
//	return error;
//}
//
//// proxylib_destroyKeys()
////
//// Destroy a public or secret key.
//
//int proxylib_destroyKeys(void *pk, void *sk, SCHEME_TYPE schemeID) {
//	int error = SM9_ERROR_NONE;
//
//	switch (schemeID) {
//	case SCHEME_PRE1: {
//		if (pk != NULL) {
//			ProxyPK_PRE1 *pubkey = (ProxyPK_PRE1*) pk;
//			delete pubkey;
//		}
//
//		if (sk != NULL) {
//			ProxySK_PRE1 *seckey = (ProxySK_PRE1*) sk;
//			delete seckey;
//		}
//	}
//		break;
//
//	case SCHEME_PRE2: {
//		if (pk != NULL) {
//			ProxyPK_PRE2 *pubkey = (ProxyPK_PRE2*) pk;
//			delete pubkey;
//		}
//
//		if (sk != NULL) {
//			ProxySK_PRE2 *seckey = (ProxySK_PRE2*) sk;
//			delete seckey;
//		}
//	}
//		break;
//	}
//
//	return error;
//}
//
//// proxylib_encrypt()
////
//// Encrypt a message using a public key.
////
//// Returns: SM9_ERROR_NONE, SM9_ERROR_OTHER, SM9_ERROR_PLAINTEXT_TOO_LONG
//
//int proxylib_encrypt(void *params, void *pk, char *message, int messageLen,
//		char *ciphertext, int *ciphLen, CIPHERTEXT_TYPE ctype,
//		SCHEME_TYPE schemeID) {
//	int error = SM9_ERROR_OTHER;
//	CurveParams *cp = (CurveParams *) params;
//	Big msg;
//
//	if (encodePlaintextAsBig(*cp, message, messageLen, msg) == FALSE) {
//		error = SM9_ERROR_PLAINTEXT_TOO_LONG;
//		return error;
//	}
//
//	switch (schemeID) {
//	case SCHEME_PRE1: {
//
//		ProxyPK_PRE1 *pubkey = (ProxyPK_PRE1 *) pk;
//		ProxyCiphertext_PRE1 *ctext_ptr = new ProxyCiphertext_PRE1();
//		switch (ctype) {
//		case CIPH_FIRST_LEVEL:
//			if (PRE1_level1_encrypt(*cp, msg, *pubkey, *ctext_ptr) == FALSE) {
//				return SM9_ERROR_OTHER;
//			}
//			break;
//		case CIPH_SECOND_LEVEL:
//			if (PRE1_level2_encrypt(*cp, msg, *pubkey, *ctext_ptr) == FALSE) {
//				return SM9_ERROR_OTHER;
//			}
//			break;
//		default:
//			return SM9_ERROR_OTHER;
//		}
//		/* No information provided in documentation (there is no documentation)
//		 * regarding required values. All chosen array sizes are completely arbitrary.
//		 */
//		*ciphLen = ctext_ptr->getSerializedSize(SM9_SERIALIZE_BINARY);
//
//		int result = ctext_ptr->serialize(SM9_SERIALIZE_BINARY, ciphertext,
//				*ciphLen+1000);
//		if (result > 0) {
//			result = result;
//			return 0;
//		}
//	}
//		break;
//	case SCHEME_PRE2: {
//		ProxyPK_PRE2 *pubkey = (ProxyPK_PRE2 *) pk;
//		ProxyCiphertext_PRE2 ctext;
//		switch (ctype) {
//		case CIPH_FIRST_LEVEL:
//			if (PRE2_level1_encrypt(*cp, msg, *pubkey, ctext) == FALSE) {
//				return SM9_ERROR_OTHER;
//			}
//			break;
//		case CIPH_SECOND_LEVEL:
//			if (PRE2_level2_encrypt(*cp, msg, *pubkey, ctext) == FALSE) {
//				return SM9_ERROR_OTHER;
//			}
//			break;
//		default:
//			return SM9_ERROR_OTHER;
//		}
//
//		*ciphLen = ctext.serialize(SM9_SERIALIZE_BINARY, ciphertext, *ciphLen);
//		if (*ciphLen > 0) {
//			error = SM9_ERROR_NONE;
//		}
//	}
//		break;
//	}
//
//	return error;
//}
//
//// proxylib_decrypt()
////
//// Decrypt a message using a secret key.  Places the result into
//// message, and returns the length in messageLen.
////
//// Returns: SM9_ERROR_NONE, SM9_ERROR_OTHER, SM9_ERROR_PLAINTEXT_TOO_LONG
//
//int proxylib_decrypt(void *params, void *sk, char *message, int *messageLen,
//		char *ciphertext, int ciphLen, SCHEME_TYPE schemeID) {
//	int error = SM9_ERROR_NONE;
//	CurveParams *cp = (CurveParams *) params;
//	Big msg;
//	switch (schemeID) {
//	case SCHEME_PRE1: {
//
//		// Deserialize the ciphertext
//		ProxyCiphertext_PRE1 ctext;
//		if (ctext.deserialize(SM9_SERIALIZE_BINARY, ciphertext, ciphLen) == FALSE) {
//			return SM9_ERROR_OTHER;
//		}
//
//		ProxySK_PRE1 *seckey = (ProxySK_PRE1 *) sk;
//		if (PRE1_decrypt(*cp, ctext, *seckey, msg) == FALSE) {
//			return SM9_ERROR_OTHER;
//		}
//	}
//		break;
//	case SCHEME_PRE2: {
//		// Deserialize the ciphertext
//		ProxyCiphertext_PRE2 ctext;
//		if (ctext.deserialize(SM9_SERIALIZE_BINARY, ciphertext, ciphLen) == FALSE) {
//			return SM9_ERROR_OTHER;
//		}
//
//		ProxySK_PRE2 *seckey = (ProxySK_PRE2 *) sk;
//		if (PRE2_decrypt(*cp, ctext, *seckey, msg) == FALSE) {
//			return SM9_ERROR_OTHER;
//		}
//	}
//		break;
//	default:
//		return SM9_ERROR_OTHER;
//	}
//
//	// Decode the result as a binary buffer
//	/* Have arbitrarily changed the third parameter
//	 * No guide on what it should be
//	 * The previous function call: (decodePlaintextFromBig(*cp, message, *messageLen, messageLen, msg)
//	 * seemed incorrect.
//	 */
//	if (decodePlaintextFromBig(*cp, message, 1000, messageLen, msg)
//			== FALSE) {
//		return SM9_ERROR_OTHER;
//	}
//
//	return error;
//}
//
//// proxylib_generateDelegationKey()
////
//// Generate a public/private keypair.  Allocates memory.
//
//int proxylib_generateDelegationKey(void *params, void *sk1, void *pk2,
//		void** delKey, SCHEME_TYPE schemeID) {
//	CurveParams *cp = (CurveParams*) params;
//	int error = SM9_ERROR_OTHER;
//
//	switch (schemeID) {
//	case SCHEME_PRE1: {
//		ProxyPK_PRE1 *pubKey = (ProxyPK_PRE1 *) pk2;
//		ProxySK_PRE1 *secKey = (ProxySK_PRE1 *) sk1;
//		DelegationKey_PRE1 *delegationKey = new DelegationKey_PRE1;
//
//		//memset(delegationKey,0,sizeof(DelegationKey_PRE1));
//
//		if (PRE1_delegate(*cp, *pubKey, *secKey, *delegationKey) == TRUE) {
//			error = SM9_ERROR_NONE;
//		}
//		*delKey = (void*) delegationKey;
//	}
//		break;
//	case SCHEME_PRE2: {
//		ProxyPK_PRE2 *pubKey = (ProxyPK_PRE2 *) pk2;
//		ProxySK_PRE2 *secKey = (ProxySK_PRE2 *) sk1;
//		DelegationKey_PRE2 *delegationKey = new DelegationKey_PRE2;
//
//		//memset(delegationKey,0,sizeof(DelegationKey_PRE2));
//
//		if (PRE2_delegate(*cp, *pubKey, *secKey, *delegationKey) == TRUE) {
//			error = SM9_ERROR_NONE;
//		}
//		*delKey = (void*) delegationKey;
//	}
//		break;
//	}
//
//	return error;
//}
//
//// proxylib_serializeDelegationKey()
////
//// Serializes a pair of keys into a buffer.
//
//int proxylib_serializeDelegationKey(void *params, void *delKey,
//		char *delKeyBuf, int *delKeyBufSize, int bufferAvailSize,
//		SCHEME_TYPE schemeID) {
//	int error = SM9_ERROR_OTHER;
//
//	CurveParams *cp = (CurveParams*) params;
//	switch (schemeID) {
//	case SCHEME_PRE1: {
//		DelegationKey_PRE1 *dk = (DelegationKey_PRE1*) delKey;
//		*delKeyBufSize = SerializeDelegationKey_PRE1(*dk, SM9_SERIALIZE_BINARY,
//				delKeyBuf, bufferAvailSize);
//		if (*delKeyBufSize > 0) {
//			error = SM9_ERROR_NONE;
//		}
//		break;
//	}
//	case SCHEME_PRE2: {
//		DelegationKey_PRE2 *dk = (DelegationKey_PRE2*) delKey;
//		*delKeyBufSize = SerializeDelegationKey_PRE2(*dk, SM9_SERIALIZE_BINARY,
//				delKeyBuf, bufferAvailSize);
//		if (*delKeyBufSize > 0) {
//			error = SM9_ERROR_NONE;
//		}
//		break;
//	}
//	}
//
//	return error;
//}
//
//// proxylib_deserializeDelegationKey()
//// ALTERNATIVE DEFINITION - WHAT??
//// Deserializes a buffer of parameters and returns a newly-allocated buffer.
///*
// int
// proxylib_deserializeParams(char *buffer, int bufferSize, void **delKey,
// SCHEME_TYPE schemeID)
// {
// int error = SM9_ERROR_OTHER;
//
// switch(schemeID) {
// case SCHEME_PRE1:
// {
// DelegationKey_PRE1 *dk = new DelegationKey_PRE1;
// if (DeserializeDelegationKey_PRE1(*dk, SM9_SERIALIZE_BINARY,
// buffer, bufferSize) == FALSE) {
// delete dk;
// *delKey = NULL;
// } else {
// error = SM9_ERROR_NONE;
// *delKey = (void*)dk;
// }
// break;
// }
// case SCHEME_PRE2:
// {
// DelegationKey_PRE2 *dk = new DelegationKey_PRE2;
// if (DeserializeDelegationKey_PRE2(*dk, SM9_SERIALIZE_BINARY,
// buffer, bufferSize) == FALSE) {
// delete dk;
// *delKey = NULL;
// } else {
// error = SM9_ERROR_NONE;
// *delKey = (void*)dk;
// }
// break;
// }
// }
//
// return error;
// }
// */
//// proxylib_destroyKeys()
////
//// Destroy a public or secret key.
//
//int proxylib_destroyDelegationKey(void *delKey, SCHEME_TYPE schemeID) {
//	int error = SM9_ERROR_NONE;
//
//	switch (schemeID) {
//	case SCHEME_PRE1: {
//		if (delKey != NULL) {
//			DelegationKey_PRE1 *dk = (DelegationKey_PRE1*) delKey;
//			delete dk;
//		}
//	}
//		break;
//
//	case SCHEME_PRE2: {
//		if (delKey != NULL) {
//			DelegationKey_PRE2 *dk = (DelegationKey_PRE2*) delKey;
//			delete dk;
//		}
//	}
//		break;
//	}
//
//	return error;
//}
//
//// proxylib_reencrypt()
////
//// Re-encryps a ciphertext given a re-encryption key.  Outputs the
//// result to a new buffer.
//
//int proxylib_reencrypt(void *params, void *rk, char *ciphertext, int ciphLen,
//		char *newciphertext, int *newCiphLen, SCHEME_TYPE schemeID) {
//	int error = SM9_ERROR_OTHER;
//	CurveParams *cp = (CurveParams *) params;
//
//	switch (schemeID) {
//	case SCHEME_PRE1: {
//		// Deserialize the original ciphertext
//		ProxyCiphertext_PRE1 ctext;
//		ProxyCiphertext_PRE1 newctext;
//		if (ctext.deserialize(SM9_SERIALIZE_BINARY, ciphertext, ciphLen) == FALSE) {
//			return SM9_ERROR_OTHER;
//		}
//
//		// Reencrypt the ciphertext using the re-encryption key
//		DelegationKey_PRE1 *delKey = (DelegationKey_PRE1 *) rk;
//		if (PRE1_reencrypt(*cp, ctext, *delKey, newctext) == FALSE) {
//			return SM9_ERROR_OTHER;
//		}
//
//		// Serialize the re-encrypted ciphertext
//		*newCiphLen = newctext.getSerializedSize(SM9_SERIALIZE_BINARY);
//		newctext.serialize(SM9_SERIALIZE_BINARY, newciphertext,*newCiphLen+1000);
//		if (*newCiphLen > 0) {
//			error = SM9_ERROR_NONE;
//		}
//	}
//		break;
//	case SCHEME_PRE2: {
//		// Deserialize the original ciphertext
//		ProxyCiphertext_PRE2 ctext;
//		ProxyCiphertext_PRE2 newctext;
//		if (ctext.deserialize(SM9_SERIALIZE_BINARY, ciphertext, ciphLen) == FALSE) {
//			return SM9_ERROR_OTHER;
//		}
//
//		// Reencrypt the ciphertext using the re-encryption key
//		DelegationKey_PRE2 *delKey = (ECn *) rk;
//		if (PRE2_reencrypt(*cp, ctext, *delKey, newctext) == FALSE) {
//			return SM9_ERROR_OTHER;
//		}
//
//		// Serialize the re-encrypted ciphertext
//		*newCiphLen = newctext.serialize(SM9_SERIALIZE_BINARY, newciphertext,
//				*newCiphLen);
//		if (*newCiphLen > 0) {
//			error = SM9_ERROR_NONE;
//		}
//	}
//		break;
//	default:
//		return SM9_ERROR_OTHER;
//	}
//
//	return error;
//}
