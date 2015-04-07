#pragma once

#include "../Common/Singleton.h"

#include "PICFAndOfficeArtData.h"
#include "BinaryStorage.h"
#include "IRunItem.h"
#include "Spa.h"
#include "Run.h"

#include "OfficeArt/OfficeArtFOPT.h"
#include "OfficeArt/OfficeArtFDG.h"
#include "OfficeArt/DrawingContainer.h"
#include "OfficeArt/OfficeArtCOLORREF.h"
#include "OfficeArt/OfficeArtSpContainer.h"
#include "OfficeArt/OfficeArtDgContainer.h"
#include "OfficeArt/OfficeArtClientAnchor.h"
#include "OfficeArt/OfficeArtChildAnchor.h"
#include "OfficeArt/OfficeArtClientTextbox.h"
#include "OfficeArt/OfficeArtClientData.h"
#include "OfficeArt/OfficeArtSpgrContainer.h"
#include "OfficeArt/OfficeArtWordDrawing.h"
#include "OfficeArt/OfficeArtDggContainer.h"
#include "OfficeArt/OfficeArtContent.h"
#include "OfficeArt/OfficeArtFDGGBlock.h"
#include "OfficeArt/OfficeArtBStoreContainer.h"
#include "OfficeArt/OfficeArtSplitMenuColorContainer.h"
#include "OfficeArt/OfficeArtFSPGR.h"

#include "OfficeArt/BlipFactory.h"
#include "OfficeArt/OfficeArtFOPT.h"

#include "../../Common/DocxFormat/Source/DocxFormat/Logic/Pict.h"
//#include "../../Common/DocxFormat/Source/DocxFormat/Logic/DrawingWrap.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Drawing/Drawing.h"
//#include "../../Common/DocxFormat/Source/DocxFormat/Logic/LineStyle.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/Media.h"

#include "ShapeSettings.h"
#include "WordStreamWriter.h"
#include "TextBox.h"

using namespace OfficeArt;
using namespace std;

/*

.	ѕо€снение по поводу картинок

.	изображени€ (not embedded) пишутс€ в общий WordStream после всех символов
.	информаци€ об изображени€х хранитс€ в FBSE, дл€ (not embedded) храним смещени€ в стриме WordStream (foDelay)
.	прив€зка изображение к автофигуре идет через свойство Blip::pib

...................................................................................................................

.	разница между начальным индексом дл€ объекта из MAIN DOCUMENT и HEADER должна быть 1000 
.	иначе ворд такие документы открывает без отображени€ содержимого

...................................................................................................................

.	Inline-автофигуры пишутс€ с дополнительными символами	-	{ SHAPE  \\* MERGEFORMAT }	//	 TODO : надо разбиратьс€ с бинарниками, в спецификации таких фигур нету
.	http://support.microsoft.com/kb/276862

*/

#define MAIN_DOCUMENT		0
#define HEADER_DOCUMENT		1

namespace ASCDocFileFormat
{
	class COArtShape		
	{
	public:

		COArtShape(int nID) : m_nID (nID), m_pSettings (NULL), m_nTextBoxRef(NULL)
		{
			m_nShapeType		=	OfficeArt::Enumerations::msosptNotPrimitive;

			m_bHaveBlip			=	FALSE;

			m_bGroup			=	false;
			m_bChild			=	false;
			m_bFlipH			=	false;
			m_bFlipV			=	false;
			m_bHaveSpt			=	true;

			m_bHaveChildAnchor	=	false;
			m_bHaveBounds		=	false;

			m_nChildX			=	0;
			m_nChildY			=	0;
			m_nChildWidth		=	0;
			m_nChildHeight		=	0;

			m_bTextureMode		=	FALSE;
			m_strTextureFile	=	L"";
			m_BlipPos			=	0;
		}

		virtual ~COArtShape()
		{
			RELEASEOBJECT (m_pSettings);
		}

		inline void SetID(int nID)
		{
			m_nID	=	nID;
		}

		inline int GetID()
		{
			return m_nID;
		}

		virtual CShapeSettings* GetSettings()
		{
			if (NULL == m_pSettings)
				m_pSettings	=	new CShapeSettings ();

			return m_pSettings; 
		}

		//	
		inline void SetGroup(bool bGroup)
		{
			m_bGroup	=	bGroup;
		}

		inline bool IsGroup() 
		{
			return m_bGroup;
		}
		inline void SetChild(bool bChild)
		{
			m_bChild	=	bChild;
		}

		inline void SetChildAnchor(bool bChild)
		{
			m_bHaveChildAnchor	=	bChild;
		}

		inline void SetHaveSpt(bool spt)
		{
			m_bHaveSpt	=	spt;
		}
		inline void SetChildeAnchorBounds (int nX, int nY, int nWidth, int nHeight, bool bAngleFlip = FALSE)
		{
			if (bAngleFlip)
			{
				m_nChildX		=	(int)(nX  + nWidth  * 0.5 - nHeight * 0.5);
				m_nChildY		=	(int)(nY  + nHeight * 0.5 - nWidth  * 0.5);
				m_nChildWidth	=	nHeight;
				m_nChildHeight	=	nWidth;
			}
			else
			{
				m_nChildX		=	nX;
				m_nChildY		=	nY;
				m_nChildWidth	=	nWidth;
				m_nChildHeight	=	nHeight;
			}
		}

		inline void SetFlipH(bool bFlipH)
		{
			m_bFlipH			=	bFlipH;
		}

		inline void SetFlipV(bool bFlipV)
		{
			m_bFlipV			=	bFlipV;
		}

		inline void SetShapeType(int nShapeType)
		{
			m_nShapeType	=	(OfficeArt::Enumerations::MSOSPT)nShapeType;
		}

		inline void SetTbRef(CTextBoxRef* pTBRef)
		{
			if (pTBRef)
			{
				m_nTextBoxRef	=	pTBRef;
				m_nTextBoxRef->SetID(m_nID);
			}
		}

		//
		virtual OfficeArtSpContainer GetShape()		//	create top-level shape
		{
			ASCDocFormatUtils::BitSet oShapeSettings (4);

			oShapeSettings.SetBit (m_bGroup,	0);		//	Group		-	A bit that specifies whether this shape is a group shape. 
			oShapeSettings.SetBit (m_bChild,	1);		//	Child		-	A bit that specifies whether this shape is a child shape.
			oShapeSettings.SetBit (0,			2);		//	Patriarch	-	A bit that specifies whether this shape is the topmost group shape. Each drawing contains one topmost group shape
			oShapeSettings.SetBit (0,			3);		//	Deleted		-	A bit that specifies whether this shape has been deleted. 
			oShapeSettings.SetBit (0,			4);		//	OleShape	-	A bit that specifies whether this shape is an OLE object.
			oShapeSettings.SetBit (0,			5);		//	HaveMaster	-	A bit that specifies whether this shape has a valid master in the hspMaster property, as defined in section
			oShapeSettings.SetBit (m_bFlipH,	6);		//	FlipH		-	A bit that specifies whether this shape is horizontally flipped. 
			oShapeSettings.SetBit (m_bFlipV,	7);		//	FlipV		-	A bit that specifies whether this shape is vertically flipped. 
			oShapeSettings.SetBit (0,			8);		//	Connector	-	A bit that specifies whether this shape is a connector shape.
			oShapeSettings.SetBit (true,		9);		//	HaveAnchor	-	A bit that specifies whether this shape has an anchor. 
			oShapeSettings.SetBit (0,			10);	//	Background	-	A bit that specifies whether this shape is a background shape. 
			oShapeSettings.SetBit (m_bHaveSpt,	11);	//	HaveSpt		-	A bit that specifies whether this shape has a shape type property.

			OfficeArt::OfficeArtFSP ShapeProp (m_nShapeType, m_nID, FormatUtils::BytesToUInt32 (oShapeSettings.GetBytes(), 0, sizeof(unsigned int)));

			OfficeArt::OfficeArtSpContainer oOArtChShape;

			// ¬нимание - пор€док добавлени€ объектов очень важен

			//OfficeArt::OfficeArtChildAnchor ChildAnchor (m_nLeft, m_nRight, m_nWidth, m_nHeight);
			oOArtChShape.PushBack (ShapeProp);
			//oOArtChShape.PushBack (ChildAnchor);
			
			if (GetSettings())
			{	
				oOArtChShape.PushBack(GetSettings()->GetPrimaryOptions());

				if (GetSettings()->SetupSecondaryOptions())
				{
					oOArtChShape.PushBack(GetSettings()->GetSecondaryOptions());
				}
			}

			if (m_bHaveChildAnchor)
				oOArtChShape.PushBack (OfficeArtChildAnchor(m_nChildX, m_nChildWidth + m_nChildX, m_nChildY, m_nChildHeight + m_nChildY));

			if (m_nTextBoxRef)
			{
				if (m_nTextBoxRef->IsValid())
				{
					oOArtChShape.PushBack (OfficeArtClientAnchor(0));
					oOArtChShape.PushBack (OfficeArtClientData(1));
					oOArtChShape.PushBack (OfficeArtClientTextbox(m_nTextBoxRef->GetIndex()));	//	Index form Array of  textBoxes

					return oOArtChShape;
				}
			}

			oOArtChShape.PushBack (OfficeArtClientData(1));

			return oOArtChShape;
		}

