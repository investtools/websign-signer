#pragma comment(lib, "crypt32.lib")

#include "nbind/api.h"

#include "Store.h"
#include "JSCertificate.h"

class Signer
{

public:
  static std::vector<JSCertificate> list()
  {
    std::vector<JSCertificate> result;
    Store("MY").for_each([&](CertContext &cert) {
      result.push_back(JSCertificate(cert));
    });
    return result;
  }

  static JSCertificate find(std::string id)
  {
    CertContext cert = Store("MY").find(id);
    return JSCertificate(cert);
  }
};

#include "nbind/nbind.h"

NBIND_CLASS(Signer)
{
  method(list);
  method(find);
}
