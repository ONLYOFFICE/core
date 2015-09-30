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
		/// Specifies the mapping mode in which the picture is drawn.
		short mm;
		/// Specifies the size of the metafile picture for all modes except the MM_ISOTROPIC and MM_ANISOTROPIC modes.
		/// (For more information about these modes, see the yExt member.)
		/// The x-extent specifies the width of the rectangle within which the picture is drawn.
		/// The coordinates are in units that correspond to the mapping mode.
		short xExt;
		/// Specifies the size of the metafile picture for all modes except the MM_ISOTROPIC and MM_ANISOTROPIC modes.
		/// The y-extent specifies the height of the rectangle within which the picture is drawn.
		/// The coordinates are in units that correspond to the mapping mode.
		/// For MM_ISOTROPIC and MM_ANISOTROPIC modes, which can be scaled, the xExt and yExt members 
		/// contain an optional suggested size in MM_HIMETRIC units.
		/// For MM_ANISOTROPIC pictures, xExt and yExt can be zero when no suggested size is supplied.
		/// For MM_ISOTROPIC pictures, an aspect ratio must be supplied even when no suggested size is given.
		/// (If a suggested size is given, the aspect ratio is implied by the size.)
		/// To give an aspect ratio without implying a suggested size, set xExt and yExt to negative values 
		/// whose ratio is the appropriate aspect ratio.
		/// The magnitude of the negative xExt and yExt values is ignored; only the ratio is used.
		short yExt;
		/// Handle to a memory metafile.
		short hMf;
	};

	class PictureDescriptor: public IVisitable
	{
		friend class DocumentMapping;
		friend class VMLPictureMapping;
		friend class VMLShapeMapping;
		friend class NumberingMapping;

	public:
		/// Parses the CHPX for a fcPic an loads the PictureDescriptor at this offset
		PictureDescriptor( CharacterPropertyExceptions* chpx, POLE::Stream* stream, int size );
		virtual ~PictureDescriptor();

	private: 

		void parse( POLE::Stream* stream, int fc, int sz );
		/// Returns the fcPic into the "data" stream, where the PIC begins.
		/// Returns -1 if the CHPX has no fcPic.
		static int GetFcPic( const CharacterPropertyExceptions* chpx );

	private:

		void Clear();

		static const short MM_SHAPE		=	0x0064;		//	Shape object
		static const short MM_SHAPEFILE =	0x0066;		//	Shape file

		/// Rectangle for window origin and extents when metafile is stored (ignored if 0).
		vector<unsigned char> rcWinMf;
		/// Horizontal measurement in twips of the rectangle the picture should be imaged within.
		short dxaGoal;
		/// Vertical measurement in twips of the rectangle the picture should be imaged within.
		short dyaGoal;
		/// Horizontal scaling factor supplied by user expressed in .001% units
		unsigned short mx;
		/// Vertical scaling factor supplied by user expressed in .001% units
		unsigned short my;
		/// The type of the picture
		PictureType Type;
		/// The name of the picture
		wstring Name;
		/// The data of the windows metafile picture (WMF)
		MetafilePicture mfp;
		/// The amount the picture has been cropped on the left in twips
		short dxaCropLeft;
		/// The amount the picture has been cropped on the top in twips
		short dyaCropTop;
		/// The amount the picture has been cropped on the right in twips
		short dxaCropRight;
		/// The amount the picture has been cropped on the bottom in twips
		short dyaCropBottom;
		/// Border above picture
		BorderCode *brcTop;
		/// Border to the left of the picture
		BorderCode *brcLeft;
		/// Border below picture
		BorderCode *brcBottom;
		/// Border to the right of the picture
		BorderCode *brcRight;
		/// Horizontal offset of hand annotation origin
		short dxaOrigin;
		/// vertical offset of hand annotation origin
		short dyaOrigin;
		/// unused
		short cProps;
		ShapeContainer* shapeContainer;
		BlipStoreEntry* blipStoreEntry;
	};
}
