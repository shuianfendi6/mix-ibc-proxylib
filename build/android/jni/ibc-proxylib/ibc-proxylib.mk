LOCAL_PATH :=$(call my-dir)/../../../../src

include $(CLEAR_VARS)

LOCAL_MODULE := libO_AllNSS

LOCAL_C_INCLUDES := -I$(LOCAL_PATH)
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nspr/pr/include
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nspr/pr/include/private
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nspr/lib/ds
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nspr/lib/libc/include
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nspr/pr/include/obsolete 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/base 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/certdb 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/certhigh 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/cryptohi 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/dev 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/freebl 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/freebl/ecl 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/freebl/mpi 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/nss 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/pk11wrap 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/pkcs7 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/pki 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/smime 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/softoken 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/ssl 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/util 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/ckfw 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/pkix_pl_nss/system
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/pkix/util
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/include
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/pkix/checker
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/pkix/results
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/pkix/certsel
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/pkix/crlsel
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/pkix/params
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/pkix/store
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/pkix/top
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/pkix_pl_nss/module
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/libpkix/pkix_pl_nss/pki
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/ssl
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/nss/lib/pkcs12
LOCAL_C_INCLUDES += -I/usr/include
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/O_All/SM3SM4
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/O_All/SqlDB
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/O_All/sqlite
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/O_All

#nspr
LOCAL_SRC_FILES := nspr/lib/ds/plarena.c
LOCAL_SRC_FILES += nspr/lib/ds/plhash.c
LOCAL_SRC_FILES += nspr/lib/libc/src/base64.c
LOCAL_SRC_FILES += nspr/lib/libc/src/plerror.c
LOCAL_SRC_FILES += nspr/lib/libc/src/plgetopt.c
LOCAL_SRC_FILES += nspr/lib/libc/src/strcase.c
LOCAL_SRC_FILES += nspr/lib/libc/src/strcat.c
LOCAL_SRC_FILES += nspr/lib/libc/src/strchr.c
LOCAL_SRC_FILES += nspr/lib/libc/src/strcmp.c
LOCAL_SRC_FILES += nspr/lib/libc/src/strcpy.c
LOCAL_SRC_FILES += nspr/lib/libc/src/strdup.c
LOCAL_SRC_FILES += nspr/lib/libc/src/strlen.c
LOCAL_SRC_FILES += nspr/lib/libc/src/strpbrk.c
LOCAL_SRC_FILES += nspr/lib/libc/src/strstr.c
LOCAL_SRC_FILES += nspr/lib/libc/src/strtok.c
LOCAL_SRC_FILES += nspr/pr/src/io/prdir.c
LOCAL_SRC_FILES += nspr/pr/src/io/prfdcach.c
#LOCAL_SRC_FILES += nspr/pr/src/io/prio.c
LOCAL_SRC_FILES += nspr/pr/src/io/priometh.c
LOCAL_SRC_FILES += nspr/pr/src/io/pripv6.c
LOCAL_SRC_FILES += nspr/pr/src/io/prlayer.c
LOCAL_SRC_FILES += nspr/pr/src/io/prlog.c
LOCAL_SRC_FILES += nspr/pr/src/io/prmapopt.c
LOCAL_SRC_FILES += nspr/pr/src/io/prmmap.c
LOCAL_SRC_FILES += nspr/pr/src/io/prmwait.c
LOCAL_SRC_FILES += nspr/pr/src/io/prpolevt.c
LOCAL_SRC_FILES += nspr/pr/src/io/prprf.c
LOCAL_SRC_FILES += nspr/pr/src/io/prscanf.c
LOCAL_SRC_FILES += nspr/pr/src/io/prstdio.c
LOCAL_SRC_FILES += nspr/pr/src/linking/prlink.c
LOCAL_SRC_FILES += nspr/pr/src/malloc/prmalloc.c
LOCAL_SRC_FILES += nspr/pr/src/malloc/prmem.c
LOCAL_SRC_FILES += nspr/pr/src/md/prosdep.c
LOCAL_SRC_FILES += nspr/pr/src/md/unix/linux.c
LOCAL_SRC_FILES += nspr/pr/src/md/unix/unix.c
LOCAL_SRC_FILES += nspr/pr/src/md/unix/unix_errors.c
LOCAL_SRC_FILES += nspr/pr/src/md/unix/uxproces.c
LOCAL_SRC_FILES += nspr/pr/src/md/unix/uxrng.c
LOCAL_SRC_FILES += nspr/pr/src/md/unix/uxshm.c
LOCAL_SRC_FILES += nspr/pr/src/md/unix/uxwrap.c

