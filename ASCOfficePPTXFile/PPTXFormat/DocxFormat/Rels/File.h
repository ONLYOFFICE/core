/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once
#ifndef PPTXOOX_RELS_FILE_INCLUDE_H_
#define PPTXOOX_RELS_FILE_INCLUDE_H_

#include "RelationTable.h"
#include "./../FileType.h"
#include "./../FileTypes.h"
#include "./../RId.h"
#include "./../External/External.h"

#include "../../../../Common/DocxFormat/Source/Base/SmartPtr.h"
#include "../../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"


namespace PPTX
{
	namespace Rels
	{
		class File
		{
		public:
			File()
			{
			}
			File(const OOX::CPath& filename)
			{
				read(filename);
			}
			~File()
			{
			}

		public:
			void read(const OOX::CPath& filename)
			{
				OOX::CPath strFile = createFileName(filename);

				if (OOX::CSystemUtility::IsFileExist(strFile))
				{
					XmlUtils::CXmlNode oNode;
					if (oNode.FromXmlFile(strFile.GetPath()))
						Relations = oNode;
				}
			}
			void read2(const OOX::CPath& filename)
			{
				OOX::CPath strFile = filename;

				if (OOX::CSystemUtility::IsFileExist(strFile))
				{
					XmlUtils::CXmlNode oNode;
					if (oNode.FromXmlFile(strFile.GetPath()))
						Relations = oNode;
				}
			}
			void write(const OOX::CPath& filename) const
			{
				if (0 < Relations.m_items.size())
				{
					OOX::CPath file = createFileName(filename);
					OOX::CSystemUtility::CreateDirectories(file.GetDirectory());

					XmlUtils::CXmlWriter oWriter;
                    oWriter.WriteNodeBegin(_T("Relationship"), true);
					oWriter.WriteAttribute(_T("xmlns"), _T("http://schemas.openxmlformats.org/package/2006/relationships"));
                    oWriter.WriteNodeEnd(_T("Relationship"), false, true);

					oWriter.WriteString(Relations.toXML());

					oWriter.WriteNodeEnd(_T("Relationship"));

					NSFile::CFileBinary::SaveToFile(file.GetPath(), oWriter.GetXmlString());
				}
			}
			const bool isValid() const
			{
				return true;
			}

		public:
			void registration(const RId& rId, const FileType& type, const OOX::CPath& filename)
			{
				if(!(type == FileTypes::Unknow))
				{
					std::wstring strFileName	= filename.m_strFilename;
					std::wstring strDir		= filename.GetDirectory() + _T("");
					if (_T("") == filename.GetExtention())
					{
                        if (type.RelationType() == _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject"))
						{
                            strFileName += _T(".bin");
							Relations.registration(rId, type.RelationType(), strDir + strFileName);
						}
                        else if (type.RelationType() == _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image"))
						{
                            strFileName += _T(".wmf");
							Relations.registration(rId, type.RelationType(), strDir + strFileName);
						}
					}
					else
					{
						//Relations.registration(rId, type.RelationType(), replace_extension(filename, L"svm", L"png"));
					}
				}
			}
			void registration(const RId& rId, const smart_ptr<External> external)
			{
				Relations.registration(rId, external);
			}

		private:
			const OOX::CPath createFileName(const OOX::CPath& filename) const
			{
                std::wstring strTemp = filename.GetDirectory() +  FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR;

                if (filename.GetFilename() == _T(""))	strTemp += _T(".rels");
				else									strTemp += (filename.GetFilename() + _T(".rels"));
				
				OOX::CPath pathTemp = strTemp;
				return pathTemp.GetPath();
			}
			
		public:
			RelationTable Relations;
		};
	} // namespace Rels
} // namespace PPTX

#endif // PPTXOOX_RELS_FILE_INCLUDE_H_
