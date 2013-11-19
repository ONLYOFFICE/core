#pragma once
#include "RtfDocument.h"
#include "RtfReader.h"
#include "OOXRelsReader.h"
#include "OOXReader.h"

class ReaderParameter
{
public: XML::IXMLDOMNodePtr oNode;
public: OOXReader* oReader;
public: RtfDocument* oDocument;
public: OOXRelsReaderPtr oRels;
public: RtfTableStylePtr poTableStyle;
};


#define COMMAND_OOX_BOOL( pattern, target, command, reader, index )\
else if( pattern == command )\
{\
	CString sParam = reader.ReadNodeAttribute( index, _T("w:val"), _T("") );\
	if( _T("false") == sParam || _T("0") == sParam || _T("off") == sParam )\
		target = 0;\
	else\
		target = 1;\
}
#define COMMAND_OOX_BOOL_ATTRIBUTE( pattern, target, reader, index )\
{\
	CString sParam = reader.ReadNodeAttribute( index, pattern, _T("") );\
	if( _T("") != sParam )\
	{\
		if( _T("false") == sParam || _T("0") == sParam || _T("off") == sParam )\
			target = 0;\
		else\
			target = 1;\
	}\
}
#define COMMAND_OOX_BOOL_ATTRIBUTE_2( pattern, target, reader )\
{\
	CString sParam = reader.ReadNodeAttribute( pattern, _T("") );\
	if( _T("") != sParam )\
	{\
		if( _T("false") == sParam || _T("0") == sParam || _T("off") == sParam || _T("f") == sParam )\
			target = 0;\
		else\
			target = 1;\
	}\
}

#define COMMAND_OOX_INT( pattern, target, command, reader, index )\
else if( pattern == command )\
{\
	CString sParam = reader.ReadNodeAttribute( index, _T("w:val"), _T("") );\
	if( _T("") != sParam )\
		target = Strings::ToInteger( sParam );\
}
#define COMMAND_OOX_INT_ATTRIBUTE( pattern, target, reader, index )\
{\
	CString sParam = reader.ReadNodeAttribute( index, pattern, _T("") );\
	if( _T("") != sParam )\
		target = Strings::ToInteger( sParam );\
}
#define COMMAND_OOX_INT_ATTRIBUTE_2( pattern, target, reader )\
{\
	CString sParam = reader.ReadNodeAttribute( pattern, _T("") );\
	if( _T("") != sParam )\
		target = Strings::ToInteger( sParam );\
}

#define COMMAND_READ_MARGIN( reader, index, eMetrics, nValue )\
{\
CString sVal =  reader.ReadNodeAttribute(index, _T("w:w"), _T("") );\
CString sType = reader.ReadNodeAttribute(index, _T("w:type"), _T("") ) ;\
if( _T("pct") == sType && _T("") != sVal )\
{\
	eMetrics = mu_Percent;\
	nValue = Strings::ToInteger( sVal );\
}\
else if( _T("dxa") == sType && _T("") != sVal )\
{\
	eMetrics = mu_Twips;\
	nValue = Strings::ToInteger( sVal );\
}\
}\

