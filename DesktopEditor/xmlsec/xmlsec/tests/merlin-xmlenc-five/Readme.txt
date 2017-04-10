Example Signatures[1] using Encryption[2] key information and
Additional Security URIs[3], Encrypted Data[2] and Decryption
Transform[4]

[1] http://www.w3.org/TR/2002/REC-xmldsig-core-20020212/
[2] http://www.w3.org/Encryption/2001/Drafts/xmlenc-core/
[3] http://www.ietf.org/internet-drafts/draft-eastlake-xmldsig-uri-02.txt
[4] http://www.w3.org/TR/xmlenc-decrypt

***
Some of these signature are WITHOUT cryptographic merit;
for example, key transport of an HMAC key is meaningless.
These are provided solely for testing purposes.
***

Private keys necessary for performing the verification
and decryption are provided in the PKCS#12 file ids.p12,
encrypted under the pass phrase "Our Little Secret". I
may have done some of the ASN.1 encoding in this P12
incorrectly; I hope not. Private keys are also available
in PKCS#8 encoding; rsa.p8, dh1.p8.

RSA/OAEP is presently poorly defined; I assume MGF1/SHA-1.

Secret keys are identified by key name as follows:

 Key Name | Algorithm | Key Value
----------+-----------+-----------
   bob    |   3des    | "abcdefghijklmnopqrstuvwx".getBytes ("ASCII")
   job    |  aes-128  | "abcdefghijklmnop".getBytes ("ASCII")
   jeb    |  aes-192  | "abcdefghijklmnopqrstuvwx".getBytes ("ASCII")
   jed    |  aes-256  | "abcdefghijklmnopqrstuvwxyz012345".getBytes ("ASCII")

. encrypt-content-aes128-cbc-kw-aes192.xml
. encrypt-content-aes192-cbc-dh-sha512.xml
. encrypt-content-tripledes-cbc.xml
. encrypt-data-aes128-cbc.xml
. encrypt-data-aes192-cbc-kw-aes256.xml
. encrypt-data-aes256-cbc-kw-tripledes.xml
. encrypt-data-tripledes-cbc-rsa-oaep-mgf1p-sha256.xml
. encrypt-data-tripledes-cbc-rsa-oaep-mgf1p.xml
. encrypt-element-aes128-cbc-rsa-1_5.xml
. encrypt-element-aes192-cbc-ref.xml
. encrypt-element-aes256-cbc-kw-aes256-dh-ripemd160.xml
. encrypt-element-tripledes-cbc-kw-aes128.xml
  Encrypted content, element and data. RSA private key has
  friendly name "RSA" in the P12, and is rsa.p8.
  DH private key has ID "DH1", and is dh1.p8.

. encrypt-content-aes256-cbc-prop.xml
  Contains a useless EncryptionProperty.

. encrypt-element-aes256-cbc-carried-kw-aes256.xml
  External EncryptedKey contains a CarriedKeyName which
  is referenced by a KeyName in the EncryptedData; the
  key for Recipient "someone else" is not for you; the
  key for Recipient "you" is.

. encrypt-element-aes256-cbc-retrieved-kw-aes256.xml
  External EncryptedKey is identified by a RetrievalMethod
  in the EncryptedData.

. decryption-transform.xml
  Decryption transform.

. decryption-transform-except.xml
  Decryption transform with Except.

All signatures are performed with nontruncated HMAC
algorithms.

For debugging purposes, where chosen, the MAC key is
"abcdefghijklmnopqrstuvwxyz012345".getBytes ("ASCII"). Where
agreed, it has length equal to the HMAC output length; e.g., 256
bits for HMAC/SHA-256.

. encsig-ripemd160-hmac-ripemd160-kw-tripledes.xml
  RIPEMD-160 message digest; HMAC/RIPEMD-160 key is wrapped
  using triple DES. The decryption key is from the above table,
  identified by the key name "bob".

. encsig-sha256-hmac-sha256-kw-aes128.xml
  SHA-256 message digest; HMAC/SHA-256 key is wrapped using
  AES-128. The decryption key is from the above table, identified
  by the key name "job".

. encsig-sha384-hmac-sha384-kw-aes192.xml
  SHA-384 message digest; HMAC/SHA-384 key is wrapped using
  AES-192. The decryption key is from the above table, identified
  by the key name "jeb".

. encsig-sha512-hmac-sha512-kw-aes256.xml
  SHA-512 message digest; HMAC/SHA-512 key is wrapped using
  AES-256. The decryption key is from the above table, identified
  by the key name "jed".

. encsig-hmac-sha256-rsa-1_5.xml
  HMAC/SHA-256 keys is transported using RSA/OAEP. Your private
  key has friendly name "RSA" in the P12.

. encsig-hmac-sha256-rsa-oaep-mgf1p.xml
  HMAC/SHA-256 keys is transported using RSA/PKCS#1. Your private
  key has friendly name "RSA" in the P12.

. encsig-hmac-sha256-dh.xml
  HMAC/SHA-256 key is agreed using Diffie Hellman. Your private
  key has friendly name "DH1" in the P12.

. encsig-hmac-sha256-kw-tripledes-dh.xml
  HMAC/SHA-256 key is wrapped using triple DES. The decryption
  key is agreed using Diffie Hellman. Your private key has
  friendly name "DH1" in the P12.

Merlin Hughes <merlin@baltimore.ie>
Baltimore Technologies, Ltd.

Monday, March 4, 2002
