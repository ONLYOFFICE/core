#pragma once
#ifndef OOX_LOGIC_OLE_Z_INDEX_INCLUDE_H_
#define OOX_LOGIC_OLE_Z_INDEX_INCLUDE_H_

#include "Common.h"
#include "./../Namespaces.h"
#include "./../Unit.h"


namespace OOX
{
	namespace Logic
	{
		class OleZIndex : public Common::ZIndex
		{
		public:
			OleZIndex();
			explicit OleZIndex(const Common::ZIndex& zIndex);
			const std::string toString() const;
			const OleZIndex& operator = (const Common::ZIndex& zIndex);
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_OLE_Z_INDEX_INCLUDE_H_