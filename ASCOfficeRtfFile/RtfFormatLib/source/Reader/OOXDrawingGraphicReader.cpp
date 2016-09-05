/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "OOXPictureGraphicReader.h"

bool OOXPictureGraphicReader::Parse( ReaderParameter oParam , RtfShape& oOutput)
{
	if (m_ooxGraphic == NULL) return false;
	
	bool bTryPicture = false;
	switch(m_ooxGraphic->m_eGraphicType)
	{
		case OOX::Drawing::graphictypePicture:
		case OOX::Drawing::graphictypeLockedCanvas:
		case OOX::Drawing::graphictypeChart:		
		case OOX::Drawing::graphictypeDiagram:
			bTryPicture = true;
			break;
		case OOX::Drawing::graphictypeShape:
		case OOX::Drawing::graphictypeGroupShape:
			break;
	}
	if (bTryPicture)
	{
		bTryPicture = false;
		for (long i=0; i < m_ooxGraphic->m_arrItems.size(); i++)
		{
			if (m_ooxGraphic->m_arrItems[i] == NULL) continue;

			if (m_ooxGraphic->m_arrItems[i]->getType() == OOX::et_pic_pic)
			{
				OOX::Drawing::CPicture *picture = dynamic_cast<OOX::Drawing::CPicture *>(m_ooxGraphic->m_arrItems[i]);
				if ( (picture) && (picture->m_oBlipFill.m_oBlip.IsInit()))
				{
					CString sImageId = picture->m_oBlipFill.m_oBlip->m_oEmbed.GetValue();

					if (oParam.oReader->m_currentContainer)
					{
						smart_ptr<OOX::File> oFile = oParam.oReader->m_currentContainer->Find(sImageId);
						
						if ( oFile.IsInit() && (OOX::FileTypes::Image == oFile->type()))
						{
							OOX::Image* pImage = (OOX::Image*)oFile.operator->();

							CString sImagePath = pImage->filename().GetPath();
							bTryPicture = WriteDataToPicture( sImagePath, *oOutput.m_oPicture, _T("") );
						}
					}
				}
			}
		}

		if (!bTryPicture)
		{
			//return false;
			//рисуем крест (todooo получать с редактора реплейсмент картинку)
			oOutput.m_nShapeType = 1;
			oOutput.m_nFillColor = 0x967bdf;
		}
	}
	else
	{
		//return false;
		//convertDrawingML2Vml();
		oOutput.m_nShapeType = 1;
		oOutput.m_nFillColor = 0x967bdf;
	}
	return true;
}

