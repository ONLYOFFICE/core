#pragma once
#include "..\ExObjRefAtom.h"
#include "..\TextBytesAtom.h"
#include "..\TextCharsAtom.h"
#include "..\TextHeaderAtom.h"
#include "..\PlaceHolderAtom.h"
#include "..\StyleTextPropAtom.h"
#include "..\OutlineTextRefAtom.h"
#include "..\InteractiveInfoAtom.h"
#include "..\TextInteractiveInfoAtom.h"
#include "..\..\Reader\Slide.h"
#include "Shape.h"
#include "ShapeProperties.h"
#include "..\..\Reader\ClassesAtom.h"


using namespace NSOfficeDrawing;

// один из самых главных классов
// он умеет отдавать интерфейс IElement...
// пока - это video, image, shape, text, color - наверное не надо
class CRecordShapeContainer : public CRecordsContainer
{
private:
	IStream* m_pStream;

public:

	RECT* m_pGroupBounds;
	RECT* m_pGroupClientAnchor;

public:
	
	CRecordShapeContainer()
	{
		m_pStream = NULL;

		m_pGroupBounds = NULL;
		m_pGroupClientAnchor = NULL;
	}

	~CRecordShapeContainer()
	{
		m_pStream = NULL;

		m_pGroupBounds = NULL;
		m_pGroupClientAnchor = NULL;
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_pStream = pStream;
		CRecordsContainer::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CRecordsContainer::ToString();
	}

