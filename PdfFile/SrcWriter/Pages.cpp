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
#include "Objects.h"
#include "Pages.h"
#include "Utils.h"
#include "GState.h"
#include "Streams.h"
#include "Annotation.h"
#include "Font.h"
#include "Image.h"
#include "Shading.h"
#include "Pattern.h"
#include "Document.h"
#include "Field.h"

#ifdef DrawText
#undef DrawText
#endif

#define MIN_HORIZONTALSCALING  0.01
#define MAX_HORIZONTALSCALING  1000
#define MIN_CHARSPACE          -30
#define MAX_CHARSPACE          300
#define MAX_FONTSIZE           1000

#define STR_BUF 200

namespace PdfWriter
{
	static const double c_dKappa = 0.552;
	static void QuarterEllipseA(CStream* pStream, double dX, double dY, double dXRad, double dYRad)
	{
		pStream->WriteReal(dX - dXRad);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY + dYRad * c_dKappa);
		pStream->WriteChar(' ');
		pStream->WriteReal(dX - dXRad * c_dKappa);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY + dYRad);
		pStream->WriteChar(' ');
		pStream->WriteReal(dX);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY + dYRad);
		pStream->WriteStr(" c\012");
	}
	static void QuarterEllipseB(CStream* pStream, double dX, double dY, double dXRad, double dYRad)
	{
		pStream->WriteReal(dX + dXRad * c_dKappa);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY + dYRad);
		pStream->WriteChar(' ');
		pStream->WriteReal(dX + dXRad);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY + dYRad * c_dKappa);
		pStream->WriteChar(' ');
		pStream->WriteReal(dX + dXRad);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY);
		pStream->WriteStr(" c\012");
	}
	static void QuarterEllipseC(CStream* pStream, double dX, double dY, double dXRad, double dYRad)
	{
		pStream->WriteReal(dX + dXRad);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY - dYRad * c_dKappa);
		pStream->WriteChar(' ');
		pStream->WriteReal(dX + dXRad * c_dKappa);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY - dYRad);
		pStream->WriteChar(' ');
		pStream->WriteReal(dX);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY - dYRad);
		pStream->WriteStr(" c\012");
	}
	static void QuarterEllipseD(CStream* pStream, double dX, double dY, double dXRad, double dYRad)
	{
		pStream->WriteReal(dX - dXRad * c_dKappa);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY - dYRad);
		pStream->WriteChar(' ');
		pStream->WriteReal(dX - dXRad);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY - dYRad * c_dKappa);
		pStream->WriteChar(' ');
		pStream->WriteReal(dX - dXRad);
		pStream->WriteChar(' ');
		pStream->WriteReal(dY);
		pStream->WriteStr(" c\012");
	}
	static double AngToEllPrm(double dAngle, double dXRad, double dYRad)
	{
		// Функция для перевода реального угла в параметрическое задание эллписа
		// т.е. x= a cos(t) y = b sin(t) - параметрическое задание эллписа.
		// x = r cos(p), y = r sin(p) => t = atan2( sin(p) / b, cos(p) / a );
		return atan2(sin(dAngle) / dYRad, cos(dAngle) / dXRad);
	}
	static void WriteEllipseArc(CStream* pStream, double dX, double dY, double dXRad, double dYRad, double dAngle1, double dAngle2, double& dXCur, double& dYCur, bool bClockDirection = false)
	{
		// Рассчитаем начальную, конечную и контрольные точки
		double dX1  = 0.0, dX2  = 0.0, dY1  = 0.0, dY2  = 0.0;
		double dCX1 = 0.0, dCX2 = 0.0, dCY1 = 0.0, dCY2 = 0.0;
	
		double dAlpha = sin(dAngle2 - dAngle1) * (sqrt(4.0 + 3.0 * tan((dAngle2 - dAngle1) / 2.0) * tan((dAngle2 - dAngle1) / 2.0)) - 1.0) / 3.0;
	
		dX1 = dX + dXRad * cos(dAngle1);
		dY1 = dY + dYRad * sin(dAngle1);
	
		dX2 = dX + dXRad * cos(dAngle2);
		dY2 = dY + dYRad * sin(dAngle2);
	
		dCX1 = dX1 - dAlpha * dXRad * sin(dAngle1);
		dCY1 = dY1 + dAlpha * dYRad * cos(dAngle1);
	
		dCX2 = dX2 + dAlpha * dXRad * sin(dAngle2);
		dCY2 = dY2 - dAlpha * dYRad * cos(dAngle2);
	
		if ( !bClockDirection )
		{
			pStream->WriteReal(dCX1);
			pStream->WriteChar(' ');
			pStream->WriteReal(dCY1);
			pStream->WriteChar(' ');
			pStream->WriteReal(dCX2);
			pStream->WriteChar(' ');
			pStream->WriteReal(dCY2);
			pStream->WriteChar(' ');
			pStream->WriteReal(dX2);
			pStream->WriteChar(' ');
			pStream->WriteReal(dY2);
			dXCur = dX2; 
			dYCur = dY2;
		}
		else
		{
			pStream->WriteReal(dCX2);
			pStream->WriteChar(' ');
			pStream->WriteReal(dCY2);
			pStream->WriteChar(' ');
			pStream->WriteReal(dCX1);
			pStream->WriteChar(' ');
			pStream->WriteReal(dCY1);
			pStream->WriteChar(' ');
			pStream->WriteReal(dX1);
			pStream->WriteChar(' ');
			pStream->WriteReal(dY1);
			dXCur = dX1; 
			dYCur = dY1;
		}
		pStream->WriteStr(" c\012");
	}
	//----------------------------------------------------------------------------------------
	// CPageTree
	//----------------------------------------------------------------------------------------
	CPageTree::CPageTree(CXref* pXref)
	{
		m_pXref = pXref;
		pXref->Add(this);

		m_pPages = new CArrayObject();
		m_pCount = new CNumberObject(0);

		Add("Type", "Pages");
		Add("Kids", m_pPages);
		Add("Count", m_pCount);
	}
	CPageTree::CPageTree(CXref* pXref, bool bEmpty)
	{
		m_pXref = pXref;
		pXref->Add(this);

		m_pPages = NULL;
		m_pCount = NULL;
	}
	void CPageTree::Fix()
	{
		// Инициализация текущего m_pPages
		CObjectBase* pPages = Get("Kids");
		if (pPages && pPages->GetType() == object_type_ARRAY)
			m_pPages = (CArrayObject*)pPages;
		else
		{
			m_pPages = new CArrayObject();
			Add("Kids", m_pPages);
		}

		// Инициализация текущего m_pCount
		CObjectBase* pCount = Get("Count");
		if (pCount && pCount->GetType() == object_type_NUMBER)
			m_pCount = (CNumberObject*)pCount;
		else
		{
			m_pCount = new CNumberObject(0);
			Add("Count", m_pCount);
		}
	}
	void CPageTree::AddPage(CDictObject* pPage)
	{
		m_pPages->Add(pPage);
		(*m_pCount)++;
	}
	CObjectBase* CPageTree::GetObj(int nPageIndex)
	{
		int nI = 0;
		return GetFromPageTree(nPageIndex, nI);
	}
	CPage* CPageTree::GetPage(int nPageIndex)
	{
		int nI = 0;
		CObjectBase* pObj = GetFromPageTree(nPageIndex, nI);
		if (pObj && pObj->GetType() == object_type_DICT && ((CDictObject*)pObj)->GetDictType() == dict_type_PAGE)
			return (CPage*)pObj;
		return NULL;
	}
	CObjectBase* CPageTree::RemovePage(int nPageIndex)
	{
		int nI = 0;
		return GetFromPageTree(nPageIndex, nI, true);
	}
	bool CPageTree::InsertPage(int nPageIndex, CPage* pPage)
	{
		if (nPageIndex >= m_pCount->Get())
		{
			AddPage(pPage);
			pPage->Add("Parent", this);
			return true;
		}
		int nI = 0;
		CObjectBase* pObj = GetFromPageTree(nPageIndex, nI, false, true, pPage);
		if (pObj)
			return true;
		return false;
	}
	CObjectBase* CPageTree::GetFromPageTree(int nPageIndex, int& nI, bool bRemove, bool bInsert, CPage* pPage)
	{
		for (int i = 0, count = m_pPages->GetCount(); i < count; ++i)
		{
			CObjectBase* pObj = m_pPages->Get(i);
			CObjectBase* pRes = NULL;
			if (pObj->GetType() == object_type_DICT && ((CDictObject*)pObj)->GetDictType() == dict_type_PAGES)
				pRes = ((CPageTree*)pObj)->GetFromPageTree(nPageIndex, nI, bRemove, bInsert, pPage);
			else
			{
				if (nPageIndex == nI)
				{
					pRes = pObj;
					if (bRemove)
						pRes = m_pPages->Remove(i);
					if (bInsert)
					{
						m_pPages->Insert(pObj, pPage);
						pPage->Add("Parent", this);
					}
				}
				nI++;
			}
			if (pRes)
			{
				if (bRemove)
					(*m_pCount)--;
				if (bInsert)
					(*m_pCount)++;
				return pRes;
			}
		}
		return NULL;
	}
	bool CPageTree::Join(CPageTree* pPageTree)
	{
		unsigned int nObjId = pPageTree->GetObjId();
		unsigned int nGenNo = pPageTree->GetGenNo();
		for (int i = 0, count = m_pPages->GetCount(); i < count; ++i)
		{
			CObjectBase* pObj = m_pPages->Get(i);
			if (pObj->GetObjId() == nObjId && pObj->GetGenNo() == nGenNo)
			{
				m_pPages->Insert(pObj, pPageTree, true);
				return true;
			}
			if (pObj->GetType() == object_type_DICT && ((CDictObject*)pObj)->GetDictType() == dict_type_PAGES && ((CPageTree*)pObj)->Join(pPageTree))
				return true;
		}
		return false;
	}
	//----------------------------------------------------------------------------------------
	// CPage
	//----------------------------------------------------------------------------------------
	CPage::CPage(CXref* pXref, CDocument* pDocument)
	{
		Init(pXref, pDocument);
	}
	void CPage::Fix()
	{
		// Инициализация текущего contents
		CObjectBase* pContents = Get("Contents");
		if (pContents)
		{
			if (pContents->GetType() == object_type_ARRAY)
				m_pContents = (CArrayObject*)pContents;
			else if (pContents->GetType() == object_type_UNKNOWN)
			{
				CProxyObject* pNewContents = new CProxyObject(pContents->Copy(), true);
				pNewContents->Get()->SetRef(pContents->GetObjId(), pContents->GetGenNo());
				m_pContents = new CArrayObject();
				m_pContents->Add(pNewContents);
				Add("Contents", m_pContents);
			}
		}
		else
		{
			m_pContents = new CArrayObject();
			Add("Contents", m_pContents);
		}

		// Инициализация текущего MediaBox
		CObjectBase* pMediaBox = Get("MediaBox");
		if (pMediaBox && pMediaBox->GetType() == object_type_ARRAY)
		{
			double dL = 0.0, dB = 0.0, dR = DEF_PAGE_WIDTH, dT = DEF_PAGE_HEIGHT;

			CObjectBase* val = ((CArrayObject*)pMediaBox)->Get(0);
			if (val && val->GetType() == object_type_NUMBER)
				dL = ((CNumberObject*)val)->Get();
			else if (val && val->GetType() == object_type_REAL)
				dL = ((CRealObject*)val)->Get();

			val = ((CArrayObject*)pMediaBox)->Get(1);
			if (val && val->GetType() == object_type_NUMBER)
				dB = ((CNumberObject*)val)->Get();
			else if (val && val->GetType() == object_type_REAL)
				dB = ((CRealObject*)val)->Get();

			val = ((CArrayObject*)pMediaBox)->Get(2);
			if (val && val->GetType() == object_type_NUMBER)
				dR = ((CNumberObject*)val)->Get();
			else if (val && val->GetType() == object_type_REAL)
				dR = ((CRealObject*)val)->Get();

			val = ((CArrayObject*)pMediaBox)->Get(3);
			if (val && val->GetType() == object_type_NUMBER)
				dT = ((CNumberObject*)val)->Get();
			else if (val && val->GetType() == object_type_REAL)
				dT = ((CRealObject*)val)->Get();

			Add("MediaBox", CArrayObject::CreateBox(dL, dB, dR, dT));
		}
		else
			Add("MediaBox", CArrayObject::CreateBox(0, 0, DEF_PAGE_WIDTH, DEF_PAGE_HEIGHT));

		// Инициализация текущего Rotate
		CObjectBase* pRotate = GetRotateItem();
		if (pRotate && pRotate->GetType() == object_type_NUMBER)
			Add("Rotate", ((CNumberObject*)pRotate)->Get() % 360);

		CDictObject* pResources = GetResourcesItem();
		if (pResources)
		{
			// Инициализация текущего fonts
			CObjectBase* pFonts = pResources->Get("Font");
			if (pFonts && pFonts->GetType() == object_type_DICT)
			{
				m_pFonts = (CDictObject*)pFonts;
				m_unFontsCount = 0;
			}

			// Инициализация текущего ExtGStates
			CObjectBase* pExtGStates = pResources->Get("ExtGState");
			if (pExtGStates && pExtGStates->GetType() == object_type_DICT)
			{
				m_pExtGStates = (CDictObject*)pExtGStates;
				m_unExtGStatesCount = m_pExtGStates->GetSize();
			}

			// Инициализация текущего XObject
			CObjectBase* pXObject = pResources->Get("XObject");
			if (pXObject && pXObject->GetType() == object_type_DICT)
			{
				m_pXObjects = (CDictObject*)pXObject;
				m_unXObjectsCount = m_pXObjects->GetSize();
			}

			// Инициализация текущего Shading
			CObjectBase* pShading = pResources->Get("Shading");
			if (pShading && pShading->GetType() == object_type_DICT)
			{
				m_pShadings = (CDictObject*)pShading;
				m_unShadingsCount = m_pShadings->GetSize();
			}

			// Инициализация текущего Pattern
			CObjectBase* pPattern = pResources->Get("Pattern");
			if (pPattern && pPattern->GetType() == object_type_DICT)
			{
				m_pPatterns = (CDictObject*)pPattern;
				m_unPatternsCount = m_pPatterns->GetSize();
			}
		}
		else
			Add("Resources", new CDictObject());

		m_pStream = NULL;
	}
	CPage::CPage(CXref* pXref, CPageTree* pParent, CDocument* pDocument)
	{
		Init(pXref, pDocument);

		m_pContents = new CArrayObject();
		CDictObject* pContent = new CDictObject(pXref);
		m_pContents->Add(pContent);
		m_pStream = pContent->GetStream();

		Add("Parent", pParent);
		Add("MediaBox", CArrayObject::CreateBox(0, 0, DEF_PAGE_WIDTH, DEF_PAGE_HEIGHT));
		Add("Type", "Page");
		Add("Contents", m_pContents);
		AddResource();
	}
	CPage::~CPage()
	{
		CGrState* pGrState = m_pGrState, *pPrev = NULL;
		while (pGrState)		
		{
			pPrev = m_pGrState->GetPrev();
			delete pGrState;
			pGrState = pPrev;
		}
	}
	void CPage::Init(CXref* pXref, CDocument* pDocument)
	{
		pXref->Add(this);

		m_pXref     = pXref;
		m_pDocument = pDocument;
		m_eGrMode   = grmode_PAGE;
		m_pGrState  = new CGrState(NULL);

		m_pExtGStates       = NULL;
		m_unExtGStatesCount = 0;
		m_pFonts            = NULL;
		m_pFont             = NULL;
		m_unFontsCount      = 0;
		m_pXObjects         = NULL;
		m_unXObjectsCount   = 0;
		m_pShadings         = NULL;
		m_unShadingsCount   = 0;
		m_pPatterns         = NULL;
		m_unPatternsCount   = 0;
	}
    void CPage::SetWidth(double dValue)
	{
        dValue = std::min(std::max(dValue, 1.0), 14400.0);
		SetMediaBoxValue(2, dValue);
	}
	double        CPage::GetWidth()
	{
        TBox oBox = GetMediaBox();
        return oBox.fRight - oBox.fLeft;
	}
    void CPage::SetHeight(double dValue)
	{
        dValue = std::min(std::max(dValue, 1.0), 14400.0);
		SetMediaBoxValue(3, dValue);
	}
	double        CPage::GetHeight()
	{
        TBox oBox = GetMediaBox();
        return oBox.fTop - oBox.fBottom;
	}
	TBox          CPage::GetMediaBox()
	{
		TBox oMediaBox = TRect( 0, 0, 0, 0 );

		CArrayObject* pArray = GetMediaBoxItem();

		if (pArray)
		{
			CRealObject* pReal;

			pReal = (CRealObject*)pArray->Get(0);
			if (pReal)
				oMediaBox.fLeft = pReal->Get();

			pReal = (CRealObject*)pArray->Get(1);
			if (pReal)
				oMediaBox.fBottom = pReal->Get();

			pReal = (CRealObject*)pArray->Get(2);
			if (pReal)
				oMediaBox.fRight = pReal->Get();

			pReal = (CRealObject*)pArray->Get(3);
			if (pReal)
				oMediaBox.fTop = pReal->Get();
		}

		return oMediaBox;
	}
    void CPage::SetMediaBoxValue(unsigned int unIndex, double dValue)
	{
		CArrayObject* pArray = GetMediaBoxItem();
		if (!pArray)
			return;

		CRealObject* pReal = (CRealObject*)pArray->Get(unIndex);
		if (!pReal)
			return;

		pReal->Set(dValue);
	}
	CArrayObject* CPage::GetMediaBoxItem()
	{
		return (CArrayObject*)Get("MediaBox");
	}
	CDictObject*  CPage::GetResourcesItem()
	{
		CObjectBase* pObject = Get("Resources");

		// Если объект Resources нулевой, тогда ищем Resources у родительского объекта рекурсивно
		if (!pObject)
		{
			CPageTree* pPageTree = (CPageTree*)Get("Parent");
			while (pPageTree)
			{
				pObject = Get("Resources");

				if (pObject)
					break;

				pPageTree = (CPageTree*)pPageTree->Get("Parent");
			}
		}

		return (CDictObject*)pObject;
	}
	CObjectBase*  CPage::GetCropBoxItem()
	{
		return Get("CropBox");
	}
	CObjectBase*  CPage::GetRotateItem()
	{
		return Get("Rotate");
	}
    void CPage::AddResource()
	{
		// TODO: Переделать на ResourcesDict
		CDictObject* pResource = new CDictObject();
		if (!pResource)
			return;
	
	    // Не смотря на то, что ProcSet - устаревший объект, добавляем
	    // его для совместимости	
	    Add("Resources", pResource);
	
		CArrayObject* pProcset = new CArrayObject();
		if (!pProcset)
			return;	
	
		pResource->Add("ProcSet", pProcset);
		pProcset->Add(new CNameObject("PDF"));
		pProcset->Add(new CNameObject("Text"));
		pProcset->Add(new CNameObject("ImageB"));
		pProcset->Add(new CNameObject("ImageC"));
		pProcset->Add(new CNameObject("ImageI"));
	}
    void CPage::BeforeWrite()
	{
		if (grmode_PATH == m_eGrMode)
			EndPath();

		if (grmode_TEXT == m_eGrMode)
			EndText();

		while (m_pGrState->GetPrev())
		{
			GrRestore();
		}
	}
    void CPage::SetGrMode(EGrMode eMode)
	{
		m_eGrMode = eMode;
		// TODO: Сделать проверку плохих ситуаций
	}
    void CPage::CheckGrMode(EGrMode eMode)
	{
		// TODO: Сделать проверку плохих ситуаций
	}
    void CPage::MoveTo (double dX, double dY)
	{
		// Operator   : m
		// Description: Начинаем новый subpath, передвигая текущий указатель в точку (x, y)(она же стартовая). 

		SetGrMode(grmode_PATH);
		m_pStream->WriteReal(dX);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY);
		m_pStream->WriteStr(" m\012");

		m_oCurPos.Set(dX, dY);
		m_oStartPos = m_oCurPos;
	}
    void CPage::LineTo (double dX, double dY)
	{
		// Operator   : l
		// Description: Добавляем линию от текущей точки до точки (x, y). Текущую точку выставляем (х, у).
		CheckGrMode(grmode_PATH);

		m_pStream->WriteReal(dX);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY);
		m_pStream->WriteStr(" l\012");

		m_oCurPos.Set(dX, dY);
	}
    void CPage::CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3)
	{
		// Operator   : c
		// Description: Добавляем кривую Безье(кубическую). Начинается кривая в текущей позиции, заканчивается
		//              в точке (x3, y3). (x1, y1) и (x2, y2) - контрольные точки. Текущую точку устанавливаем
		//              в (х3, у3).
		CheckGrMode(grmode_PATH);

		m_pStream->WriteReal(dX1);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY1);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dX2);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY2);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dX3);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY3);
		m_pStream->WriteStr(" c\012");

		m_oCurPos.Set(dX3, dY3);
	}
    void CPage::Ellipse(double dX, double dY, double dXRay, double dYRay)
	{
		SetGrMode(grmode_PATH);

		m_pStream->WriteReal(dX - dXRay);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY);
		m_pStream->WriteStr(" m\012");    
	
	    QuarterEllipseA(m_pStream, dX, dY, dXRay, dYRay);
	    QuarterEllipseB(m_pStream, dX, dY, dXRay, dYRay);
	    QuarterEllipseC(m_pStream, dX, dY, dXRay, dYRay);
	    QuarterEllipseD(m_pStream, dX, dY, dXRay, dYRay);
	
		m_oCurPos.Set(dX - dXRay, dY);
		m_oStartPos = m_oCurPos;
	}
    void CPage::EllipseArc(double dX, double dY, double dXRad, double dYRad, double _dAngle1, double _dAngle2, bool bClockDirection)
	{
		CheckGrMode(grmode_PATH);
	
		// переведем углы в радианы	
		double dAngle1 = _dAngle1 * 3.141592f / 180;
		double dAngle2 = _dAngle2 * 3.141592f / 180;
	
		// Выясним в каких четвертях находятся начальная и конечная точки
		int nFirstPointQuard  = int(_dAngle1) / 90 + 1;
		int nSecondPointQuard = int(_dAngle2) / 90 + 1;
	
        nSecondPointQuard = std::min(4, std::max(1, nSecondPointQuard));
        nFirstPointQuard  = std::min(4, std::max(1, nFirstPointQuard));

		// Проведем линию в начальную точку дуги
		double dStartX = 0.0, dStartY = 0.0, dEndX = 0.0, dEndY = 0.0;
	
		dStartX = dX + dXRad * cos(AngToEllPrm(dAngle1, dXRad, dYRad));
		dStartY = dY + dYRad * sin(AngToEllPrm(dAngle1, dXRad, dYRad));
	
		//m_pStream->WriteReal(dStartX);
		//m_pStream->WriteChar(' ');
		//m_pStream->WriteReal(dStartY);
		//m_pStream->WriteStr(" m\012");

		// Дальше рисуем по четверям	
		double dCurX = dStartX, dCurY = dStartY;
		double dStartAngle = dAngle1;
		double dEndAngle = 0;
	
		if ( !bClockDirection )
		{
			for (unsigned int nIndex = nFirstPointQuard; nIndex <= nSecondPointQuard; nIndex++)
			{
				if (nIndex == nSecondPointQuard)
					dEndAngle = dAngle2;
				else
					dEndAngle = (90 * (nIndex)) * 3.141592f / 180;
				if (!(nIndex == nFirstPointQuard))
					dStartAngle = (90 * (nIndex - 1)) * 3.141592f / 180;

				WriteEllipseArc(m_pStream, dX, dY, dXRad, dYRad, AngToEllPrm(dStartAngle, dXRad, dYRad), AngToEllPrm(dEndAngle, dXRad, dYRad), dEndX, dEndY, false);
			}
		}
		else
		{
			for( unsigned int nIndex = nFirstPointQuard; nIndex >= nSecondPointQuard; nIndex-- ) 
			{
				if ( nIndex == nFirstPointQuard )
					dStartAngle = dAngle1;
				else
					dStartAngle = (90 * (nIndex ) ) * 3.141592f / 180;
				if ( !( nIndex == nSecondPointQuard ) )
					dEndAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;
				else
					dEndAngle = dAngle2;
	
				WriteEllipseArc(m_pStream, dX, dY, dXRad, dYRad, AngToEllPrm(dStartAngle, dXRad, dYRad), AngToEllPrm(dEndAngle, dXRad, dYRad), dEndX, dEndY, false);
			}
		}
	
		m_oCurPos.Set(dEndX, dEndY);
		m_oStartPos = m_oCurPos;
	}
    void CPage::EllipseArcTo(double dX, double dY, double dXRad, double dYRad, double _dAngle1, double _dAngle2, bool bClockDirection)
	{
		// Проверяем эллипс на невырожденность
		if (dXRad < 0.001 || dYRad < 0.001)
		{
			double dAngle1 = _dAngle1 * 3.141592f / 180;
			double dAngle2 = _dAngle2 * 3.141592f / 180;

			if (dXRad < 0.001 && dYRad < 0.001)
				LineTo(dX, dY);
			else if (dXRad < 0.001)
			{
				LineTo(dX, dY + sin(dAngle1) * dYRad);
				LineTo(dX, dY + sin(dAngle2) * dYRad);
			}
			else // if (dYRad < 0.001) 
			{
				LineTo(dX + cos(dAngle1) * dXRad, dY);
				LineTo(dX + cos(dAngle2) * dXRad, dY);
			}
			return;
		}


		while (_dAngle1 < 0)
			_dAngle1 += 360;

		while (_dAngle1 > 360)
			_dAngle1 -= 360;

		while (_dAngle2 < 0)
			_dAngle2 += 360;

		while (_dAngle2 > 360)
			_dAngle2 -= 360;

		if (!bClockDirection)
		{
			if (_dAngle1 <= _dAngle2)
				EllipseArc(dX, dY, dXRad, dYRad, _dAngle1, _dAngle2, false);
			else
			{
				EllipseArc(dX, dY, dXRad, dYRad, _dAngle1, 360, false);
				EllipseArc(dX, dY, dXRad, dYRad, 0, _dAngle2, false);
			}
		}
		else
		{
			if (_dAngle1 >= _dAngle2)
				EllipseArc(dX, dY, dXRad, dYRad, _dAngle1, _dAngle2, true);
			else
			{
				EllipseArc(dX, dY, dXRad, dYRad, _dAngle1, 0, true);
				EllipseArc(dX, dY, dXRad, dYRad, 360, _dAngle2, true);
			}
		}
	}
    void CPage::ClosePath()
	{
		// Operator   : h
		// Description: Закрываем subpath, соединяя текущую точку с начальной прямой линией. Если subpath
		//              уже закрыт, тогда ничего не делаем
		CheckGrMode(grmode_PATH);
		m_pStream->WriteStr("h\012");
		m_oCurPos = m_oStartPos;
	}
    void CPage::Stroke()
	{
		// Operator   : S
		// Description: Обводим path.

		SetGrMode(grmode_PAGE);
		m_pStream->WriteStr("S\012");
		m_oCurPos.Reset();
	}
    void CPage::Fill()
	{
		// Operator   : f
		// Description: Заливка path по правилу Nonzero Winding Number Rule(см. спецификацию PDF Part1: PDF 1.7 
		//              стр. 136, закладка 8.5.3.3.2). 
		SetGrMode(grmode_PAGE);
		m_pStream->WriteStr("f\012");
		m_oCurPos.Reset();
	}
    void CPage::EoFill()
	{
		// Operator   : f*
		// Description: Заливка path по правилу Even-Odd Rule(см. спецификацию PDF Part1: PDF 1.7 стр. 137, 
		//              закладка 8.5.3.3.3). 
		SetGrMode(grmode_PAGE);
		m_pStream->WriteStr("f*\012");
		m_oCurPos.Reset();
	}
    void CPage::FillStroke()
	{
		// Operator   : B
		// Description: Заливка и обоводка path, используя правило для заливки Nonzero Winding Number Rule(см. 
		//              спецификацию PDF Part1: PDF 1.7 стр. 136, закладка 8.5.3.3.2). Этот оператор должен 
		//              привести к тому же самому результату как строительство двух идентичных объектов path, 
		//              применяя к первому оператор f и ко второму - S.
		SetGrMode(grmode_PAGE);
		m_pStream->WriteStr("B\012");
		m_oCurPos.Reset();
	}
    void CPage::EoFillStroke()
	{
		// Operator   : B*
		// Description: Заливка и обоводка path, используя правило для заливки Even-Odd Rule(см. 
		//              спецификацию PDF Part1: PDF 1.7 стр. 137, закладка 8.5.3.3.3). Этот оператор должен 
		//              привести к тому же самому результату как строительство двух идентичных объектов path, 
		//              применяя к первому оператор f* и ко второму - S.
		SetGrMode(grmode_PAGE);
		m_pStream->WriteStr("B*\012");
		m_oCurPos.Reset();
	}
    void CPage::EndPath()
	{
		// Operator   : n
		// Description: Закрываем path, не заливая и не обводя его. Этот оператор используется прежде всего для 
		//              изменения текущего path.
		SetGrMode(grmode_PAGE);
		m_pStream->WriteStr("n\012");
		m_oCurPos.Reset();
	}
    void CPage::SetLineWidth(double dLineWidth)
	{
		// Operator   : w 
		// Descriprion: устанавливаем толщину линии

        dLineWidth = std::max(dLineWidth, 0.0);
		m_pStream->WriteReal(dLineWidth);
		m_pStream->WriteStr(" w\012");
		m_pGrState->m_dLineWidth = dLineWidth;
	}
    void CPage::SetLineCap(ELineCapStyle eLineCap)
	{
		// Operator   : J 
		// Descriprion: устанавливаем вид окончания линии (LineCapStyle)

        eLineCap = std::max(linecap_Min, std::min(linecap_Max, eLineCap));
		m_pStream->WriteInt((unsigned int)eLineCap);
		m_pStream->WriteStr(" J\012");
		m_pGrState->m_eLineCap = eLineCap;
	}
    void CPage::SetLineJoin(ELineJoinStyle eLineJoin)
	{
		// Operator   : j
		// Descriprion: устанавливаем вид соединения линий (LineJoinStyle)
        eLineJoin = std::max(linejoin_Min, std::min(linejoin_Max, eLineJoin));
		m_pStream->WriteInt((unsigned int)eLineJoin);
		m_pStream->WriteStr(" j\012");
		m_pGrState->m_eLineJoin = eLineJoin;
	}
    void CPage::SetMiterLimit(double dMiterLimit)
	{
		// Operator   : M
		// Descriprion: устанавливаем MiterLimit - константа, относящаяся к виду соединения линий
        dMiterLimit = std::max(1.0, dMiterLimit);
		m_pStream->WriteReal(dMiterLimit);
		m_pStream->WriteStr(" M\012");
		m_pGrState->m_dMiterLimit = dMiterLimit;
	}
    void CPage::SetDash(const double* pPattern, unsigned int unCount, double dPhase)
	{
		// Operator   : d
		// Descriprion: устанавливаем вид линий (DashMode)

		if (0 == unCount || !pPattern)
			return;

		bool bFalseDash = true;
		for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
		{
			if (0 != pPattern[unIndex])
			{
				bFalseDash = false;
				break;
			}
		}

		if (bFalseDash)
			return;

		m_pStream->WriteChar('[');
		for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
		{
			m_pStream->WriteReal(pPattern[unIndex]);
			m_pStream->WriteChar(' ');
		}
		m_pStream->WriteStr("] ");
		m_pStream->WriteReal(dPhase);
		m_pStream->WriteStr(" d\012");

		m_pGrState->m_oDashMode.Set(pPattern, unCount, dPhase);
	}
    void CPage::SetFlat(double dFlatness)
	{
		// Operator   : i
		// Descriprion: устанавливаем порог ошибки линии (Flatness tolerance)
        dFlatness = std::min(100.0, std::max(0.0, dFlatness));
		m_pStream->WriteReal(dFlatness);
		m_pStream->WriteStr(" i\012");
		m_pGrState->m_dFlatness = dFlatness;
	}
    void CPage::GrSave()
	{
		// Operator   : q
		// Description: сохраняем текущий GState в графическом стеке
		CheckGrMode(grmode_PAGE);
		CGrState* pState = new CGrState(m_pGrState);
		if (!pState)
			return;

		m_pStream->WriteStr("q\012");
		m_pGrState = pState;
	}
    void CPage::GrRestore()
	{
		// Operator   : Q
		// Description: Восстанавливаем GState, удаляя самый последний GState, и делаем данный GState текущим
		CheckGrMode(grmode_PAGE);

		if (!m_pGrState->m_pPrev)
			return;

		CGrState* pPrev = m_pGrState->m_pPrev;
		delete m_pGrState;
		m_pGrState = pPrev;

		m_pStream->WriteStr("Q\012");
	}
    void CPage::SetStrokeColor(unsigned char unR, unsigned char unG, unsigned char unB)
	{
		// Operator   : RG
		// Description: Устанавливаем цветовое пространтсво для обводки в DeviceRGB и устанавливаем цвет для 
		//              операций связанных с обведением фигур.

		double dR = unR / 255.0;
		double dG = unG / 255.0;
		double dB = unB / 255.0;

		m_pStream->WriteReal(dR);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dG);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dB);
		m_pStream->WriteStr(" RG\012");

		m_pGrState->m_oStrokeColor.r = dR;
		m_pGrState->m_oStrokeColor.g = dG;
		m_pGrState->m_oStrokeColor.b = dB;
	}
    void CPage::SetFillColor(unsigned char unR, unsigned char unG, unsigned char unB)
	{
		// Operator   : rg
		// Description: Устанавливаем цветовое пространтсво для заливки в DeviceRGB и устанавливаем цвет для 
		//              операций связанных с заливкой фигур.

		double dR = unR / 255.0;
		double dG = unG / 255.0;
		double dB = unB / 255.0;

		m_pStream->WriteReal(dR);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dG);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dB);
		m_pStream->WriteStr(" rg\012");

		m_pGrState->m_oFillColor.r = dR;
		m_pGrState->m_oFillColor.g = dG;
		m_pGrState->m_oFillColor.b = dB;
	}
    void CPage::Concat(double dM11, double dM12, double dM21, double dM22, double dX, double dY)
	{
		// Operator   : cm
		// Description: меняем матрицу преобразований (CTM - Current Transformation Matrix)

		m_pStream->WriteReal(dM11);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dM12);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dM21);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dM22);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dX);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY);
		m_pStream->WriteStr(" cm\012");
	
		CMatrix oCTM = m_pGrState->m_oMatrix;
	
		// перемножаем матрицы oCTM(новая)= oCTM(преобразования(которая параметрами задана)) x oCTM(старая)
		m_pGrState->m_oMatrix.m11 = dM11 * oCTM.m11 + dM12 * oCTM.m21;
		m_pGrState->m_oMatrix.m12 = dM11 * oCTM.m12 + dM12 * oCTM.m22;
		m_pGrState->m_oMatrix.m21 = dM21 * oCTM.m11 + dM22 * oCTM.m21;
		m_pGrState->m_oMatrix.m22 = dM21 * oCTM.m12 + dM22 * oCTM.m22;
		m_pGrState->m_oMatrix.x   =   dX * oCTM.m11 + dY * oCTM.m21 + oCTM.x;
		m_pGrState->m_oMatrix.y   =   dX * oCTM.m12 + dY * oCTM.m22 + oCTM.y;
	}
    void CPage::SetTransform(double dM11, double dM12, double dM21, double dM22, double dX, double dY)
	{
		CMatrix oInverse = m_pGrState->m_oMatrix.Inverse();

		CMatrix oResult;
		oResult.m11 = dM11 * oInverse.m11 + dM12 * oInverse.m21;
		oResult.m12 = dM11 * oInverse.m12 + dM12 * oInverse.m22;
		oResult.m21 = dM21 * oInverse.m11 + dM22 * oInverse.m21;
		oResult.m22 = dM21 * oInverse.m12 + dM22 * oInverse.m22;
		oResult.x   =   dX * oInverse.m11 +   dY * oInverse.m21 + oInverse.x;
		oResult.y   =   dX * oInverse.m12 +   dY * oInverse.m22 + oInverse.y;

		if (!oResult.IsIdentity())
			Concat(oResult.m11, oResult.m12, oResult.m21, oResult.m22, oResult.x, oResult.y);
	}
    void CPage::Clip()
	{
		// Operator   : W
		// Description: Изменяем текущий clipping path, пересакая его с текущим path, ипользуя правило Nonzero 
		//              Winding Number Rule, для определения какие регионы лежат внутри clipping path.
		SetGrMode(grmode_CLIP);
		m_pStream->WriteStr("W\012");
	}
    void CPage::Eoclip()
	{
		// Operator   : W*
		// Description: Изменяем текущий clipping path, пересакая его с текущим path, ипользуя правило Even-Odd 
		//              Rule, для определения какие регионы лежат внутри clipping path.
		SetGrMode(grmode_CLIP);
		m_pStream->WriteStr("W*\012");
	}
    void CPage::SetExtGrState(CExtGrState* pState)
	{
		if (!pState)
			return;

		// Operator   : gs
		// Description: устанавливаем сразу все настройки данного графического состояния(ExtGState)

		const char* sGsName = GetExtGrStateName(pState);
		if (!sGsName)
			return;

		m_pStream->WriteEscapeName(sGsName);
		m_pStream->WriteStr(" gs\012");
	}
	const char*   CPage::GetExtGrStateName(CExtGrState* pState)
	{
		const char *sKey;

		if (!m_pExtGStates)
		{
			CDictObject* pResources = (CDictObject*)GetResourcesItem();
			if (!pResources)
				return NULL;

			m_pExtGStates = new CDictObject();
			if (!m_pExtGStates)
				return NULL;

			pResources->Add("ExtGState", m_pExtGStates);
		}

		sKey = m_pExtGStates->GetKey(pState);
		if (!sKey)
		{
			// Если ExtGState не зарегистрирован в Resource, регистрируем.
			char sExtGrStateName[LIMIT_MAX_NAME_LEN + 1];
			char *pPointer;
			char *pEndPointer = sExtGrStateName + LIMIT_MAX_NAME_LEN;

			pPointer = (char*)StrCpy(sExtGrStateName, "E", pEndPointer);
			ItoA(pPointer, ++m_unExtGStatesCount, pEndPointer);
			m_pExtGStates->Add(sExtGrStateName, pState);
			sKey = m_pExtGStates->GetKey(pState);
		}

		return sKey;
	}
    void CPage::AddAnnotation(CDictObject* pAnnot)
	{
		CArrayObject* pArray = (CArrayObject*)Get("Annots");
		if (!pArray)
		{
			pArray = new CArrayObject();
			if (!pArray)
				return;
	
	        Add("Annots", pArray);
	    }
	
	    return pArray->Add(pAnnot);
	}
    void CPage::BeginText()
	{
		// Operator   : BT
		// Description: Начало текста
		SetGrMode(grmode_TEXT);
		m_pStream->WriteStr("BT\012");

		m_oTextPos.Reset();
		m_oTextMatrix.Reset();
	}
    void CPage::EndText()
	{
		// Operator   : ET
		// Description: Окончание текста
		CheckGrMode(grmode_TEXT);
		m_pStream->WriteStr("ET\012");	
		SetGrMode(grmode_PAGE);
	}
    void CPage::MoveTextPos(double dX, double dY)
	{
		// Operator   : Td
		// Description: Переходим к началу следующей линии, сдвигаясь от начала текущей на ( fX, fY ).
		CheckGrMode(grmode_TEXT);

		m_pStream->WriteReal(dX);
	    m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY);
		m_pStream->WriteStr(" Td\012");
	
		m_oTextMatrix.x += dX * m_oTextMatrix.m11 + dY * m_oTextMatrix.m21;
		m_oTextMatrix.y += dX * m_oTextMatrix.m12 + dY * m_oTextMatrix.m22;
		m_oTextPos.Set(m_oTextMatrix.x, m_oTextMatrix.y);
	}
    void CPage::ShowText(const BYTE* sText, unsigned int unLen)
	{
		// Operator   : Tj
		// Description: Показать текстовую строку.
		CheckGrMode(grmode_TEXT);
		WriteText(sText, unLen);
		m_pStream->WriteStr(" Tj\012");
	}
    void CPage::WriteText(const BYTE* sText, unsigned int unLen)
	{
		EFontType eType = m_pFont->GetFontType();
		if (fontCIDType0 == eType || fontCIDType0C == eType || fontCIDType0COT == eType || fontCIDType2 == eType || fontCIDType2OT == eType)
		{
			m_pStream->WriteChar('<');
			m_pStream->WriteBinary(sText, unLen, NULL);
			m_pStream->WriteChar('>');
		}
		else
		{
			m_pStream->WriteEscapeText(sText, unLen);
		}
	}
    void CPage::DrawText(double dXpos, double dYpos, const BYTE* sText, unsigned int unLen)
	{
		CheckGrMode(grmode_TEXT);

		double dX = 0.0;
		double dY = 0.0;

		if (0 == m_oTextMatrix.m11)
		{
			dY = (dXpos - m_oTextMatrix.x) / m_oTextMatrix.m21;
			dX = (dYpos - m_oTextMatrix.y - (dXpos - m_oTextMatrix.x) * m_oTextMatrix.m22 / m_oTextMatrix.m21) / m_oTextMatrix.m12;
		}
		else
		{
			dY = (dYpos - m_oTextMatrix.y - (dXpos - m_oTextMatrix.x) * m_oTextMatrix.m12 / m_oTextMatrix.m11) / (m_oTextMatrix.m22 - m_oTextMatrix.m21 * m_oTextMatrix.m12 / m_oTextMatrix.m11);
			dX = (dXpos - m_oTextMatrix.x - dY * m_oTextMatrix.m21) / m_oTextMatrix.m11;
		}

		MoveTextPos(dX, dY);
		ShowText(sText, unLen);
	}
    void CPage::DrawTextLine(const CTextLine* pTextLine)
	{
		if (!pTextLine)
			return;

		int nCount = pTextLine->m_vWords.size();
		if (nCount <= 0)
			return;

		CheckGrMode(grmode_TEXT);

		double dXpos = pTextLine->m_dX;
		double dYpos = pTextLine->m_dY;

		double dX = 0.0;
		double dY = 0.0;

		if (0 == m_oTextMatrix.m11)
		{
			dY = (dXpos - m_oTextMatrix.x) / m_oTextMatrix.m21;
			dX = (dYpos - m_oTextMatrix.y - (dXpos - m_oTextMatrix.x) * m_oTextMatrix.m22 / m_oTextMatrix.m21) / m_oTextMatrix.m12;
		}
		else
		{
			dY = (dYpos - m_oTextMatrix.y - (dXpos - m_oTextMatrix.x) * m_oTextMatrix.m12 / m_oTextMatrix.m11) / (m_oTextMatrix.m22 - m_oTextMatrix.m21 * m_oTextMatrix.m12 / m_oTextMatrix.m11);
			dX = (dXpos - m_oTextMatrix.x - dY * m_oTextMatrix.m21) / m_oTextMatrix.m11;
		}
		MoveTextPos(dX, dY);

		if (1 == nCount)
		{
			CTextWord* pWord = pTextLine->m_vWords.at(0);
			ShowText(pWord->m_pText, pWord->m_nIndex * 2);
		}
		else
		{
			CTextWord* pWord = NULL;
			double dShift = 0;
			m_pStream->WriteChar('[');
			for (int nIndex = 0; nIndex < nCount; nIndex++)
			{
				pWord  = pTextLine->m_vWords.at(nIndex);
				WriteText(pWord->m_pText, pWord->m_nIndex * 2);
				if (nIndex != nCount - 1)
				{
					dShift = pTextLine->m_vShifts.at(nIndex);
					m_pStream->WriteReal(dShift);
				}
			}
			m_pStream->WriteStr("]TJ\012");
		}
	}
    void CPage::SetCharSpace(double dValue)
	{
		// Operator   : Tc
		// Description: Устанавливаем расстояние между буквами
		CheckGrMode(grmode_TEXT);

        dValue = std::min((double)MAX_CHARSPACE, std::max((double)MIN_CHARSPACE, dValue));
		m_pStream->WriteReal(dValue);
		m_pStream->WriteStr(" Tc\012");
	}
    void CPage::SetHorizontalScalling(double dValue)
	{
		// Operator   : Tz
		// Description: Устанавливаем горизонтальное растяжение/сжатие
		CheckGrMode(grmode_TEXT);

        dValue = std::min((double)MAX_HORIZONTALSCALING, std::max((double)MIN_HORIZONTALSCALING, dValue));
		m_pStream->WriteReal(dValue);
		m_pStream->WriteStr(" Tz\012");
	}
    void CPage::SetFontAndSize(CFontDict* pFont, double dSize)
	{
		// Operator   : Tf
		// Description: Устанавливаем фонт и размер фонта

        dSize = std::min((double)MAX_FONTSIZE, std::max(0.0, dSize));
		const char* sFontName = GetLocalFontName(pFont);
		if (!sFontName)
			return;

		m_pStream->WriteEscapeName(sFontName);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dSize);
		m_pStream->WriteStr(" Tf\012");

		m_pFont = pFont;
	}
	const char*   CPage::GetLocalFontName(CFontDict* pFont)
	{
		if (!m_pFonts)
		{
			CDictObject* pResources = GetResourcesItem();
			if (!pResources)
				return NULL;

			m_pFonts = new CDictObject();
			if (!m_pFonts)
				return NULL;

			pResources->Add("Font", m_pFonts);
		}

		const char *sKey = m_pFonts->GetKey(pFont);
		if (!sKey)
		{
			// если фонт не зарегистрирован в ресурсах, тогда регистрируем его
			char sFontName[LIMIT_MAX_NAME_LEN + 1];
			char *pPointer = NULL;
			char *pEndPointer = sFontName + LIMIT_MAX_NAME_LEN;

			++m_unFontsCount;
			while (m_unFontsCount < LIMIT_MAX_DICT_ELEMENT)
			{
				if (m_pFonts->Get("F" + std::to_string(m_unFontsCount)))
					++m_unFontsCount;
				else
					break;
			}

			pPointer = (char*)StrCpy(sFontName, "F", pEndPointer);
			ItoA(pPointer, m_unFontsCount, pEndPointer);
			m_pFonts->Add(sFontName, pFont);
			sKey = m_pFonts->GetKey(pFont);
		}

		return sKey;
	}
    void CPage::SetTextRenderingMode(ETextRenderingMode eMode)
	{
		// Operator   : Tr
		// Description: Устанавливаем тип закрашивания символов (TextRenderingMode)
		CheckGrMode(grmode_TEXT);
		m_pStream->WriteInt((int)eMode);
		m_pStream->WriteStr(" Tr\012");
	}
    void CPage::SetTextMatrix(double dM11, double dM12, double dM21, double dM22, double dX, double dY)
	{
		// Operator   : Tm
		// Description: Устанавливаем матрицу преобразования для текста.
		CheckGrMode(grmode_TEXT);

		m_pStream->WriteReal(dM11);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dM12);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dM21);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dM22);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dX);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY);
		m_pStream->WriteStr(" Tm\012");
	
		m_oTextMatrix.m11 = dM11;
		m_oTextMatrix.m12 = dM12;
		m_oTextMatrix.m21 = dM21;
		m_oTextMatrix.m22 = dM22;
		m_oTextMatrix.x = dX;
		m_oTextMatrix.y = dY;
		m_oTextPos.Set(m_oTextMatrix.x, m_oTextMatrix.y);
	}
    void CPage::ExecuteXObject(CXObject* pXObject)
	{
		const char* sXObjectName = GetXObjectName(pXObject);

		if (!sXObjectName)
			return;

		m_pStream->WriteEscapeName(sXObjectName);
		m_pStream->WriteStr(" Do\012");
	}
    void CPage::DrawImage(CImageDict* pImage, double dX, double dY, double dWidth, double dHeight)
	{
		GrSave();
		Concat(dWidth, 0, 0, dHeight, dX, dY);
		ExecuteXObject(pImage);
		GrRestore();
	}
	const char*   CPage::GetXObjectName(CXObject* pObject)
	{
		if (!m_pXObjects)
		{
			CDictObject* pResources = GetResourcesItem();
			if (!pResources)
				return NULL;

			m_pXObjects = new CDictObject();
			if (!m_pXObjects)
				return NULL;

			pResources->Add("XObject", m_pXObjects);
		}

		const char* sKey = m_pXObjects->GetKey(pObject);
		if (!sKey)
		{
			char sXObjName[LIMIT_MAX_NAME_LEN + 1];
			char *pPointer;
			char *pEndPointer = sXObjName + LIMIT_MAX_NAME_LEN;

			pPointer = (char*)StrCpy(sXObjName, "X", pEndPointer);
			ItoA(pPointer, ++m_unXObjectsCount, pEndPointer);
			m_pXObjects->Add(sXObjName, pObject);
			sKey = m_pXObjects->GetKey(pObject);
		}

		return sKey;
	}
    void CPage::DrawShading(CShading* pShading)
	{
		// Operator   : sh
		// Description: отрисовываем градиент

		const char* sShadingName = GetLocalShadingName(pShading);
		if (!sShadingName)
			return;

		m_pStream->WriteEscapeName(sShadingName);
		m_pStream->WriteStr(" sh\012");
	}
    void CPage::SetStrokeAlpha(unsigned char unAlpha)
	{
		CExtGrState* pExtGrState = m_pDocument->GetStrokeAlpha((double)(unAlpha / 255.0));
		if (pExtGrState)
			SetExtGrState(pExtGrState);
	}
    void CPage::SetFillAlpha(unsigned char unAlpha)
	{
		CExtGrState* pExtGrState = m_pDocument->GetFillAlpha((double)(unAlpha / 255.0));
		if (pExtGrState)
			SetExtGrState(pExtGrState);
	}
	const char*   CPage::GetLocalShadingName(CShading* pShading)
	{
		if (!m_pShadings)
		{
			CDictObject* pResources = GetResourcesItem();
			if (!pResources)
				return NULL;

			m_pShadings = new CDictObject();
			if (!m_pShadings)
				return NULL;

			pResources->Add("Shading", m_pShadings);
		}

		const char* sKey = m_pShadings->GetKey(pShading);
		if (!sKey)
		{
			char sShadingName[LIMIT_MAX_NAME_LEN + 1];
			char *pPointer;
			char *pEndPointer = sShadingName + LIMIT_MAX_NAME_LEN;

			pPointer = (char*)StrCpy(sShadingName, "S", pEndPointer);
			ItoA(pPointer, ++m_unShadingsCount, pEndPointer);
			m_pShadings->Add(sShadingName, pShading);
			sKey = m_pShadings->GetKey(pShading);
		}

		return sKey;
	}
	const char*   CPage::GetLocalPatternName(CImageTilePattern* pPattern)
	{
		if (!m_pPatterns)
		{
			CDictObject* pResources = GetResourcesItem();
			if (!pResources)
				return NULL;

			m_pPatterns = new CDictObject();
			if (!m_pPatterns)
				return NULL;

			pResources->Add("Pattern", m_pPatterns);
		}

		const char* sKey = m_pPatterns->GetKey(pPattern);
		if (!sKey)
		{
			char sPatternName[LIMIT_MAX_NAME_LEN + 1];
			char *pPointer;
			char *pEndPointer = sPatternName + LIMIT_MAX_NAME_LEN;

			pPointer = (char*)StrCpy(sPatternName, "P", pEndPointer);
			ItoA(pPointer, m_unPatternsCount + 1, pEndPointer);
			m_unPatternsCount++;
			m_pPatterns->Add(sPatternName, pPattern);
			sKey = m_pPatterns->GetKey(pPattern);
		}

		return sKey;
	}
    void CPage::SetPatternColorSpace(CImageTilePattern* pPattern)
	{
		// Operator   : csn
		// Description: задаем паттерн для рисования

		const char* sPatternName = GetLocalPatternName(pPattern);
		if (!sPatternName)
			return;

		m_pStream->WriteStr("/Pattern cs\012");
		m_pStream->WriteEscapeName(sPatternName);
		m_pStream->WriteStr(" scn\012");		
	}
    void CPage::SetFilter(unsigned int unFiler)
	{
		if (m_pContents)
		{
			for (unsigned int unKidIndex = 0, unKidsCount = m_pContents->GetCount(); unKidIndex < unKidsCount; ++unKidIndex)
			{
				CObjectBase* pKid = m_pContents->Get(unKidIndex);
				if (pKid->GetType() == object_type_DICT)
					((CDictObject*)pKid)->SetFilter(unFiler);
			}
		}
	}
	CMatrix*      CPage::GetTransform()
	{
		return &m_pGrState->m_oMatrix;
	}
    void CPage::AddGroup(CDictObject* pDict)
	{
		Add("Group", pDict);
	}
	void CPage::AddContents(CXref* pXref)
	{
		CDictObject* pContent = new CDictObject(pXref);
		m_pContents->Add(pContent);
		m_pStream = pContent->GetStream();
	}
	void CPage::SetRotate(int nRotate)
	{
		// The value shall be a multiple of 90
		if (nRotate > 0 && nRotate % 90 == 0)
		{
			CNumberObject* pRotate = (CNumberObject*)GetRotateItem();
			if (pRotate)
				Add("Rotate", (nRotate + pRotate->Get()) % 360);
			else
				Add("Rotate", nRotate % 360);
		}
	}
    int CPage::GetRotate()
    {
        CNumberObject* pRotate = (CNumberObject*)GetRotateItem();
        return pRotate ? pRotate->Get() : 0;
    }
    //----------------------------------------------------------------------------------------
	// CTextWord
	//----------------------------------------------------------------------------------------
	CTextWord::CTextWord()
	{
		m_nIndex = 0;
		m_pText = (unsigned char*)malloc(STR_BUF);
		m_nSize = STR_BUF;
	}
	CTextWord::~CTextWord()
	{
		if (m_pText)
			free(m_pText);
	}
	void CTextWord::CheckBuffer()
	{
		if (2 * m_nIndex >= m_nSize)
		{
			m_nSize += STR_BUF;
			m_pText = (unsigned char*)realloc(m_pText, m_nSize);
		}
	}
	bool CTextWord::Add(unsigned char* pCodes, unsigned int unLen, double dX, double dY, double dWidth)
	{
		if (2 != unLen)
			return false;

		CheckBuffer();

		if (0 == m_nIndex)
		{
			m_pText[0] = pCodes[0];
			m_pText[1] = pCodes[1];
			m_nIndex++;
			m_dStartX = dX;
			m_dStartY = dY;

			m_dCurX = dX + dWidth;
		}
		else
		{
			if (fabs(dY - m_dStartY) > 0.001 || fabs(dX - m_dCurX) > 0.01)
				return false;

			m_pText[m_nIndex * 2 + 0] = pCodes[0];
			m_pText[m_nIndex * 2 + 1] = pCodes[1];
			m_nIndex++;

			m_dCurX = dX + dWidth;
		}

		return true;
	}
	//----------------------------------------------------------------------------------------
	// CTextLine
	//----------------------------------------------------------------------------------------
	CTextLine::CTextLine()
	{
	}
	CTextLine::~CTextLine()
	{
		Clear();
	}
	bool CTextLine::Add(unsigned char* pCodes, unsigned int unLen, double dX, double dY, double dWidth, double dSize)
	{
		if (2 != unLen)
			return false;

		if (0 == m_vWords.size())
		{
			CTextWord* pText = new CTextWord();
			if (!pText || !pText->Add(pCodes, unLen, dX, dY, dWidth))
				return false;

			m_vWords.push_back(pText);
			m_dX = dX;
			m_dY = dY;
			return true;
		}

		if (fabs(dY - m_dY) > 0.001)
			return false;

		CTextWord* pLastText = m_vWords.at(m_vWords.size() - 1);
		if (pLastText->Add(pCodes, unLen, dX, dY, dWidth))
			return true;

		CTextWord* pText = new CTextWord();
		if (!pText || !pText->Add(pCodes, unLen, dX, dY, dWidth))
			return false;

		m_vWords.push_back(pText);
		double dShift = (pLastText->m_dCurX - dX) * 1000 / dSize;
		m_vShifts.push_back(dShift);
		return true;
	}
	void CTextLine::Flush(CPage* pPage)
	{
		pPage->DrawTextLine(this);
		Clear();
	}
	void CTextLine::Clear()
	{
		for (int nIndex = 0, nCount = m_vWords.size(); nIndex < nCount; nIndex++)
		{
			CTextWord* pText = m_vWords.at(nIndex);
			RELEASEOBJECT(pText);
		}
		m_vWords.clear();
		m_vShifts.clear();
	}
}
