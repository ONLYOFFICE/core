#include "stdafx.h"
#include "LayersManager.h"

#include "ATLDefine.h"

#include "mediaformatdefine.h"

bool  ImageEditor::Layers::CLayer::Load(const CString& filePath, int& errCode)
{
	errCode = 0;
	if (!m_app_manager)
		return false;

	ImageFile::IImageFile3Ptr imagePtr;
	imagePtr.CreateInstance ( ImageFile::CLSID_ImageFile3 );
	VARIANT_BOOL Success = VARIANT_TRUE;
	IUnknown* pUnknown = NULL;
	CComPtr<MediaCore::IAVSUncompressedVideoFrame> frame;
	
	ClearImageData();

	if (m_app_manager->GetProgresser())
		m_app_manager->GetProgresser()->StartProgress();
	if ( SUCCEEDED ( imagePtr->LoadImage2 ( filePath.AllocSysString(), &pUnknown, &Success ) ) )
	{
		if ( NULL != pUnknown )
		{
			pUnknown->QueryInterface( &frame );
			pUnknown->Release ();
			
			BYTE*  imData = NULL;
			frame->get_Buffer ( &imData );
			frame->get_Width ( &m_width[SLOW] );	
			frame->get_Height ( &m_height[SLOW] );
			long clSp = 0;
			frame->get_ColorSpace (&clSp);

			if (m_width[SLOW]*m_height[SLOW]>36000000 ||
				m_width[SLOW]>10000 || m_height[SLOW]>10000)
			{
				ClearImageData();
				if (m_app_manager->GetProgresser())
					m_app_manager->GetProgresser()->FinishProgress();
				errCode = 2;
				return false;
			}

			m_image_data[SLOW] = new BYTE[4*m_width[SLOW]*m_height[SLOW]];

			try
			{
				if (clSp & CSP_VFLIP)
				{
					for (long rowInd = 0; rowInd<m_height[SLOW]; rowInd++)
					{
						if (IsBadReadPtr(imData+4*m_width[SLOW]*(m_height[SLOW]-1-rowInd), 4*m_width[SLOW]))
						{	
							ClearImageData();
							if (m_app_manager->GetProgresser())
								m_app_manager->GetProgresser()->FinishProgress();
							return false;
						}
						memcpy(m_image_data[SLOW]+4*m_width[SLOW]*rowInd, imData+4*m_width[SLOW]*(m_height[SLOW]-1-rowInd), 4*m_width[SLOW]);
						if (m_app_manager->GetProgresser())
							m_app_manager->GetProgresser()->Progress(rowInd*100/m_height[SLOW]);
					}
				}
				else
				{
					memcpy(m_image_data[SLOW], imData, 4*m_width[SLOW]*m_height[SLOW]);
				}
			}
			catch(...)
			{
				ClearImageData();
				if (m_app_manager->GetProgresser())
					m_app_manager->GetProgresser()->FinishProgress();
				errCode = 3;
				return false;
			}

			{
				m_doubles_infos.RemoveAll();
				m_strings_infos.RemoveAll();
				long tmpLongVal = 0;
				double tmpDblVal = 0.0;
				imagePtr->get_JpegQuality(&tmpLongVal);
				tmpDblVal = (double)tmpLongVal;
				m_doubles_infos[_T("JpegQuality")] = tmpDblVal;

				imagePtr->get_TiffCompression(&tmpLongVal);
				tmpDblVal = (double)tmpLongVal;
				m_doubles_infos[_T("TiffCompression")] = tmpDblVal;

				imagePtr->get_BitmapType(&tmpLongVal);
				tmpDblVal = (double)tmpLongVal;
				m_doubles_infos[_T("BitmapType")] = tmpDblVal;

				/*BSTR  tmpBstrVal;
				imagePtr->get_InfoTitle(&tmpBstrVal);
				m_strings_infos["InfoTitle"] = CString(tmpBstrVal);
				imagePtr->get_InfoDescription(&tmpBstrVal);
				m_strings_infos["InfoDescription"] = CString(tmpBstrVal);
				imagePtr->get_InfoDateTime(&tmpBstrVal);
				m_strings_infos["InfoDateTime"] = CString(tmpBstrVal);
				imagePtr->get_InfoAuthor(&tmpBstrVal);
				m_strings_infos["InfoAuthor"] = CString(tmpBstrVal);
				imagePtr->get_InfoCopyright(&tmpBstrVal);
				m_strings_infos["InfoCopyright"] = CString(tmpBstrVal);
				imagePtr->get_InfoEquipmentType(&tmpBstrVal);
				m_strings_infos["InfoEquipmentType"] = CString(tmpBstrVal);
				imagePtr->get_InfoEquipmentModel(&tmpBstrVal);
				m_strings_infos["InfoEquipmentModel"] = CString(tmpBstrVal);*/

				imagePtr->get_InfoVerticalResolution(&tmpDblVal);
				if (tmpDblVal<0.0)
					tmpDblVal = 72.0;//(double)m_height[SLOW]/72.0;
				m_doubles_infos[_T("InfoVerticalResolution")] = tmpDblVal;
				imagePtr->get_InfoHorizontalResolution(&tmpDblVal);
				if (tmpDblVal<0.0)
					tmpDblVal = 72.0;//(double)m_width[SLOW]/72.0;
				m_doubles_infos[_T("InfoHorizontalResolution")] = tmpDblVal;

			}


			CComPtr<ImageFile::IImageMetaData> meta_data;
			imagePtr->QueryInterface ( &meta_data );
			BSTR tmpMD;
			meta_data->GetMetaData(&tmpMD);
			m_meta_data = CString(tmpMD);

			m_was_load = true;
		}
		else
		{
			if (m_app_manager->GetProgresser())
				m_app_manager->GetProgresser()->FinishProgress();
			errCode = 1;
			return false;
		}
	}
	else
	{
		if (m_app_manager->GetProgresser())
			m_app_manager->GetProgresser()->FinishProgress();
		errCode = 1;
		return false;
	}

	if (m_app_manager->GetProgresser())
		m_app_manager->GetProgresser()->FinishProgress();

	return true;
}

void   ImageEditor::Layers::CLayer::CalcHistogramm()
{
	m_histogramm.clear();

	BYTE* imData = m_image_data[THUMBNAIL];

	for(long y=0; y<m_height[THUMBNAIL]; y++)
	{
		for(long x=0; x<m_width[THUMBNAIL]; x++)
		{
			int  nIndex = 4*(x + y*m_width[THUMBNAIL]);

			int gray = (int)imData[nIndex+0]+(int)imData[nIndex+1]+(int)imData[nIndex+2];

			m_histogramm.m_values[m_histogramm.get_red_shift()+imData[nIndex+0]]++;
			m_histogramm.m_values[m_histogramm.get_green_shift()+imData[nIndex+1]]++;
			m_histogramm.m_values[m_histogramm.get_blue_shift()+imData[nIndex+2]]++;
			m_histogramm.m_values[m_histogramm.get_gray_shift()+(BYTE)(gray/3)]++;

		}
	}
}

