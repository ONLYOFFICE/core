#ifndef CCONTAINER_H
#define CCONTAINER_H

#include "CObjectBase.h"

class CSvgFile;

namespace SVG
{
	class CSvgParser;

	template<typename TypeObject>
	class CContainer
	{
		using const_iterator = typename std::vector<TypeObject*>::const_iterator;
	public:
		CContainer() = default;
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

		const_iterator begin() const
		{
			return m_arObjects.cbegin();
		}

		const_iterator end() const
		{
			return m_arObjects.cend();
		}
	protected:
		std::vector<TypeObject*> m_arObjects;
	};

	class CGraphicsContainer : public CContainer<CRenderedObject>, public CRenderedObject
	{
		friend class CObject;
		CGraphicsContainer(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		virtual ~CGraphicsContainer() = default;

		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;

		TRect GetWindow() const;
		TRect GetViewBox() const;
		TBounds GetBounds(SvgMatrix* pTransform = nullptr) const override;
	private:

		friend class CPattern;
		friend class CMarker;
		friend class CSymbol;
		friend class CMask;

		TRect m_oWindow;
		TRect m_oViewBox;
	};
}

#endif // CCONTAINER_H
