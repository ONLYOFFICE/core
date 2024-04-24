/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#ifndef CCONTAINER_H
#define CCONTAINER_H

#include "CObjectBase.h"

#include "../../../graphics/pro/Fonts.h"

class CSvgFile;

namespace SVG
{
	class CSvgParser;

	template<typename TypeObject>
	class CContainer
	{
	public:
		CContainer(){}
		virtual ~CContainer()
		{
			Clear();
		}

		virtual void Clear()
		{
			for (TypeObject* pObject : m_arObjects)
				RELEASEINTERFACE(pObject);

			m_arObjects.clear();
		}

		bool Empty() const
		{
			return m_arObjects.empty();
		}

		virtual bool AddObject(TypeObject* pObject)
		{
			if (NULL != pObject)
			{
				m_arObjects.push_back(pObject);
				return true;
			}
			return false;
		}

		virtual TypeObject* GetObject(const std::wstring& wsId) const
		{
			if (wsId.empty() || m_arObjects.empty())
				return NULL;

			std::wstring wsNewId = wsId;

			size_t unFound = wsNewId.find(L'#');

			if (std::wstring::npos != unFound)
				wsNewId.erase(0, unFound + 1);

			typename std::vector<TypeObject*>::const_iterator oFound = std::find_if(m_arObjects.begin(), m_arObjects.end(), [&wsNewId](TypeObject* pObject){ if (wsNewId == pObject->GetId()) return true; else return false;});

			if (m_arObjects.end() != oFound)
				return *oFound;

			return NULL;
		}
	private:
		std::vector<TypeObject*> m_arObjects;

		friend class CText;
		friend class CMask;
		friend class CTSpan;
		friend class CMarker;
		friend class CPattern;
		friend class CGradient;
		friend class CClipPath;
		friend class CTextPath;
		friend class CLinearGradient;
		friend class CGraphicsContainer;
	};

	class CGraphicsContainer : public CContainer<CRenderedObject>, public CRenderedObject
	{
	public:
		CGraphicsContainer(const std::wstring& wsName = L"GraphicsContainer");
		CGraphicsContainer(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL);
		CGraphicsContainer(double dWidth, double dHeight, XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL);

		virtual ~CGraphicsContainer();

		void SetData(XmlUtils::CXmlNode& oNode);

		bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL) const override;

		TRect GetWindow() const;
		TRect GetViewBox() const;
	private:
		TBounds GetBounds() const override;

		friend class CPattern;
		friend class CMarker;
		friend class CSymbol;
		friend class CMask;

		TRect m_oWindow;
		TRect m_oViewBox;
	};
}

#endif // CCONTAINER_H
