#include "nbind/api.h"

#include "Store.h"
#include "CertContext.h"
#include "Error.h"

Store::Store(const char *name)
{
  if (!(hCertStore = CertOpenStore(
            CERT_STORE_PROV_SYSTEM_A,
            (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING),
            NULL,
            CERT_SYSTEM_STORE_CURRENT_USER,
            name)))
  {
    JSError(TEXT("The system store did not open."));
  }
}

void Store::for_each(std::function<void(CertContext &cert)> callback)
{
  PCCERT_CONTEXT pCertContext = NULL;
  while (pCertContext = CertEnumCertificatesInStore(
             hCertStore,
             pCertContext))
  {
    CertContext certificate(pCertContext);
    callback(certificate);
    certificate.skipFree = true;
  }
}

CertContext Store::find(std::string &id)
{
  const char *strId = id.c_str();
  Buffer<BYTE *> rawId;
  if (!CryptStringToBinaryA(strId,
                            0,
                            CRYPT_STRING_HEXRAW,
                            NULL,
                            &rawId.size,
                            NULL,
                            NULL))
  {
    throw WinException("Store::find CryptStringToBinaryA 1");
  }
  rawId.allocate();
  if (!CryptStringToBinaryA(strId,
                            0,
                            CRYPT_STRING_HEXRAW,
                            rawId.memory,
                            &rawId.size,
                            NULL,
                            NULL))
  {
    throw WinException("Store::find CryptStringToBinaryA 2");
  }

  CRYPT_HASH_BLOB hashBlob;
  hashBlob.cbData = rawId.size;
  hashBlob.pbData = rawId.memory;

  PCCERT_CONTEXT pCertContext = NULL;
  if (!(pCertContext = CertFindCertificateInStore(
            hCertStore,
            (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING),
            0,
            CERT_FIND_SHA1_HASH,
            &hashBlob,
            NULL)))
  {
    throw WinException("Store::find CertFindCertificateInStore");
  }
  return CertContext(pCertContext);
}

Store::~Store()
{
  CertCloseStore(hCertStore, CERT_CLOSE_STORE_CHECK_FLAG);
}