ifeq ($(TARGET_ARCH), arm)

else ifeq ($(TARGET_ARCH), x86)
LOCAL_SRC_FILES += nspr/pr/src/md/unix/os_Linux_x86.s
endif

LOCAL_SRC_FILES += nspr/pr/src/memory/prseg.c
LOCAL_SRC_FILES += nspr/pr/src/memory/prshm.c
LOCAL_SRC_FILES += nspr/pr/src/memory/prshma.c
LOCAL_SRC_FILES += nspr/pr/src/misc/pralarm.c
LOCAL_SRC_FILES += nspr/pr/src/misc/pratom.c
LOCAL_SRC_FILES += nspr/pr/src/misc/praton.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prcountr.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prdtoa.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prenv.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prerr.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prerror.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prerrortable.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prinit.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prinrval.c
LOCAL_SRC_FILES += nspr/pr/src/misc/pripc.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prlog2.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prlong.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prnetdb.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prolock.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prrng.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prsystem.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prthinfo.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prtime.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prtpool.c
LOCAL_SRC_FILES += nspr/pr/src/misc/prtrace.c
LOCAL_SRC_FILES += nspr/pr/src/pthreads/ptio.c
LOCAL_SRC_FILES += nspr/pr/src/pthreads/ptmisc.c
LOCAL_SRC_FILES += nspr/pr/src/pthreads/ptsynch.c
LOCAL_SRC_FILES += nspr/pr/src/pthreads/ptthread.c
LOCAL_SRC_FILES += nspr/pr/src/threads/prcmon.c
LOCAL_SRC_FILES += nspr/pr/src/threads/prrwlock.c
LOCAL_SRC_FILES += nspr/pr/src/threads/prsem.c
LOCAL_SRC_FILES += nspr/pr/src/threads/prtpd.c

