#pragma once

#include <string>

#include "IRunItem.h"
#include "PICFAndOfficeArtData.h"
#include "BinaryStorage.h"
#include "OfficeArt/BlipFactory.h"
#include "OfficeArt/OfficeArtFOPT.h"
#include <boost/scoped_ptr.hpp>
#include "OfficeArt/OfficeArtClientAnchor.h"

#define RATIO	1000	// TODO : пересчет

namespace AVSDocFileFormat
{
	class Picture : public IRunItem, public IOperand
	{
	public:

		Picture () : m_oBinPictureInfo(), m_sTextType (std::wstring (&TextMark::Picture)), m_bOK(FALSE)
		{

		}

		Picture (std::wstring strFileName, 
			short iWidth,
			short iHeight,
			unsigned short iRatioX	=	RATIO, 
			unsigned short iRatioY	=	RATIO,
			Brc80 brcTop80			=	Brc80(2, 0x00, 0x00, 0, false, false),
			Brc80 brcLeft80			=	Brc80(2, 0x00, 0x00, 0, false, false),
			Brc80 brcBottom80		=	Brc80(2, 0x00, 0x00, 0, false, false),
			Brc80 brcRight80		=	Brc80(2, 0x00, 0x00, 0, false, false)) : m_oBinPictureInfo(), m_sTextType (std::wstring (&TextMark::Picture)), m_bOK(FALSE)
		{
			/*
			// ONLY FOR TEST

			if (0 == strFileName.length())
			{
			AVSDocFormatUtils::BitSet oShapeSettings (4);

			oShapeSettings.SetBit (0,		0);		//	Group		-	A bit that specifies whether this shape is a group shape. 
			oShapeSettings.SetBit (0,		1);		//	Child		-	A bit that specifies whether this shape is a child shape.
			oShapeSettings.SetBit (0,		2);		//	Patriarch	-	A bit that specifies whether this shape is the topmost group shape. Each drawing contains one topmost group shape
			oShapeSettings.SetBit (0,		3);		//	Deleted		-	A bit that specifies whether this shape has been deleted. 
			oShapeSettings.SetBit (0,		4);		//	OleShape	-	A bit that specifies whether this shape is an OLE object.
			oShapeSettings.SetBit (0,		5);		//	HaveMaster	-	A bit that specifies whether this shape has a valid master in the hspMaster property, as defined in section
			oShapeSettings.SetBit (0,		6);		//	FlipH		-	A bit that specifies whether this shape is horizontally flipped. 
			oShapeSettings.SetBit (0,		7);		//	FlipV		-	A bit that specifies whether this shape is vertically flipped. 
			oShapeSettings.SetBit (0,		8);		//	Connector	-	A bit that specifies whether this shape is a connector shape.
			oShapeSettings.SetBit (true,	9);		//	HaveAnchor	-	A bit that specifies whether this shape has an anchor. 
			oShapeSettings.SetBit (0,		10);	//	Background	-	A bit that specifies whether this shape is a background shape. 
			oShapeSettings.SetBit (true,	11);	//	HaveSpt		-	A bit that specifies whether this shape has a shape type property.

			OfficeArt::OfficeArtFSP ShapeProp (OfficeArt::Enumerations::msosptUpArrow, 1024, FormatUtils::BytesToUInt32 (oShapeSettings.GetBytes(), 0, sizeof(unsigned int)));

			OfficeArt::OfficeArtSpContainer shape;
			shape.PushBack (ShapeProp);

			OfficeArt::OfficeArtRGFOPTE oTable;

			OfficeArt::OfficeArtFOPTE oEntry (OfficeArt::OfficeArtFOPTEOPID (OfficeArt::Enumerations::protectionBooleans, false, false), 0x01400140);
			oTable.PushComplexProperty (OfficeArt::ComplexOfficeArtProperty (oEntry, NULL));

			OfficeArt::OfficeArtFOPTE oCropFromTop (OfficeArt::OfficeArtFOPTEOPID (OfficeArt::Enumerations::cropFromTop, false, false), 0xffff0010);
			oTable.PushComplexProperty (OfficeArt::ComplexOfficeArtProperty (oCropFromTop, NULL));

			OfficeArt::OfficeArtFOPTE oCropFromBottom (OfficeArt::OfficeArtFOPTEOPID (OfficeArt::Enumerations::cropFromBottom, false, false), 0x0000fff0);
			oTable.PushComplexProperty (OfficeArt::ComplexOfficeArtProperty (oCropFromBottom, NULL));

			OfficeArt::OfficeArtRGFOPTE oTable2;
			OfficeArt::OfficeArtFOPTE oDiagramBooleans (OfficeArt::OfficeArtFOPTEOPID (OfficeArt::Enumerations::diagramBooleans, false, false), 0x00010001);
			oTable2.PushComplexProperty (OfficeArt::ComplexOfficeArtProperty (oDiagramBooleans, NULL));

			OfficeArt::OfficeArtFOPT fopt (oTable);
			shape.PushBack (fopt);

			OfficeArt::OfficeArtFOPT fopt2 (oTable2);
			shape.PushBack (fopt2);

			OfficeArt::OfficeArtClientAnchor anchor (0x80000000);
			shape.PushBack (anchor);

			OfficeArt::OfficeArtInlineSpContainer oPicture (shape);
			OfficeArt::OfficeArtFBSE oBlipStoreEntry (FALSE, OfficeArt::Enumerations::msoblipJPEG, OfficeArt::Enumerations::msoblipJPEG, 0);
			oPicture.PushBack(oBlipStoreEntry);

			PICMID oBorders (iWidth, iHeight, iRatioX, iRatioY, brcTop80, brcLeft80, brcBottom80, brcRight80);
			//PICMID oBorders (2925, 3045, iRatioX, iRatioY, brcTop80, brcLeft80, brcBottom80, brcRight80);
			PICF oPictureInfo (oPicture.Size(), oBorders);

			m_oBinPictureInfo			=	PICFAndOfficeArtData (oPictureInfo, oPicture);

			if (BinaryStorageSingleton::Instance())
			{
			int dataStreamOffset	=	BinaryStorageSingleton::Instance()->PushData( (const byte*)m_oBinPictureInfo, m_oBinPictureInfo.Size());

			m_arProperties.push_back (Prl((short)DocFileFormat::sprmCPicLocation,	(byte*)(&dataStreamOffset)));
			m_arProperties.push_back (Prl((short)DocFileFormat::sprmCFSpec,			(byte*)(&CFSpec)));
			}

			return;
			}
			*/

			OfficeArt::BlipFactory oBlipFactory (strFileName);
			boost::scoped_ptr<OfficeArt::OfficeArtBlip> oImage(oBlipFactory.GetOfficeArtBlip());

			if (oImage.get())
			{
				OfficeArt::OfficeArtSpContainer shape;
				OfficeArt::OfficeArtFSP shapeProp (OfficeArt::Enumerations::msosptPictureFrame);
				shape.PushBack (shapeProp);

				OfficeArt::OfficeArtRGFOPTE oTable;
				OfficeArt::OfficeArtFOPTE oEntry (OfficeArt::OfficeArtFOPTEOPID (OfficeArt::Enumerations::pib, true, false), 1);
				oTable.PushComplexProperty (OfficeArt::ComplexOfficeArtProperty (oEntry, NULL));

				OfficeArt::OfficeArtFOPT fopt (oTable);
				shape.PushBack (fopt);

				OfficeArt::OfficeArtInlineSpContainer oPicture (shape);
				OfficeArt::OfficeArtFBSE oBlipStoreEntry (TRUE, oBlipFactory.GetBlipType(), oBlipFactory.GetBlipType(), oImage->Size() );
				oPicture.PushBack(oBlipStoreEntry);
				oPicture.PushBack(*oImage);

				PICMID oBorders (iWidth, iHeight, iRatioX, iRatioY, brcTop80, brcLeft80, brcBottom80, brcRight80);
				PICF oPictureInfo (oPicture.Size(), oBorders);

				m_oBinPictureInfo			=	PICFAndOfficeArtData (oPictureInfo, oPicture);

				if (BinaryStorageSingleton::Instance())
				{
					int dataStreamOffset	=	BinaryStorageSingleton::Instance()->PushData( (const byte*)m_oBinPictureInfo, m_oBinPictureInfo.Size());

					m_arProperties.push_back (Prl((short)DocFileFormat::sprmCPicLocation,	(byte*)(&dataStreamOffset)));
					m_arProperties.push_back (Prl((short)DocFileFormat::sprmCFSpec,			(byte*)(&CFSpec)));

					m_bOK					=	TRUE;
				}
			}
		}

