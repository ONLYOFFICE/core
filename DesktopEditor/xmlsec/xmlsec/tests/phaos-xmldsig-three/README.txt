Sample XML Signatures Produced Using the Phaos XML Toolkit

November 26, 2002


Contents of phaos-xmldsig-three
===============================

	signature-rsa-enveloped.xml
	---------------------------	
	Contains an RSA enveloped signature.


	signature-rsa-enveloping.xml
	----------------------------
	Contains an RSA enveloping signature.


	signature-rsa-detached.xml
	--------------------------
	Contains an RSA detached signature.


	signature-dsa-enveloped.xml
	---------------------------	
	Contains a DSA enveloped signature.


	signature-dsa-enveloping.xml
	----------------------------
	Contains a DSA enveloping signature.


	signature-dsa-detached.xml
	--------------------------
	Contains a DSA detached signature.


	signature-hmac-md5-c14n-enveloping.xml
	--------------------------------------
	Contains an enveloping MD5 HMAC signature and uses XML Canonicalization 
	as the canonicalization method.  The HMAC secret is the ASCII encoding of
	the word "test".


	signature-hmac-sha1-exclusive-c14n-enveloped.xml
	------------------------------------------------
	Contains an enveloped SHA-1 HMAC signature and uses the Exclusive XML
	Canonicalization canonicalization method.  The HMAC secret is the ASCII 
	encoding of the word "test".


	signature-hmac-sha1-exclusive-c14n-comments-detached.xml
	--------------------------------------------------------
	Contains a detached SHA-1 HMAC signature and uses the Exclusive XML
	Canonicalization With Comments canonicalization method.  The HMAC secret 
	is the ASCII encoding of the word "test".


	signature-hmac-sha1-40-c14n-comments-detached.xml
	-------------------------------------------------
	Contains a detached 40-byte SHA-1 HMAC signature and uses the XML
	Canonicalization With Comments canonicalization method.  The HMAC secret is 
	the ASCII encoding of the word "test".
		

	signature-hmac-sha1-40-exclusive-c14n-comments-detached.xml
	-----------------------------------------------------------
	Contains a detached 40 byte SHA-1 HMAC signature and uses the Exclusive
	XML Canonicalization With Comments canonicalization method.  The HMAC secret 
	is the ASCII encoding of the word "test".


	signature-dsa-detached-manifest.xml
	-----------------------------------
	Contains a detached DSA signature with a manifest.


	signature-rsa-detached-manifest.xml
	----------------------------------
	Contains a detached RSA signature with a manifest.

	
	signature-rsa-detached-b64-transform.xml
	----------------------------------------
	Contains a detached RSA signature with a Base64 decode transform.

	
	signature-rsa-detached-xpath-transform.xml
	------------------------------------------
	Contains a detached RSA signature with an XPath transform.

	
	signature-rsa-xpath-transform-enveloped.xml
	------------------------------------------
	Contains an RSA signature with an XPath transform that produces the
	same result as the enveloped signature algorithm.

	
	signature-rsa-detached-xslt-transform.xml
	------------------------------------------
	Contains a detached RSA signature with an XSLT transformation.


	signature-rsa-detached-x509-data.xml
	------------------------------------
	Contains a detached RSA signature with several X509Data subelements.


	signature-rsa-detached-x509-data-subject-name.xml
	------------------------------------
	Contains a detached RSA signature with an X509SubjectName that 
	references the subject name of the certificate stored in
	certs/rsa-client-cert.der.


	signature-rsa-detached-x509-data-issuer-serial.xml
	------------------------------------
	Contains a detached RSA signature with an X509IssuerSerial that 
	references the issuer and serial number of the certificate stored in
	certs/rsa-client-cert.der.


	signature-rsa-detached-x509-data-ski.xml
	------------------------------------
	Contains a detached RSA signature with an X509SKI that 
	references the Subject Key Identifier of the certificate stored in
	certs/rsa-client-cert.der.


	signature-rsa-detached-x509-data-client-cert.xml
	------------------------------------
	Contains a detached RSA signature with an X509Certificate that 
	represents the certificate stored in certs/rsa-client-cert.der.


	signature-rsa-detached-x509-data-cert-chain.xml
	------------------------------------
	Contains a detached RSA signature with two X509Certificate 
	elements that represent the certificates stored in  
	certs/rsa-client-cert.der and certs/rsa-ca-cert.der.


	signature-rsa-detached-xslt-transform-retrieval-method.xml
	------------------------------------
	Contains a detached RSA signature with an XSLT transform and a KeyInfo 
	element that refers to an external X.509 certificate.  The certificate 
	is located in certs/rsa-client-cert.der.


	signature-big.xml
	-----------------
	Contains a larger detached RSA signature that contains a manifest and many 
	references that test various transformation algorithms, URI reference syntax 
	formats, etc. The KeyInfo contains a KeyName whose value is the subject
	name of the certificate stored in certs/rsa-client-cert.der.

	
	signature-rsa-detached-xslt-transform-bad-retrieval-method.xml
	---------------------------------------------------------------
	Contains a detached RSA signature with an XSLT transform and a KeyInfo 
	element that refers to an INCORRECT external X.509 certificate. (The correct
	X.509 certificate is located in certs/rsa-client-cert.der.)  Verification 
	should FAIL.


	signature-rsa-enveloped-bad-digest-val.xml
	------------------------------------------
	Contains an enveloped RSA signature that contains a reference with an INCORRECT
	digest value.  Verification should FAIL.


	signature-rsa-enveloped-bad-sig.xml
	-----------------------------------
	Contains an enveloped RSA signature that contains a reference that was added 
	after the signature value was computed.  Verification should FAIL.

	
	document.xml
	------------
	An XML document that is referenced by some of the signature examples.


	document-stylesheet.xml
	-----------------------
	The same as document.xml with the addition of an xsl-stylesheet processing
	instruction.

	
	document.xsl
	------------
	An XSL stylesheet referenced by several of the signature examples and 
	document-stylesheet.xml.


	document.b64
	------------
	A Base64 encoded XML document.


	README.txt
	----------
	This file.



Contents of phaos-xmldsig-three/certs
=====================================

	rsa-cert.der
	------------
	An RSA end entity certificate issued by the CA certificate contained in
	rsa-ca-cert.der.

	enc-rsa-key.der
	---------------
	The RSA private key that matches rsa-cert.der, password-encrypted using
	the PKCS#8 format. The password is "test".

	dsa-cert.der
	------------
	An DSA end entity certificate issued by the CA certificate contained in
	dsa-ca-cert.der.

	enc-dsa-key.der
	---------------
	The DSA private key that matches dsa-cert.der, password-encrypted using
	the PKCS#8 format. The password is "test".

	rsa-ca-cert.der
	---------------
	An RSA self-signed CA certificate.

	dsa-ca-cert.der
	---------------
	A DSA self-signed CA certificate.

	crl.der
	-------
	A sample CRL that revokes rsa-cert.der.



Joe Morgan
Software Engineer
Phaos Technology
jmorgan@phaos.com	
http://www.phaos.com/
