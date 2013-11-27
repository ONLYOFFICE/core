// ASCOfficeHtmlFileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ObjBase.h>
#include "Test.h"


int wmain(int argc, wchar_t* argv[])
{	  
	Test::Docx2Html();
//	Test::Html2Docx();
	
	return 0;
}