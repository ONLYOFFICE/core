Sample XML Signatures[1][2]

[1] http://www.w3.org/TR/2002/REC-xmldsig-core-20020212/
[2] http://www.w3.org/TR/2001/REC-xml-c14n-20010315

1. A large and complex signature:

This includes internal and external base 64, references of the forms
"", "#xpointer(/)", "#foo" and "#xpointer(id('foo'))" (with and
without comments), manifests, signature properties, simple xpath
with here(), xslt, retrieval method and odd interreferential
dependencies.

  signature.xml - A signature
  signature.tmpl - The template from which the signature was created
  signature-c14n-*.txt - All intermediate c14n output

2. Some basic signatures:

The key for the HMAC-SHA1 signatures is "secret".getBytes("ASCII")
which is, in hex, (73 65 63 72 65 74). No key info is provided for
these signatures.

  signature-enveloped-dsa.xml
  signature-enveloping-b64-dsa.xml
  signature-enveloping-dsa.xml
  signature-enveloping-hmac-sha1-40.xml
  signature-enveloping-hmac-sha1.xml
  signature-enveloping-rsa.xml
  signature-external-b64-dsa.xml
  signature-external-dsa.xml - The signatures
  signature-*-c14n-*.txt - The intermediate c14n output

3. Varying key information:

To resolve the key associated with the KeyName in `signature-keyname.xml'
you must perform a cunning transformation from the name `Xxx' to the
certificate that resides in the directory `certs/' that has a subject name
containing the common name `Xxx', which happens to be in the file
`certs/xxx.crt'.

To resolve the key associated with the X509Data in `signature-x509-is.xml',
`signature-x509-ski.xml' and `signature-x509-sn.xml' you need to resolve
the identified certificate from those in the `certs' directory.

In `signature-x509-crt-crl.xml' an X.509 CRL is present which has revoked
the X.509 certificate used for signing. So verification should be
qualified.

  signature-keyname.xml
  signature-retrievalmethod-rawx509crt.xml
  signature-x509-crt-crl.xml
  signature-x509-crt.xml
  signature-x509-is.xml
  signature-x509-ski.xml
  signature-x509-sn.xml - The signatures
  certs/*.crt - The certificates

Merlin Hughes <merlin@baltimore.ie>
Baltimore Technologies, Ltd.
http://www.baltimore.com/

Thursday, April 4, 2002
