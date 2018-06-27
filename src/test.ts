import Signer from '.'


console.info('------> Signer.list()')
console.info(Signer.list().map(c => c.subject))
console.info('------> Signer.find()')
console.info(Signer.list().map(c => Signer.find(c.id).subject))
console.info('------> JSCertificate.sign()')
Signer.list().forEach(c => {
  try {
    console.info(c.sign(Buffer.from('test')))
  } catch (e) {
    console.info(`Exception: ${e}`)
  }
})
