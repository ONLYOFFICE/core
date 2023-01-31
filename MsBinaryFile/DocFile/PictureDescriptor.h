/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "BorderCode.h"
#include "VirtualStreamReader.h"
#include "OfficeDrawing/ShapeContainer.h"
#include "CharacterPropertyExceptions.h"
#include "IVisitable.h"
#include "OfficeDrawing/BlipStoreEntry.h"

namespace DocFileFormat
{
	typedef enum _PictureType
	{
		jpg,
		png,
		wmf
	} PictureType;

	struct MetafilePicture
	{
		// Specifies the mapping mode in which the picture is drawn.
		short mm;
		// Specifies the size of the metafile picture for all modes except the MM_ISOTROPIC and MM_ANISOTROPIC modes.
		// (For more information about these modes, see the yExt member.)
		// The x-extent specifies the width of the rectangle within which the picture is drawn.
		// The coordinates are in units that correspond to the mapping mode.
		short xExt;
		// Specifies the size of the metafile picture for all modes except the MM_ISOTROPIC and MM_ANISOTROPIC modes.
		// The y-extent specifies the height of the rectangle within which the picture is drawn.
		// The coordinates are in units that correspond to the mapping mode.
		// For MM_ISOTROPIC and MM_ANISOTROPIC modes, which can be scaled, the xExt and yExt members 
		// contain an optional suggested size in MM_HIMETRIC units.
		// For MM_ANISOTROPIC pictures, xExt and yExt can be zero when no suggested size is supplied.
		// For MM_ISOTROPIC pictures, an aspect ratio must be supplied even when no suggested size is given.
		// (If a suggested size is given, the aspect ratio is implied by the size.)
		// To give an aspect ratio without implying a suggested size, set xExt and yExt to negative values 
		// whose ratio is the appropriate aspect ratio.
		// The magnitude of the negative xExt and yExt values is ignored; only the ratio is used.
		short yExt;
		// Handle to a memory metafile.
		short hMf;
	};

	class PictureDescriptor: public IVisitable
	{
		friend class DocumentMapping;
		friend class VMLPictureMapping;
		friend class VMLShapeMapping;
		friend class NumberingMapping;
		friend class OleObject;

	public:
		// Parses the CHPX for a fcPic an loads the PictureDescriptor at this offset
		PictureDescriptor ( );
		PictureDescriptor ( CharacterPropertyExceptions* chpx, POLE::Stream* stream, int size, int nWordVersion);
		virtual ~PictureDescriptor();
		
		void parse( POLE::Stream* stream, int fc, int sz, int nWordVersion);

		
		// Returns the fcPic into the "data" stream, where the PIC begins.
		// Returns -1 if the CHPX has no fcPic.
		static int GetFcPic( const CharacterPropertyExceptions* chpx );
	private: 

		void Clear();

		static const short MM_SHAPE		=	0x0064;		//	Shape object
		static const short MM_SHAPEFILE =	0x0066;		//	Shape file

		std::vector<unsigned char> rcWinMf;		// Rectangle for window origin and extents when metafile is stored (ignored if 0).

		short dxaGoal;			// Horizontal measurement in twips of the rectangle the picture should be imaged within.
		short dyaGoal;			// Vertical measurement in twips of the rectangle the picture should be imaged within.

		unsigned short mx;		// Horizontal scaling factor supplied by user expressed in .001% units
		unsigned short my;		// Vertical scaling factor supplied by user expressed in .001% units

		PictureType Type;		// The type of the picture
		
		MetafilePicture mfp;		

		short dxaCropLeft;			// The amount the picture has been cropped on the left in twips
		short dyaCropTop;			// The amount the picture has been cropped on the top in twips
		short dxaCropRight;			// The amount the picture has been cropped on the right in twips
		short dyaCropBottom;		// The amount the picture has been cropped on the bottom in twips

		BorderCode *brcTop;			// Border above picture
		BorderCode *brcLeft;		// Border to the left of the picture
		BorderCode *brcBottom;		// Border below picture
		BorderCode *brcRight;		// Border to the right of the picture

		short dxaOrigin;			// horizontal offset of hand annotation origin
		short dyaOrigin;			// vertical offset of hand annotation origin

		short cProps;
//------------------
		ShapeContainer	* shapeContainer;
		BlipStoreEntry	* blipStoreEntry;
		
		unsigned char	*embeddedDataHeader;
		unsigned char	*embeddedData;
		int				embeddedDataSize;						
	};
}


namespace ImageHelper
{
    struct __BITMAPINFOHEADER
    {
        _UINT32     biSize;
        _INT32      biWidth;
        _INT32      biHeight;
        _UINT16     biPlanes;
        _UINT16     biBitCount;
        _UINT32     biCompression;
        _UINT32     biSizeImage;
        _INT32      biXPelsPerMeter;
        _INT32      biYPelsPerMeter;
        _UINT32     biClrUsed;
        _UINT32     biClrImportant;
    };

    struct __BITMAPCOREHEADER
    {
        _UINT32     bcSize;                 /* used to get to color table */
        _UINT16     bcWidth;
        _UINT16     bcHeight;
        _UINT16     bcPlanes;
        _UINT16     bcBitCount;
    };

	Global::_BlipType SaveImageToFileFromDIB(unsigned char* data, int size, std::wstring& file_name);
}

