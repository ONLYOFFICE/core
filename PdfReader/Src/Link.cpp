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
#include <stddef.h>
#include <string.h>
#include "MemoryUtils.h"
#include "StringExt.h"
#include "Object.h"
#include "Array.h"
#include "Dict.h"
#include "Link.h"

namespace PdfReader
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkAction
	//-------------------------------------------------------------------------------------------------------------------------------

	LinkAction *LinkAction::ParseDestination(Object *pObject)
	{
		LinkAction *pAction = new LinkGoTo(pObject);

		if (!pAction->CheckValidate())
		{
			delete pAction;
			return NULL;
		}
		return pAction;
	}

	LinkAction *LinkAction::ParseAction(Object *pObject, StringExt *seBaseURI)
	{
		LinkAction *pAction = NULL;

		if (!pObject->IsDict())
		{
			// TO DO: Error "Bad annotation action"
			return NULL;
		}

		// S (тип Action)
		Object oType;
		pObject->DictLookup("S", &oType);

		// GoTo action
		if (oType.IsName("GoTo"))
		{
			Object oD;
			pObject->DictLookup("D", &oD);
			pAction = new LinkGoTo(&oD);
			oD.Free();
		}
		else if (oType.IsName("GoToR")) // GoToR action
		{
			Object oF, oD;
			pObject->DictLookup("F", &oF);
			pObject->DictLookup("D", &oD);
			pAction = new LinkGoToR(&oF, &oD);
			oF.Free();
			oD.Free();
		}
		else if (oType.IsName("Launch")) // Launch action
		{
			pAction = new LinkLaunch(pObject);
		}
		else if (oType.IsName("URI")) // URI action
		{
			Object oURI;
			pObject->DictLookup("URI", &oURI);
			pAction = new LinkURI(&oURI, seBaseURI);
			oURI.Free();
		}
		else if (oType.IsName("Named")) // Named action
		{
			Object oN;
			pObject->DictLookup("N", &oN);
			pAction = new LinkNamed(&oN);
			oN.Free();
		}
		else if (oType.IsName("Movie")) // Movie action
		{
			Object oAnnot, oT;
			pObject->DictLookupAndCopy("Annot", &oAnnot);
			pObject->DictLookup("T", &oT);
			pAction = new LinkMovie(&oAnnot, &oT);
			oAnnot.Free();
			oT.Free();
		}
		else if (oType.IsName()) // unknown action
		{
			pAction = new LinkUnknown(oType.GetName());
		}
		else
		{
			// TO DO: Error "Bad annotation action"
			pAction = NULL;
		}

		oType.Free();

		if (pAction && !pAction->CheckValidate())
		{
			delete pAction;
			return NULL;
		}
		return pAction;
	}

	StringExt *LinkAction::GetFileSpecName(Object *pFileSpecObject)
	{
		StringExt *seName = NULL;

		if (pFileSpecObject->IsString())
		{
			seName = pFileSpecObject->GetString()->Copy();
		}
		else if (pFileSpecObject->IsDict())
		{
			Object oTemp;
			if (!pFileSpecObject->DictLookup("DOS", &oTemp)->IsString())
			{
				oTemp.Free();
				pFileSpecObject->DictLookup("F", &oTemp);
			}
			if (oTemp.IsString())
			{
				seName = oTemp.GetString()->Copy();
			}
			else
			{
				// TO DO: Error "Illegal file spec in link"
			}
			oTemp.Free();
		}
		else
		{
			// TO DO: Error "Illegal file spec in link"
		}

		if (seName)
		{
			// Производим преобразования, связанные с формой записи путей

			// "//...."             --> "\...."
			// "/x/...."            --> "x:\...."
			// "/server/share/...." --> "\\server\share\...."

			int nCurIndex = 0;
			if (seName->GetAt(0) == '/')
			{
				if (seName->GetLength() >= 2 && seName->GetAt(1) == '/')
				{
					seName->Delete(0);
					nCurIndex = 0;
				}
				else if (seName->GetLength() >= 2 && ((seName->GetAt(1) >= 'a' && seName->GetAt(1) <= 'z') || (seName->GetAt(1) >= 'A' && seName->GetAt(1) <= 'Z')) && (seName->GetLength() == 2 || seName->GetAt(2) == '/'))
				{
					seName->SetAt(0, seName->GetAt(1));
					seName->SetAt(1, ':');
					nCurIndex = 2;
				}
				else
				{
					int nJ;
					for (nJ = 2; nJ < seName->GetLength(); ++nJ)
					{
						if (seName->GetAt(nJ - 1) != '\\' && seName->GetAt(nJ) == '/')
						{
							break;
						}
					}
					if (nJ < seName->GetLength())
					{
						seName->SetAt(0, '\\');
						seName->Insert(0, '\\');
						nCurIndex = 2;
					}
				}
			}
			for (; nCurIndex < seName->GetLength(); ++nCurIndex)
			{
				if (seName->GetAt(nCurIndex) == '/')
				{
					seName->SetAt(nCurIndex, '\\');
				}
				else if (seName->GetAt(nCurIndex) == '\\' && nCurIndex + 1 < seName->GetLength() && seName->GetAt(nCurIndex + 1) == '/')
				{
					seName->Delete(nCurIndex);
				}
			}
		}

		return seName;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkDest
	//-------------------------------------------------------------------------------------------------------------------------------

	LinkDestination::LinkDestination(Array *pArray)
	{
		m_dLeft = m_dBottom = m_dRight = m_dTop = m_dZoom = 0;
		m_bValidate = false;

		if (pArray->GetCount() < 2)
		{
			// TO DO: Error "Annotation destination array is too short"
			return;
		}

		Object oArrayItem;
		pArray->GetCopy(0, &oArrayItem);

		if (oArrayItem.IsInt())
		{
			m_nPageNum = oArrayItem.GetInt() + 1;
			m_bPageIsRef = false;
		}
		else if (oArrayItem.IsRef())
		{
			m_oPageRef.nNum = oArrayItem.GetRefNum();
			m_oPageRef.nGen = oArrayItem.GetRefGen();
			m_bPageIsRef = true;
		}
		else
		{
			// TO DO: Error "Bad annotation destination"
			oArrayItem.Free();
			return;
		}
		oArrayItem.Free();

		// Destination type
		pArray->Get(1, &oArrayItem);

		if (oArrayItem.IsName("XYZ"))
		{
			m_eType = destXYZ;
			Object oTemp;
			if (pArray->GetCount() < 3)
			{
				m_bChangeLeft = false;
			}
			else
			{
				pArray->Get(2, &oTemp);
				if (oTemp.IsNull())
				{
					m_bChangeLeft = false;
				}
				else if (oTemp.IsNum())
				{
					m_bChangeLeft = true;
					m_dLeft = oTemp.GetNum();
				}
				else
				{
					// TO DO: Error "Bad annotation destination position"
					oTemp.Free();
					oArrayItem.Free();
					return;
				}
				oTemp.Free();
			}
			if (pArray->GetCount() < 4)
			{
				m_bChangeTop = false;
			}
			else
			{
				pArray->Get(3, &oTemp);
				if (oTemp.IsNull())
				{
					m_bChangeTop = false;
				}
				else if (oTemp.IsNum())
				{
					m_bChangeTop = true;
					m_dTop = oTemp.GetNum();
				}
				else
				{
					// TO DO: Error "Bad annotation destination position"
					oTemp.Free();
					oArrayItem.Free();
					return;
				}
				oTemp.Free();
			}
			if (pArray->GetCount() < 5)
			{
				m_bChangeZoom = false;
			}
			else
			{
				pArray->Get(4, &oTemp);
				if (oTemp.IsNull())
				{
					m_bChangeZoom = false;
				}
				else if (oTemp.IsNum())
				{
					m_bChangeZoom = true;
					m_dZoom = oTemp.GetNum();
				}
				else
				{
					// TO DO: Error "Bad annotation destination position"
					oTemp.Free();
					oArrayItem.Free();
					return;
				}
				oTemp.Free();
			}
		}
		else if (oArrayItem.IsName("Fit"))
		{
			if (pArray->GetCount() < 2)
			{
				// TO DO: Error "Annotation destination array is too short"
				oArrayItem.Free();
				return;
			}
			m_eType = destFit;
		}
		else if (oArrayItem.IsName("FitH"))
		{
			if (pArray->GetCount() < 3)
			{
				// TO DO: Error "Annotation destination array is too short"
				oArrayItem.Free();
				return;
			}
			m_eType = destFitH;

			Object oTemp;
			if (!pArray->Get(2, &oTemp)->IsNum())
			{
				// TO DO: Error "Bad annotation destination position"
				m_eType = destFit;
			}
			m_dTop = oTemp.GetNum();
			oTemp.Free();
		}
		else if (oArrayItem.IsName("FitV"))
		{
			if (pArray->GetCount() < 3)
			{
				// TO DO: Error "Annotation destination array is too short"
				oArrayItem.Free();
				return;
			}
			m_eType = destFitV;

			Object oTemp;
			if (!pArray->Get(2, &oTemp)->IsNum())
			{
				// TO DO: Error "Bad annotation destination position"
				m_eType = destFit;
			}
			m_dLeft = oTemp.GetNum();
			oTemp.Free();
		}
		else if (oArrayItem.IsName("FitR"))
		{
			if (pArray->GetCount() < 6)
			{
				// TO DO: Error "Annotation destination array is too short"
				oArrayItem.Free();
				return;
			}
			m_eType = destFitR;

			Object oTemp;
			if (!pArray->Get(2, &oTemp)->IsNum())
			{
				// TO DO: Error "Bad annotation destination position"
				m_eType = destFit;
			}
			m_dLeft = oTemp.GetNum();
			oTemp.Free();

			if (!pArray->Get(3, &oTemp)->IsNum())
			{
				// TO DO: Error "Bad annotation destination position"
				m_eType = destFit;
			}
			m_dBottom = oTemp.GetNum();
			oTemp.Free();

			if (!pArray->Get(4, &oTemp)->IsNum())
			{
				// TO DO: Error "Bad annotation destination position"
				m_eType = destFit;
			}
			m_dRight = oTemp.GetNum();
			oTemp.Free();

			if (!pArray->Get(5, &oTemp)->IsNum())
			{
				// TO DO: Error "Bad annotation destination position"
				m_eType = destFit;
			}
			m_dTop = oTemp.GetNum();
			oTemp.Free();
		}
		else if (oArrayItem.IsName("FitB"))
		{
			if (pArray->GetCount() < 2)
			{
				// TO DO: Error "Annotation destination array is too short"
				oArrayItem.Free();
				return;
			}
			m_eType = destFitB;
		}
		else if (oArrayItem.IsName("FitBH"))
		{
			if (pArray->GetCount() < 3)
			{
				// TO DO: Error "Annotation destination array is too short"
				oArrayItem.Free();
				return;
			}
			m_eType = destFitBH;

			Object oTemp;
			if (!pArray->Get(2, &oTemp)->IsNum())
			{
				// TO DO: Error "Bad annotation destination position"
				m_eType = destFit;
			}
			m_dTop = oTemp.GetNum();
			oTemp.Free();
		}
		else if (oArrayItem.IsName("FitBV"))
		{
			if (pArray->GetCount() < 3)
			{
				// TO DO: Error "Annotation destination array is too short"
				oArrayItem.Free();
				return;
			}
			m_eType = destFitBV;

			Object oTemp;
			if (!pArray->Get(2, &oTemp)->IsNum())
			{
				// TO DO: Error "Bad annotation destination position"
				m_eType = destFit;
			}
			m_dLeft = oTemp.GetNum();
			oTemp.Free();
		}
		else
		{
			// TO DO: Error "Unknown annotation destination type"
			oArrayItem.Free();
			return;
		}

		oArrayItem.Free();
		m_bValidate = true;
		return;
	}

	LinkDestination::LinkDestination(LinkDestination *pDest)
	{
		m_eType = pDest->m_eType;
		m_bPageIsRef = pDest->m_bPageIsRef;

		if (m_bPageIsRef)
			m_oPageRef = pDest->m_oPageRef;
		else
			m_nPageNum = pDest->m_nPageNum;

		m_dLeft       = pDest->m_dLeft;
		m_dBottom     = pDest->m_dBottom;
		m_dRight      = pDest->m_dRight;
		m_dTop        = pDest->m_dTop;
		m_dZoom       = pDest->m_dZoom;
		m_bChangeLeft = pDest->m_bChangeLeft;
		m_bChangeTop  = pDest->m_bChangeTop;
		m_bChangeZoom = pDest->m_bChangeZoom;
		m_bValidate   = true;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkGoTo
	//-------------------------------------------------------------------------------------------------------------------------------

	LinkGoTo::LinkGoTo(Object *pDestObject)
	{
		m_pDestination = NULL;
		m_pNamedDestination = NULL;

		if (pDestObject->IsName())
		{
			m_pNamedDestination = new StringExt(pDestObject->GetName());
		}
		else if (pDestObject->IsString())
		{
			m_pNamedDestination = pDestObject->GetString()->Copy();
		}
		else if (pDestObject->IsArray())
		{
			m_pDestination = new LinkDestination(pDestObject->GetArray());
			if (!m_pDestination->CheckValidate())
			{
				delete m_pDestination;
				m_pDestination = NULL;
			}
		}
		else
		{
			// TO DO: Error "Illegal annotation destination"
		}
	}

	LinkGoTo::~LinkGoTo()
	{
		if (m_pDestination)
			delete m_pDestination;
		if (m_pNamedDestination)
			delete m_pNamedDestination;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkGoToR
	//-------------------------------------------------------------------------------------------------------------------------------

	LinkGoToR::LinkGoToR(Object *pFileSpecObject, Object *pDestObject)
	{
		m_pDestination = NULL;
		m_pNamedDestination = NULL;

		m_seFileName = GetFileSpecName(pFileSpecObject);

		if (pDestObject->IsName())
		{
			m_pNamedDestination = new StringExt(pDestObject->GetName());
		}
		else if (pDestObject->IsString())
		{
			m_pNamedDestination = pDestObject->GetString()->Copy();
		}
		else if (pDestObject->IsArray())
		{
			m_pDestination = new LinkDestination(pDestObject->GetArray());
			if (!m_pDestination->CheckValidate())
			{
				delete m_pDestination;
				m_pDestination = NULL;
			}
		}
		else
		{
			// TO DO: Error "Illegal annotation destination"
		}
	}

	LinkGoToR::~LinkGoToR()
	{
		if (m_seFileName)
			delete m_seFileName;
		if (m_pDestination)
			delete m_pDestination;
		if (m_pNamedDestination)
			delete m_pNamedDestination;
	}


	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkLaunch
	//-------------------------------------------------------------------------------------------------------------------------------

	LinkLaunch::LinkLaunch(Object *pActionObject)
	{
		m_seFileName = NULL;
		m_pParameters = NULL;

		if (pActionObject->IsDict())
		{
			Object oSpecObject;
			if (!pActionObject->DictLookup("F", &oSpecObject)->IsNull())
			{
				m_seFileName = GetFileSpecName(&oSpecObject);
			}
			else
			{
				oSpecObject.Free();
				if (pActionObject->DictLookup("Win", &oSpecObject)->IsDict())
				{
					Object oTemp;
					oSpecObject.DictLookup("F", &oTemp);
					m_seFileName = GetFileSpecName(&oTemp);
					oTemp.Free();
					if (oSpecObject.DictLookup("P", &oTemp)->IsString())
					{
						m_pParameters = oTemp.GetString()->Copy();
					}
					oTemp.Free();
				}
				else
				{
					// TO DO: Error "Bad launch-type link action"
				}
			}
			oSpecObject.Free();
		}
	}

	LinkLaunch::~LinkLaunch()
	{
		if (m_seFileName)
			delete m_seFileName;
		if (m_pParameters)
			delete m_pParameters;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkURI
	//-------------------------------------------------------------------------------------------------------------------------------

	LinkURI::LinkURI(Object *pUriObject, StringExt *seBaseURI)
	{
		m_seURI = NULL;
		if (pUriObject->IsString())
		{
			StringExt *seTempURI = pUriObject->GetString()->Copy();
			if (seBaseURI && seBaseURI->GetLength() > 0)
			{
				int nCount = strcspn(seTempURI->GetBuffer(), "/:");
				if (nCount == seTempURI->GetLength() || seTempURI->GetAt(nCount) == '/')
				{
					m_seURI = seBaseURI->Copy();
					int nChar = m_seURI->GetAt(m_seURI->GetLength() - 1);
					if (nChar == '/' || nChar == '?')
					{
						if (seTempURI->GetAt(0) == '/')
						{
							seTempURI->Delete(0);
						}
					}
					else
					{
						if (seTempURI->GetAt(0) != '/')
						{
							m_seURI->Append('/');
						}
					}
					m_seURI->Append(seTempURI);
					delete seTempURI;
				}
				else
				{
					m_seURI = seTempURI;
				}
			}
			else
			{
				m_seURI = seTempURI;
			}
		}
		else
		{
			// TO DO: Error "Illegal URI-type link"
		}
	}

	LinkURI::~LinkURI()
	{
		if (m_seURI)
			delete m_seURI;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkNamed
	//-------------------------------------------------------------------------------------------------------------------------------

	LinkNamed::LinkNamed(Object *pNameObject)
	{
		m_seName = NULL;
		if (pNameObject->IsName())
		{
			m_seName = new StringExt(pNameObject->GetName());
		}
	}

	LinkNamed::~LinkNamed()
	{
		if (m_seName)
		{
			delete m_seName;
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkMovie
	//-------------------------------------------------------------------------------------------------------------------------------

	LinkMovie::LinkMovie(Object *pAnnotObject, Object *pTitleObject)
	{
		m_oAnnotRef.nNum = -1;
		m_seTitle = NULL;
		if (pAnnotObject->IsRef())
		{
			m_oAnnotRef = pAnnotObject->GetRef();
		}
		else if (pTitleObject->IsString())
		{
			m_seTitle = pTitleObject->GetString()->Copy();
		}
		else
		{
			// TO DO: Error "Movie action is missing both the Annot and T keys"
		}
	}

	LinkMovie::~LinkMovie()
	{
		if (m_seTitle)
		{
			delete m_seTitle;
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkUnknown
	//-------------------------------------------------------------------------------------------------------------------------------

	LinkUnknown::LinkUnknown(char *sAction)
	{
		m_seAction = new StringExt(sAction);
	}

	LinkUnknown::~LinkUnknown()
	{
		if (m_seAction)
			delete m_seAction;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Link
	//-------------------------------------------------------------------------------------------------------------------------------

	Link::Link(Dict *pDict, StringExt *seBaseURI)
	{
		m_pAction = NULL;
		m_bValid = false;

		// Rect
		Object oDictItem;
		if (!pDict->Search("Rect", &oDictItem)->IsArray())
		{
			// TO DO: Error "Annotation rectangle is wrong type"
			oDictItem.Free();
			return;
		}

		Object oTemp;
		if (!oDictItem.ArrayGet(0, &oTemp)->IsNum())
		{
			// TO DO: Error "Bad annotation rectangle"
			oTemp.Free();
			oDictItem.Free();
			return;
		}
		m_dLeft = oTemp.GetNum();
		oTemp.Free();

		if (!oDictItem.ArrayGet(1, &oTemp)->IsNum())
		{
			// TO DO: Error "Bad annotation rectangle"
			oTemp.Free();
			oDictItem.Free();
			return;
		}
		m_dBottom = oTemp.GetNum();
		oTemp.Free();

		if (!oDictItem.ArrayGet(2, &oTemp)->IsNum())
		{
			// TO DO: Error "Bad annotation rectangle"
			oTemp.Free();
			oDictItem.Free();
			return;
		}
		m_dRight = oTemp.GetNum();
		oTemp.Free();

		if (!oDictItem.ArrayGet(3, &oTemp)->IsNum())
		{
			// TO DO: Error "Bad annotation rectangle"
			oTemp.Free();
			oDictItem.Free();
			return;
		}
		m_dTop = oTemp.GetNum();
		oTemp.Free();
		oDictItem.Free();

		if (m_dLeft > m_dRight)
		{
			double dTemp = m_dLeft;
			m_dLeft = m_dRight;
			m_dRight = dTemp;
		}
		if (m_dBottom > m_dTop)
		{
			double dTemp = m_dBottom;
			m_dBottom = m_dTop;
			m_dTop = dTemp;
		}

		// Dest
		if (!pDict->Search("Dest", &oDictItem)->IsNull())
		{
			m_pAction = LinkAction::ParseDestination(&oDictItem);
		}
		else // A (Action)
		{
			oDictItem.Free();
			if (pDict->Search("A", &oDictItem)->IsDict())
			{
				m_pAction = LinkAction::ParseAction(&oDictItem, seBaseURI);
			}
		}
		oDictItem.Free();

		if (m_pAction)
		{
			m_bValid = true;
		}

		return;
	}

	Link::~Link()
	{
		if (m_pAction)
		{
			delete m_pAction;
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Links
	//-------------------------------------------------------------------------------------------------------------------------------

	Links::Links(Object *pAnnots, StringExt *seBaseURI)
	{
		m_ppLinks = NULL;
		m_nLinksCount = 0;

		int nSize = 0;

		if (pAnnots->IsArray())
		{
			for (int nIndex = 0; nIndex < pAnnots->ArrayGetLength(); ++nIndex)
			{
				Object oArrayItem;
				if (pAnnots->ArrayGet(nIndex, &oArrayItem)->IsDict())
				{
					Object oTemp;
					if (oArrayItem.DictLookup("Subtype", &oTemp)->IsName("Link"))
					{
						Link *pLink = new Link(oArrayItem.GetDict(), seBaseURI);
						if (pLink->CheckValidate())
						{
							if (m_nLinksCount >= nSize)
							{
								nSize += 16;
								m_ppLinks = (Link **)MemUtilsReallocArray(m_ppLinks, nSize, sizeof(Link *));
							}
							m_ppLinks[m_nLinksCount++] = pLink;
						}
						else
						{
							if (pLink)
								delete pLink;
						}
					}
					oTemp.Free();
				}
				oArrayItem.Free();
			}
		}
	}

	Links::~Links()
	{
		for (int nIndex = 0; nIndex < m_nLinksCount; ++nIndex)
		{
			if (m_ppLinks[nIndex])
				delete m_ppLinks[nIndex];
		}
		MemUtilsFree(m_ppLinks);
	}

	LinkAction *Links::Find(double dX, double dY)
	{
		for (int nIndex = m_nLinksCount - 1; nIndex >= 0; --nIndex)
		{
			if (m_ppLinks[nIndex]->InRect(dX, dY))
			{
				return m_ppLinks[nIndex]->GetAction();
			}
		}
		return NULL;
	}

	bool Links::OnLink(double dX, double dY)
	{
		for (int nIndex = 0; nIndex < m_nLinksCount; ++nIndex)
		{
			if (m_ppLinks[nIndex]->InRect(dX, dY))
				return true;
		}
		return false;
	}
}