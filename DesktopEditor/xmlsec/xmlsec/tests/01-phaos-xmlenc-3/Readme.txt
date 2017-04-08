This directory contains the following files:

 Readme.txt   -- This file.
 
 payment.xml  -- A sample XML file.

 key.txt      -- Keys for decrypting the encrypted XML files enc-*-*-*-*.xml.

 dh-priv-key.der
             -- Diffie-Hellman private key encoded in PKCS#8 PrivateKeyInfo formate.
		This key can be used to decrypt the files enc-*-*-ka-dh.xml.
 rsa-priv-key.der 
	     -- RSA private key encoded in PKCS#8 PrivateKeyInfo formate.
		This key can be used to decrypt the files enc-*-*-kt-*.xml.

 enc-*-*-*-*.xml
              -- XML document obtained by encrypting and replacing part of the
                 sample payment.xml. The data encryption key is in turn
                 encrypted.

                 Each file has the form: enc-A-B-C-D.xml, where:
                 
                 A indicates the type of the data encrypted. This includes:

                     element = The CreditCard element.
                     content = The content of the CreditCard element
                               (multiple elements).
                     text =    The content of the Number element (text node).

                 B indicates the data encryption algorithm.

                 C indicates the key encryption protocols:
                     kt = key transport
                     kw = key wrap
                     ka = key agreement

                 D indicates the key encryption algorithm.

bad-*-enc-*-*-*-*.xml
		-- some "screw-up" samples

The sample encrypted XML files are produced from Phaos XML toolkit.

Jiandong Guo
Phaos Technology
http://www.phaos.com

jguo@phaos.com