		Picture (const Picture& oPicture) : m_oBinPictureInfo(oPicture.m_oBinPictureInfo), m_sTextType(oPicture.m_sTextType), m_arProperties(oPicture.m_arProperties), m_bOK(oPicture.m_bOK)
		{

		}

		virtual ~Picture()
		{
		}

		virtual operator byte*() const
		{
			return (byte*)(m_oBinPictureInfo);
		}

		virtual operator const byte*() const
		{
			return (const byte*)(m_oBinPictureInfo);
		}

		virtual unsigned int Size() const
		{
			return m_oBinPictureInfo.Size();
		}

		virtual IVirtualConstructor* New() const
		{
			return new Picture();
		}

		virtual IVirtualConstructor* Clone() const
		{
			return new Picture(*this);
		}

		virtual unsigned long GetTextSize() const
		{
			return m_sTextType.size();
		}

		virtual std::wstring GetAllText() const
		{
			return m_sTextType;
		}

		virtual std::vector<Prl> GetRunProperties() const
		{
			std::vector<Prl> prls;

			for (std::list<Prl>::const_iterator iter = m_arProperties.begin(); iter != m_arProperties.end(); ++iter)
				prls.push_back(*iter);

			return prls;
		}

		virtual unsigned int PrlSize() const
		{
			return (unsigned int)m_arProperties.size();
		}


		//
		inline BOOL IsValid()
		{
			return m_bOK;
		}

	private:

		BOOL					m_bOK;

		static const byte CFSpec = 1;

		PICFAndOfficeArtData	m_oBinPictureInfo;

		std::wstring			m_sTextType;
		std::list<Prl>			m_arProperties;
	};
}