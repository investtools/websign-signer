#pragma once

#include <wincrypt.h>
#include <windows.h>

#include "Buffer.h"

class CertContext
{
protected:
  PCCERT_CONTEXT pCtx;

public:
  bool skipFree = false;
  CertContext(PCCERT_CONTEXT pCtx) : pCtx(pCtx) {}
  std::string subject() { return getString(0); }
  std::string issuer() { return getString(CERT_NAME_ISSUER_FLAG); }
  std::string id();
  std::string sign(unsigned char *data, size_t length);
  ~CertContext();

protected:
  std::string binaryToString(Buffer<BYTE *> *bin, DWORD dwFlags);
  std::string getString(DWORD dwFlags);
};