		inline OfficeArtSpContainer GetSimplyShape()
		{
			ASCDocFormatUtils::BitSet oShapeSettings (4);

			oShapeSettings.SetBit (0,			0);		//	Group		-	A bit that specifies whether this shape is a group shape. 
			oShapeSettings.SetBit (0,			1);		//	Child		-	A bit that specifies whether this shape is a child shape.
			oShapeSettings.SetBit (0,			2);		//	Patriarch	-	A bit that specifies whether this shape is the topmost group shape. Each drawing contains one topmost group shape
			oShapeSettings.SetBit (0,			3);		//	Deleted		-	A bit that specifies whether this shape has been deleted. 
			oShapeSettings.SetBit (0,			4);		//	OleShape	-	A bit that specifies whether this shape is an OLE object.
			oShapeSettings.SetBit (0,			5);		//	HaveMaster	-	A bit that specifies whether this shape has a valid master in the hspMaster property, as defined in section
			oShapeSettings.SetBit (0,			6);		//	FlipH		-	A bit that specifies whether this shape is horizontally flipped. 
			oShapeSettings.SetBit (0,			7);		//	FlipV		-	A bit that specifies whether this shape is vertically flipped. 
			oShapeSettings.SetBit (0,			8);		//	Connector	-	A bit that specifies whether this shape is a connector shape.
			oShapeSettings.SetBit (true,		9);		//	HaveAnchor	-	A bit that specifies whether this shape has an anchor. 
			oShapeSettings.SetBit (0,			10);	//	Background	-	A bit that specifies whether this shape is a background shape. 
			oShapeSettings.SetBit (m_bHaveSpt,	11);	//	HaveSpt		-	A bit that specifies whether this shape has a shape type property.

			OfficeArt::OfficeArtFSP oShapeProp (m_nShapeType, m_nID, FormatUtils::BytesToUInt32 (oShapeSettings.GetBytes(), 0, sizeof(unsigned int)));

			OfficeArt::OfficeArtSpContainer oOArtChShape;

			oOArtChShape.PushBack(oShapeProp);
			oOArtChShape.PushBack(GetSettings()->GetPrimaryOptions());

			return oOArtChShape;
		}

		virtual OfficeArtSpgrContainer GetShapes()
		{
			return OfficeArtSpgrContainer ();
		}

		//
		inline bool IsAngleBoundFlip()	// нужно только дл€ SPA(rca)
		{
			if (GetSettings ())
				return GetSettings ()->GetTransformStyle().IsAngleBoundFlip();

			return FALSE;
		}

		// XML

		inline void SetupFromStyleXml	(const OOX::Logic::ShapeStyle& oStyle)
		{
			SetRotation(oStyle);
			SetInternalFlip(oStyle);
			SetHidden(oStyle);
			SetPositioning(oStyle);
			SetupInternalTb(oStyle);
		}

		inline void SetInternalFlip		(const OOX::Logic::ShapeStyle& oStyle)
		{
			if (oStyle.flip.is_init())
			{
				SetFlipH (DOCXDOCUTILS::GetFlipH(oStyle.flip));
				SetFlipV (DOCXDOCUTILS::GetFlipV(oStyle.flip));
			}
		}

		inline void SetRotation			(const OOX::Logic::ShapeStyle& oStyle)
		{
			if (oStyle.rotation.is_init())
			{
				if (GetSettings())
					GetSettings ()->GetTransformStyle().SetRotate (oStyle.rotation);
			}
		}

		inline void SetHidden			(const OOX::Logic::ShapeStyle& oStyle)
		{
			if (oStyle.visibility.is_init())
			{
				if (GetSettings())
					GetSettings()->GetGroupShapeStyle().SetVisible (oStyle.visibility);
			}
		}

		inline void SetPositioning		(const OOX::Logic::ShapeStyle& oStyle)
		{
			if (GetSettings())
			{
				CGroupShapeStyle& oSgcStyle = GetSettings()->GetGroupShapeStyle();
				oSgcStyle.SetPositioning(oStyle.msoPositionHorizontal, 
					oStyle.msoPositionHorizontalRelative, 
					oStyle.msoPositionVertical,
					oStyle.msoPositionVerticalRelative);

				oSgcStyle.SetPercentageSettings(oStyle.msoWidthPercent,
					oStyle.msoHeightPercent,
					oStyle.msoPositionHorizontalRelative,
					oStyle.msoPositionVerticalRelative);
			}
		}


		// TODO : <v:textbox style="layout-flow:vertical">	-	 Ќаправление шрифта

		//
		inline void SetTextureFill(bool mode, const std::wstring& textureFile)
		{
			if (TRUE == mode && textureFile.length() > 0)
			{
				m_bTextureMode		=	mode;
				m_strTextureFile	=	textureFile;

				if (GetSettings()) GetSettings()->GetFillStyle().SetType(Enumerations::msofillPicture);
			}
		}

		inline bool IsTextureMode() const
		{
			return m_bTextureMode;
		}

		// “екстура дл€ автофигуры или просто изображение пишутс€ в WORD_STREAM, а в FBSE записываем смещение картинки

		virtual void SetImageIndex(int index)	//	индекс картинки в WordStream
		{
			if (GetSettings())
			{
				GetSettings()->GetFillStyle().SetBlip(index);
			}
		}

		virtual void SetBlipPos(unsigned int bufpos)
		{
			m_BlipPos	=	bufpos;
		}
		virtual OfficeArtFBSE GetFBSE()
		{
			OfficeArt::BlipFactory oBlipFactory (m_strTextureFile);
			OfficeArt::OfficeArtBlip* blip = oBlipFactory.GetOfficeArtBlip();
			if (blip)
			{
				OfficeArt::OfficeArtFBSE oBlipStoreEntry (FALSE, oBlipFactory.GetBlipType(), oBlipFactory.GetBlipType(), blip->Size(), oBlipFactory.Get_rgbUid1());
				oBlipStoreEntry.SetFoDelay(m_BlipPos);
				
				RELEASEOBJECT(blip);
				return oBlipStoreEntry;
			}

			return OfficeArt::OfficeArtFBSE();
		}

		virtual OfficeArtBlip* CreateBlip()
		{
			OfficeArt::BlipFactory oBlipFactory (m_strTextureFile.c_str());
			return oBlipFactory.GetOfficeArtBlip();
		}


	protected:

		inline void SetupInternalTb(const OOX::Logic::ShapeStyle& oStyle)		// Word портит форматирование при сохранение в (*.doc)
		{
			if (GetSettings())
			{
				if (oStyle.textAnchor.is_init())
				{
					CTextStyle& oTbStyle = GetSettings()->GetTextStyle();
					oTbStyle.SetAnchor(oStyle.textAnchor);
				}
			}
		}

	protected:

		int										m_nID;
		OfficeArt::Enumerations::MSOSPT			m_nShapeType;
		CShapeSettings*							m_pSettings;

		bool									m_bHaveBlip;	//	прив€зка картинки

		bool									m_bGroup;
		bool									m_bChild;
		bool									m_bHaveSpt;
		bool									m_bHaveChildAnchor;
		bool									m_bHaveBounds;
		bool									m_bFlipH;
		bool									m_bFlipV;

		int										m_nChildX;
		int										m_nChildY;
		int										m_nChildWidth;
		int										m_nChildHeight;

		bool									m_bTextureMode;
		std::wstring							m_strTextureFile;
		unsigned int							m_BlipPos;			

