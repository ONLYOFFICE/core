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

#include "OfficeArtContent.h"

namespace DocFileFormat
{
	OfficeArtContent::OfficeArtContent (const FileInformationBlock* pFIB, POLE::Stream* pStream): m_pDrawingGroupData(NULL), m_pBackgroud(NULL), m_uLastShapeId(1024)
	{
		VirtualStreamReader oStearmReader(pStream, 0 , pFIB->m_nWordVersion);

		if (pFIB->m_FibWord97.fcDggInfo > oStearmReader.GetSize()) return;

		oStearmReader.Seek (pFIB->m_FibWord97.fcDggInfo, 0/*STREAM_SEEK_SET*/);

		if (pFIB->m_FibWord97.lcbDggInfo > 0)
		{
			unsigned int maxPosition	=	(int)(pFIB->m_FibWord97.fcDggInfo + pFIB->m_FibWord97.lcbDggInfo);

			// read the DrawingGroupData
			m_pDrawingGroupData			=	static_cast<DrawingGroup*>(RecordFactory::ReadRecord (&oStearmReader, 0));

			while (oStearmReader.GetPosition() < maxPosition)
			{
				OfficeArtWordDrawing drawing;
				drawing.dgglbl			=	(DrawingType)oStearmReader.ReadByte();
				drawing.container		=	static_cast<DrawingContainer*>(RecordFactory::ReadRecord (&oStearmReader, 0));

				for (size_t i = 0; i < drawing.container->Children.size(); ++i)
				{
					Record* groupChild = drawing.container->Children[i];
					if (groupChild)
					{
						if (GroupContainer::TYPE_CODE_0xF003 == groupChild->TypeCode)
						{
							GroupContainer* group	=	static_cast<GroupContainer*>(groupChild);
							if (group)
							{
								group->Index =	(int)i;
							}
						}
						else if (ShapeContainer::TYPE_CODE_0xF004 == groupChild->TypeCode)
						{
							ShapeContainer* shape	=	static_cast<ShapeContainer*>(groupChild);
							if (shape)
							{
								shape->m_nIndex = (int)i;
								if (shape->m_bBackground)
								{
									m_pBackgroud = shape;
								}
							}
						}
						else if (DrawingRecord::TYPE_CODE_0xF008 == groupChild->TypeCode)
						{
							DrawingRecord* dr	=	static_cast<DrawingRecord*>(groupChild);
							if (dr)
							{
								m_uLastShapeId = dr->spidCur;
							}
						}
					}
				}

				m_arrDrawings.push_back( drawing );
			}
		}
	}
	OfficeArtContent::~OfficeArtContent()
	{
		RELEASEOBJECT (m_pDrawingGroupData);

		for ( std::list<OfficeArtWordDrawing>::iterator iter = m_arrDrawings.begin(); iter != m_arrDrawings.end(); ++iter)
			RELEASEOBJECT(iter->container);
	}

	ShapeContainer* OfficeArtContent::GetShapeContainer (int spid)
	{
		ShapeContainer* ret = NULL;

		for (std::list<OfficeArtWordDrawing>::iterator iter = m_arrDrawings.begin(); iter != m_arrDrawings.end(); ++iter)
		{
			GroupContainer* group = iter->container->FirstChildWithType<GroupContainer>();
			if (group)
			{
				for (size_t i = 1; i < group->Children.size(); ++i)
				{
					Record* groupChild = group->Children[i];

					if ( groupChild->TypeCode == GroupContainer::TYPE_CODE_0xF003)
					{
						//It's a group of shapes
						GroupContainer* subgroup	=	static_cast<GroupContainer*>(groupChild);

						//the referenced shape must be the first shape in the group
						ShapeContainer* container	=	static_cast<ShapeContainer*>(subgroup->Children[0]);
						Shape* shape				=	static_cast<Shape*>(container->Children[1]);

						if (shape->GetShapeID() == spid)
						{
							ret = container;
							break;
						}
					}
					else if ( groupChild->TypeCode == ShapeContainer::TYPE_CODE_0xF004 )
					{
						//It's a singe shape
						ShapeContainer* container	=	static_cast<ShapeContainer*>(groupChild);
						Shape* shape				=	static_cast<Shape*>(container->Children[0]);

						if (shape->GetShapeID() == spid)
						{
							ret	 =	container;
							break;
						}
					}
				}
			}
			else
			{
				continue;
			}

			if (ret)
				break;
		}

		return ret;
	}
	const DrawingGroup* OfficeArtContent::GetDrawingGroup () const
	{
		return m_pDrawingGroupData;
	}
}