void   ImageEditor::Layers::CLayer::ChangeImage(ILayer::IMAGE_TYPE imType, IChangesSubscriber* fromCommand, bool repaint, bool useOtherSubscribers)
{
	if (m_freeze)
	{
		m_freeze = false;
		return;
	}
	POSITION myPos = m_changes_subscribes.GetHeadPosition();
	while (myPos) 
	{
		IChangesSubscriber* cyrCS = m_changes_subscribes.GetNext(myPos);
		if (cyrCS!=fromCommand)
			if (!cyrCS->BeforeChangesHandler(imType))
				return;
	};

	bool  was_change_preview = false;
	switch(imType)
	{
	case SLOW:  // помен€ли самую большую начальную картинку
		{
			// затираем копию большой  картинки
			if (m_image_data[SLOW_COPY] && 
				(m_width[SLOW]!=m_width[SLOW_COPY] || m_height[SLOW]!=m_height[SLOW_COPY]))
			{
				delete[]  m_image_data[SLOW_COPY];
				m_image_data[SLOW_COPY] = NULL;
			}
			UpdateCopy(SLOW);

#ifdef _DEBUG
	#define    CRITICAL_IMAGE_SIZE     400
#else
	#define    CRITICAL_IMAGE_SIZE     500
#endif

			// расчитываем маленькую картинку
			if (m_image_data[FAST])
			{
				delete[]  m_image_data[FAST];
				m_image_data[FAST] = NULL;
			}

			bool noResize = (m_width[SLOW]<CRITICAL_IMAGE_SIZE && m_height[SLOW]<CRITICAL_IMAGE_SIZE);
			if (noResize)
			{
				m_width[FAST] = m_width[SLOW];
				m_height[FAST] = m_height[SLOW];
			}
			else
			{
				if (m_width[SLOW]>m_height[SLOW])
				{
					m_width[FAST] = CRITICAL_IMAGE_SIZE;
					/*for (int tmpI=2;tmpI<100;tmpI++)
						if ((m_width[SLOW]/tmpI)<CRITICAL_IMAGE_SIZE)
						{
							m_width[FAST] = m_width[SLOW]/tmpI;
							break;
						}*/
					double tmpVal = ((double)(m_height[SLOW]*m_width[FAST]))/((double)m_width[SLOW]);
					if (tmpVal<1.0)
						m_height[FAST] = 1;
					else
						m_height[FAST] = (long)tmpVal;
				}
				else
				{
					m_height[FAST] = CRITICAL_IMAGE_SIZE;
					/*for (int tmpI=2;tmpI<100;tmpI++)
						if ((m_height[SLOW]/tmpI)<CRITICAL_IMAGE_SIZE)
						{
							m_height[FAST] = m_height[SLOW]/tmpI;
							break;
						}*/
					double tmpVal = ((double)(m_width[SLOW]*m_height[FAST]))/((double)m_height[SLOW]);
					if (tmpVal<1.0)
						m_width[FAST] = 1;
					else
						m_width[FAST] = (long)tmpVal;
				}
			}
			m_image_data[FAST] = new BYTE[4*m_width[FAST]*m_height[FAST]];
			if (noResize)
			{
				memcpy(m_image_data[FAST], m_image_data[SLOW],4*m_width[FAST]*m_height[FAST]);
			}
			else
			{
				BGRA_TransformResize_(m_image_data[SLOW], m_width[SLOW], m_height[SLOW], m_image_data[FAST], m_width[FAST], m_height[FAST], 0, 16, (double)m_width[SLOW] / (double)m_height[SLOW], 0);
			}
		}
	case FAST:
		{
			// затираем копию маленькой  картинки
			if (m_image_data[FAST_COPY] && 
				(m_width[FAST]!=m_width[FAST_COPY] || m_height[FAST]!=m_height[FAST_COPY]))
			{
				delete[]  m_image_data[FAST_COPY];
				m_image_data[FAST_COPY] = NULL;
			}
			UpdateCopy(FAST);
		}
		if (m_image_data[THUMBNAIL])
			break;
	case FAST_COPY:
	case SLOW_COPY:
		{
			// пересчитываем тумбнейл и гистограмму
			if (!m_image_data[THUMBNAIL] || m_height[THUMBNAIL]==0 || m_width[THUMBNAIL]==0 ||
				fabs( ((double)m_width[FAST]/(double)m_height[FAST]) - ((double)m_width[THUMBNAIL]/(double)m_height[THUMBNAIL])  )>0.1)
			{
				int thm_sz = 200;
				if (m_width[FAST]>m_height[FAST])
				{
					if (m_width[SLOW]<200)
						thm_sz = m_width[SLOW];
					m_width[THUMBNAIL] = thm_sz;
					m_height[THUMBNAIL] = m_height[FAST]*m_width[THUMBNAIL]/m_width[FAST];
					if (m_height[THUMBNAIL]<1)
						m_height[THUMBNAIL] = 1;
				}
				else
				{
					if (m_height[SLOW]<200)
						thm_sz = m_height[SLOW];
					m_height[THUMBNAIL] = thm_sz;
					m_width[THUMBNAIL] = m_width[FAST]*m_height[THUMBNAIL]/m_height[FAST];
					if (m_width[THUMBNAIL]<1)
						m_width[THUMBNAIL] = 1;
				}
				if(m_image_data[THUMBNAIL])
					delete[] m_image_data[THUMBNAIL];
				m_image_data[THUMBNAIL] = new BYTE[4*m_width[THUMBNAIL]*m_height[THUMBNAIL]];
			}
			
			BGRA_TransformResize_(m_image_data[FAST_COPY], m_width[FAST_COPY], m_height[FAST_COPY], m_image_data[THUMBNAIL], m_width[THUMBNAIL], m_height[THUMBNAIL], 0, 0, (double)m_width[FAST_COPY] / m_height[FAST_COPY], 0);

			/************************************************************************/
			/* HISTOGRAMM CALULATION                                                */
			/************************************************************************/
			CalcHistogramm();
			was_change_preview = true;
		}
		break;
	default:
		break;
	}

	if (useOtherSubscribers)
	{
		POSITION myPos = m_changes_subscribes.GetHeadPosition();
		while (myPos) 
		{
			IChangesSubscriber* cyrCS = m_changes_subscribes.GetNext(myPos);
			if (cyrCS!=fromCommand)
				cyrCS->ChangesHandler(imType,repaint);
		};

		if (m_layer_manager_changes_subscriber && was_change_preview)
			m_layer_manager_changes_subscriber->ChangesHandler(imType,repaint);
	}
}