###nss
LOCAL_SRC_FILES += nss/lib/base/arena.c
LOCAL_SRC_FILES += nss/lib/base/error.c
LOCAL_SRC_FILES += nss/lib/base/errorval.c
LOCAL_SRC_FILES += nss/lib/base/hash.c
LOCAL_SRC_FILES += nss/lib/base/hashops.c
LOCAL_SRC_FILES += nss/lib/base/item.c
LOCAL_SRC_FILES += nss/lib/base/libc.c
LOCAL_SRC_FILES += nss/lib/base/list.c
LOCAL_SRC_FILES += nss/lib/base/nssutf8.c
LOCAL_SRC_FILES += nss/lib/base/tracker.c
LOCAL_SRC_FILES += nss/lib/certdb/alg1485.c
LOCAL_SRC_FILES += nss/lib/certdb/certdb.c
LOCAL_SRC_FILES += nss/lib/certdb/certv3.c
LOCAL_SRC_FILES += nss/lib/certdb/certxutl.c
LOCAL_SRC_FILES += nss/lib/certdb/crl.c
LOCAL_SRC_FILES += nss/lib/certdb/genname.c
LOCAL_SRC_FILES += nss/lib/certdb/polcyxtn.c
LOCAL_SRC_FILES += nss/lib/certdb/secname.c
LOCAL_SRC_FILES += nss/lib/certdb/stanpcertdb.c
LOCAL_SRC_FILES += nss/lib/certdb/xauthkid.c
LOCAL_SRC_FILES += nss/lib/certdb/xbsconst.c
LOCAL_SRC_FILES += nss/lib/certdb/xconst.c
LOCAL_SRC_FILES += nss/lib/certhigh/certhigh.c
LOCAL_SRC_FILES += nss/lib/certhigh/certhtml.c
LOCAL_SRC_FILES += nss/lib/certhigh/certreq.c
LOCAL_SRC_FILES += nss/lib/certhigh/certvfy.c
LOCAL_SRC_FILES += nss/lib/certhigh/certvfypkix.c
LOCAL_SRC_FILES += nss/lib/certhigh/crlv2.c
LOCAL_SRC_FILES += nss/lib/certhigh/ocsp.c
LOCAL_SRC_FILES += nss/lib/certhigh/ocspsig.c
LOCAL_SRC_FILES += nss/lib/certhigh/xcrldist.c
LOCAL_SRC_FILES += nss/lib/ckfw/builtins/anchor.c
LOCAL_SRC_FILES += nss/lib/ckfw/builtins/bfind.c
LOCAL_SRC_FILES += nss/lib/ckfw/builtins/binst.c
LOCAL_SRC_FILES += nss/lib/ckfw/builtins/bobject.c
LOCAL_SRC_FILES += nss/lib/ckfw/builtins/bsession.c
LOCAL_SRC_FILES += nss/lib/ckfw/builtins/bslot.c
LOCAL_SRC_FILES += nss/lib/ckfw/builtins/btoken.c
LOCAL_SRC_FILES += nss/lib/ckfw/builtins/certdata.c
LOCAL_SRC_FILES += nss/lib/ckfw/builtins/ckbiver.c
LOCAL_SRC_FILES += nss/lib/ckfw/builtins/constants.c
LOCAL_SRC_FILES += nss/lib/ckfw/crypto.c
LOCAL_SRC_FILES += nss/lib/ckfw/find.c
LOCAL_SRC_FILES += nss/lib/ckfw/hash.c
LOCAL_SRC_FILES += nss/lib/ckfw/instance.c
LOCAL_SRC_FILES += nss/lib/ckfw/mechanism.c
LOCAL_SRC_FILES += nss/lib/ckfw/mutex.c
LOCAL_SRC_FILES += nss/lib/ckfw/object.c
LOCAL_SRC_FILES += nss/lib/ckfw/session.c
LOCAL_SRC_FILES += nss/lib/ckfw/sessobj.c
LOCAL_SRC_FILES += nss/lib/ckfw/slot.c
LOCAL_SRC_FILES += nss/lib/ckfw/token.c
LOCAL_SRC_FILES += nss/lib/ckfw/wrap.c
LOCAL_SRC_FILES += nss/lib/cryptohi/dsautil.c
LOCAL_SRC_FILES += nss/lib/cryptohi/sechash.c
LOCAL_SRC_FILES += nss/lib/cryptohi/seckey.c
LOCAL_SRC_FILES += nss/lib/cryptohi/secsign.c
LOCAL_SRC_FILES += nss/lib/cryptohi/secvfy.c
LOCAL_SRC_FILES += nss/lib/dev/ckhelper.c
LOCAL_SRC_FILES += nss/lib/dev/devslot.c
LOCAL_SRC_FILES += nss/lib/dev/devtoken.c
LOCAL_SRC_FILES += nss/lib/dev/devutil.c
LOCAL_SRC_FILES += nss/lib/freebl/aeskeywrap.c
LOCAL_SRC_FILES += nss/lib/freebl/alg2268.c
LOCAL_SRC_FILES += nss/lib/freebl/alghmac.c
LOCAL_SRC_FILES += nss/lib/freebl/arcfive.c
LOCAL_SRC_FILES += nss/lib/freebl/arcfour.c
LOCAL_SRC_FILES += nss/lib/freebl/camellia.c
LOCAL_SRC_FILES += nss/lib/freebl/ctr.c
LOCAL_SRC_FILES += nss/lib/freebl/cts.c
LOCAL_SRC_FILES += nss/lib/freebl/des.c
LOCAL_SRC_FILES += nss/lib/freebl/desblapi.c
LOCAL_SRC_FILES += nss/lib/freebl/dh.c
LOCAL_SRC_FILES += nss/lib/freebl/drbg.c
LOCAL_SRC_FILES += nss/lib/freebl/dsa.c
LOCAL_SRC_FILES += nss/lib/freebl/ec.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ec_naf.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecl.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecl_curve.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecl_gf.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecl_mult.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecp_256.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecp_256_32.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecp_384.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecp_521.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecp_aff.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecp_jac.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecp_jm.c
LOCAL_SRC_FILES += nss/lib/freebl/ecl/ecp_mont.c
LOCAL_SRC_FILES += nss/lib/freebl/gcm.c
LOCAL_SRC_FILES += nss/lib/freebl/hmacct.c
LOCAL_SRC_FILES += nss/lib/freebl/jpake.c
LOCAL_SRC_FILES += nss/lib/freebl/md2.c
LOCAL_SRC_FILES += nss/lib/freebl/md5.c
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mp_gf2m.c
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mpcpucache.c

