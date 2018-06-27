#include "nbind/api.h"

#include <wincrypt.h>

#include "CertContext.h"
#include "Error.h"
#include "Buffer.h"

std::string CertContext::id()
{
  Buffer<BYTE *> binaryId;
  Buffer<LPTSTR> stringId;
  if (!CertGetCertificateContextProperty(pCtx,
                                         CERT_SHA1_HASH_PROP_ID,
                                         NULL,
                                         &binaryId.size))
  {
    throw WinException("CertGetCertificateContextProperty 1");
  }
  binaryId.allocate();
  if (!CertGetCertificateContextProperty(pCtx,
                                         CERT_SHA1_HASH_PROP_ID,
                                         binaryId.memory,
                                         &binaryId.size))
  {
    throw WinException("CertGetCertificateContextProperty 2");
  }
  return binaryToString(&binaryId, CRYPT_STRING_HEXRAW | CRYPT_STRING_NOCRLF);
}

std::string CertContext::sign(unsigned char *data, size_t length)
{
  CRYPT_SIGN_MESSAGE_PARA SigParams;
  Buffer<BYTE *> signedMessageBlob;
  Buffer<LPTSTR> base64String;

  const BYTE *MessageArray[] = {data};
  DWORD MessageSizeArray[1];
  MessageSizeArray[0] = length;

  // Initialize the signature structure.
  SigParams.cbSize = sizeof(CRYPT_SIGN_MESSAGE_PARA);
  SigParams.dwMsgEncodingType = (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING);
  SigParams.pSigningCert = pCtx;
  SigParams.HashAlgorithm.pszObjId = szOID_RSA_SHA256RSA;
  SigParams.HashAlgorithm.Parameters.cbData = NULL;
  SigParams.cMsgCert = 1;
  SigParams.rgpMsgCert = &pCtx;
  SigParams.cAuthAttr = 0;
  SigParams.dwInnerContentType = 0;
  SigParams.cMsgCrl = 0;
  SigParams.cUnauthAttr = 0;
  SigParams.dwFlags = 0;
  SigParams.pvHashAuxInfo = NULL;
  SigParams.rgAuthAttr = NULL;

  // First, get the size of the signed BLOB.
  if (!CryptSignMessage(
          &SigParams,
          FALSE,
          1,
          MessageArray,
          MessageSizeArray,
          NULL,
          &signedMessageBlob.size))
  {
    throw WinException("CryptSignMessage 1");
  }

  // Allocate memory for the signed BLOB.
  signedMessageBlob.allocate();

  // Get the signed message BLOB.
  if (!CryptSignMessage(&SigParams,
                        FALSE,
                        1,
                        MessageArray,
                        MessageSizeArray,
                        signedMessageBlob.memory,
                        &signedMessageBlob.size))
  {
    throw WinException("CryptSignMessage 2");
  }
  return binaryToString(&signedMessageBlob, CRYPT_STRING_BASE64);
}

CertContext::~CertContext()
{
  if (!skipFree)
  {
    CertFreeCertificateContext(pCtx);
  }
}

std::string CertContext::binaryToString(Buffer<BYTE *> *bin, DWORD dwFlags)
{
  Buffer<LPTSTR> str;
  if (!CryptBinaryToStringA(bin->memory,
                            bin->size,
                            dwFlags,
                            NULL,
                            &str.size))
  {
    throw WinException("CryptBinaryToStringA 1");
  }
  str.allocate();
  if (!CryptBinaryToStringA(bin->memory,
                            bin->size,
                            dwFlags,
                            str.memory,
                            &str.size))
  {
    throw WinException("CryptBinaryToStringA 2");
  }
  return str.memory;
}
std::string CertContext::getString(DWORD dwFlags)
{
  Buffer<LPSTR> nameBuffer;
  // LPTSTR pszName;
  DWORD cbSize;
  if (!(nameBuffer.size = CertGetNameString(
            pCtx,
            CERT_NAME_SIMPLE_DISPLAY_TYPE,
            dwFlags,
            NULL,
            NULL,
            0)))
  {
    throw WinException("CertGetNameString 1");
  }
  nameBuffer.allocate();
  if (CertGetNameString(
          pCtx,
          CERT_NAME_SIMPLE_DISPLAY_TYPE,
          dwFlags,
          NULL,
          nameBuffer.memory,
          nameBuffer.size))

  {
    return nameBuffer.memory;
  }
  else
  {
    throw WinException("CertGetNameString 2");
  }
}