void   ImageEditor::Layers::CLayer::InvalidateImagePart(int lft, int tp, int rght, int bttom)
{
	if (!m_image_data[SLOW] || m_width[SLOW]<1 || m_height[SLOW]<1)
		return;

	long  realW = rght-lft;
	long  realH = bttom-tp;
	long  realX = lft;
	long  realY = tp;
	
	if (lft<0)
	{
		realW+=lft;
		realX=0;
	}
	if (tp<0)
	{
		realH+=tp;
		realY=0;
	}

	if (realX+realW>m_width[SLOW])
		realW=m_width[SLOW]-realX;

	if (realY+realH>m_height[SLOW])
		realH=m_height[SLOW]-realY;

	if (realW<0 || realH<0)
		return;

	BYTE*   partBuffer = new BYTE[realW*realH*4];

	for (int i=0; i<realH; i++)
		memcpy(partBuffer+i*realW*4, m_image_data[SLOW]+(i+realY)*m_width[SLOW]*4+realX*4, realW*4);


    m_app_manager->ApplyBackgroundCommands(partBuffer, realW, realH,false);

	for (int i=0; i<realH; i++)
		memcpy(m_image_data[SLOW_COPY]+(i+realY)*m_width[SLOW_COPY]*4+realX*4, partBuffer+i*realW*4, realW*4);

	delete[] partBuffer;

	if (m_width[FAST]==m_width[SLOW] && m_height[FAST]==m_height[SLOW])
	{
		memcpy(m_image_data[FAST], m_image_data[SLOW],4*m_width[FAST]*m_height[FAST]);
	}
	else
	{
		BGRA_TransformResize_(m_image_data[SLOW], m_width[SLOW], m_height[SLOW], m_image_data[FAST], m_width[FAST], m_height[FAST], 0, 16, (double)m_width[SLOW] / (double)m_height[SLOW], 0);
	}


	if (m_width[FAST_COPY]==m_width[SLOW_COPY] && m_height[FAST_COPY]==m_height[SLOW_COPY])
	{
		memcpy(m_image_data[FAST_COPY], m_image_data[SLOW_COPY],4*m_width[FAST_COPY]*m_height[FAST_COPY]);
	}
	else
	{
		BGRA_TransformResize_(m_image_data[SLOW_COPY], m_width[SLOW_COPY], m_height[SLOW_COPY], m_image_data[FAST_COPY], m_width[FAST_COPY], m_height[FAST_COPY], 0, 16, (double)m_width[SLOW_COPY] / (double)m_height[SLOW_COPY], 0);
	}

	BGRA_TransformResize_(m_image_data[FAST_COPY], m_width[FAST_COPY], m_height[FAST_COPY], m_image_data[THUMBNAIL], m_width[THUMBNAIL], m_height[THUMBNAIL], 0, 0, (double)m_width[FAST_COPY] / m_height[FAST_COPY], 0);
	CalcHistogramm();

	if (m_app_manager)
		m_app_manager->SendMsg(WM_IEM_UPDATE_THUMBNAIL, NULL);
}

bool    ImageEditor::Layers::CLayer::Undo(bool asReset)
{
	if(m_undo_blocks.GetCount()<1)
		return false;
	CHistoryBlock* curBlock = m_undo_blocks.GetTail();

	if (!curBlock || curBlock->m_elements.GetCount()<1)
		return false;

	POSITION myPos = curBlock->m_elements.GetTailPosition();

	while (myPos) 
	{
		CHistoryBlockElementBase* curBlockElement = curBlock->m_elements.GetPrev(myPos);
		if (!curBlockElement)
			continue;
		switch (curBlockElement->GetType())
		{
		case CHistoryBlockElementBase::ET_PART_OF_IMAGE:
			{
				CHistoryBlockElementImagePart* elem = reinterpret_cast<CHistoryBlockElementImagePart*>(curBlockElement);
				BYTE*  tempBuffer = new BYTE[elem->GetWidth()*4];
				for (long rowInd = elem->GetStartY(); rowInd<elem->GetStartY()+elem->GetHeight(); rowInd++)
				{
					memcpy(tempBuffer,
						m_image_data[SLOW]+rowInd*m_width[SLOW]*4+elem->GetStartX()*4,
						elem->GetWidth()*4);
					memcpy(m_image_data[SLOW]+rowInd*m_width[SLOW]*4+elem->GetStartX()*4,
						elem->GetData()+(rowInd-elem->GetStartY())*elem->GetWidth()*4, 
						elem->GetWidth()*4);
					memcpy(elem->GetData()+(rowInd-elem->GetStartY())*elem->GetWidth()*4, 
						tempBuffer,
						elem->GetWidth()*4);
				}
				delete[] tempBuffer;

			}
			break;
		case CHistoryBlockElementBase::ET_FULL_IMAGE:
			{
				CHistoryBlockElementImage* elem = reinterpret_cast<CHistoryBlockElementImage*>(curBlockElement);
				BYTE* tmpData = elem->GetData();
				long  tmpW = elem->GetWidth();
				long  tmpH = elem->GetHeight();
				elem->ResetData(m_image_data[SLOW], m_width[SLOW], m_height[SLOW]);
				m_image_data[SLOW] = tmpData;
				m_width[SLOW] = tmpW;
				m_height[SLOW] = tmpH;
				if (m_app_manager)
					m_app_manager->ResizeImage(tmpW, tmpH);
			}
			break;
		case CHistoryBlockElementBase::ET_EXTERNAL_HISTORY_BLOCK:
			{
				CExternalHistoryBlock* elem = reinterpret_cast<CExternalHistoryBlock*>(curBlockElement);
				elem->GetCallbacker()->OnUndo(1, asReset);
			}
			break;
		default:
			break;
		}
	};

	m_redo_blocks.AddHead(m_undo_blocks.RemoveTail());

	return true;
}