ifeq ($(TARGET_ARCH), arm)
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mpi.c
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mpi_arm.c
else ifeq ($(TARGET_ARCH), x86)
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mpi.c
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mpi_x86.s
else ifeq ($(TARGET_ARCH), mips)
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mpi.c
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mpi_mips.s
else ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mpi.c
endif

#LOCAL_SRC_FILES += nss/lib/freebl/mpi/mpi_arm_mac.c
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mplogic.c
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mpmontg.c
LOCAL_SRC_FILES += nss/lib/freebl/mpi/mpprime.c
#LOCAL_SRC_FILES += nss/lib/freebl/mpi/primes.c
LOCAL_SRC_FILES += nss/lib/freebl/pqg.c
LOCAL_SRC_FILES += nss/lib/freebl/rawhash.c
LOCAL_SRC_FILES += nss/lib/freebl/rijndael.c
LOCAL_SRC_FILES += nss/lib/freebl/rsa.c
LOCAL_SRC_FILES += nss/lib/freebl/rsapkcs.c
LOCAL_SRC_FILES += nss/lib/freebl/seed.c
LOCAL_SRC_FILES += nss/lib/freebl/sha512.c
LOCAL_SRC_FILES += nss/lib/freebl/sha_fast.c
LOCAL_SRC_FILES += nss/lib/freebl/shvfy.c
LOCAL_SRC_FILES += nss/lib/freebl/sysrand.c
LOCAL_SRC_FILES += nss/lib/freebl/tlsprfalg.c
LOCAL_SRC_FILES += nss/lib/freebl/unix_rand.c
LOCAL_SRC_FILES += nss/lib/freebl/win_rand.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/certsel/pkix_certselector.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/certsel/pkix_comcertselparams.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_basicconstraintschecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_certchainchecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_crlchecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_ekuchecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_expirationchecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_namechainingchecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_nameconstraintschecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_ocspchecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_policychecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_revocationchecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_revocationmethod.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_signaturechecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/checker/pkix_targetcertchecker.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/crlsel/pkix_comcrlselparams.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/crlsel/pkix_crlselector.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/params/pkix_procparams.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/params/pkix_resourcelimits.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/params/pkix_trustanchor.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/params/pkix_valparams.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/results/pkix_buildresult.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/results/pkix_policynode.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/results/pkix_valresult.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/results/pkix_verifynode.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/store/pkix_store.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/top/pkix_build.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/top/pkix_lifecycle.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/top/pkix_validate.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/util/pkix_error.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/util/pkix_errpaths.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/util/pkix_list.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/util/pkix_logger.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix/util/pkix_tools.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_aiamgr.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_colcertstore.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_httpcertstore.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_httpdefaultclient.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_ldapcertstore.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_ldapdefaultclient.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_ldaprequest.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_ldapresponse.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_ldaptemplates.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_nsscontext.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_pk11certstore.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/module/pkix_pl_socket.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_basicconstraints.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_cert.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_certpolicyinfo.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_certpolicymap.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_certpolicyqualifier.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_crl.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_crldp.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_crlentry.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_date.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_generalname.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_infoaccess.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_nameconstraints.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_ocspcertid.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_ocsprequest.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_ocspresponse.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_publickey.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/pki/pkix_pl_x500name.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_bigint.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_bytearray.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_common.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_error.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_hashtable.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_lifecycle.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_mem.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_monitorlock.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_mutex.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_object.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_oid.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_primhash.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_rwlock.c
LOCAL_SRC_FILES += nss/lib/libpkix/pkix_pl_nss/system/pkix_pl_string.c
LOCAL_SRC_FILES += nss/lib/nss/nssinit.c
LOCAL_SRC_FILES += nss/lib/nss/nssver.c
LOCAL_SRC_FILES += nss/lib/nss/utilwrap.c
#LOCAL_SRC_FILES += nss/lib/pk11wrap/debug_module.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/dev3hack.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11akey.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11auth.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11cert.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11cxt.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11err.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11kea.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11list.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11load.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11mech.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11merge.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11nobj.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11obj.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11pars.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11pbe.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11pk12.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11pqg.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11sdr.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11skey.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11slot.c
LOCAL_SRC_FILES += nss/lib/pk11wrap/pk11util.c
LOCAL_SRC_FILES += nss/lib/pkcs7/certread.c
LOCAL_SRC_FILES += nss/lib/pkcs7/p7common.c
LOCAL_SRC_FILES += nss/lib/pkcs7/p7create.c
LOCAL_SRC_FILES += nss/lib/pkcs7/p7decode.c
LOCAL_SRC_FILES += nss/lib/pkcs7/p7encode.c
LOCAL_SRC_FILES += nss/lib/pkcs7/p7local.c
LOCAL_SRC_FILES += nss/lib/pkcs7/secmime.c
LOCAL_SRC_FILES += nss/lib/pki/asymmkey.c
LOCAL_SRC_FILES += nss/lib/pki/certdecode.c
LOCAL_SRC_FILES += nss/lib/pki/certificate.c
LOCAL_SRC_FILES += nss/lib/pki/cryptocontext.c
LOCAL_SRC_FILES += nss/lib/pki/pki3hack.c
LOCAL_SRC_FILES += nss/lib/pki/pkibase.c
LOCAL_SRC_FILES += nss/lib/pki/pkistore.c
LOCAL_SRC_FILES += nss/lib/pki/symmkey.c
LOCAL_SRC_FILES += nss/lib/pki/tdcache.c
LOCAL_SRC_FILES += nss/lib/pki/trustdomain.c
LOCAL_SRC_FILES += nss/lib/softoken/ecdecode.c
LOCAL_SRC_FILES += nss/lib/softoken/fipsaudt.c
LOCAL_SRC_FILES += nss/lib/softoken/fipstest.c
LOCAL_SRC_FILES += nss/lib/softoken/fipstokn.c
LOCAL_SRC_FILES += nss/lib/softoken/jpakesftk.c
LOCAL_SRC_FILES += nss/lib/softoken/lgglue.c
LOCAL_SRC_FILES += nss/lib/softoken/lowkey.c
LOCAL_SRC_FILES += nss/lib/softoken/lowpbe.c
LOCAL_SRC_FILES += nss/lib/softoken/padbuf.c
LOCAL_SRC_FILES += nss/lib/softoken/pkcs11.c
LOCAL_SRC_FILES += nss/lib/softoken/pkcs11c.c
LOCAL_SRC_FILES += nss/lib/softoken/pkcs11u.c
#LOCAL_SRC_FILES += nss/lib/softoken/rsawrapr.c
LOCAL_SRC_FILES += nss/lib/softoken/sdb.c
LOCAL_SRC_FILES += nss/lib/softoken/sftkdb.c
LOCAL_SRC_FILES += nss/lib/softoken/sftkhmac.c
LOCAL_SRC_FILES += nss/lib/softoken/sftkpars.c
LOCAL_SRC_FILES += nss/lib/softoken/sftkpwd.c
LOCAL_SRC_FILES += nss/lib/softoken/softkver.c
LOCAL_SRC_FILES += nss/lib/softoken/tlsprf.c
LOCAL_SRC_FILES += nss/lib/util/derdec.c
LOCAL_SRC_FILES += nss/lib/util/derenc.c
LOCAL_SRC_FILES += nss/lib/util/dersubr.c
LOCAL_SRC_FILES += nss/lib/util/dertime.c
LOCAL_SRC_FILES += nss/lib/util/errstrs.c
LOCAL_SRC_FILES += nss/lib/util/nssb64d.c
LOCAL_SRC_FILES += nss/lib/util/nssb64e.c
LOCAL_SRC_FILES += nss/lib/util/nssilock.c
LOCAL_SRC_FILES += nss/lib/util/nssrwlk.c
LOCAL_SRC_FILES += nss/lib/util/oidstring.c
LOCAL_SRC_FILES += nss/lib/util/portreg.c
LOCAL_SRC_FILES += nss/lib/util/quickder.c
LOCAL_SRC_FILES += nss/lib/util/secalgid.c
LOCAL_SRC_FILES += nss/lib/util/secasn1d.c
LOCAL_SRC_FILES += nss/lib/util/secasn1e.c
LOCAL_SRC_FILES += nss/lib/util/secasn1u.c
LOCAL_SRC_FILES += nss/lib/util/secdig.c
LOCAL_SRC_FILES += nss/lib/util/secitem.c
LOCAL_SRC_FILES += nss/lib/util/secoid.c
LOCAL_SRC_FILES += nss/lib/util/secport.c
LOCAL_SRC_FILES += nss/lib/util/sectime.c
LOCAL_SRC_FILES += nss/lib/util/templates.c
LOCAL_SRC_FILES += nss/lib/util/utf8.c
LOCAL_SRC_FILES += nss/lib/util/utilmod.c
LOCAL_SRC_FILES += nss/lib/util/utilpars.c
LOCAL_SRC_FILES += nss/lib/util/pkcs1sig.c
###ssl
LOCAL_SRC_FILES += nss/lib/ssl/authcert.c
LOCAL_SRC_FILES += nss/lib/ssl/cmpcert.c
LOCAL_SRC_FILES += nss/lib/ssl/derive.c
LOCAL_SRC_FILES += nss/lib/ssl/dtlscon.c
LOCAL_SRC_FILES += nss/lib/ssl/os2_err.c
LOCAL_SRC_FILES += nss/lib/ssl/prelib.c
LOCAL_SRC_FILES += nss/lib/ssl/ssl3con.c
LOCAL_SRC_FILES += nss/lib/ssl/ssl3ecc.c
LOCAL_SRC_FILES += nss/lib/ssl/ssl3ext.c
LOCAL_SRC_FILES += nss/lib/ssl/ssl3gthr.c
LOCAL_SRC_FILES += nss/lib/ssl/sslauth.c
LOCAL_SRC_FILES += nss/lib/ssl/sslcon.c
LOCAL_SRC_FILES += nss/lib/ssl/ssldef.c
LOCAL_SRC_FILES += nss/lib/ssl/sslenum.c
LOCAL_SRC_FILES += nss/lib/ssl/sslerr.c
LOCAL_SRC_FILES += nss/lib/ssl/sslerrstrs.c
LOCAL_SRC_FILES += nss/lib/ssl/sslgathr.c
LOCAL_SRC_FILES += nss/lib/ssl/sslinfo.c
LOCAL_SRC_FILES += nss/lib/ssl/sslinit.c
LOCAL_SRC_FILES += nss/lib/ssl/sslmutex.c
LOCAL_SRC_FILES += nss/lib/ssl/sslnonce.c
LOCAL_SRC_FILES += nss/lib/ssl/sslreveal.c
LOCAL_SRC_FILES += nss/lib/ssl/sslsecur.c
LOCAL_SRC_FILES += nss/lib/ssl/sslsnce.c
LOCAL_SRC_FILES += nss/lib/ssl/sslsock.c
LOCAL_SRC_FILES += nss/lib/ssl/ssltrace.c
LOCAL_SRC_FILES += nss/lib/ssl/sslver.c
LOCAL_SRC_FILES += nss/lib/ssl/unix_err.c
#LOCAL_SRC_FILES += nss/lib/ssl/win32err.c

