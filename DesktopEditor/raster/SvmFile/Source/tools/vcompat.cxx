/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: vcompat.cxx,v $
 * $Revision: 1.5 $
 *
 * This file is part of OpenOffice.org.
 *
 * OpenOffice.org is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * OpenOffice.org is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with OpenOffice.org.  If not, see
 * <http://www.openoffice.org/license.html>
 * for a copy of the LGPLv3 License.
 *
 ************************************************************************/
#ifdef AVS
// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_tools.hxx"

#define _VCOMPAT_CXX
#include <tools/stream.hxx>
#include <tools/vcompat.hxx>
#endif
#include "../tools/vcompat.hxx"
#include "../ASC/ASCStreamReader.h"

using namespace SVMCore;
// -----------------
// - VersionCompat -
// -----------------

VersionCompat::VersionCompat( SvStream& rStm, USHORT nStreamMode, USHORT nVersion ) :
            mpRWStm     ( &rStm ),
            mnStmMode   ( nStreamMode ),
            mnVersion   ( nVersion )
{
#ifdef AVS
    if( !mpRWStm->GetError() )
    {
        if( STREAM_WRITE == mnStmMode )
        {
            *mpRWStm << mnVersion;
            mnTotalSize = ( mnCompatPos = mpRWStm->Tell() ) + 4UL;
            mpRWStm->SeekRel( 4L );
        }
        else
		{
#endif
            *mpRWStm >> mnVersion;
            *mpRWStm >> mnTotalSize;
            mnCompatPos = mpRWStm->Tell();
#ifdef AVS
        }
    }
#endif
}

// ------------------------------------------------------------------------

VersionCompat::~VersionCompat()
{
#ifdef AVS
    if( STREAM_WRITE == mnStmMode )
    {
        const UINT32 nEndPos = mpRWStm->Tell();

        mpRWStm->Seek( mnCompatPos );
        *mpRWStm << ( nEndPos - mnTotalSize );
        mpRWStm->Seek( nEndPos );
    }
    else
    {
#endif
        const UINT32 nReadSize = mpRWStm->Tell() - mnCompatPos;

        if( mnTotalSize > nReadSize )
            mpRWStm->SeekRel( mnTotalSize - nReadSize );
#ifdef AVS
    }
#endif
}
