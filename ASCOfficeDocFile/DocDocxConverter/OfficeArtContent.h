#pragma once

#include "OfficeDrawing/RecordFactory.h"

#include "OfficeDrawing/DrawingContainer.h"
#include "OfficeDrawing/DrawingGroup.h"
#include "OfficeDrawing/GroupContainer.h"
#include "OfficeDrawing/Shape.h"

namespace DocFileFormat
{
	class OfficeArtContent
	{
	private:
		enum DrawingType
		{
			MainDocument,
			Header
		};

		struct OfficeArtWordDrawing
		{
			DrawingType dgglbl;
			DrawingContainer* container;
		};

	public:

		OfficeArtContent (const FileInformationBlock* pFIB, POLE::Stream* pStream): m_pDrawingGroupData(NULL)
		{
			VirtualStreamReader oStearmReader(pStream);
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

					for (unsigned int i = 0; i < drawing.container->Children.size(); ++i)
					{
						Record* groupChild = drawing.container->Children[i];
						if (groupChild)
						{
							if (GroupContainer::TYPE_CODE_0xF003 == groupChild->TypeCode)
							{
								// the child is a subgroup
								GroupContainer* group	=	static_cast<GroupContainer*>(groupChild);
								group->Index			=	i;
								drawing.container->Children[i] = group;
							}
							else if (ShapeContainer::TYPE_CODE_0xF004 == groupChild->TypeCode)
							{
								// the child is a shape
								ShapeContainer* shape	=	static_cast<ShapeContainer*>(groupChild);
								shape->Index			=	i;
								drawing.container->Children[i] = shape;
							}
						}
					}

					m_arrDrawings.push_back( drawing );
				}
			}
		}

		~OfficeArtContent()
		{
			RELEASEOBJECT (m_pDrawingGroupData);

			for ( list<OfficeArtWordDrawing>::iterator iter = m_arrDrawings.begin(); iter != m_arrDrawings.end(); ++iter)
				RELEASEOBJECT(iter->container);  
		}

		inline ShapeContainer* GetShapeContainer (int spid)
		{
			ShapeContainer* ret = NULL;

			for (list<OfficeArtWordDrawing>::iterator iter = m_arrDrawings.begin(); iter != m_arrDrawings.end(); ++iter)
			{
				GroupContainer* group = iter->container->FirstChildWithType<GroupContainer>();
				if (group)
				{
					for (unsigned int i = 1; i < group->Children.size(); ++i)
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
								ret						=	container;
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

		inline const DrawingGroup* GetDrawingGroup () const
		{
			return m_pDrawingGroupData;
		}

	private:

		DrawingGroup*				m_pDrawingGroupData;
		list<OfficeArtWordDrawing>	m_arrDrawings;
	};
}
