/**
 *	PROJECT - HTML Reader Class Library
 *
 *	LiteHTMLCommon.h - Common Function/Macros/etc.
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
#ifndef __LITEHTMLCOMMON_H__
#define __LITEHTMLCOMMON_H__

#pragma warning(push, 4)

/***************************************************************
 * UTILITY MACROS
 *
 * @author Gurmeet S. Kochar
 ***************************************************************/

/**
 * SAFE_POINTER_DELETE
 * Deletes the specified pointer, if non-null, and sets 
 * it to NULL after deletion.
 *
 * @param p pointer to delete
 *
 * @since Mar 08, 2004
 * @author Gurmeet S. Kochar
 */
#define SAFE_DELETE_POINTER(_P) \
(void)(_P != NULL ? delete _P, _P = NULL : 0) \

#pragma warning(pop)

#endif	// !__LITEHTMLCOMMON_H__