		CTextBoxRef*							m_nTextBoxRef;
	};

	class COArtImage : public COArtShape		
	{
	public:
		COArtImage (std::wstring strFileName, int nID) : COArtShape (nID)
		{
			m_nShapeType		=	OfficeArt::Enumerations::msosptPictureFrame;
			m_bHaveBlip			=	TRUE;

			m_strTextureFile	=	strFileName;
			m_BlipPos			=	0;
		}

		virtual CShapeSettings* GetSettings()
		{
			if (NULL == m_pSettings)
				m_pSettings	= new CImageSettings ();

			return m_pSettings; 
		}

		virtual void SetImageIndex(int index)	//	индекс картинки в WordStream
		{
			if (GetSettings())
			{
				((CImageSettings*)m_pSettings)->GetBlipStyle().SetPib(index);
			}
		}

		//
		virtual OfficeArtSpContainer GetShape ()		//	create top-level shape
		{
			ASCDocFormatUtils::BitSet oShapeSettings (4);

			oShapeSettings.SetBit (m_bGroup,	0);		//	Group		-	A bit that specifies whether this shape is a group shape. 
			oShapeSettings.SetBit (m_bChild,	1);		//	Child		-	A bit that specifies whether this shape is a child shape.
			oShapeSettings.SetBit (0,			2);		//	Patriarch	-	A bit that specifies whether this shape is the topmost group shape. Each drawing contains one topmost group shape
			oShapeSettings.SetBit (0,			3);		//	Deleted		-	A bit that specifies whether this shape has been deleted. 
			oShapeSettings.SetBit (0,			4);		//	OleShape	-	A bit that specifies whether this shape is an OLE object.
			oShapeSettings.SetBit (0,			5);		//	HaveMaster	-	A bit that specifies whether this shape has a valid master in the hspMaster property, as defined in section
			oShapeSettings.SetBit (m_bFlipH,	6);		//	FlipH		-	A bit that specifies whether this shape is horizontally flipped. 
			oShapeSettings.SetBit (m_bFlipV,	7);		//	FlipV		-	A bit that specifies whether this shape is vertically flipped. 
			oShapeSettings.SetBit (0,			8);		//	Connector	-	A bit that specifies whether this shape is a connector shape.
			oShapeSettings.SetBit (true,		9);		//	HaveAnchor	-	A bit that specifies whether this shape has an anchor. 
			oShapeSettings.SetBit (0,			10);	//	Background	-	A bit that specifies whether this shape is a background shape. 
			oShapeSettings.SetBit (m_bHaveSpt,	11);	//	HaveSpt		-	A bit that specifies whether this shape has a shape type property.

			OfficeArt::OfficeArtFSP ShapeProp (m_nShapeType, m_nID, FormatUtils::BytesToUInt32 (oShapeSettings.GetBytes(), 0, sizeof(unsigned int)));

			OfficeArt::OfficeArtSpContainer oOArtChShape;
			oOArtChShape.PushBack (ShapeProp);

			if (GetSettings())
			{			
				oOArtChShape.PushBack (GetSettings()->GetPrimaryOptions());
				if (GetSettings()->SetupSecondaryOptions())
				{
					oOArtChShape.PushBack(GetSettings()->GetSecondaryOptions());
				}
			}

			if (m_bHaveChildAnchor)
				oOArtChShape.PushBack (OfficeArtChildAnchor(m_nChildX, m_nChildWidth + m_nChildX, m_nChildY, m_nChildHeight + m_nChildY));

			//oOArtChShape.PushBack (OfficeArtClientAnchor(0));
			oOArtChShape.PushBack (OfficeArtClientData(1));

			if (m_nTextBoxRef)
			{
				if (m_nTextBoxRef->IsValid())
				{
					oOArtChShape.PushBack (OfficeArtClientTextbox(m_nTextBoxRef->GetIndex()));		//	Index form Array of  textBoxes
				}
			}

			return oOArtChShape;
		}

		// XML
		inline void SetRotationImage (const OOX::Logic::Drawing& oXml)
		{
			if (oXml.Inline->Graphic->Pic->rot.is_init())
			{
				if (GetSettings())
					GetSettings ()->GetTransformStyle().SetRotate (oXml.Inline->Graphic->Pic->rot, true);
			}
		}

		inline void SetInternalFlipImage (const OOX::Logic::Drawing& oXml)
		{
			if (oXml.Inline->Graphic->Pic->flipH.is_init())
				SetFlipH (DOCXDOCUTILS::GetFlipH(oXml.Inline->Graphic->Pic->flipH));

			if (oXml.Inline->Graphic->Pic->flipV.is_init())
				SetFlipV (DOCXDOCUTILS::GetFlipV(oXml.Inline->Graphic->Pic->flipV));
		}

		inline void SetPositioningImage	(const OOX::Logic::Drawing& oXml)
		{
			if (GetSettings())
			{
				CGroupShapeStyle& oSgcStyle			=	GetSettings()->GetGroupShapeStyle();

				std::string msoPositionHorizontal	=	std::string ("");
				if (oXml.Inline->positionHAlign.is_init())
					msoPositionHorizontal			=	oXml.Inline->positionHAlign;

				std::string msoPositionHorizontalRelative	=	std::string ("");
				if (oXml.Inline->positionHRelativeFrom.is_init())
					msoPositionHorizontalRelative	=	oXml.Inline->positionHRelativeFrom;

				std::string msoPositionVertical	=	std::string ("");
				if (oXml.Inline->positionVAlign.is_init())
					msoPositionVertical				=	oXml.Inline->positionVAlign;

				std::string msoPositionVerticalRelative	=	std::string ("");
				if (oXml.Inline->positionVRelativeFrom.is_init())
					msoPositionVerticalRelative		=	oXml.Inline->positionVRelativeFrom;

				oSgcStyle.SetPositioning(msoPositionHorizontal, msoPositionHorizontalRelative, msoPositionVertical, msoPositionVerticalRelative);
			}
		}
	};

	class COArtGroup : public COArtShape
	{
	public:
		COArtGroup(int nID) : COArtShape (nID)
		{
			m_bGroup		=	true;

			m_nBoundWidth	=	0;
			m_nBoundHeight	=	0;
		}

		~COArtGroup()
		{
			for (size_t i = 0; i < m_arShapes.size(); ++i)
				RELEASEOBJECT(m_arShapes[i]);
		}

		inline bool IsValid()
		{
			return ((long)m_arShapes.size() > 0 &&
				(((int)m_oCoordSize.X() != 0) && ((int)m_oCoordSize.Y() != 0)) );
		}

		inline void Append (COArtShape* pShape)
		{
			if (pShape)
			{
				pShape->SetChild (true);
				pShape->SetChildAnchor (true);

				m_arShapes.push_back (pShape);
			}
		}

		virtual OfficeArtSpgrContainer GetShapes ()		
		{
			OfficeArtSpgrContainer oMain;				//	0xF003	-	OfficeArtSpgrContainer

			oMain.PushBack(CreateTopShape(m_nID));

			for (size_t i = 0; i < m_arShapes.size(); ++i)
			{
				if (m_arShapes[i]->IsGroup())
					oMain.PushBack(m_arShapes[i]->GetShapes());
				else
					oMain.PushBack(m_arShapes[i]->GetShape());
			}

			return oMain;
		}

		//
		inline void SetBounds (const OOX::Logic::ShapeStyle& oStyle)
		{
			Unit<int, Pt> nX (0);
			Unit<int, Pt> nY (0);			

			if (oStyle.leftTop.is_init())
			{
				nX = *oStyle.leftTop->X;
				nY = *oStyle.leftTop->Y;
			}

			Unit<int, Pt> nWidth (*oStyle.Size->Width);
			Unit<int, Pt> nHeight (*oStyle.Size->Height);

			SetChildeAnchorBounds (nX, nY, nWidth, nHeight);
		}

		inline void SetCoord (const DOCX::CFPoint& oOrigin, const DOCX::CFPoint& oSize)
		{
			m_oCoordOrigin	=	oOrigin;
			m_oCoordSize	=	oSize;
		}

	private:

		inline OfficeArtSpContainer CreateTopShape (int nID)		//	create top-level shape
		{
			ASCDocFormatUtils::BitSet oShapeSettings (4);

			oShapeSettings.SetBit (m_bGroup,0);		//	Group		-	A bit that specifies whether this shape is a group shape. 
			oShapeSettings.SetBit (m_bChild,1);		//	Child		-	A bit that specifies whether this shape is a child shape.
			oShapeSettings.SetBit (0,		2);		//	Patriarch	-	A bit that specifies whether this shape is the topmost group shape. Each drawing contains one topmost group shape
			oShapeSettings.SetBit (0,		3);		//	Deleted		-	A bit that specifies whether this shape has been deleted. 
			oShapeSettings.SetBit (0,		4);		//	OleShape	-	A bit that specifies whether this shape is an OLE object.
			oShapeSettings.SetBit (0,		5);		//	HaveMaster	-	A bit that specifies whether this shape has a valid master in the hspMaster property, as defined in section
			oShapeSettings.SetBit (m_bFlipH,6);		//	FlipH		-	A bit that specifies whether this shape is horizontally flipped. 
			oShapeSettings.SetBit (m_bFlipV,7);		//	FlipV		-	A bit that specifies whether this shape is vertically flipped. 
			oShapeSettings.SetBit (0,		8);		//	Connector	-	A bit that specifies whether this shape is a connector shape.
			oShapeSettings.SetBit (true,	9);		//	HaveAnchor	-	A bit that specifies whether this shape has an anchor. 
			oShapeSettings.SetBit (0,		10);	//	Background	-	A bit that specifies whether this shape is a background shape. 
			oShapeSettings.SetBit (0,		11);	//	HaveSpt		-	A bit that specifies whether this shape has a shape type property.

			OfficeArtFSP ShapeProp (OfficeArt::Enumerations::msosptNotPrimitive, nID, FormatUtils::BytesToUInt32 (oShapeSettings.GetBytes(), 0, sizeof(unsigned int)));

			// ¬нимание - пор€док добавлени€ объектов очень важен

			OfficeArtSpContainer oShapeGroup;
			oShapeGroup.PushBack (OfficeArtFSPGR((int)m_oCoordOrigin.X(),(int)m_oCoordOrigin.Y(), 
				(int)m_oCoordSize.X() + (int)m_oCoordOrigin.X(),(int)m_oCoordSize.Y() + (int)m_oCoordOrigin.Y() ));

			oShapeGroup.PushBack (ShapeProp);
			oShapeGroup.PushBack (GetSettings()->GetPrimaryOptions());

			if (m_bHaveChildAnchor)
				oShapeGroup.PushBack (OfficeArtChildAnchor(m_nChildX, m_nChildWidth + m_nChildX, m_nChildY, m_nChildHeight + m_nChildY));
			else
				oShapeGroup.PushBack (OfficeArtClientAnchor(0));

			oShapeGroup.PushBack (OfficeArtClientData(1));

			return oShapeGroup;
		}

	private:

		std::vector <COArtShape*>	m_arShapes;

		int								m_nBoundWidth;
		int								m_nBoundHeight;	

		DOCX::CFPoint					m_oCoordSize;
		DOCX::CFPoint					m_oCoordOrigin;
	};
}

namespace ASCDocFileFormat
{
	class CShapeRun : public IRunItem
	{
	public:
		CShapeRun ()	: m_bInline(FALSE)
		{

		}

		CShapeRun (COArtShape* pShape, int nID, unsigned int nSSpa, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0) : m_oSpa (nID, nX, nY, nWidth, nHeight), ___refShape(NULL), m_bInline(NULL)
		{
			Init (nSSpa);

			___refShape	=	pShape;
		}

		//
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
			return m_arProperties;
		}

		virtual unsigned int PrlSize () const	
		{
			return m_arProperties.size();
		}

		virtual IVirtualConstructor* New() const
		{
			return new CShapeRun();
		}

		virtual IVirtualConstructor* Clone() const
		{
			return new CShapeRun(*this);
		}

		//
		inline void UpdateSizes(const OOX::Logic::ShapeStyle& refStyle)
		{
			Unit<int, Dx> nX (0);
			Unit<int, Dx> nY (0);			

			if (refStyle.Point.is_init())
			{
				nX		=	*refStyle.Point->X;
				nY		=	*refStyle.Point->Y;
			}

			Unit<int, Dx> nWidth  (0);
			Unit<int, Dx> nHeight (0);

			nWidth		=	*refStyle.Size->Width;
			nHeight		=	*refStyle.Size->Height;

			m_oSpa		=	Spa (m_oSpa.m_lid, nX, nY, nWidth, nHeight);
			m_oSpa.Update();
		}

		inline void UpdateSizes(int nX, int nY, int nWidth, int nHeight)
		{
			m_oSpa		=	Spa (m_oSpa.m_lid, nX, nY, nWidth, nHeight);
			m_oSpa.Update();
		}

		inline void SetWrap(const nullable<OOX::Logic::Wrap>& oWrap)
		{
			if (oWrap.is_init())
			{
				SetWrap (oWrap->Type);
			}
		}

		inline void SetWrap(std::string oType)
		{
			if (oType.length())
			{
				m_oSpa.wr	=	DOCXDOCUTILS::GetStyleWrapType(oType);
				m_oSpa.Update();
			}
		}

		inline void SetAnchor(bool bHaveAnchor)
		{
			if (bHaveAnchor)
			{
				m_oSpa.wr			=	3;
				m_oSpa.fAnchorLock	=	true;
				m_oSpa.fBelowText	=	false;
				m_oSpa.bx			=	Spa::TEXT;
				m_oSpa.by			=	Spa::TEXT;

				m_oSpa.m_rca.left	=	0;
				m_oSpa.m_rca.top	=	0;

				m_oSpa.Update();

				m_bInline			=	TRUE;
			}
		}

		inline void UpdateAnchorPosition(const OOX::Logic::ShapeStyle& style)
		{
			std::string strPsh		=	style.msoPositionHorizontal;			
			std::string strPshRel	=	style.msoPositionHorizontalRelative;
			std::string strPsv		=	style.msoPositionVertical;
			std::string strPsvRel	=	style.msoPositionVerticalRelative;

			if (strPsh.length() || strPshRel.length() || strPsv.length() || strPsvRel.length())
			{
				if (___refShape)
					___refShape->GetSettings()->GetFillStyle().SetUseShapeAnchor(true);
			}

			if (std::string("margin") == strPshRel)
				m_oSpa.bx	=	Spa::MARGIN;
			else if (std::string("page") == strPshRel)
				m_oSpa.bx	=	Spa::PAGE;	

			if (std::string("margin") == strPsvRel)
				m_oSpa.by	=	Spa::MARGIN;	
			else if (std::string("page") == strPsvRel)
				m_oSpa.by	=	Spa::PAGE;

			m_oSpa.Update();
		}

		inline void UpdateAnchorPositionImage(const OOX::Logic::Drawing& oXml)
		{
			std::string strPsh;			
			std::string strPshRel;
			std::string strPsv;
			std::string strPsvRel;
			
			if (oXml.Inline->positionHAlign.is_init())
				strPsh		=	oXml.Inline->positionHAlign;
			if (oXml.Inline->positionHRelativeFrom.is_init())
				strPshRel	=	oXml.Inline->positionHRelativeFrom;
			if (oXml.Inline->positionVAlign.is_init())
				strPsv		=	oXml.Inline->positionVAlign;
			if (oXml.Inline->positionVRelativeFrom.is_init())
				strPsvRel	=	oXml.Inline->positionVRelativeFrom;

			if (std::string("margin") == strPshRel)
				m_oSpa.bx	=	Spa::MARGIN;
			else if (std::string("page") == strPshRel)
				m_oSpa.bx	=	Spa::PAGE;	

			if (std::string("margin") == strPsvRel)
				m_oSpa.by	=	Spa::MARGIN;	
			else if (std::string("page") == strPsvRel)
				m_oSpa.by	=	Spa::PAGE;

			m_oSpa.Update();
		}
	
		inline void SetUseBehind(const OOX::Logic::ShapeStyle& refStyle)
		{
			if (___refShape)
				___refShape->GetSettings ()->SetUseBehind(refStyle);
		}
	