	virtual void GetElement(IElement** ppElement, CSimpleMap<DWORD, CExFilesInfo*>* pMapIDs,
		long lSlideWidth, long lSlideHeight, CSlide* pSchemeSlide, CSlide* pMaster = NULL)
	{
		if (NULL == ppElement)
			return;

		*ppElement = NULL;

		CSimpleArray<CRecordShape*> oArrayShape;
		this->GetRecordsByType(&oArrayShape, true, true);

		if (0 == oArrayShape.GetSize())
			return;

		CSimpleArray<CRecordShapeProperties*> oArrayOptions;
		this->GetRecordsByType(&oArrayOptions, true, /*true*/false/*secondary & tetriary*/);
		
		if ((NULL != pMaster) && (oArrayOptions.GetSize() > 0))
		{
			// проверяем - что это за shape, вдруг из мастера...
			// тогда создавать ничего не нужно, просто надо взять из карты мастера
			bool bIsMaster = oArrayShape[0]->m_bHaveMaster;
			if (bIsMaster)
			{
				bool bIsFound = false;
				for (int i = 0; i < oArrayOptions[0]->m_oProperties.m_lCount; ++i)
				{
					if (bIsFound)
						break;

					if (hspMaster == oArrayOptions[0]->m_oProperties.m_arProperties[i].m_ePID)
					{
						LONG lID = oArrayOptions[0]->m_oProperties.m_arProperties[i].m_lValue;
						for (int j = 0; j < pMaster->m_arElements.GetSize(); ++j)
						{
							if (lID == pMaster->m_arElements[j]->m_lID)
							{
								LONG lNewType = GetTypeElem((SPT)oArrayShape[0]->m_oHeader.RecInstance);
								ElementType eElemType = pMaster->m_arElements[j]->m_etType;

								// вот может тип поменяться... тогда просто забиваем на мастера...
								if (lNewType == eElemType || ((lNewType = etPicture) && (eElemType == etVideo)))
								{							
									*ppElement = pMaster->m_arElements[j]->CreateDublicate();

									if ((etShape == (*ppElement)->m_etType) && (-1 != (*ppElement)->m_lPlaceholderPosition))
									{
										CShapeElement* pShapeEl = dynamic_cast<CShapeElement*>(*ppElement);
										if (NULL != pShapeEl)
										{
											pShapeEl->m_oShape.m_oText.m_sText.Empty();
										}
									}
								}
								
								bIsFound = true;
								break;
							}
						}
					}
				}
			}
			if (oArrayShape[0]->m_bBackground)
			{
				// аналог мастера
				if (pMaster->m_arElements.GetSize() > 0)
				{
					*ppElement = pMaster->m_arElements[0]->CreateDublicate();
				}
			}
		}
		
		PPTShapes::ShapeType eType = (PPTShapes::ShapeType)oArrayShape[0]->m_oHeader.RecInstance;
		IElement* pElem = *ppElement;

		BOOL bIsMasterShape = (NULL != pElem);

		if (NULL == pElem)
		{
			switch (eType)
			{
			//case sptMin:
			case sptMax:
			case sptNil:
				{
					break;
				}
			case sptPictureFrame:
				{
					CSimpleArray<CRecordExObjRefAtom*> oArrayEx;
					this->GetRecordsByType(&oArrayEx, true, true);

					CExFilesInfo::ExFilesType eType = CExFilesInfo::eftNone;

					CExFilesInfo oInfo;

					CString strPathPicture = _T("");

					// по умолчанию картинка (или оле объект)
					CExFilesInfo* pInfo = pMapIDs->Lookup(0xFFFFFFFF);
					if (NULL != pInfo)
					{
						oInfo = *pInfo;
						strPathPicture = oInfo.m_strFilePath;
					}

					if (0 != oArrayEx.GetSize())
					{
						CExFilesInfo* pInfo = pMapIDs->Lookup(oArrayEx[0]->m_nExObjID);
						if (NULL != pInfo)
						{
							oInfo = *pInfo;
						}
					}
					
					if (CExFilesInfo::eftVideo == oInfo.m_eType)
					{
						CVideoElement* pVideoElem	= new CVideoElement();
						pVideoElem->m_strFileName	= oInfo.m_strFilePath;

						VideoFile::IVideoFile3* pVideoFile = NULL;
						
						if (SUCCEEDED(CoCreateInstance(VideoFile::CLSID_CVideoFile3, NULL, CLSCTX_ALL, VideoFile::IID_IVideoFile3, (void**)(&pVideoFile))))
						{
							if (NULL != pVideoFile)
							{
								BSTR bsFile = pVideoElem->m_strFileName.AllocSysString();
								if (S_OK == pVideoFile->OpenFile(bsFile))
								{
									pVideoFile->get_videoDuration(&pVideoElem->m_dVideoDuration);

									CAudioElement* pAudioElem	= new CAudioElement();
									pAudioElem->m_bWithVideo	= true;

									pAudioElem->m_dAudioDuration	= pVideoElem->m_dVideoDuration;
									pAudioElem->m_strFileName		= pVideoElem->m_strFileName;

									pAudioElem->m_dStartTime	= pSchemeSlide->m_dStartTime;
									pAudioElem->m_dEndTime		= pSchemeSlide->m_dEndTime;
									
									pSchemeSlide->m_arElements.Add(pAudioElem);
								}
								else
								{
									// поуэрпоинт смотрит еще на директорию, где лежит файл
									SysFreeString(bsFile);
									
									CString strFile = oInfo.m_strPresentationDirectory + GetFileName(oInfo.m_strFilePath);
									bsFile = strFile.AllocSysString();

									if (S_OK == pVideoFile->OpenFile(bsFile))
									{
										pVideoElem->m_strFileName = strFile;
										
										pVideoFile->get_videoDuration(&pVideoElem->m_dVideoDuration);

										CAudioElement* pAudioElem	= new CAudioElement();
										pAudioElem->m_bWithVideo	= true;

										pAudioElem->m_dAudioDuration	= pVideoElem->m_dVideoDuration;
										pAudioElem->m_strFileName		= pVideoElem->m_strFileName;

										pAudioElem->m_dStartTime	= pSchemeSlide->m_dStartTime;
										pAudioElem->m_dEndTime		= pSchemeSlide->m_dEndTime;
										
										pSchemeSlide->m_arElements.Add(pAudioElem);
									}
								}
								SysFreeString(bsFile);

								RELEASEINTERFACE(pVideoFile);
							}
						}

						pElem = (IElement*)pVideoElem;
					}
					else if (CExFilesInfo::eftAudio == oInfo.m_eType)
					{
						CAudioElement* pAudioElem		= new CAudioElement();
						pAudioElem->m_strFileName		= oInfo.m_strFilePath;

						pAudioElem->m_dClipStartTime	= oInfo.m_dStartTime;
						pAudioElem->m_dClipEndTime		= oInfo.m_dEndTime;

						pAudioElem->m_bLoop				= oInfo.m_bLoop;

						VideoFile::IVideoFile3* pVideoFile = NULL;
						if (SUCCEEDED(CoCreateInstance(VideoFile::CLSID_CVideoFile3, NULL, CLSCTX_ALL, VideoFile::IID_IVideoFile3, (void**)(&pVideoFile))))
						{
							if (NULL != pVideoFile)
							{
								BSTR bsFile = pAudioElem->m_strFileName.AllocSysString();
								if (S_OK == pVideoFile->OpenFile(bsFile))
								{
									pVideoFile->get_audioDuration(&pAudioElem->m_dAudioDuration);						
								}
								else
								{
									// поуэрпоинт смотрит еще на директорию, где лежит файл
									SysFreeString(bsFile);
									
									CString strFile = oInfo.m_strPresentationDirectory + GetFileName(oInfo.m_strFilePath);
									bsFile = strFile.AllocSysString();

									if (S_OK == pVideoFile->OpenFile(bsFile))
									{
										pAudioElem->m_strFileName = strFile;
										pVideoFile->get_audioDuration(&pAudioElem->m_dAudioDuration);
									}
								}
								SysFreeString(bsFile);

								RELEASEINTERFACE(pVideoFile);
							}
						}

						//
						pAudioElem->m_dStartTime = pSchemeSlide->m_dStartTime;
						pAudioElem->m_dEndTime = pSchemeSlide->m_dEndTime;

						pSchemeSlide->m_arElements.Add(pAudioElem);

						CImageElement* pImageElem	= new CImageElement();
						pImageElem->m_strFileName	= strPathPicture;
						
						pElem = (IElement*)pImageElem;
						
						//
						//pElem = (IElement*)pAudioElem;
					}
					else
					{
						CImageElement* pImageElem	= new CImageElement();
						pImageElem->m_strFileName	= oInfo.m_strFilePath;
						
						pElem = (IElement*)pImageElem;
					}
					break;
				}
			/*case sptTextBox:
				{
					CTextElement* pText = NULL;
					CSimpleArray<CRecordTextCharsAtom*> oArrayText;
					this->GetRecordsByType(&oArrayText, true, true);
					if (0 != oArrayText.GetSize())
					{
						pText = new CTextElement();
						pText->m_oSettings.m_sText = oArrayText[0]->m_strText;

						pElem = (IElement*)pText;
						//break;
					}

					CSimpleArray<CRecordTextBytesAtom*> oArrayText2;
					this->GetRecordsByType(&oArrayText2, true, true);
					if (0 != oArrayText2.GetSize())
					{
						pText = new CTextElement();
						pText->m_oSettings.m_sText = (CStringW)oArrayText2[0]->m_strText;

						pElem = (IElement*)pText;
						//break;
					}
					
					if (NULL != pText)
					{
						CSimpleArray<CRecordStyleTextPropAtom*> oArrayText3;
						this->GetRecordsByType(&oArrayText3, true, true);
						if (0 != oArrayText3.GetSize())
						{
							pText->m_lOffsetTextStyle = oArrayText3[0]->m_lOffsetInStream;
							pText->m_pStream = m_pStream;
						}

						CSimpleArray<CRecordTextSpecInfoAtom*> oArrayText4;
						this->GetRecordsByType(&oArrayText4, true, true);
						if (0 != oArrayText4.GetSize())
						{
							pText->m_lOffsetTextProp = oArrayText4[0]->m_lOffsetInStream;
							pText->m_pStream = m_pStream;
						}
					}
					break;
				}*/
			default:
				{
					// shape
					CShapeElement* pShape = new CShapeElement(NSBaseShape::ppt, eType);
					if (TRUE)//if (/*отключим пока кастом*/OOXMLShapes::sptCustom != pShape->m_oShape.m_eType)
					{
						CExFilesInfo* pTextureInfo = pMapIDs->Lookup(0xFFFFFFFF);
						
						if (NULL != pTextureInfo)
						{
							pShape->m_oShape.m_oBrush.m_sTexturePath = pTextureInfo->m_strFilePath;
						}

						pElem = (IElement*)pShape;
					}
					else
					{
						delete pShape;
						pShape = NULL;
					}
					break;
				}
			};
		}

		if (NULL == pElem)
			return;

		pElem->m_lID = oArrayShape[0]->m_nID;
		
		CSimpleArray<CRecordClientAnchor*> oArrayAnchor;
		this->GetRecordsByType(&oArrayAnchor, true, true);

		if (0 != oArrayAnchor.GetSize())
		{
			pElem->m_rcBoundsOriginal.left		= (LONG)oArrayAnchor[0]->m_oBounds.Left;
			pElem->m_rcBoundsOriginal.top		= (LONG)oArrayAnchor[0]->m_oBounds.Top;
			pElem->m_rcBoundsOriginal.right		= (LONG)oArrayAnchor[0]->m_oBounds.Right;
			pElem->m_rcBoundsOriginal.bottom	= (LONG)oArrayAnchor[0]->m_oBounds.Bottom;
		}
		else
		{
			CSimpleArray<CRecordChildAnchor*> oArrayChildAnchor;
			this->GetRecordsByType(&oArrayChildAnchor, true, true);

			if (0 != oArrayChildAnchor.GetSize())
			{
				pElem->m_rcBoundsOriginal.left		= oArrayChildAnchor[0]->m_oBounds.left;
				pElem->m_rcBoundsOriginal.top		= oArrayChildAnchor[0]->m_oBounds.top;
				pElem->m_rcBoundsOriginal.right		= oArrayChildAnchor[0]->m_oBounds.right;
				pElem->m_rcBoundsOriginal.bottom	= oArrayChildAnchor[0]->m_oBounds.bottom;

				RecalcGroupShapeAnchor(pElem->m_rcBoundsOriginal);
			}
			else
			{			
				if (oArrayShape[0]->m_bBackground)
				{
					// здесь background
					pElem->m_rcBoundsOriginal.left		= 0;
					pElem->m_rcBoundsOriginal.top		= 0;
					pElem->m_rcBoundsOriginal.right		= lSlideWidth;
					pElem->m_rcBoundsOriginal.bottom	= lSlideHeight;
				}
				else
				{
					// не понятно...					
					pElem->m_rcBoundsOriginal.left		= 0;
					pElem->m_rcBoundsOriginal.top		= 0;
					pElem->m_rcBoundsOriginal.right		= 0;
					pElem->m_rcBoundsOriginal.bottom	= 0;

					delete pElem;
					return;
				}
			}
		}

		double dScaleX = /*(double)pSchemeSlide->m_lWidth / pSchemeSlide->m_lOriginalWidth*/c_dMasterUnitsToMillimetreKoef;
		double dScaleY = /*(double)pSchemeSlide->m_lHeight / pSchemeSlide->m_lOriginalHeight*/c_dMasterUnitsToMillimetreKoef;

		pElem->NormalizeCoords(dScaleX, dScaleY);
		
		pElem->m_bFlipH = oArrayShape[0]->m_bFlipH;
		pElem->m_bFlipV = oArrayShape[0]->m_bFlipV;
		
		// проверка на наличие текста
		CShapeElement* pShapeElem = dynamic_cast<CShapeElement*>(pElem);
		if (NULL != pShapeElem)
		{
			// запишем, если нужно, тип мастертекста
			if (bIsMasterShape)
			{
				pShapeElem->m_oShape.m_oText.m_lMasterTextType = pShapeElem->m_oShape.m_oText.m_lTextType;
			}

			pShapeElem->m_oShape.m_dWidthLogic  = ShapeSizeVML;
			pShapeElem->m_oShape.m_dHeightLogic = ShapeSizeVML;

			// проверка на textheader present
			CSimpleArray<CRecordTextHeaderAtom*> oArrayTextHeader;
			this->GetRecordsByType(&oArrayTextHeader, true, true);
			if (0 < oArrayTextHeader.GetSize())
			{
				pShapeElem->m_oShape.m_oText.m_lTextType = oArrayTextHeader[0]->m_nTextType;
			}
			else
			{
				pShapeElem->m_oShape.m_oText.m_lTextType = NSOfficePPT::NoPresent;
			}

			// проверка на ссылку в персист
			CSimpleArray<CRecordOutlineTextRefAtom*> oArrayTextRefs;
			this->GetRecordsByType(&oArrayTextRefs, true, true);
			if (0 < oArrayTextRefs.GetSize())
			{
				pElem->m_lPersistIndex = oArrayTextRefs[0]->m_nIndex;
			}
			
			// проверка на placeholder
			CSimpleArray<CRecordPlaceHolderAtom*> oArrayPlace;
			this->GetRecordsByType(&oArrayPlace, true, true);

			if (0 < oArrayPlace.GetSize())
			{
				pElem->m_lPlaceholderPosition = (LONG)(oArrayPlace[0]->m_nPosition);
				pElem->m_lPlaceholderID = (LONG)(oArrayPlace[0]->m_nPlacementID);

				// проверка на placeholder - ничего проверять не нужно, нужно
				// определить, надо ли добавлять
				LONG lCountPlaceHolders = (LONG)pSchemeSlide->m_arTextPlaceHolders.GetCount();
				if (/*(pElem->m_lPlaceholderPosition < 0) || */(pElem->m_lPlaceholderPosition >= lCountPlaceHolders) && (NULL == pMaster))
				{
					if (NULL != pMaster)
					{
						RELEASEOBJECT(pElem);
						*ppElement = pElem;
						return;
					}
					//DWORD dwTextType = pSchemeSlide->m_arTextPlaceHolders[pShapeElem->m_lPlaceholderPosition].m_nTextType;
					//pSchemeSlide->m_arTextPlaceHolders[pShapeElem->m_lPlaceholderPosition].ApplyProperties(pSchemeSlide, &(pShapeElem->m_pOptions->m_oText));
				}
			}

			// теперь сам текст...
			CSimpleArray<CRecordTextBytesAtom*> oArrayTextBytes;
			this->GetRecordsByType(&oArrayTextBytes, true, true);
			if (0 < oArrayTextBytes.GetSize())
			{
				pShapeElem->m_oShape.m_oText.m_sText = (CStringW)(oArrayTextBytes[0]->m_strText);
			}
			CSimpleArray<CRecordTextCharsAtom*> oArrayTextChars;
			this->GetRecordsByType(&oArrayTextChars, true, true);

			if (0 < oArrayTextChars.GetSize())
			{
				pShapeElem->m_oShape.m_oText.m_sText = oArrayTextChars[0]->m_strText;
			}
			
			// теперь настройки этого текста...
		
			// сначала - проперти
			for (int nIndexProp = 0; nIndexProp < oArrayOptions.GetSize(); ++nIndexProp)
			{
				pElem->SetUpProperties(&oArrayOptions[nIndexProp]->m_oProperties, pSchemeSlide);
			}

			if (!oArrayShape[0]->m_bHaveMaster)
			{
				pElem->m_lMasterID = -1;
			}

			CSimpleArray<CRecordStyleTextPropAtom*> oArrayTextStyle;
			this->GetRecordsByType(&oArrayTextStyle, true, true);
			if (0 != oArrayTextStyle.GetSize())
			{
				pShapeElem->m_lOffsetTextStyle = oArrayTextStyle[0]->m_lOffsetInStream;
				pShapeElem->m_pStream = m_pStream;
			}

			CSimpleArray<CRecordTextSpecInfoAtom*> oArrayTextProp;
			this->GetRecordsByType(&oArrayTextProp, true, true);
			if (0 != oArrayTextProp.GetSize())
			{
				pShapeElem->m_lOffsetTextProp = oArrayTextProp[0]->m_lOffsetInStream;
				pShapeElem->m_pStream = m_pStream;
			}

			CSimpleArray<CRecordTextRulerAtom*> oArrayTextRuler;
			this->GetRecordsByType(&oArrayTextRuler, true, true);
			if (0 != oArrayTextRuler.GetSize())
			{
				pShapeElem->m_oShape.m_oText.m_oRuler = oArrayTextRuler[0]->m_oTextRuler;
			}

			CSimpleArray<CRecordTextInteractiveInfoAtom*> oArrayTextInteractive;
			this->GetRecordsByType(&oArrayTextInteractive, true);
			if (0 != oArrayTextInteractive.GetSize())
			{
				pShapeElem->m_oActions.m_bPresent = true;

				int nSize = oArrayTextInteractive.GetSize();
				for (int i = 0; i < nSize; ++i)
				{
					CInteractiveInfo::CTextRange oRange;

					oRange.m_lStart = oArrayTextInteractive[i]->m_lStart;
					oRange.m_lEnd	= oArrayTextInteractive[i]->m_lEnd;

					pShapeElem->m_oActions.m_arRanges.Add(oRange);
				}
			}

			// теперь смотрим какой угол поворота. если он ближе к 90 и 270 чем 0 и 180 - то 
			// его надо повернуть обратно на 90 градусов.
			double dAngle = pShapeElem->m_dRotate;
			if (0 <= dAngle)
			{
				LONG lCount = (LONG)dAngle / 360;
				dAngle -= (lCount * 360.0);
			}
			else
			{
				LONG lCount = (LONG)dAngle / 360;
				dAngle += ((-lCount + 1) * 360.0);
			}
			if (((dAngle > 45) && (dAngle < 135)) || ((dAngle > 225) && (dAngle < 315)))
			{
				double dW = pShapeElem->m_rcBounds.GetWidth();
				double dH = pShapeElem->m_rcBounds.GetHeight();

				double dCx = (pShapeElem->m_rcBounds.left + pShapeElem->m_rcBounds.right) / 2.0;
				double dCy = (pShapeElem->m_rcBounds.top + pShapeElem->m_rcBounds.bottom) / 2.0;

				pShapeElem->m_rcBounds.left		= dCx - dH / 2.0;
				pShapeElem->m_rcBounds.right	= dCx + dH / 2.0;

				pShapeElem->m_rcBounds.top		= dCy - dW / 2.0;
				pShapeElem->m_rcBounds.bottom	= dCy + dW / 2.0;
			}						
		}
		else
		{
			for (int nIndexProp = 0; nIndexProp < oArrayOptions.GetSize(); ++nIndexProp)
			{
				pElem->SetUpProperties(&oArrayOptions[nIndexProp]->m_oProperties, pSchemeSlide);
			}

			if (!oArrayShape[0]->m_bHaveMaster)
			{
				pElem->m_lMasterID = -1;
			}
		}

		pElem->m_dStartTime = pSchemeSlide->m_dStartTime;
		pElem->m_dEndTime = pSchemeSlide->m_dEndTime;

		pElem->m_oMetric.SetUnitsContainerSize(pSchemeSlide->m_lOriginalWidth, pSchemeSlide->m_lOriginalHeight);
		pElem->m_bIsBackground = (true == oArrayShape[0]->m_bBackground);
		
		*ppElement = pElem;
	}

