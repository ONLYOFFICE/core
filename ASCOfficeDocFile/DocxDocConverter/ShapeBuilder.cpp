#include "stdafx.h"

#include "ShapeBuilder.h"

namespace AVSDocFileFormat
{
	BOOL COArtBuilder::BuildShapeRun (const OOX::Logic::Pict& oPicture, CShapeRun& oOdbflRun) 
	{
		if (oPicture.rect.is_init())
		{
			oOdbflRun		=	BuildOdbflRun <OOX::Logic::Rect> (oPicture.rect, CMapShape(OfficeArt::Enumerations::msosptRectangle));
			return TRUE;
		}

		if (oPicture.oval.is_init())
		{
			oOdbflRun		=	BuildOdbflRun <OOX::Logic::Oval> (oPicture.oval, CMapShape(OfficeArt::Enumerations::msosptEllipse));
			return TRUE;
		}

		if (oPicture.roundrect.is_init())
		{
			oOdbflRun		=	BuildOdbflRun <OOX::Logic::Roundrect> (oPicture.roundrect, CMapShape(OfficeArt::Enumerations::msosptRoundRectangle));
			return TRUE;
		}

		if (oPicture.line.is_init())
		{
			oOdbflRun		=	BuildOdbflRun <OOX::Logic::Line> (oPicture.line, CMapShape(OfficeArt::Enumerations::msosptLine));
			return TRUE;
		}

		if (oPicture.shape.is_init())
		{
			if (oPicture.shape->imageData.is_init())
				return FALSE;

			oOdbflRun		=	BuildOdbflRun <OOX::Logic::Shape> (oPicture.shape, GetRefShape(oPicture));
			return TRUE;
		}

		return FALSE;
	}

	BOOL COArtBuilder::BuildImageRun (const OOX::Logic::Shape& oXml, const std::wstring& strFileName, Run& oRun)
	{
		COArtStorage* pStorage	=	COArtStorage::Instance();
		if (pStorage)
		{
			COArtImage* pImage	=	new COArtImage(strFileName, pStorage->GenID (m_nLocation));
			if (pImage)
			{
				CShapeRun oShapeRun (pImage, pImage->GetID(), pStorage->GetOffSetSpa(m_nLocation));	//	смещение берем от предыдущего элемента
				oShapeRun.UpdateSizes	(*oXml.style);																		//	размеры
				oShapeRun.SetWrap		(oXml.Wrap);																		//	обтекание
				//oShapeRun.SetAnchor	((BOOL)(oXml.anchorlock.is_init()));
				oShapeRun.SetUseBehind	(*oXml.style);													

				pImage->SetupFromStyleXml(oXml.style);

				pStorage->AddImage (pImage, oShapeRun.GetSpa(), m_nLocation);						

				oRun.AddRunItem (oShapeRun);					

				return TRUE;
			}
		}

		return FALSE;
	}
	BOOL COArtBuilder::BuildImageRun (const OOX::Image& oXml, const OOX::Logic::Drawing& oXml2, Run& oRun)
	{
		COArtStorage* pStorage		=	COArtStorage::Instance();
		if (pStorage)
		{
			COArtImage* pImage	=	new COArtImage(oXml.filename().string(), pStorage->GenID (m_nLocation));
			if (pImage)
			{
				CShapeRun oShapeRun (pImage, pImage->GetID(), pStorage->GetOffSetSpa(m_nLocation));	//	смещение берем от предыдущего элемента
				((CImageSettings*)(pImage->GetSettings()))->SetWrapDist(oXml2.Inline);				

				pImage->SetRotationImage(oXml2);
				pImage->SetInternalFlipImage(oXml2);
				pImage->SetPositioningImage(oXml2);

				oShapeRun.SetImageSize		(oXml2);
				oShapeRun.SetImageWrap		(oXml2.Inline->Wrap);											
				oShapeRun.SetImageUseBehind (oXml2.Inline->BehindDoc);						
				oShapeRun.UpdateAnchorPositionImage(oXml2);

				pStorage->AddImage (pImage, oShapeRun.GetSpa(), m_nLocation);						

				oRun.AddRunItem (oShapeRun);					

				return TRUE;
			}
		}

		return FALSE;
	}
	CShapeRun COArtBuilder::BuildGroupRun (const OOX::Logic::Group& oXml, COArtGroup* pShape)
	{		
		if (pShape)
		{
			COArtStorage* pStorage		=	COArtStorage::Instance();

			CShapeRun oShapeRun (pShape, pShape->GetID(), pStorage->GetOffSetSpa(m_nLocation));					
			oShapeRun.UpdateSizes		(*oXml.style);																	
			//oShapeRun.SetWrap			(oXmlShape.Wrap);																	
			//oShapeRun.SetAnchor		((BOOL)(oXmlShape.anchorlock.is_init()));
			//oShapeRun.SetUseBehind	(*oXml.style);	

			if (FALSE == oShapeRun.IsInline())
				pStorage->AddGroup (pShape, oShapeRun.GetSpa(), m_nLocation);						

			return oShapeRun;
		}

		return CShapeRun();
	}
}