		//
		inline void SetImageSize(const OOX::Logic::Drawing& oDrawing)
		{
			Unit<int, Dx> wDx	=	*oDrawing.Inline->Extent->Size->Width;
			Unit<int, Dx> hDx	=	*oDrawing.Inline->Extent->Size->Height;

			Unit<int, Dx> nX	=	*oDrawing.Inline->anchorXY->X;
			Unit<int, Dx> nY	=	*oDrawing.Inline->anchorXY->Y;

			UpdateSizes (nX, nY, wDx, hDx);
		}
		inline void SetImageWrap(const nullable<OOX::Logic::DrawingWrap>& oWrap)
		{
			if (oWrap.is_init())
			{				
				if (oWrap->Type == std::string("wrapTopAndBottom"))
					m_oSpa.wr	=	1;
				else if (oWrap->Type == std::string("wrapSquare"))
					m_oSpa.wr	=	2;
				else if (oWrap->Type == std::string("wrapTight"))
					m_oSpa.wr	=	4;
				else if (oWrap->Type == std::string("wrapThrough"))
					m_oSpa.wr	=	5;
				else if (oWrap->Type == std::string("wrapNone"))
					m_oSpa.wr	=	3;
				else
					m_oSpa.wr	=	0;

				m_oSpa.Update();
			}
		}

		inline void SetImageUseBehind(const nullable<bool>& oBehindDoc)
		{
			if (oBehindDoc.is_init())
			{
				if (___refShape)
					___refShape->GetSettings ()->SetUseBehind((bool)oBehindDoc);
			}
		}

		inline Spa& GetSpa()
		{
			if (___refShape)
			{
				m_oSpa.SetFlip(___refShape->IsAngleBoundFlip());
			}

			return m_oSpa;
		}

		//
		inline bool IsInline() const
		{
			return m_bInline;
		}

		inline COArtShape* GetReference() const
		{
			return ___refShape;
		}
		inline int GetWidth() const
		{
			return m_oSpa.m_rca.right - m_oSpa.m_rca.left;
		}

		inline int GetHeight() const
		{
			return m_oSpa.m_rca.bottom - m_oSpa.m_rca.top;
		}

	private:

		inline void Init (unsigned int nSSpa)
		{
			m_sTextType		=	std::wstring(&TextMark::DrawnObject);

			m_arProperties.push_back (Prl ((short)DocFileFormat::sprmCFSpec, (unsigned char*)(&CFSpec)));
			m_arProperties.push_back (Prl ((short)DocFileFormat::sprmCPicLocation, (unsigned char*)(&nSSpa)));	//	offset spa in storage from 0
		}

	private:

		static const unsigned char CFSpec = 1;	

		Spa					m_oSpa;
		std::wstring		m_sTextType;
		std::vector <Prl>	m_arProperties;

		COArtShape*			___refShape;	//	ONLY FOR INNER SETTER

		bool				m_bInline;
	};

	class CInlineShape : public IRunItem, public IOperand	//	дл€ картинок c обтеканием вокруг текста
	{
	public:

		CInlineShape () : m_oBinPictureInfo(), m_sTextType (std::wstring (&TextMark::Picture))
		{

		}

		CInlineShape (const CShapeRun& oRun) : m_oBinPictureInfo(), m_sTextType (std::wstring (&TextMark::Picture))
		{		
			/*

			// ONLY FOR TEST

			#ifdef _DEBUG

			ASCDocFormatUtils::BitSet oShapeSettings (4);

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

			#endif

			*/			

			OfficeArt::OfficeArtSpContainer oShape	=	oRun.GetReference()->GetSimplyShape();

			//OfficeArt::OfficeArtClientAnchor oAnchor (0x80000000);
			//oShape.PushBack (oAnchor);

			OfficeArt::OfficeArtInlineSpContainer oPicture (oShape);

			//OfficeArt::OfficeArtInlineSpContainer oPicture (shape);
			//OfficeArt::OfficeArtFBSE oBlipStoreEntry (FALSE, OfficeArt::Enumerations::msoblipJPEG, OfficeArt::Enumerations::msoblipJPEG, 0);
			//oPicture.PushBack(oBlipStoreEntry);

			int IMAGE_RATIO				=	1000;	// TODO : пересчет

			PICMID	oBorders		(oRun.GetWidth(), oRun.GetHeight(), IMAGE_RATIO, IMAGE_RATIO, Brc80(0), Brc80(0), Brc80(0), Brc80(0));
			PICF	oPictureInfo	(oPicture.Size(), oBorders);

			m_oBinPictureInfo			=	PICFAndOfficeArtData (oPictureInfo, oPicture);

			if (BinaryStorageSingleton::Instance())
			{
				int dataStreamOffset	=	BinaryStorageSingleton::Instance()->PushData( (const unsigned char*)m_oBinPictureInfo, m_oBinPictureInfo.Size());

				m_arProperties.push_back (Prl((short)DocFileFormat::sprmCPicLocation,	(unsigned char*)(&dataStreamOffset)));
				m_arProperties.push_back (Prl((short)DocFileFormat::sprmCFSpec,			(unsigned char*)(&CFSpec)));
			}
		}

		CInlineShape (const CInlineShape& oShape) : m_oBinPictureInfo(oShape.m_oBinPictureInfo), m_sTextType(oShape.m_sTextType), m_arProperties(oShape.m_arProperties)
		{

		}

		virtual ~CInlineShape()
		{
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(m_oBinPictureInfo);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)(m_oBinPictureInfo);
		}

		virtual unsigned int Size() const
		{
			return m_oBinPictureInfo.Size();
		}

		virtual IVirtualConstructor* New() const
		{
			return new CInlineShape();
		}

		virtual IVirtualConstructor* Clone() const
		{
			return new CInlineShape(*this);
		}

		virtual unsigned long GetTextSize() const
		{
			return m_sTextType.size();
		}

		virtual wstring GetAllText() const
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

	private:

		static const unsigned char CFSpec = 1;

		PICFAndOfficeArtData	m_oBinPictureInfo;

		std::wstring			m_sTextType;
		std::list<Prl>			m_arProperties;
	};
}

namespace ASCDocFileFormat
{	
	class COfficeArtStorage		//	формирует буфер данных из всех объектов OfficeArt
	{
	private:
		friend class COArtStorage;
	public:
		COfficeArtStorage () : m_oMainDrawings (MAIN_DOCUMENT), m_oHeaderDrawings(HEADER_DOCUMENT)
		{
			m_nSize			=	0;

			m_nID			=	1025;
			m_nHdID			=	2048;

			m_nImageID		=	0;

			m_nMainTbID		=	1;
			m_nHeadTbID		=	1;
		}

		virtual ~COfficeArtStorage ()
		{
			Clear ();
		}

		inline int GenID (int Location)
		{
			if (HEADER_DOCUMENT == Location)
				return ++m_nHdID;

			return ++m_nID;
		}

		//
		inline void Add (COArtShape* pShape, const Spa& oSpa, int Location)
		{
			if (pShape)
			{
				if (MAIN_DOCUMENT == Location)
				{
					pShape->SetID(m_nID);

					m_arMainShapes.push_back (pShape);
					m_arrSpa.push_back (oSpa);
				}

				if (HEADER_DOCUMENT == Location)
				{
					pShape->SetID(m_nHdID);

					m_arHeaderShapes.push_back (pShape);
					m_arrHeadSpa.push_back (oSpa);
				}

				if (pShape->IsTextureMode())
				{
					SaveBlipImage(pShape);
				}
			}
		}

		inline void AddImage (COArtShape* pImage, const Spa& oSpa, int Location)
		{					
			if (pImage)
			{
				Add (pImage, oSpa, Location);

				SaveBlipImage(pImage);
			}
		}

		inline void AddGroup (COArtGroup* pShape, const Spa& oSpa, int Location)
		{
			if (pShape)
			{
				if (MAIN_DOCUMENT == Location)
				{
					//pShape->SetID(m_nID);

					m_arMainShapes.push_back (pShape);
					m_arrSpa.push_back (oSpa);
				}

				if (HEADER_DOCUMENT == Location)
				{
					//pShape->SetID(m_nHdID);

					m_arHeaderShapes.push_back (pShape);
					m_arrHeadSpa.push_back (oSpa);
				}
			}
		}

		inline void SaveBlipImage (COArtShape* pImage)
		{
			if (pImage)
			{
				pImage->SetImageIndex (++m_nImageID);
				m_arrRefImages.push_back (pImage);
			}
		}

		//
		inline unsigned char* Get()
		{
			return (unsigned char*)(m_oArtContent);
		}

		inline unsigned int Size()
		{
			if ((0 == m_arMainShapes.size()) && (0 == m_arHeaderShapes.size()))
				return 0;

			return m_oArtContent.Size();
		}