###PKCS12
LOCAL_SRC_FILES += nss/lib/pkcs12/p12creat.c
LOCAL_SRC_FILES += nss/lib/pkcs12/p12d.c
LOCAL_SRC_FILES += nss/lib/pkcs12/p12dec.c
LOCAL_SRC_FILES += nss/lib/pkcs12/p12e.c
LOCAL_SRC_FILES += nss/lib/pkcs12/p12local.c
LOCAL_SRC_FILES += nss/lib/pkcs12/p12plcy.c
LOCAL_SRC_FILES += nss/lib/pkcs12/p12tmpl.c

###O_All
#LOCAL_SRC_FILES := O_All/type_def.c
LOCAL_SRC_FILES += O_All/O_All.c
#LOCAL_SRC_FILES += O_All/func_def.c
LOCAL_SRC_FILES += O_All/pkcs11-gotrust.c
LOCAL_SRC_FILES += O_All/sqlite/sqlite3secure.c
LOCAL_SRC_FILES += O_All/SM3SM4/sm3.c
LOCAL_SRC_FILES += O_All/SM3SM4/sms4.c
LOCAL_SRC_FILES += O_All/SM3SM4/sm4blapi.c
LOCAL_SRC_FILES += O_All/SM3SM4/sm3blapi.c
LOCAL_SRC_FILES += O_All/SqlDB/sqldb.c
LOCAL_SRC_FILES += O_All/FILE_LOG.c
#LOCAL_SRC_FILES += O_All/FilePfxImport.c
#LOCAL_SRC_FILES += O_All/ssl_func_def.c
LOCAL_SRC_FILES += test_pk11mode/pk11mode-c.c
LOCAL_SRC_FILES += test_pk11mode/pk11table.c
LOCAL_SRC_FILES += O_All/FacialComparison/GT_FacialComparison.c


ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_CFLAGS += -D__aarch64__ -DNSS_USE_64
endif


