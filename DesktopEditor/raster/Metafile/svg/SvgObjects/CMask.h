#ifndef CMASK_H
#define CMASK_H

#include "CClipPath.h"

namespace SVG
{
	class CMask : public CClipPath
	{
		friend class CObject;
		CMask(CSvgReader& oReader);
	public:
		bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) override;
	};
}

#endif // CMASK_H