		// Spa
		inline const std::vector<Spa>& GetSpa (int nLocation)
		{
			if (HEADER_DOCUMENT == nLocation)
				return m_arrHeadSpa;

			return m_arrSpa;
		}

		inline unsigned int GetOffSetSpa (int nLocation)
		{
			unsigned int nSize	=	0;

			if (MAIN_DOCUMENT == nLocation)
			{
				for (size_t i = 0; i < m_arrHeadSpa.size(); ++i)
					nSize	+=	m_arrHeadSpa[i].Size();
			}

			if (HEADER_DOCUMENT == nLocation)
			{
				for (size_t i = 0; i < m_arrHeadSpa.size(); ++i)
					nSize	+=	m_arrHeadSpa[i].Size();
			}

			return nSize;
		}

		inline bool Compile ()
		{
			if ((0 == m_arMainShapes.size()) && (0 == m_arHeaderShapes.size()))
				return FALSE;

			int SavedShapes		=	0;			
			int DrawingsCount	=	0;
			int IdClusters		=	1;

			if (m_arMainShapes.size())
			{
				SavedShapes		+=	m_arMainShapes.size() + 1;
				++DrawingsCount;
				++IdClusters;
			}

			if (m_arHeaderShapes.size())
			{
				SavedShapes		+=	m_arHeaderShapes.size() + 1;
				++DrawingsCount;
				++IdClusters;
			}

			OfficeArtFDGGBlock drawingGroup;
			drawingGroup.SetHead (OfficeArtFDGG (max(m_nID,m_nHdID), IdClusters, SavedShapes, DrawingsCount ) );

			int ClusterID = 0;
			if (m_arHeaderShapes.size())
			{
				++ClusterID;
				drawingGroup.PushBack(OfficeArtIDCL (ClusterID, 3));
			}

			if (m_arMainShapes.size())
			{
				++ClusterID;
				drawingGroup.PushBack(OfficeArtIDCL (ClusterID, 2));
			}

			OfficeArtSplitMenuColorContainer oSplitColors (0x0000ffff, 0x00ff0000, 0x00808080, 0x100000f7);

			m_oDrawingGroupData.PushBack (drawingGroup);
			m_oDrawingGroupData.PushBack (oSplitColors);

			if (m_arrRefImages.size())
			{
				m_oDrawingGroupData.PushBack(CreateImageStorage());	//	ссылки на индексы изображений пишутс€ в свойство Pib
			}

			m_oArtContent.PushBack (m_oDrawingGroupData);

			if (m_arHeaderShapes.size())
			{
				m_oHeaderDrawings.PushBack (CreateDrawing (HEADER_DOCUMENT));
				m_oArtContent.PushBack (m_oHeaderDrawings);
			}

			if (m_arMainShapes.size())
			{
				m_oMainDrawings.PushBack (CreateDrawing (MAIN_DOCUMENT));
				m_oArtContent.PushBack (m_oMainDrawings);
			}

			return TRUE;
		}
		//
		inline void WriteBlips ()   //  артинка пишетс€ в WordStream, в FBSE пишем смещение Blip в этом стриме 
		{
			STREAMS::CSWordWriter* pBin	= STREAMS::CSWordWriter::Instance();
			if (pBin)
			{
				if (pBin->Get())
				{					
					for (size_t i = 0; i < m_arrRefImages.size(); ++i)
					{
						OfficeArtBlip* pBlip = m_arrRefImages[i]->CreateBlip();
						if (pBlip)
						{
							unsigned long bufpos = pBin->BufferPos();
							if (pBin->Push((*pBlip), pBlip->Size()))
							{
								m_arrRefImages[i]->SetBlipPos(bufpos);
							}

							RELEASEOBJECT(pBlip);
						}
					}
				}
			}
		}

		// TB
		inline CTextBoxRef* GenTbRef (int nLocation)
		{
			CTextBoxRef* pTbRef = new CTextBoxRef (GenTbID(nLocation));
			if (pTbRef)
			{
				if (HEADER_DOCUMENT == nLocation)
					m_arHeaderTbRef.push_back (pTbRef);
				else
					m_arMainTbRef.push_back (pTbRef);
			}

			return pTbRef;
		}

		inline const std::vector<CTextBoxRef*>& GetTbRefs (int Location)
		{
			if (HEADER_DOCUMENT == Location)
				return m_arHeaderTbRef;

			return m_arMainTbRef;
		}


	private:

		inline void Clear ()
		{
			for (size_t i = 0; i < m_arMainShapes.size(); ++i)
				RELEASEOBJECT(m_arMainShapes[i]);

			for (size_t i = 0; i < m_arHeaderShapes.size(); ++i)
				RELEASEOBJECT(m_arHeaderShapes[i]);

			for (size_t i = 0; i < m_arMainTbRef.size(); ++i)
				RELEASEOBJECT(m_arMainTbRef[i]);

			for (size_t i = 0; i < m_arHeaderTbRef.size(); ++i)
				RELEASEOBJECT(m_arHeaderTbRef[i]);
		}

		//
		inline OfficeArtDgContainer		CreateDrawing (int Location)
		{
			OfficeArtDgContainer container;

			if (MAIN_DOCUMENT == Location)
			{
				if (m_arMainShapes.size())
				{
					OfficeArtFDG drawingData ((OfficeArt::MSOSPID)(m_arMainShapes[0]->GetID() + 1), m_arMainShapes.size() * 2);			//	0xF008

					container.PushBack(drawingData);
					container.PushBack(CreateGroup(Location));				
				}
			}

			if (HEADER_DOCUMENT == Location)
			{
				if (m_arHeaderShapes.size())
				{
					OfficeArtFDG drawingData ((OfficeArt::MSOSPID)(m_arHeaderShapes[0]->GetID() + 1), m_arHeaderShapes.size() * 2);			//	0xF008

					container.PushBack(drawingData);
					container.PushBack(CreateGroup(Location));				
				}
			}

			return container;
		}

		inline OfficeArtSpgrContainer	CreateGroup (int Location)			//	пока группировка не сделана, структура будет плоской по содержанию элементов
		{
			OfficeArtSpgrContainer oGroup;									//	0xF003	-	OfficeArtSpgrContainer

			if (MAIN_DOCUMENT == Location)
			{
				oGroup.PushBack(CreateTopShape(m_arMainShapes[0]->GetID() - 1));

				for (size_t i = 0; i < m_arMainShapes.size(); ++i)
				{
					if (m_arMainShapes[i]->IsGroup())
						oGroup.PushBack(m_arMainShapes[i]->GetShapes());
					else
						oGroup.PushBack(m_arMainShapes[i]->GetShape());
				}
			}

			if (HEADER_DOCUMENT == Location)
			{
				oGroup.PushBack(CreateTopShape(m_arHeaderShapes[0]->GetID() - 1));

				for (size_t i = 0; i < m_arHeaderShapes.size(); ++i)
				{
					if (m_arHeaderShapes[i]->IsGroup())
						oGroup.PushBack(m_arHeaderShapes[i]->GetShapes());
					else
						oGroup.PushBack(m_arHeaderShapes[i]->GetShape());
				}
			}

			return oGroup;
		}

		inline OfficeArtSpContainer		CreateTopShape (int nID)		//	create top-level shape
		{
			FSPGR Coordinates (0,0,0,0);

			ASCDocFormatUtils::BitSet oShapeSettings (4);

			oShapeSettings.SetBit (true,	0);		//	Group		-	A bit that specifies whether this shape is a group shape. 
			oShapeSettings.SetBit (0,		1);		//	Child		-	A bit that specifies whether this shape is a child shape.
			oShapeSettings.SetBit (true,	2);		//	Patriarch	-	A bit that specifies whether this shape is the topmost group shape. Each drawing contains one topmost group shape
			oShapeSettings.SetBit (0,		3);		//	Deleted		-	A bit that specifies whether this shape has been deleted. 
			oShapeSettings.SetBit (0,		4);		//	OleShape	-	A bit that specifies whether this shape is an OLE object.
			oShapeSettings.SetBit (0,		5);		//	HaveMaster	-	A bit that specifies whether this shape has a valid master in the hspMaster property, as defined in section
			oShapeSettings.SetBit (0,		6);		//	FlipH		-	A bit that specifies whether this shape is horizontally flipped. 
			oShapeSettings.SetBit (0,		7);		//	FlipV		-	A bit that specifies whether this shape is vertically flipped. 
			oShapeSettings.SetBit (0,		8);		//	Connector	-	A bit that specifies whether this shape is a connector shape.
			oShapeSettings.SetBit (0,		9);		//	HaveAnchor	-	A bit that specifies whether this shape has an anchor. 
			oShapeSettings.SetBit (0,		10);	//	Background	-	A bit that specifies whether this shape is a background shape. 
			oShapeSettings.SetBit (0,		11);	//	HaveSpt		-	A bit that specifies whether this shape has a shape type property.

			OfficeArtFSP ShapeProp (OfficeArt::Enumerations::msosptNotPrimitive, nID, FormatUtils::BytesToUInt32 (oShapeSettings.GetBytes(), 0, sizeof(unsigned int)));

			OfficeArtSpContainer oShapeGroup;
			oShapeGroup.PushBack (Coordinates);
			oShapeGroup.PushBack (ShapeProp);

			return oShapeGroup;
		}

