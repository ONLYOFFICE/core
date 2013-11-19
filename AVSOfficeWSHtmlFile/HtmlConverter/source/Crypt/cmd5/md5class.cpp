// md5class.cpp: implementation of the CMD5 class.
//See internet RFC 1321, "The MD5 Message-Digest Algorithm"
//
//Use this code as you see fit. It is provided "as is"
//without express or implied warranty of any kind.

//////////////////////////////////////////////////////////////////////

#include "md5class.h"
#include "md5.h" //declarations from RFC 1321
#include <string.h>	  
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMD5::CMD5()
{
	m_digestValid = false; //we don't have a plaintext string yet
	m_digestString[32]=0;  //the digest string is always 32 characters, so put a null in position 32
}

CMD5::~CMD5()
{

}

CMD5::CMD5(const char* plainText)  
{	
	m_plainText =  const_cast<char*>(plainText); //get a pointer to the plain text.  If casting away the const-ness worries you,
												 //you could make a local copy of the plain text string.
   	m_digestString[32]=0;
	m_digestValid = calcDigest();
}

//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////

void CMD5::setPlainText(const char* plainText)
{
  //set plaintext with a mutator, it's ok to 
  //to call this multiple times.  If casting away the const-ness of plainText 
  //worries you, you could either make a local copy of the plain 
  //text string instead of just pointing at the user's string, or 
  //modify the RFC 1321 code to take 'const' plaintext, see example below. 

  m_plainText = const_cast<char*>(plainText);
  m_digestValid = calcDigest();
}

/* Use a function of this type with your favorite string class
   if casting away the const-ness of the user's text buffer violates you
   coding standards. 

void CMD5::setPlainText(CString& strPlainText)
{
	static CString plaintext(strPlainText);
	m_plainText = strPlainText.GetBuffer();
	m_digestValid = calcDigest();
}
*/

const char* CMD5::getMD5Digest()
 {	//access message digest (aka hash), return 0 if plaintext has not been set
	if(m_digestValid)
	{
		return m_digestString;
	} else return 0;
 }




bool CMD5::calcDigest()
{
  //See RFC 1321 for details on how MD5Init, MD5Update, and MD5Final 
  //calculate a digest for the plain text
  MD5_CTX context;
  MD5Init(&context); 

  //the alternative to these ugly casts is to go into the RFC code and change the declarations
  MD5Update(&context, reinterpret_cast<unsigned char *>(m_plainText), ::strlen(m_plainText));
  MD5Final(reinterpret_cast <unsigned char *>(m_digest),&context);
  
  //make a string version of the numeric digest value
  int p=0;
  for (int i = 0; i<16; i++)
  {
	::sprintf(&m_digestString[p],"%02x", m_digest[i]);
	p+=2;
  }
  return true;
}


