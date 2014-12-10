#pragma once

#include "FileInformationBlock.h"
#include "VirtualStreamReader.h"

namespace DocFileFormat
{
	class AnnotationOwnerList: public vector<wstring>
	{
	public: 
		AnnotationOwnerList(FileInformationBlock* fib, IStream* tableStream) : std::vector<wstring>()
		{
			VirtualStreamReader reader(tableStream, fib->m_FibWord97.fcGrpXstAtnOwners);
			while (reader.GetPosition() < (fib->m_FibWord97.fcGrpXstAtnOwners + fib->m_FibWord97.lcbGrpXstAtnOwners))
			{
				push_back(reader.ReadXst());
			}
		}
	};
}