bool    ImageEditor::Layers::CLayer::Redo()
{
	if(m_redo_blocks.GetCount()<1)
		return false;
	CHistoryBlock* curBlock = m_redo_blocks.GetHead();

	if (!curBlock || curBlock->m_elements.GetCount()<1)
		return false;

	POSITION myPos = curBlock->m_elements.GetHeadPosition();

	while (myPos) 
	{
		CHistoryBlockElementBase* curBlockElement = curBlock->m_elements.GetNext(myPos);
		if (!curBlockElement)
			continue;
		switch (curBlockElement->GetType())
		{
		case CHistoryBlockElementBase::ET_PART_OF_IMAGE:
			{
				CHistoryBlockElementImagePart* elem = reinterpret_cast<CHistoryBlockElementImagePart*>(curBlockElement);
				BYTE*  tempBuffer = new BYTE[elem->GetWidth()*4];
				for (long rowInd = elem->GetStartY(); rowInd<elem->GetStartY()+elem->GetHeight(); rowInd++)
				{
					memcpy(tempBuffer,
						m_image_data[SLOW]+rowInd*m_width[SLOW]*4+elem->GetStartX()*4,
						elem->GetWidth()*4);
					memcpy(m_image_data[SLOW]+rowInd*m_width[SLOW]*4+elem->GetStartX()*4,
						elem->GetData()+(rowInd-elem->GetStartY())*elem->GetWidth()*4, 
						elem->GetWidth()*4);
					memcpy(elem->GetData()+(rowInd-elem->GetStartY())*elem->GetWidth()*4, 
						tempBuffer,
						elem->GetWidth()*4);
				}
				delete[] tempBuffer;

			}
			break;
		case CHistoryBlockElementBase::ET_FULL_IMAGE:
			{
				CHistoryBlockElementImage* elem = reinterpret_cast<CHistoryBlockElementImage*>(curBlockElement);
				BYTE* tmpData = elem->GetData();
				long  tmpW = elem->GetWidth();
				long  tmpH = elem->GetHeight();
				elem->ResetData(m_image_data[SLOW], m_width[SLOW], m_height[SLOW]);
				m_image_data[SLOW] = tmpData;
				m_width[SLOW] = tmpW;
				m_height[SLOW] = tmpH;
				if (m_app_manager)
					m_app_manager->ResizeImage(tmpW, tmpH);
			}
			break;
		case CHistoryBlockElementBase::ET_EXTERNAL_HISTORY_BLOCK:
			{
				CExternalHistoryBlock* elem = reinterpret_cast<CExternalHistoryBlock*>(curBlockElement);
				elem->GetCallbacker()->OnRedo(1);

			}
			break;
		default:
			break;
		}
	};

	m_undo_blocks.AddTail(m_redo_blocks.RemoveHead());

	return true;
}


// »зменить часть картинки - 
// imPart - буфер с новым участком
// wdth, hgth - его размеры
// startX, startY - положение левого верхнего угла нового куска на слое
bool  ImageEditor::Layers::CLayer::ChangePartOfLayer(BYTE* imPart, long wdth, long hght, long startX, long startY) 
{
	if (!imPart || !m_was_load || !m_image_data[SLOW] || startX>=m_width[SLOW] || startY>=m_height[SLOW])
		return false;
	CHistoryBlock* curBlock = GetCurrentBlock();
	if (!curBlock)
		return false;

	long  usingW = wdth;
	long  usingH = hght;
	long  usingSrcX = 0;
	long  usingSrcY = 0;
	long  usingDstX = startX;
	long  usingDstY = startY;

	if (startX<0)
	{
		usingW+=startX;
		usingSrcX=-startX;
	}
	if (startY<0)
	{
		usingH+=startY;
		usingSrcY=-startY;
	}

	usingDstX = max(0,startX);
	usingDstY = max(0,startY);

	if (usingDstX+usingW>m_width[SLOW])
		usingW-=(usingDstX+usingW)-m_width[SLOW];

	if (usingDstY+usingH>m_height[SLOW])
		usingH-=(usingDstY+usingH)-m_height[SLOW];

	if (usingH<0 || usingW<0)
		return false;

	BYTE* savingData = new BYTE[4*usingW*usingH];

	for (long rowInd = 0; rowInd<usingH; rowInd++)
	{
		memcpy(savingData+rowInd*usingW*4, 
			m_image_data[SLOW]+(rowInd+usingDstY)*m_width[SLOW]*4+usingDstX*4,
			usingW*4);
		memcpy(m_image_data[SLOW]+(rowInd+usingDstY)*m_width[SLOW]*4+usingDstX*4,
			imPart+(rowInd+usingSrcY)*wdth*4+usingSrcX*4, 
			usingW*4);
	}

	curBlock->AddImagePart(savingData, usingW, usingH, usingDstX,usingDstY);

	return true;
}

// »зменить часть картинки - 
// imPart - буфер с новым участком
// shiftX, shiftY , wdth, hgth - смещение и размеры нового учаска на большем массиве
// scrBufferWidth - ширина большого буфера
// startX, startY - положение левого верхнего угла нового куска на слое
bool  ImageEditor::Layers::CLayer::ChangePartOfLayer(BYTE* imPart, long shiftSrcX, long shiftSrcY, long scrBufferWidth, long wdth, long hght, long startX, long startY)
{
	if (!imPart || !m_was_load || !m_image_data[SLOW] || startX>=m_width[SLOW] || startY>=m_height[SLOW])
		return false;
	CHistoryBlock* curBlock = GetCurrentBlock();
	if (!curBlock)
		return false;

	long  usingW = wdth;
	long  usingH = hght;
	long  usingSrcX = 0;
	long  usingSrcY = 0;
	long  usingDstX = startX;
	long  usingDstY = startY;

	if (startX<0)
	{
		usingW+=startX;
		usingSrcX=-startX;
	}
	if (startY<0)
	{
		usingH+=startY;
		usingSrcY=-startY;
	}

	usingDstX = max(0,startX);
	usingDstY = max(0,startY);

	if (usingDstX+usingW>m_width[SLOW])
		usingW-=(usingDstX+usingW)-m_width[SLOW];

	if (usingDstY+usingH>m_height[SLOW])
		usingH-=(usingDstY+usingH)-m_height[SLOW];

	if (usingH<0 || usingW<0)
		return false;

	BYTE* savingData = new BYTE[4*usingW*usingH];

	for (long rowInd = 0; rowInd<usingH; rowInd++)
	{
		memcpy(savingData+rowInd*usingW*4, 
			m_image_data[SLOW]+(rowInd+usingDstY)*m_width[SLOW]*4+usingDstX*4,
			usingW*4);
		memcpy(m_image_data[SLOW]+(rowInd+usingDstY)*m_width[SLOW]*4+usingDstX*4,
			imPart+(shiftSrcY*scrBufferWidth+shiftSrcX)*4+(rowInd+usingSrcY)*scrBufferWidth*4+usingSrcX*4, 
			usingW*4);
	}

	curBlock->AddImagePart(savingData, usingW, usingH, usingDstX,usingDstY);

	return true;
}

