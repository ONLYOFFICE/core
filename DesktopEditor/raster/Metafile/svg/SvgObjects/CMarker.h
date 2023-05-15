#ifndef CMARKER_H
#define CMARKER_H

#include "CDefs.h"

namespace SVG
{
	typedef enum
	{
		Marker_StrokeWidth,
		Marker_UserSpaceOnUse
	} MarkerUnits;

	class CMarker : public CGraphicsContainer, public CDefObject
	{
	public:
		CMarker(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);
		virtual ~CMarker();

		void SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode) override;

		bool Apply(IRenderer* pRenderer, const CDefs *pDefs, const TBounds &oObjectBounds) override;

		void Update(const CDefs *pDefs);

		void Draw(IRenderer* pRenderer, const std::vector<Point>& arPoints) const;
	private:
		TBounds GetBounds() const override;

		MarkerUnits m_enUnits;

		Aggplus::CImage *m_pImage;
	};
}

#endif // CMARKER_H
