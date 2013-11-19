/**
 *	PROJECT - HTML Reader Class Library
 *
 *	LiteHTMLAttributes.cpp
 *	
 *	Written By Gurmeet S. Kochar <gomzygotit@hotmail.com>
 *	Copyright (c) 2004. All rights reserved.
 *
 *	This code may be used in compiled form in any way you desire 
 *	(including commercial use). The code may be redistributed 
 *	unmodified by any means PROVIDING it is not sold for profit 
 *	without the authors written consent, and providing that this 
 *	notice and the authors name and all copyright notices remains 
 *	intact. However, this file and the accompanying source code may 
 *	not be hosted on a website or bulletin board without the authors 
 *	written permission.
 *	
 *	This file is provided "AS IS" with no expressed or implied warranty.
 *	The author accepts no liability for any damage/loss of business that
 *	this product may cause.
 *
 *	Although it is not necessary, but if you use this code in any of 
 *	your application (commercial or non-commercial), please INFORM me 
 *	so that I may know how useful this library is. This will encourage 
 *	me to keep updating it.
 */
#include "stdafx.h"
#include "LiteHTMLAttributes.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif	// _DEBUG

#pragma warning(push, 4)

const COLORREF CLiteHTMLElemAttr::_clrInvalid = (COLORREF)0xFFFFFFFF;
const unsigned short CLiteHTMLElemAttr::_percentMax = USHRT_MAX;

// the reason behind setting the block size of our collection 
// to 166 is that we have a total of 166 known named colors
CLiteHTMLElemAttr::CNamedColors CLiteHTMLElemAttr::_namedColors(166 /* block size */);

#pragma warning(pop)
