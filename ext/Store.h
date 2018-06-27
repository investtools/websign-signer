#pragma once

#include <functional>
#include "CertContext.h"

class Store
{
public:
  Store(const char *name);
  void for_each(std::function<void(CertContext &cert)> callback);
  CertContext find(std::string &id);
  ~Store();

protected:
  HCERTSTORE hCertStore;
};