LOCAL_CFLAGS += $(LOCAL_C_INCLUDES) -DPACKED="__attribute__((packed))" -DPKCS11_USE_GOTRUST_TRUSTED_LOGIN -DPKCS11_USE_GOTRUST_INFO -DPKCS11_ENABLE_LOCK_PIN -DPKCS11_DB_DATA_KEY_USE_KEYSTORE -DPKCS11_INIT_PIN_NOT_SUPPORTED -DSELF_DEF_INITTOKEN -DBIOMETRIC_ANDROID_VERSION -DDB_CRYPTO_USE_ANDROID_KEY_STORE -DSM2_ENCRYPT_BLOB_SWAP_C2_AND_C3 -DNO_FORK_CHECK -DNO_SYSINFO -DLINUX -DSQLITE_HAS_CODEC -DANDROID -DHAVE_BSD_FLOCK -D__linux__ -D_NSPR_BUILD_ -DXP_UNIX -D_PR_PTHREADS -DHAVE_SOCKLEN_T -DHAVE_STRERROR -DNSPR_STATIC -DNDEBUG -DMP_API_COMPATIBLE -DNSS_DISABLE_DBM -DNSS_ENABLE_ECC -DNSS_STATIC -DNSS_USE_STATIC_LIBS -DRIJNDAEL_INCLUDE_TABLES -DSHLIB_VERSION=\"3\" -DSOFTOKEN_SHLIB_VERSION="3" -DUSE_UTIL_DIRECTLY -DNSS_DISABLE_ROOT_CERTS -DNSS_DISABLE_LIBPKIX -DSHLIB_SUFFIX=\"so\" -DSHLIB_PREFIX=\"lib\" -DSOFTOKEN_LIB_NAME=\"libsoftokn3.dylib\" -DNO_NSPR_10_SUPPORT

