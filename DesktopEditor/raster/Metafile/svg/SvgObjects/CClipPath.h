#ifndef CCLIPPATH_H
#define CCLIPPATH_H

#include "CContainer.h"

namespace SVG
{
	typedef enum
	{
		ClipU_ObjectBoundingBox,
		ClipU_UserSpaceOnUse
	}ClipUnits;

	class CClipPath : public CAppliedObject
	{
	public:
		CClipPath(XmlUtils::CXmlNode& oNode);

		void SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode) override;

		bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) override;

		CContainer<CRenderedObject>& GetContainer();
	private:
		friend class CMask;

		CContainer<CRenderedObject> m_oContainer;
		ClipUnits m_enUnits;
	};
}

#endif // CCLIPPATH_H
