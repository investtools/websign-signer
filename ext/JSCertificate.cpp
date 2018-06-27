#include "nbind/api.h"

#include "JSCertificate.h"
#include "Store.h"

void JSCertificate::initialize(CertContext &certificate)
{
  id = certificate.id();
  subject = certificate.subject();
  issuer = certificate.issuer();
}

#include "nbind/nbind.h"

NBIND_CLASS(JSCertificate)
{
  method(sign);
  getter(getId);
  getter(getSubject);
  getter(getIssuer);
}