include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
#-------------------------libO_AllNSS-------------------------#


#-------------------------libgtvsep11-------------------------#
include $(CLEAR_VARS) 

LOCAL_C_INCLUDES := -I$(LOCAL_PATH)/pkcs11wrapper/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/pkcs11wrapper/native
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/pkcs11wrapper/native/common
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/pkcs11wrapper/native/unix/src


LOCAL_SRC_FILES := pkcs11wrapper/native/common/pkcs11wrapper.c

LOCAL_MODULE    := libgtvsep11-bio-$(TARGET_ARCH_ABI)
LOCAL_MODULE    := libgtvsep11-bio

LOCAL_STATIC_LIBRARIES := libO_AllNSS

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

LOCAL_CFLAGS += $(LOCAL_C_INCLUDES) -DPACKED="__attribute__((packed))" -DPKCS11_USE_GOTRUST_TRUSTED_LOGIN -DPKCS11_USE_GOTRUST_INFO -DPKCS11_ENABLE_LOCK_PIN -DPKCS11_DB_DATA_KEY_USE_KEYSTORE -DPKCS11_INIT_PIN_NOT_SUPPORTED -DSELF_DEF_INITTOKEN -DBIOMETRIC_ANDROID_VERSION -DDB_CRYPTO_USE_ANDROID_KEY_STORE -DUNIX 

include $(BUILD_SHARED_LIBRARY)