namespace AVSDocFileFormat
{	
	COArtShape* COArtBuilder::BuildOArtShape (const OOX::Logic::Shape& oXml, const OOX::Logic::Group& oXmlGroup)
	{
		COArtShape* pShape	=	InternalBuildOArtShape <OOX::Logic::Shape> (oXml,GetRefShape(oXml,oXmlGroup));
		if (pShape)
		{
			const OOX::Logic::ShapeStyle& oStyle	=	(*oXml.style);
			pShape->SetupFromStyleXml(oStyle);

			Unit<int, Pt> nX (0);
			Unit<int, Pt> nY (0);			

			if (oStyle.leftTop.is_init())
			{
				nX = *oStyle.leftTop->X;
				nY = *oStyle.leftTop->Y;
			}

			Unit<int, Pt> nWidth (*oStyle.Size->Width);
			Unit<int, Pt> nHeight (*oStyle.Size->Height);

			pShape->SetChildeAnchorBounds (nX, nY, nWidth, nHeight, pShape->IsAngleBoundFlip());

			if (pShape->IsTextureMode())
			{
				COArtStorage* storage = COArtStorage::Instance();
				if (storage)
				{
					storage->SaveBlipImage(pShape);
				}
			}
		}			

		return pShape;
	}

	COArtShape* COArtBuilder::BuildOArtImage (const OOX::Logic::Shape& oXml, const std::wstring& strFileName)
	{
		if (0 == strFileName.length())
			return NULL;

		COArtStorage* storage	=	COArtStorage::Instance();
		if (storage)
		{
			COArtImage* pImage	=	new COArtImage(strFileName, storage->GenID (m_nLocation));
			if (pImage)
			{
				const OOX::Logic::ShapeStyle& oStyle = (*oXml.style);

				pImage->SetRotation(oStyle);
				pImage->SetInternalFlip(oStyle);
				pImage->SetHidden(oStyle);

				Unit<int, Pt> nX (0);
				Unit<int, Pt> nY (0);			

				if (oStyle.leftTop.is_init())
				{
					nX = *oStyle.leftTop->X;
					nY = *oStyle.leftTop->Y;
				}

				Unit<int, Pt> nWidth (*oStyle.Size->Width);
				Unit<int, Pt> nHeight (*oStyle.Size->Height);

				pImage->SetChildeAnchorBounds (nX, nY, nWidth, nHeight, pImage->IsAngleBoundFlip());

				storage->SaveBlipImage(pImage);

				return pImage;
			}
		}

		return NULL;
	}
}

namespace AVSDocFileFormat
{
	template<class T> CShapeRun	COArtBuilder::BuildOdbflRun (const T& oXmlShape, CMapShape& oInnerRef)
	{		
		COArtShape* pShape	=	InternalBuildOArtShape <T> (oXmlShape,oInnerRef);
		if (pShape)
		{
			COArtStorage* pStorage	=	COArtStorage::Instance();

			CShapeRun oShapeRun (pShape, pShape->GetID(), pStorage->GetOffSetSpa(m_nLocation));	//	смещение берем от предыдущего элемента
			oShapeRun.UpdateSizes	(*oXmlShape.style);																	//	размеры
			oShapeRun.SetWrap		(oXmlShape.Wrap);																	//	обтекание
			oShapeRun.SetAnchor		((BOOL)(oXmlShape.anchorlock.is_init()));
			oShapeRun.SetUseBehind	(*oXmlShape.style);	
			oShapeRun.UpdateAnchorPosition (*oXmlShape.style);

			if (typeid(T) == typeid(OOX::Logic::Line))	//	для DOC файла точки начала и конца линии устанавливаются в структуре SPA
			{
				OOX::Logic::Line* pLine = (OOX::Logic::Line*)(&oXmlShape);
				if (pLine)
				{
					if (pLine->from.is_init() && pLine->to.is_init())
					{
						DOCX::CPointF oFrom(pLine->from);
						DOCX::CPointF oTo(pLine->to);

						Spa& oSpa			=	oShapeRun.GetSpa();

						oSpa.m_rca.left		=	oFrom.GetTX();
						oSpa.m_rca.top		=	oFrom.GetTY();
						oSpa.m_rca.right	=	oTo.GetTX();
						oSpa.m_rca.bottom	=	oTo.GetTY();

						oSpa.Update();
					}
				}
			}

			if (oXmlShape.textbox.is_init())		// Привязка текста к автофигуры 
			{
				m_pLastTbRef = pStorage->GenTbRef(m_nLocation);
				if (m_pLastTbRef)
				{
					pShape->SetTbRef(m_pLastTbRef);
				}
			}

			if (FALSE == oShapeRun.IsInline())
			{
				pStorage->Add (pShape, oShapeRun.GetSpa(), m_nLocation);
			}

			return oShapeRun;
		}

		return CShapeRun();
	}

	template<class T> COArtShape* COArtBuilder::InternalBuildOArtShape (const T& oXmlShape, CMapShape& oInnerRef)
	{
		COArtStorage* pStorage = COArtStorage::Instance();
		if (pStorage)
		{
			COArtShape* pShape = new COArtShape(pStorage->GenID (m_nLocation));
			if (pShape)
			{
				int nType	=	oInnerRef.m_nType;
				if (nType == OfficeArt::Enumerations::msosptTextStop)		//	пока не понятно что делать с такими фигурами
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
						pShape->GetSettings()->GetGeometryStyle().SetAdjustValues(oInnerRef.m_strAdjustValues);

					if (typeid(T) == typeid(OOX::Logic::Roundrect))		//	для DOC файла велична арки пишется в adjust свойство
					{
						OOX::Logic::Roundrect* roundrect = (OOX::Logic::Roundrect*)(&oXmlShape);
						if (roundrect)
						{
							if(roundrect->arcsize.is_init())
							{
								pShape->GetSettings()->GetGeometryStyle().SetAdjustValues(roundrect->arcsize, true);
							}
						}
					}

					pShape->GetSettings()->SetWrapDist (oXmlShape.style);
				}

				// имеет место быть заливка картинкой

				if (m_strTextureFile.length())
				{
					pShape->SetTextureFill(TRUE, m_strTextureFile);
					m_strTextureFile = L"";
				}

				return pShape;					
			}
		}

		return NULL;
	}
}