// »зменить полностью слой - с новыми размерами
// imPart - буфер новым слоем
// wdth, hgth - его размеры
// !!!! ≈сли надо изменить слой полностью, но без изменени€ размеров - то лчше использовать предыдущую функцию!!!!
bool  ImageEditor::Layers::CLayer::ChangeAllLayer(BYTE* imPart, long wdth, long hght)
{
	if (!imPart || !m_was_load || !m_image_data[SLOW] || wdth<0 || hght<0 )
		return false;
	CHistoryBlock* curBlock = GetCurrentBlock();
	if (!curBlock)
		return false;

	curBlock->AddImage(m_image_data[SLOW], m_width[SLOW], m_height[SLOW]);

	m_image_data[SLOW] = imPart;
	m_width[SLOW] = wdth;
	m_height[SLOW] = hght;

	if (m_app_manager)
		m_app_manager->ResizeImage(wdth, hght);

	return true;
}


bool  ImageEditor::Layers::CLayer::ChangeManually(IUndoRedoCallback* hist_blocl)
{
	CHistoryBlock* curBlock = GetCurrentBlock();
	if (!curBlock || !hist_blocl)
		return false;

	curBlock->AddExternalBlock(hist_blocl);

	return true;
}



Gdiplus::Bitmap* BitmapFromResource(HINSTANCE hInstance, LPCTSTR szResName, LPCTSTR szResType);

void  ImageEditor::Layers::CLayersManager::LoadNonActivateBitmaps()
{
	for (int i=0;i<3;i++)
		RELEASEOBJECT(m_pNonActivateBitmaps[i]);
	for (int i=0;i<3;i++)
	   m_pNonActivateBitmaps[i] = BitmapFromResource(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IBD_NON_ACTIV_BIG+i), _T("PNG"));
}

bool  ImageEditor::Layers::CLayersManager::Save(const CString& filePath, CWatermarks* watermarks)
{
	CLayer*  curLayer = m_layers.GetHead();
	if (!curLayer || !curLayer->m_was_load)
		return false;

	ILayer::IMAGE_TYPE imTp = ILayer::SLOW_COPY;//curLayer->GetActualImage();

	return SaveData(filePath, curLayer->m_image_data[imTp],curLayer->m_width[imTp],curLayer->m_height[imTp],true, watermarks,curLayer->m_meta_data);
}

bool ImageEditor::Layers::CLayersManager::SaveData(const CString& filePath, BYTE* imData, long width, long hght, bool flip, CWatermarks* watermarks,CString &MetaDataXML)
{
	VARIANT_BOOL Success = VARIANT_TRUE;

	long saveFormat = IMAGEFORMAT_PNG;
	bool bRemoveAlpha = false;

	CString  ext = filePath.Right(3);
	ext.MakeLower();

	if (ext=="bmp")
	{
		saveFormat = IMAGEFORMAT_BMP;
		bRemoveAlpha = true;
	}
	else
	if (ext=="gif")
	{
		saveFormat = IMAGEFORMAT_GIF;
		bRemoveAlpha = true;
	}
	else
	if (ext=="jpg" || ext=="peg")
	{
		saveFormat = IMAGEFORMAT_JPE;
		bRemoveAlpha = true;
	}
	else
	if (ext=="png")
	{
		saveFormat = IMAGEFORMAT_PNG;
		bRemoveAlpha = false;
	}
	else
	if (ext=="tif" || ext=="iff")
	{
		saveFormat = IMAGEFORMAT_TIF;
		bRemoveAlpha = false;
	}
	else
	if (ext=="ras")
	{
		saveFormat = IMAGEFORMAT_RAS;
		bRemoveAlpha = true;
	}
	else
	if (ext=="tga")
	{
		saveFormat = IMAGEFORMAT_TGA;
		bRemoveAlpha = false;
	}
	else
	if (ext=="pcx")
	{
		saveFormat = IMAGEFORMAT_PCX;
		bRemoveAlpha = true;
	}
	else
	if (ext=="wmf")
	{
		saveFormat = IMAGEFORMAT_WMF;
		bRemoveAlpha = true;
	}
	else
	if (ext=="emf")
	{
		saveFormat = IMAGEFORMAT_EMF;
		bRemoveAlpha = true;
	}

	CString filePath2 = filePath;
	if (saveFormat == IMAGEFORMAT_PNG && ext!="png")
		filePath2 += ".png";


	MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;

	CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
	if (NULL == pMediaData)
		return false;

	ILayer::IMAGE_TYPE imTp = ILayer::SLOW_COPY;//curLayer->GetActualImage();

	// specify settings
	pMediaData->put_ColorSpace( CSP_BGRA | CSP_VFLIP );
	pMediaData->put_Width(width);
	pMediaData->put_Height(hght);
	pMediaData->put_AspectRatioX(width);
	pMediaData->put_AspectRatioY(hght);
	pMediaData->put_Interlaced(VARIANT_FALSE);
	pMediaData->put_Stride(0, 4*width);
	pMediaData->AllocateBuffer(4*width*hght);

	// allocate necesasry buffer
	BYTE* pBufferPtr = 0;
	long nCreatedBufferSize = 0;
	pMediaData->get_Buffer(&pBufferPtr);
	pMediaData->get_BufferSize(&nCreatedBufferSize);
	pMediaData->put_Plane(0, pBufferPtr);

	if( bRemoveAlpha )
	{
		BYTE* src = imData;
		BYTE* dst = pBufferPtr;
		int size = width * hght;
		
		for( int i = 0; i < size; ++i, src += 4, dst += 4 )
		{
			DWORD alpha = src[3];
			if( alpha < 255 )
			{
				DWORD offset = 255 * (255 - alpha) + 127;
				offset *= 0x8081U;
				alpha  *= 0x8081U;
				
				dst[0] = (BYTE)((src[0] * alpha + offset) >> 23);
				dst[1] = (BYTE)((src[1] * alpha + offset) >> 23);
				dst[2] = (BYTE)((src[2] * alpha + offset) >> 23);
				dst[3] = 255;
				continue;
			}

			*(DWORD*)dst = *(DWORD*)src;
		}
	}
	else
	{
		memcpy( pBufferPtr, imData, width * hght * 4 );
	}


	if (watermarks)
	{
		watermarks->DrawOnData(pBufferPtr,width,hght);
	}

	DrawLogo(&pMediaData, false);


	ImageFile::IImageFile3Ptr imagePtr;

	imagePtr.CreateInstance ( ImageFile::CLSID_ImageFile3 );

	imagePtr->put_BitmapType(0);
	BSTR fP = filePath2.AllocSysString();
//////////////////////////////////////////////////////////////////////////////
	if (MetaDataXML.GetLength()>1)
	{
		ImageFile::IImageMetaData* piMetaData = NULL;
		if( SUCCEEDED( imagePtr.QueryInterface( __uuidof( ImageFile::IImageMetaData ), (void**)&piMetaData ) ) )
		{
			piMetaData->SetMetaData(MetaDataXML.AllocSysString());
		}
		RELEASEINTERFACE( piMetaData );
	}
///////////////////////////////////////////////////////////////////////////////
	bool ret = (bool)( SUCCEEDED ( imagePtr->SaveImage2 ( (IUnknown**)&pMediaData, saveFormat, fP, &Success ) ) );
	SysFreeString(fP);
	RELEASEINTERFACE(pMediaData);
	return ret;
	
}