		// FBSE
		inline OfficeArtBStoreContainer	CreateImageStorage ()	//	пишетс€ отдельным паком
		{
			OfficeArtBStoreContainer container;

			for (size_t i = 0; i < m_arrRefImages.size(); ++i)
				container.PushBack(m_arrRefImages[i]->GetFBSE());

			return container;
		}


		// TB
		inline int GenTbID (int Location)
		{
			if (HEADER_DOCUMENT == Location)
				return m_nHeadTbID++;

			return m_nMainTbID++;
		}

	private:

		int								m_nID;
		int								m_nHdID;
		int								m_nImageID;				//	»ндекс картинки в OfficeArtBStoreContainer

		unsigned int					m_nSize;	

		// ODRAW

		OfficeArtContent				m_oArtContent;			//	DrawingGroupData(OfficeArtDggContainer) + Drawings(OfficeArtWordDrawing)
		OfficeArtDggContainer			m_oDrawingGroupData;	//	DrawingGroupData		0xF000

		// MAIN DOCUMENT

		std::vector<Spa>				m_arrSpa;
		OfficeArtWordDrawing			m_oMainDrawings;		//	dgglbl(0)
		std::vector<COArtShape*>		m_arMainShapes;

		// HEADER DOCUMENT

		std::vector<Spa>				m_arrHeadSpa;
		OfficeArtWordDrawing			m_oHeaderDrawings;		//	dgglbl(1)		
		std::vector<COArtShape*>		m_arHeaderShapes;

		std::vector<COArtShape*>		m_arrRefImages;			//	 FBSE

		// TEXT BOXES

		int								m_nMainTbID;
		int								m_nHeadTbID;

		std::vector<CTextBoxRef*>		m_arMainTbRef;
		std::vector<CTextBoxRef*>		m_arHeaderTbRef;
	};

	class COArtStorage : public Singleton<COArtStorage>, public COfficeArtStorage
	{
	protected:
		COArtStorage () : COfficeArtStorage() {}
		virtual ~COArtStorage() {}

		friend class Singleton<COArtStorage>;
	};
}

namespace ASCDocFileFormat
{
	class COArtBuilder
	{
	private:
		struct CMapShape
		{
		public:

			CMapShape () : m_nType(0)
			{

			}

			CMapShape (int nType) : m_nType(nType)
			{

			}

			CMapShape (std::string ID, int nType) : m_ID(ID), m_nType(nType)
			{

			}

		public:

			std::string				m_ID;
			int						m_nType;

			std::string				m_strPath;
			std::string				m_strCoordSize;
			std::string				m_strConnection;
			std::string				m_strAdjustValues;
			std::string				m_strSites;
			std::string				m_strSitesDir;
			std::string				m_strFormulas;
			std::string				m_textboxrect;
		};

	public:
		COArtBuilder() : m_nLocation (0), m_pLastTbRef(NULL)
		{

		}

		bool BuildShapeRun (const OOX::Logic::Pict& oPicture, CShapeRun& oShapeRun);
		bool BuildImageRun (const OOX::Logic::Shape& oXml, const std::wstring& strFileName, Run& oRun);
		bool BuildImageRun (const OOX::Image& oXml, const OOX::Logic::Drawing& oXml2, Run& oRun);

		template<class T> bool BuildShapeWithTextureFill (const T& oXml, const std::wstring& strFileName, int Type, const OOX::Logic::Pict& oPicture, Run& oRun)
		{
			if (typeid(T) == typeid(OOX::Logic::Shape))	
			{
				m_strTextureFile	=	strFileName;

				CShapeRun shapeRun = BuildOdbflRun <OOX::Logic::Shape> (oPicture.shape, GetRefShape(oPicture));
				oRun.AddRunItem (shapeRun);

				return TRUE;			
			}

			COArtStorage* pStorage	=	COArtStorage::Instance();
			if (pStorage)
			{
				COArtImage* pImage	=	new COArtImage(strFileName, pStorage->GenID (m_nLocation));
				if (pImage)
				{
					CShapeRun oShapeRun		(pImage, pImage->GetID(), pStorage->GetOffSetSpa(m_nLocation));	//	смещение берем от предыдущего элемента

					oShapeRun.UpdateSizes	(*oXml.style);													//	размеры
					oShapeRun.SetWrap		(oXml.Wrap);													//	обтекание
					oShapeRun.SetAnchor		((bool)(oXml.anchorlock.is_init()));
					oShapeRun.SetUseBehind	(*oXml.style);													

					pImage->SetupFromStyleXml(oXml.style);

					SetupTextureShape <T>(oXml, CMapShape(), pImage);
					pImage->SetShapeType(Type);

					pStorage->AddImage (pImage, oShapeRun.GetSpa(), m_nLocation);										

					oRun.AddRunItem (oShapeRun);					

					return TRUE;
				}
			}

			return FALSE;
		}

		//
		COArtShape* BuildOArtShape (const OOX::Logic::Shape& oXml, const OOX::Logic::Group& oXmlGroup);
		COArtShape* BuildOArtImage (const OOX::Logic::Shape& oXml, const std::wstring& strFileName);
		//
		CShapeRun BuildGroupRun (const OOX::Logic::Group& oXml, COArtGroup* pShape);
		template<class T> COArtShape* BuildOArtGroupShape (const T& oXml, int nType)
		{
			COArtShape* pShape	=	InternalBuildOArtShape <T> (oXml,CMapShape(nType));
			if (pShape)
			{
				const OOX::Logic::ShapeStyle& oStyle	=	(*oXml.style);

				Unit<int, Pt> nX (0);
				Unit<int, Pt> nY (0);			

				if (oStyle.leftTop.is_init())
				{
					nX = *oStyle.leftTop->X;
					nY = *oStyle.leftTop->Y;
				}

				Unit<int, Pt> nWidth (*oStyle.Size->Width);
				Unit<int, Pt> nHeight (*oStyle.Size->Height);

				if (typeid(T) == typeid(OOX::Logic::Line))	//	дл€ DOC файла точки начала и конца линии устанавливаютс€ в структуре SPA
				{
					OOX::Logic::Line* pLine = (OOX::Logic::Line*)(&oXml);
					if (pLine)
					{
						if (pLine->from.is_init() && pLine->to.is_init())
						{
							DOCX::CPointF oFrom(pLine->from);
							DOCX::CPointF oTo(pLine->to);

							nX		=	oFrom.GetTX();
							nY		=	oFrom.GetTY();
							nWidth	=	oTo.GetTX() - oFrom.GetTX();
							nHeight	=	oTo.GetTY() - oFrom.GetTY();
						}
					}
				}

				pShape->SetChildeAnchorBounds (nX, nY, nWidth, nHeight, pShape->IsAngleBoundFlip());
			}			

			return pShape;
		}

		//
		inline void SetLocation (int nLocation)
		{
			//	0	-	Main	Document
			//	1	-	Header	Document

			m_nLocation		=	nLocation;
		}

		inline int Location ()
		{
			return m_nLocation;
		}

		inline CTextBoxRef* LastTbRef ()	//	если у фигуры есть внутренний контент, индексируем его (обычный список)
		{
			CTextBoxRef* pLastTbRef	=	m_pLastTbRef;
			m_pLastTbRef			=	NULL;

			return pLastTbRef;
		}


		// 
		inline void SetTextureFill (const std::wstring& file)
		{
			m_strTextureFile	=	file;
		}

	private:

		template<class T> CShapeRun BuildOdbflRun (const T& oXmlShape, CMapShape& oInnerRef);
		template<class T> COArtShape* InternalBuildOArtShape (const T& oXmlShape, CMapShape& oInnerRef);
		//
		inline CMapShape GetRefShape (const OOX::Logic::Pict& oPicture)	//	DOCX может содержать ссылки на дубликаты shapetype
		{
			const OOX::Logic::Shape& shape	= oPicture.shape;

			map <string, CMapShape>::iterator id = m_oMapShapes.find (shape.Type);
			if (id != m_oMapShapes.end())
				return id->second;

			CMapShape oMapShape;
			if (oPicture.shapetype.is_init())
			{
				const OOX::Logic::ShapeType& shapetype = *oPicture.shapetype;		

				oMapShape.m_strPath					=	shapetype.path;
				oMapShape.m_strCoordSize			=	shapetype.coordsize;				
				if (shapetype.adj.is_init())
					oMapShape.m_strAdjustValues		=	shapetype.adj;
				if (shapetype.formulas.is_init())
					oMapShape.m_strFormulas			=	shapetype.formulas->toXML().ToString();

				if (shapetype.PathElement.is_init())
				{
					const OOX::Logic::Path& path	=	shapetype.PathElement;

					if (path.ConnectType.is_init())
						oMapShape.m_strConnection	=	path.ConnectType;					
					if (path.ConnectLocs.is_init())
						oMapShape.m_strSites		=	path.ConnectLocs;
					if (path.ConnectAngles.is_init())
						oMapShape.m_strSitesDir		=	path.ConnectAngles;
					if (path.TextBoxRect.is_init())
						oMapShape.m_textboxrect		=	path.TextBoxRect;
				}
			}

			if (shape.path.is_init())
				oMapShape.m_strPath			=	shape.path;
			if (shape.coordsize.is_init())
				oMapShape.m_strCoordSize	=	shape.coordsize;
			if (shape.adj.is_init())
				oMapShape.m_strAdjustValues	=	shape.adj;

			if (oPicture.shapetype.is_init())
			{
				int nType = (*oPicture.shapetype).Spt;
				std::string strID = std::string("#"); strID += oPicture.shapetype->Id;

				oMapShape.m_ID		=	strID;
				oMapShape.m_nType	=	nType;

				if (m_oMapShapes.find (strID) == m_oMapShapes.end())
					m_oMapShapes [strID] = oMapShape;
			}

			return oMapShape;
		}

		inline CMapShape GetRefShape (const OOX::Logic::Shape& oShape, const OOX::Logic::Group& oXmlGroup)	//	DOCX может содержать ссылки на дубликаты shapetype
		{
			map <string, CMapShape>::iterator id = m_oMapShapes.find (oShape.Type);
			if (id != m_oMapShapes.end())
				return id->second;

			CMapShape oMapShape;

			for (size_t i = 0; i < oXmlGroup.items->size(); ++i)
			{
				const OOX::Logic::GroupItem& oXmlItem = oXmlGroup.items->operator[](i);
				if (oXmlItem.is<OOX::Logic::ShapeType>())
				{
					const OOX::Logic::ShapeType& shapetype = oXmlItem.as<OOX::Logic::ShapeType>();

					std::string strID	=	std::string("#"); strID += shapetype.Id;

					if (strID == oShape.Type)
					{
						oMapShape.m_strPath				=	shapetype.path;
						oMapShape.m_strCoordSize		=	shapetype.coordsize;				
						if (shapetype.adj.is_init())
							oMapShape.m_strAdjustValues	=	shapetype.adj;
						if (shapetype.formulas.is_init())
							oMapShape.m_strFormulas		=	shapetype.formulas->toXML().ToString();

						if (shapetype.PathElement.is_init())
						{
							const OOX::Logic::Path& oPath = shapetype.PathElement;

							if (oPath.ConnectType.is_init())
								oMapShape.m_strConnection = oPath.ConnectType;					
							if (oPath.ConnectLocs.is_init())
								oMapShape.m_strSites = oPath.ConnectLocs;
							if (oPath.ConnectAngles.is_init())
								oMapShape.m_strSitesDir = oPath.ConnectAngles;
							if (oPath.TextBoxRect.is_init())
								oMapShape.m_textboxrect	= oPath.TextBoxRect;
						}

						if (oShape.path.is_init())
							oMapShape.m_strPath	= oShape.path;
						if (oShape.coordsize.is_init())
							oMapShape.m_strCoordSize = oShape.coordsize;
						if (oShape.adj.is_init())
							oMapShape.m_strAdjustValues	= oShape.adj;

						oMapShape.m_ID		=	strID;
						oMapShape.m_nType	=	shapetype.Spt;

						if (m_oMapShapes.find (strID) == m_oMapShapes.end())
							m_oMapShapes [strID] = oMapShape;

						return oMapShape;
					}
				}
			}

			if (oShape.path.is_init())
				oMapShape.m_strPath	= oShape.path;
			if (oShape.coordsize.is_init())
				oMapShape.m_strCoordSize = oShape.coordsize;
			if (oShape.adj.is_init())
				oMapShape.m_strAdjustValues	= oShape.adj;

			return oMapShape;
		}

		//
		template<class T> void SetupTextureShape (const T& oXmlShape, CMapShape& oInnerRef, COArtImage* pShape)
		{
			if (pShape)
			{
				int nType	=	oInnerRef.m_nType;
				if (nType == OfficeArt::Enumerations::msosptTextStop)		//	пока не пон€тно что делать с такими фигурами
					nType	=	0;

				pShape->SetShapeType (nType);

				if (pShape->GetSettings ())
				{
					// fill

					if (oXmlShape.fillstyle.is_init())
						pShape->GetSettings ()->GetFillStyle().Read (oXmlShape.fillstyle);

					// line

					if (oXmlShape.linestyle.is_init())
						pShape->GetSettings ()->GetLineStyle().Read (oXmlShape.linestyle);

					// shadow

					if (oXmlShape.shadow.is_init())
						pShape->GetSettings ()->GetShadowStyle().Read (oXmlShape.shadow);

					pShape->SetupFromStyleXml(oXmlShape.style);

					if (0 == nType)
					{
						if (oInnerRef.m_strPath.length())
							pShape->GetSettings()->GetGeometryStyle().SetPath(oInnerRef.m_strPath, oInnerRef.m_strAdjustValues, oInnerRef.m_strFormulas);

						if (oInnerRef.m_strCoordSize.length())
							pShape->GetSettings()->GetGeometryStyle().SetRightBottom(oInnerRef.m_strCoordSize);

						if (oInnerRef.m_strConnection.length())
							pShape->GetSettings()->GetGeometryStyle().SetConnection(oInnerRef.m_strConnection);

						if (oInnerRef.m_strSites.length())
							pShape->GetSettings()->GetGeometryStyle().SetSites(oInnerRef.m_strSites);

						if (oInnerRef.m_strSitesDir.length())
							pShape->GetSettings()->GetGeometryStyle().SetSitesDir(oInnerRef.m_strSitesDir);
						
						if (oInnerRef.m_textboxrect.length())
							pShape->GetSettings()->GetGeometryStyle().SetInscribe(oInnerRef.m_textboxrect);
					}

					if (oInnerRef.m_strAdjustValues.length())
						pShape->GetSettings()->GetGeometryStyle().SetAdjustValues (oInnerRef.m_strAdjustValues);

					if (typeid(T) == typeid(OOX::Logic::Roundrect))		//	дл€ DOC файла велична арки пишетс€ в adjust свойство
					{
						OOX::Logic::Roundrect* roundrect = (OOX::Logic::Roundrect*)(&oXmlShape);
						if (roundrect)
						{
							if(roundrect->arcsize.is_init())
							{
								pShape->GetSettings()->GetGeometryStyle().SetAdjustValues (roundrect->arcsize, true);
							}
						}
					}

					pShape->GetSettings ()->SetWrapDist (oXmlShape.style);
				}				
			}
		}

		//
		//inline void UpdateAnchorSpa(const OOX::Logic::ShapeStyle& style)
		//{
		//
		//}

	private:

		int									m_nLocation;
		std::map <std::string, CMapShape>	m_oMapShapes;		// < id="_x0000_t13" // o:spt="13" >	- v:shapetype - reference for all same shapes
		CTextBoxRef*						m_pLastTbRef;


		std::wstring	m_strTextureFile;
	};
}