	void RecalcGroupShapeAnchor(RECT& rcChildAnchor)
	{
		if ((NULL == m_pGroupBounds) || (NULL == m_pGroupClientAnchor))
			return;

		// здесь переводим координаты, чтобы они не зависили от группы
		LONG lWidthClient = m_pGroupClientAnchor->right - m_pGroupClientAnchor->left;
		LONG lHeightClient = m_pGroupClientAnchor->bottom - m_pGroupClientAnchor->top;
		LONG lWidthGroup = m_pGroupBounds->right - m_pGroupBounds->left;
		LONG lHeightGroup = m_pGroupBounds->bottom - m_pGroupBounds->top;

		double dScaleX = (double)(lWidthClient) / (lWidthGroup);
		double dScaleY = (double)(lHeightClient) / (lHeightGroup);
		
		rcChildAnchor.left = m_pGroupClientAnchor->left + (LONG)(dScaleX * (rcChildAnchor.left - m_pGroupBounds->left));
		rcChildAnchor.right = m_pGroupClientAnchor->left + (LONG)(dScaleX * (rcChildAnchor.right - m_pGroupBounds->left));

		rcChildAnchor.top = m_pGroupClientAnchor->top + (LONG)(dScaleY * (rcChildAnchor.top - m_pGroupBounds->top));
		rcChildAnchor.bottom = m_pGroupClientAnchor->top + (LONG)(dScaleY * (rcChildAnchor.bottom - m_pGroupBounds->top));
	}

	LONG GetTypeElem(SPT eType)
	{
		switch (eType)
		{
		//case sptMin:
		case sptMax:
		case sptNil:
			{
				return -1;
			}
		case sptPictureFrame:
			{
				return etPicture;
			}
		default:
			{
				return etShape;
			}
		};
		return -1;
	}

	CString GetFileName(CString strFilePath)
	{
		int nIndex = strFilePath.ReverseFind(TCHAR('\\'));
		if (-1 != nIndex)
		{
			return strFilePath.Mid(nIndex + 1);
		}
		return strFilePath;
	}
};