bool ImageEditor::Layers::CLayersManager::DrawLogo(MediaCore::IAVSUncompressedVideoFrame** pMediaData, bool bPrint)
{
	//добавл€ем текст дл€ незарегистрированной версии
		//1024х683 unregistered : 60 Arial bold #000000 alpha - 70 Edge: Size-5 #000000 alpha - 80
		// avs4you : 50 Arial bold #000000 alpha - 70 Edge: Size-5 #000000 alpha - 80
	if (!pMediaData || !(*pMediaData))
		return false;

	if( bPrint )
	{
		if ( GetRegCheck() )
			return false;
	}

	//  FLIP
	{
		long width; (*pMediaData)->get_Width( &width );
		long hght;  (*pMediaData)->get_Height( &hght );
		BYTE* pBufferPtr; (*pMediaData)->get_Buffer(&pBufferPtr);
		
		if( pBufferPtr )
		{
			int nLineSize = 4*width;
			BYTE* pBGRALineCopy = new BYTE[nLineSize];

			BYTE* pBGRALineTop = pBufferPtr;
			BYTE* pBGRALineBottom = pBufferPtr + nLineSize*(hght - 1);

			for (int nY = 0; nY < hght/2; ++nY)
			{
				memcpy(pBGRALineCopy, pBGRALineTop, nLineSize);
				memcpy(pBGRALineTop, pBGRALineBottom, nLineSize);
				memcpy(pBGRALineBottom, pBGRALineCopy, nLineSize);

				pBGRALineTop += nLineSize;
				pBGRALineBottom -= nLineSize;
			}

			delete[] pBGRALineCopy;
		}
	}

	if ( GetRegCheck() )
	{
		return false;
	}

	/*VectorImage::IRasterizer* piVectorImage = NULL;
	HRESULT hRes = CoCreateInstance( VectorImage::CLSID_Rasterizer, NULL, CLSCTX_INPROC, VectorImage::IID_IRasterizer, (void**)(&piVectorImage));

	VARIANT_BOOL scs;

	piVectorImage->Open(CString("D:\\Projects\\wmarks\\00.svg"),&scs);


	RELEASEINTERFACE(piVectorImage);*/


	long wdth = 0;
	long hght = 0;
	(*pMediaData)->get_Width(&wdth);
	(*pMediaData)->get_Height(&hght);

	if (!wdth || !hght)
		return false;

		double dRatio = (double)hght / 683;
		if( wdth * 683 < hght * 1024 )
			dRatio = (double)wdth / 1024;

		int nEdgeSize = (int)(5 * dRatio);
		if( nEdgeSize < 2 )
			nEdgeSize = 2;
		int nLine1Size = (int)(60 * dRatio);
		if( nLine1Size < 6 )
			nLine1Size = 6;
		int nLine1Left = 0;
		int nLine1Top = 0;
		int nLine1Right = wdth;
		int nLine1Bottom = hght / 2;
		int nLine2Size = (int)(50 * dRatio);
		if( nLine2Size < 8 )
			nLine2Size = 8;
		int nLine2Left = 0;
		int nLine2Top = hght / 2;
		int nLine2Right = wdth;
		int nLine2Bottom = hght;
		if( (nLine1Right - nLine1Left) > 90 || (nLine2Right - nLine2Left) > 90 )
		{
			CString sLogoTransform;
			sLogoTransform.AppendFormat(_T("<ImagePaint-DrawAnimatedText rect-left=\"%d\" rect-top=\"%d\" rect-right=\"%d\" rect-bottom=\"%d\" metric=\"0\" text=\"UNREGISTERED VERSION\" text-typeeffects=\"0\" text-fillmode=\"0\" font-italic=\"0\" font-underline=\"0\" font-strikeout=\"0\" font-angle=\"0\" font-stringalignmenthorizontal=\"1\" font-stringalignmentvertical=\"2\" marginhorizontal=\"0\" marginvertical=\"0\" brush-color2=\"16777215\" brush-alpha2=\"179\" brush-texturealpha=\"255\" brush-texturepath=\"\" brush-texturemode=\"1\" brush-rectable=\"0\" brush-rect-left=\"0\" brush-rect-top=\"0\" brush-rect-width=\"0\" brush-rect-height=\"0\" shadow-distancex=\"3\" shadow-distancey=\"3\" shadow-blursize=\"3\" shadow-color=\"0\" shadow-alpha=\"120\" brush-type=\"0\" edge-visible=\"-1\" font-name=\"Arial\" brush-color1=\"16777215\" shadow-visible=\"0\" brush-alpha1=\"179\" edge-color=\"0\" edge-dist=\"%d\" edge-alpha=\"204\" font-bold=\"-1\" font-size=\"%d\"/>"), nLine1Left, nLine1Top, nLine1Right, nLine1Bottom, nEdgeSize, nLine1Size);
			sLogoTransform.AppendFormat(_T("<ImagePaint-DrawAnimatedText rect-left=\"%d\" rect-top=\"%d\" rect-right=\"%d\" rect-bottom=\"%d\" metric=\"0\" text=\"www.avs4you.com\" text-typeeffects=\"0\" text-fillmode=\"0\" font-italic=\"0\" font-underline=\"0\" font-strikeout=\"0\" font-angle=\"0\" font-stringalignmenthorizontal=\"1\" font-stringalignmentvertical=\"0\" marginhorizontal=\"0\" marginvertical=\"0\" brush-color2=\"16777215\" brush-alpha2=\"179\" brush-texturealpha=\"255\" brush-texturepath=\"\" brush-texturemode=\"1\" brush-rectable=\"0\" brush-rect-left=\"0\" brush-rect-top=\"0\" brush-rect-width=\"0\" brush-rect-height=\"0\" shadow-distancex=\"3\" shadow-distancey=\"3\" shadow-blursize=\"3\" shadow-color=\"0\" shadow-alpha=\"120\" brush-type=\"0\" edge-visible=\"-1\" font-name=\"Arial\" brush-color1=\"16777215\" shadow-visible=\"0\" brush-alpha1=\"179\" edge-color=\"0\" edge-dist=\"%d\" edge-alpha=\"204\" font-bold=\"-1\" font-size=\"%d\"/>"), nLine2Left, nLine2Top, nLine2Right, nLine2Bottom, nEdgeSize,nLine2Size);

			ImageStudio3::IImageTransforms* piTransform = m_app_manager->GetImageStudio();//NULL;
			//HRESULT hRes = CoCreateInstance( ImageStudio3::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio3::IID_IImageTransforms, (void**)(&piTransform));

			//if (hRes==S_OK)
			{
				VARIANT vImage;
				vImage.vt = VT_UNKNOWN;
				vImage.punkVal =  *pMediaData;
				//(*pMediaData)->put_ColorSpace( CSP_BGRA );
				piTransform->SetSource(0, vImage);

				CString sFullTransform =_T("<transforms>");
				sFullTransform.Append( sLogoTransform );
				sFullTransform.Append(_T("</transforms>"));

				BSTR bstrSave= sFullTransform.AllocSysString();
				VARIANT_BOOL scss;
				piTransform->SetXml( bstrSave,&scss );
				piTransform->Transform(&scss);
				piTransform->GetResult( 0, &vImage );
				SysFreeString( bstrSave );

				RELEASEINTERFACE( (*pMediaData) );
				vImage.punkVal->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)pMediaData );
				//(*pMediaData)->put_ColorSpace( CSP_BGRA | CSP_VFLIP );
				RELEASEINTERFACE((vImage.punkVal));
				//RELEASEINTERFACE(piTransform);


			}
		}

		return true;
}









