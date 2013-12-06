
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "RelationShip.h"

#include "ASCStlUtils.h"
#include "Encoding.h"

namespace OOX
{
	namespace Rels
	{
		RelationShip::RelationShip(const RId& rId, const std::string& type, const OOX::CPath& filename)
		{
			m_rId		=	rId;
			m_target	=	filename;

			std::wstring newFilename = filename.GetPath();
			StlUtils::ReplaceString(newFilename, L" ", L"_");
			m_target	=	OOX::CPath(newFilename.c_str());
		}

		RelationShip::RelationShip(const RId& rId, const NSCommon::smart_ptr<External> external) 
		{
			m_rId		=	rId;
			m_target	=	external->Uri();
			m_type		=	external->type().RelationType();
			m_mode		=	std::wstring(L"External");
		}

		RelationShip::~RelationShip()
		{
		}

		RelationShip::RelationShip(XmlUtils::CXmlNode& oNode)
		{
			fromXML(oNode);
		}

		const RelationShip& RelationShip::operator =(XmlUtils::CXmlNode& oNode)
		{
			fromXML(oNode);
			return *this;
		}

		void RelationShip::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_rId		=	std::wstring(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("Id"))));
			m_target	=	OOX::CPath(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("Target"))));
			m_type		=	std::wstring(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("Type"))));

			if (oNode.GetAttributeBase(_T("TargetMode")).GetLength())
			{
				m_mode	=	std::wstring(static_cast<const wchar_t*>(oNode.GetAttributeBase(_T("TargetMode"))));
			}
		}

		const bool RelationShip::operator <(const RelationShip& rhs) const
		{
			return m_rId < rhs.m_rId;
		}

		const std::wstring RelationShip::type() const
		{
			return m_type;
		}

		const OOX::CPath RelationShip::filename() const
		{
			return m_target;
		}

		const OOX::CPath RelationShip::target() const
		{
			return m_target;
		}

		const bool RelationShip::isExternal()const
		{
			if (m_mode.IsInit())
				return (m_mode.get() == std::wstring(L"External"));

			return false;
		}

		const RId RelationShip::rId() const
		{
			return m_rId;
		}

	} // namespace Rels
} // namespace OOX