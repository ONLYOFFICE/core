//Example program for the CMD5 class wrapper of the RFC 1321 Message Digest 5 code.
//Jim Howard
//jnhtx@jump.net
//http://www.jump.net/~jnhtx

//Use this code as you see fit. It is provided "as is"
//without express or implied warranty of any kind.

#ifdef _DEBUG
#pragma warning( disable : 4786) //This warning always occurs with the ms compiler when using <vector>, it's nothing to worry about
#endif

#include <stdio.h>
#include "md5class.h"
#include <vector>
#include <string>

using namespace std;  //required to use STL with MS compiler

int main()
{
  //This program calculates message digests for the test strings given
  //in RFC 1321, "The MD5 Message-Digest Algorithm".  
   	
  vector<string> v;
  v.push_back(std::string(""));
  v.push_back(std::string("a"));
  v.push_back(std::string("abc"));
  v.push_back(std::string("message digest"));
  v.push_back(std::string("abcdefghijklmnopqrstuvwxyz"));
  v.push_back(std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
  v.push_back(std::string("12345678901234567890123456789012345678901234567890123456789012345678901234567890"));
	
  CMD5 md5;
  for(int i=0; i<v.size(); i++)
  {
	 md5.setPlainText( v[i].c_str() );
	printf("%s -> %s\n", v[i].c_str(),md5.getMD5Digest());
  }

  return 0; 
}