#include "ViewManager.h"

void  ImageEditor::CWatermarks::Draw(Gdiplus::Graphics* pGr)
{
	//Gdiplus::StringFormat format;
	//format.SetAlignment(StringAlignmentCenter);
	//format.SetLineAlignment(StringAlignmentCenter);
	//Gdiplus::Font font(L"Arial", 10, FontStyleBold);

	//Gdiplus::SolidBrush		oBlackSolidBrush ( Gdiplus::Color::Black );

	/*pGr->DrawString(L"Watermark Text", -1, &font, m_rects[0]->GetScreenRect(), 
		&format, &oBlackSolidBrush);
	pGr->DrawString(L"Watermark Image", -1, &font, m_rects[1]->GetScreenRect(), 
		&format, &oBlackSolidBrush);*/

	for (int i=0; i<2; i++)
	{
		if (m_pBitmaps[i] && m_rects[i]->m_isVisible)
		{
			pGr->DrawImage(m_pBitmaps[i], m_rects[i]->GetScreenRect());
			if (m_edit_regime)
				m_rects[i]->Draw(pGr);
		}
	}
}

void  ImageEditor::CWatermarks::DrawOnData(BYTE* imData, long wdth, long hght)
{
	// check draw data
	bool bDrawData[2] = {
		(m_pBitmaps[0] && m_rects[0]->m_isVisible),
		(m_pBitmaps[1] && m_rects[1]->m_isVisible)
	};

	int nMinX, nMinY;
	int nMaxX, nMaxY;

	// calculate draw rect
	if( bDrawData[0] )	
	{
		nMinX = m_rects[0]->m_x;
		nMinY = m_rects[0]->m_y;
		nMaxX = m_rects[0]->m_x + m_rects[0]->m_w;
		nMaxY = m_rects[0]->m_y + m_rects[0]->m_h;
	
		if( bDrawData[1] )
		{
			if( nMinX > m_rects[1]->m_x ) nMinX = m_rects[1]->m_x;
			if( nMinY > m_rects[1]->m_y ) nMinY = m_rects[1]->m_x;
			if( nMaxX < (m_rects[1]->m_x + m_rects[1]->m_w) ) nMaxX = m_rects[1]->m_x + m_rects[1]->m_w;
			if( nMaxY < (m_rects[1]->m_y + m_rects[1]->m_h) ) nMaxY = m_rects[1]->m_y + m_rects[1]->m_h;
		}
	}
	else
	if( bDrawData[1] )
	{
		nMinX = m_rects[1]->m_x;
		nMinY = m_rects[1]->m_y;
		nMaxX = m_rects[1]->m_x + m_rects[1]->m_w;
		nMaxY = m_rects[1]->m_y + m_rects[1]->m_h;
	}
	else
	{
		return;
	}

	if( nMinX < 0 ) nMinX = 0;
	if( nMinY < 0 ) nMinY = 0;
	if( nMaxX > wdth ) nMaxX = wdth;
	if( nMaxY > hght ) nMaxY = hght;

	int nWidth  = nMaxX - nMinX;
	int nHeight = nMaxY - nMinY;

	if( nWidth <= 0 || nHeight <= 0 )
		return;
	
	// draw watermarks
	Gdiplus::Bitmap oImage( nWidth, nHeight, wdth * 4, PixelFormat32bppARGB, imData + (nMinY * wdth + nMinX) * 4 );
	if( Gdiplus::Ok == oImage.GetLastStatus() )
	{
		Gdiplus::Graphics oRender( &oImage );
		if( Gdiplus::Ok == oRender.GetLastStatus() )
		{
			for (int i=0; i<2; i++)
			{
				if (bDrawData[i])
				{
					oRender.DrawImage(m_pBitmaps[i], m_rects[i]->m_x - nMinX, m_rects[i]->m_y - nMinY, m_rects[i]->m_w, m_rects[i]->m_h);
				}
			}
		}
	}
}

void  ImageEditor::CWatermarks::SetImageSizes(int szX, int szY)
{
	for (int i=0; i<2; i++)
	{
		if (m_rects[i]->m_w==0 || m_rects[i]->m_h==0)
		{
			m_rects[i]->SetPos(0, 0, szX/2, szY/2);
		}
		m_rects[i]->RecalcSizes(szX, szY);
	}
}

bool ImageEditor::CWatermarks::OnMouseDown(LONG nX, LONG nY, LONG nButton)
{
	if (m_edit_regime)
      for (int i=0; i<2; i++)
		if (m_rects[i]->OnMouseDown( nX, nY, nButton))
			return true;

	return false;
}

bool ImageEditor::CWatermarks::OnMouseUp(LONG nX, LONG nY, LONG nButton)
{
	if (m_edit_regime)
     for (int i=0; i<2; i++)
	 {
		if (m_rects[i]->OnMouseUp( nX, nY, nButton))
			{
				if (i==0)
				{
					BSTR tmpBs = m_xmls[i].AllocSysString();
					SetXML((i==0)?CString("WatermarkText"):CString("WatermarkImage"),&tmpBs );
					SysFreeString(tmpBs);
				}
				return true;
			}
			
	 }

	return false;
}

