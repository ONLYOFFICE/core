#pragma once
#ifndef COMMON_Z_INDEX_INCLUDE_H_
#define COMMON_Z_INDEX_INCLUDE_H_

namespace Common
{
	class ZIndex
	{
	public:
		ZIndex();
		ZIndex(const int& index);

	public:
		int Index;
		
	protected:
		void fromBase(const ZIndex& zIndex);
	};
} // namespace Common

#endif // COMMON_Z_INDEX_INCLUDE_H_