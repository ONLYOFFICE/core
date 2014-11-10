#pragma once
#ifndef OOX_CHART_FILE_INCLUDE_H_
#define OOX_CHART_FILE_INCLUDE_H_

#include "../CommonInclude.h"
#include "ChartSerialize.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CChartSpace : public OOX::FileGlobalEnumerated, public OOX::Spreadsheet::IFileContainer
		{
		public:
			CChartSpace()
			{
			}
			CChartSpace(const CPath& oRootPath, const CPath& oPath)
			{
				read( oRootPath, oPath );
			}
			virtual ~CChartSpace()
			{
			}
		public:

			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
			{
				m_oReadPath = oPath;
				IFileContainer::Read( oRootPath, oPath );

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;

				m_oChartSpace.fromXML(oReader);
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
                CString sPath = oPath.GetPath();
                write2(sPath);
				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write(oPath, oDirectory, oContent);
			}
			virtual void write2(CString& sFilename) const
			{
				XmlUtils::CStringWriter sXml;
				toXML(sXml);
				CDirectory::SaveToFile( sFilename, _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n") + sXml.GetData() );
			}
			void toXML(XmlUtils::CStringWriter& writer) const
			{
                CString sNodeName = _T("c:chartSpace");
                m_oChartSpace.toXML (sNodeName, writer);
			}
			bool isValid() const
			{
				return true;
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Charts;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}

		private:
			CPath									m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			CT_ChartSpace         m_oChartSpace;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CHART_FILE_INCLUDE_H_