bool ImageEditor::CWatermarks::OnMouseMove(LONG nX, LONG nY, LONG nButton)
{
	if (m_edit_regime)
      for (int i=0; i<2; i++)
	  {
		if (m_rects[i]->OnMouseMove( nX, nY, nButton))
		{
			return true;
		}
	  }

	return false;
}

void  ImageEditor::CWatermarks::SetParam(const CString& paramName, double param)
{
	if (paramName==CString("WatermarkParamTextSizesProportions") && param>0.01)
	{
		m_rects[0]->SetProportions(1.0f/(float)param);
		m_rects[0]->Recalc();
		BSTR tmpBs = m_xmls[0].AllocSysString();
		SetXML(CString("WatermarkText"),&tmpBs );
		SysFreeString(tmpBs);
		
	}
	else
		if (paramName==CString("WatermarkParamImageKeepSizes"))
		{
			if (param>0.01)
			{
				if (m_rects[1]->m_w>1)
					m_rects[1]->SetProportions((float)m_rects[1]->m_h/(float)m_rects[1]->m_w);
				else
					m_rects[1]->SetProportions(1);
			}
			else
				m_rects[1]->SetProportions(0.0f);


		}
}

void  ImageEditor::CWatermarks::SetXML(const CString& paramName, BSTR* xmlStr)
{
	int index = 0;
	if (paramName==CString("WatermarkText"))
		index=0;
	else
		if (paramName==CString("WatermarkImage"))
			index=1;
		else
			return;
			
	if (CString(*xmlStr).IsEmpty())
	{
		m_rects[index]->m_isVisible = false;
		return;
	}
	else
		m_rects[index]->m_isVisible = true;

	m_xmls[index] = CString(*xmlStr);

	MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;

	ImageStudio3::IImageTransforms* pTransform = m_app_manager->GetImageStudio();//NULL;
	//HRESULT hRes = CoCreateInstance( ImageStudio3::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio3::IID_IImageTransforms, (void**)(&pTransform));

		
	CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);
	if (NULL == pFrame)
			return;

	LONG lWidth		= m_rects[index]->m_w+50;
	LONG lHeight	= m_rects[index]->m_h;

	pFrame->put_ColorSpace(64); // BGRA
	pFrame->put_Width(lWidth);
	pFrame->put_Height(lHeight);
	pFrame->SetDefaultStrides();
	pFrame->AllocateBuffer(-1);

	BYTE* pBuffer = NULL;
	pFrame->get_Buffer(&pBuffer);

	memset(pBuffer, 0, 4 * lWidth * lHeight);
	
	VARIANT var;
	var.vt = VT_UNKNOWN;
	var.punkVal = (IUnknown*)pFrame;

	CString sFullTransform =_T("<transforms>");
	//sFullTransform.AppendFormat("<ImagePaint-DrawAnimatedText rect-left=\"%d\" rect-top=\"%d\" rect-right=\"%d\" rect-bottom=\"%d\" metric=\"0\" text=\"UNREGISTERED VERSION\" text-typeeffects=\"0\" text-fillmode=\"0\" font-italic=\"0\" font-underline=\"0\" font-strikeout=\"0\" font-angle=\"0\" font-stringalignmenthorizontal=\"1\" font-stringalignmentvertical=\"2\" marginhorizontal=\"0\" marginvertical=\"0\" brush-color2=\"16777215\" brush-alpha2=\"179\" brush-texturealpha=\"255\" brush-texturepath=\"\" brush-texturemode=\"1\" brush-rectable=\"0\" brush-rect-left=\"0\" brush-rect-top=\"0\" brush-rect-width=\"0\" brush-rect-height=\"0\" shadow-distancex=\"3\" shadow-distancey=\"3\" shadow-blursize=\"3\" shadow-color=\"0\" shadow-alpha=\"120\" brush-type=\"0\" edge-visible=\"-1\" font-name=\"Arial\" brush-color1=\"16777215\" shadow-visible=\"0\" brush-alpha1=\"179\" edge-color=\"0\" edge-dist=\"%d\" edge-alpha=\"204\" font-bold=\"-1\" font-size=\"%d\"/>", 0, 0, 100, 100, 10, 10);
				
	sFullTransform.Append( CString(*xmlStr) );
	sFullTransform.Append(_T("</transforms>"));

	CString strRect = _T("");
	if (0 == index)
	{
		long ts = (long) ( (double)lHeight * (72.0 / 96.0) );
		strRect.Format(_T(" rect-left=\"0\" rect-top=\"0\" rect-right=\"%d\" rect-bottom=\"%d\"   font-size=\"%d\"   "), lWidth, lHeight, ts);
	}
	else
	{
		strRect.Format(_T(" left=\"0\" top=\"0\" right=\"%d\" bottom=\"%d\"   "), lWidth, lHeight, lHeight);
	}

	CString templ;
	if (index==0)
	{
		templ = CString(_T("ImagePaint-DrawAnimatedText "));
		sFullTransform.Insert(sFullTransform.Find(templ)+templ.GetLength(), strRect);
	}
	else
	{
		templ = CString(_T("ImagePaint-DrawImageFromFile "));
		sFullTransform.Insert(sFullTransform.Find(templ)+templ.GetLength(), strRect);
	}
	

	VARIANT_BOOL vbRes=VARIANT_TRUE;
	pTransform->SetSource(0, var);
	BSTR tmpBs = sFullTransform.AllocSysString();
	pTransform->SetXml(tmpBs, &vbRes);
	pTransform->Transform(&vbRes);
	SysFreeString(tmpBs);

	pTransform->GetResult(0, &var);

	if (NULL != var.punkVal)
	{
		RELEASEINTERFACE(pFrame);
		var.punkVal->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);
		RELEASEINTERFACE((var.punkVal));

		if (m_pBitmaps[index])
			delete m_pBitmaps[index];

		/*
		if (index==1)
				{
					pFrame->get_Width(&lWidth);
					pFrame->get_Height(&lHeight);
					lWidth++;
				}*/
		

		m_pBitmaps[index] = new Gdiplus::Bitmap(lWidth, lHeight, PixelFormat32bppARGB);
		
		BYTE* pBuf = NULL;
		pFrame->get_Buffer(&pBuf);

		BitmapData oBitmapData; 
		if (Gdiplus::Ok == m_pBitmaps[index]->LockBits (NULL, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &oBitmapData))
		{
			BYTE* pData = (BYTE*)oBitmapData.Scan0;
			memcpy(pData, pBuf, 4 * lWidth * lHeight);
		
			m_pBitmaps[index]->UnlockBits(&oBitmapData);
		}
	}
	//RELEASEINTERFACE(pTransform);
	RELEASEINTERFACE(pFrame);
}

