#pragma once

#include "nbind/Buffer.h"
#include "CertContext.h"
#include "Store.h"

class JSCertificate
{
protected:
  std::string id;
  std::string subject;
  std::string issuer;

public:
  JSCertificate(CertContext &certificate) { initialize(certificate); };
  std::string sign(nbind::Buffer data) { return Store("MY").find(id).sign(data.data(), data.length()); }
  std::string getId() { return id; }
  std::string getSubject() { return subject; }
  std::string getIssuer() { return issuer; }

protected:
  void initialize(CertContext &certificate);
};
