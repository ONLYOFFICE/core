#pragma once
#ifndef OOX_LOGIC_TABLE_BORDERS_INCLUDE_H_
#define OOX_LOGIC_TABLE_BORDERS_INCLUDE_H_

#include "./../WritingElement.h"
#include "Border.h"

namespace OOX
{
	namespace Logic
	{
		class TableBorders : public WritingElement
		{
		public:
			TableBorders();
			virtual ~TableBorders();
			explicit TableBorders(const XML::XNode& node);
			const TableBorders& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			//

			inline bool GetBorder (int nInd, Border& oBorder) const
			{
				if (0 == nInd)
				{
					if (top.is_init())
					{
						oBorder	= top;
						return true;
					}
				}
			
				if (1 == nInd)
				{
					if (bottom.is_init())
					{
						oBorder	= bottom;
						return true;
					}
				}
				
				if (2 == nInd)
				{
					if (left.is_init())
					{
						oBorder	= left;
						return true;
					}
				}
				
				if (3 == nInd)
				{
					if (right.is_init())
					{
						oBorder	= right;
						return true;
					}
				}
			
				if (4 == nInd)
				{
					if (insideH.is_init())
					{
						oBorder	= insideH;
						return true;
					}
				}
				
				if (5 == nInd)
				{
					if (insideV.is_init())
					{
						oBorder	= insideV;
						return true;
					}
				}

				return false;
			}

			inline bool ValidBorder (int nInd)	const
			{
				if (0 == nInd)
				{
					if (top.is_init())
						return true;

					return false;
				}

				if (1 == nInd)
				{
					if (bottom.is_init())
						return true;

					return false;
				}

				if (2 == nInd)
				{
					if (left.is_init())
						return true;

					return false;
				}

				if (3 == nInd)
				{
					if (right.is_init())
						return true;

					return false;
				}

				if (4 == nInd)
				{
					if (insideH.is_init())
						return true;

					return false;
				}

				if (5 == nInd)
				{
					if (insideV.is_init())
						return true;

					return false;
				}

				return false;
			}


		public:

			nullable_property<Border>	top;
			nullable_property<Border>	bottom;
			nullable_property<Border>	left;
			nullable_property<Border>	right;
			nullable_property<Border>	insideH;
			nullable_property<Border>	insideV;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_BORDERS_INCLUDE_H_