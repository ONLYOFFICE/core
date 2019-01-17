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
#ifndef _PDF_READER_ANNOT_H
#define _PDF_READER_ANNOT_H

namespace PdfReader
{
	class XRef;
	class Catalog;
	class Graphics;
	class GrFontDict;

	//-------------------------------------------------------------------------------------------------------------------------------
	// AnnotBorderStyle
	//-------------------------------------------------------------------------------------------------------------------------------

	enum AnnotBorderType
	{
		annotBorderSolid,
		annotBorderDashed,
		annotBorderBeveled,
		annotBorderInset,
		annotBorderUnderlined
	};

	class AnnotBorderStyle
	{
	public:

		AnnotBorderStyle(AnnotBorderType eType, double dWidth, double *pDash, int nDashLength, double dR, double dG, double dB);
		~AnnotBorderStyle();

		AnnotBorderType GetType()
		{
			return m_eType;
		}
		double          GetWidth()
		{
			return m_dWidth;
		}
		void            GetDash(double **ppDash, int *pnDashLength)
		{
			*ppDash = m_pDash;
			*pnDashLength = m_nDashLength;
		}
		void            GetColor(double *pdR, double *pdG, double *pdB)
		{
			*pdR = m_dR;
			*pdG = m_dG;
			*pdB = m_dB;
		}

	private:

		AnnotBorderType m_eType;       // Тип 
		double          m_dWidth;      // Ширина рамки
		double         *m_pDash;       // Если линия пунктирная, тогда определяем длину штриха
		int             m_nDashLength; // Количество элементов в массиве m_pDash
		double          m_dR;          //
		double          m_dG;          // Цвет
		double          m_dB;          //
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// Annot
	//-------------------------------------------------------------------------------------------------------------------------------

	class Annot
	{
	public:

		Annot(GlobalParams *pGlobalParams, XRef *pXref, Dict *pAcroForm, Dict *pDict, Ref *pRef);
		~Annot();
		bool CheckValidation()
		{
			return m_bValid;
		}

		void Draw(Graphics *pGraphics, bool pBrinting);
		Object *GetAppearance(Object *pObject)
		{
			return m_oAppearance.Fetch(m_pXref, pObject);
		}

		AnnotBorderStyle *GetBorderStyle()
		{
			return m_pBorderStyle;
		}

		bool Match(Ref *pRef)
		{
			return m_oRef.nNum == pRef->nNum && m_oRef.nGen == pRef->nGen;
		}

		void GenerateFieldAppearance(Dict *pField, Dict *pAnnot, Dict *pAcroForm);

	private:

		void SetColor(Array *pArray, bool bFill, int nAdjust);
		void DrawText(StringExt *seText, StringExt *seDA, GrFontDict *pFontDict, bool bMultiline, int nComb, int nQuadding, bool bTextField, bool bForceZapfDingbats);
		void DrawListBox(StringExt **ppText, bool *pSelection, int nOptionsCount, int nTopIndex, StringExt *seDA, GrFontDict *pFontDict, int nQuadding);
		void GetNextLine(StringExt *seText, int nStart, GrFont *pFont, double dFontSize, double dMaxW, int *pEnd, double *pWidth, int *pNext);
		void DrawCircle(double dX, double dY, double dRad, bool bFill);
		void DrawCircleTopLeft(double dX, double dY, double dRad);
		void DrawCircleBottomRight(double dX, double dY, double dRad);
		Object *FieldLookup(Dict *pField, char *sKey, Object *pObject);

	private:

		XRef        *m_pXref;       // Таблица XRef для данного PDF-документа
		Ref          m_oRef;        // Ссылка на объект, определяющий данной Annotation
		StringExt   *m_seType;      // Тип Annotation
		Object       m_oAppearance; // Ссылка на Form XObject-поток
		StringExt   *m_seAppBuffer;

		double       m_dMinX;       // 
		double       m_dMinY;       // Прямоугольник, ограничивающий данную Annotation
		double       m_dMaxX;       //
		double       m_dMaxY;       //

		unsigned int m_nFlags;
		AnnotBorderStyle *m_pBorderStyle;

		bool         m_bValid;

		GlobalParams *m_pGlobalParams;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// Annots
	//-------------------------------------------------------------------------------------------------------------------------------

	class Annots
	{
	public:

		Annots(GlobalParams *pGlobalParams, XRef *pXref, Catalog *pCatalog, Object *pAnnotsObject);

		~Annots();

		int GetAnnotsCount()
		{
			return m_nAnnotsCount;
		}
		Annot *GetAnnot(int nIndex)
		{
			return m_ppAnnots[nIndex];
		}


		void GenerateAppearances(Dict *pAcroForm);

	private:

		void ScanFieldAppearances(Dict *pNode, Ref *pRef, Dict *pParent, Dict *pAcroForm);
		Annot *FindAnnot(Ref *pRef);

	private:
		Annot **m_ppAnnots;     // Список Annotations
		int     m_nAnnotsCount; // Количество элементов

		GlobalParams *m_pGlobalParams;
	};
}

#endif // _PDF_READER_ANNOT_H
