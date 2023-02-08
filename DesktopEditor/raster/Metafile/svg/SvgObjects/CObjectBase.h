#ifndef COBJECTBASE_H
#define COBJECTBASE_H

#include "../../../../Common/3dParty/html/css/src/CNode.h"
#include "../../../../Common/3dParty/html/css/src/StaticFunctions.h"
#include "../../../xml/include/xmlutils.h"
#include "../../IRenderer.h"
#include "../SvgTypes.h"

namespace SVG
{
	class CObjectBase
	{
	public:
		CObjectBase(CObjectBase* pParent = NULL);
		virtual ~CObjectBase();

		virtual bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) = 0;
		virtual bool Draw(IRenderer* pRenderer) const = 0;

		std::vector<NSCSS::CNode> GetFullPath() const;

		void SetData(const std::wstring wsStyles, unsigned short ushLevel, bool bHardMode = false);

		virtual void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) = 0;
	private:
		void SaveNodeData(XmlUtils::CXmlNode& oNode);

		void SetStroke(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetFill(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);
		void SetTransform(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false);

		virtual void ApplyStyle(IRenderer* pRenderer, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const = 0;

		void ApplyDefaultStroke(IRenderer* pRenderer, int& nTypePath) const;
		void ApplyStroke(IRenderer* pRenderer, int& nTypePath, bool bUseDedault = false) const;
		void ApplyDefaultFill(IRenderer* pRenderer, int& nTypePath) const;
		void ApplyFill(IRenderer* pRenderer, int& nTypePath, bool bUseDedault = false) const;
		void ApplyTransform(IRenderer* pRenderer, Aggplus::CMatrix& oOldMatrix) const;

		friend class CLine;
		friend class CRect;
		friend class CCircle;
		friend class CEllipse;
		friend class CPath;
		friend class CText;
		friend class CTspan;
		friend class CPolyline;
		friend class CPolygon;
		friend class CContainer;

		CObjectBase   *m_pParent;
		NSCSS::CNode   m_oXmlNode;

		//Styles
		SvgColor     m_oFill;
		CStroke      m_oStroke;
		SvgTransform m_oTransform;
	};
}


#endif // COBJECTBASE_H
