/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#include "xlsx_conversion_context.h"
#include "oox_rels.h"
#include "mediaitems_utils.h"

#include <boost/make_shared.hpp>

#include "../../Common/Utils/simple_xml_writer.h"

#if defined(_WIN32) || defined(_WIN64)
	#pragma comment(lib, "Ole32.lib")
#endif

namespace oox {

class oox_activeX_context::Impl
{
    struct rel_
    { 
        rel_(bool is_internal, std::wstring const & rid, std::wstring const & target, std::wstring const & type) :
        is_internal_(is_internal),
        rid_(rid),
		target_(target),
        type_(type)
        {}

        bool is_internal_;
        std::wstring rid_;
        std::wstring target_;
        std::wstring type_;
    };

public:
	Impl() {}
    
	std::wstring		dataBinRid;
	std::wstring		progId;
	std::wstring		license;

	std::vector<rel_>	activeXRels_;
   
	void dump_rels(rels & Rels)
    {
		for (size_t i = 0; i < activeXRels_.size(); i++)
        {
			rel_ & r = activeXRels_[i];
			
			Rels.add(relationship(r.rid_, r.type_, r.target_, (r.is_internal_ ? L"" : L"External")));	
		}
    }


    void add_rels(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
		std::wstring const & type)
    {
		bool present = false;
        
		for (size_t i = 0; i < activeXRels_.size(); i++)
        {
			rel_ & r = activeXRels_[i];
			if (r.rid_ == rid && r.target_ == target)
				present = true;
		}
		if (!present)
		{
			activeXRels_.push_back(rel_(isInternal, rid, target, type));
		}
    }

};

oox_activeX_context_ptr oox_activeX_context::create()
{
    return boost::make_shared<oox_activeX_context>();
}

oox_activeX_context::oox_activeX_context() : impl_(new oox_activeX_context::Impl())
{
}
oox_activeX_context::~oox_activeX_context()
{
}
void oox_activeX_context::setDataBinRid(const std::wstring &rid, const std::wstring &bin_data)
{
    impl_->dataBinRid = rid;
	
	impl_->add_rels(true, rid, bin_data, L"http://schemas.microsoft.com/office/2006/relationships/activeXControlBinary");
}
void oox_activeX_context::setProgId(const std::wstring &progId)
{
    impl_->progId = progId;
}
void oox_activeX_context::setLicense(const std::wstring &license)
{
    impl_->license = license;
}
void oox_activeX_context::add_rels(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
        external_items::Type type)
{
	impl_->add_rels(isInternal, rid, target, utils::media::get_rel_type(type));
}
void oox_activeX_context::dump_rels(rels & Rels)
{
	impl_->dump_rels(Rels);
}
void oox_activeX_context::write_to(std::wostream & strm)
{
	//https://msdn.microsoft.com/en-us/library/ff533853(v=office.12).aspx

	bool badClassId = false;
	CP_XML_WRITER(strm)    
	{
		CP_XML_NODE(L"ax:ocx")
		{ 
			std::wstring classId;

			if (std::wstring::npos != impl_->progId.find(L"Forms.")) // Microsoft Forms 2.0
			{
				if (std::wstring::npos != impl_->progId.find(L".CommandButton."))
				{
					classId = L"{D7053240-CE69-11CD-A777-00DD01143C57}";
				}
				else if (std::wstring::npos != impl_->progId.find(L".CheckBox."))
				{
					classId = L"{8BD21D40-EC42-11CE-9E0D-00AA006002F3}";
				}
				else if (std::wstring::npos != impl_->progId.find(L".ComboBox."))
				{
					classId = L"{8BD21D30-EC42-11CE-9E0D-00AA006002F3}";
				}			
				else if (std::wstring::npos != impl_->progId.find(L".Form."))
				{
					classId = L"{C62A69F0-16DC-11CE-9E98-00AA00574A4F}";
				}	
				else if (std::wstring::npos != impl_->progId.find(L".Frame."))
				{
					classId = L"{6E182020-F460-11CE-9BCD-00AA00608E01}";
				}	
				else if (std::wstring::npos != impl_->progId.find(L".Image."))
				{
					classId = L"{4C599241-6926-101B-9992-00000B65C6F9}";
				}	
				else if (std::wstring::npos != impl_->progId.find(L".OptionButton."))
				{
					classId = L"{8BD21D50-EC42-11CE-9E0D-00AA006002F3}";
				}	
				else if (std::wstring::npos != impl_->progId.find(L".Label."))
				{
					classId = L"{978C9E23-D4B0-11CE-BF2D-00AA003F40D0}";
				}	
				else if (std::wstring::npos != impl_->progId.find(L".ListBox."))
				{
					classId = L"{8BD21D20-EC42-11CE-9E0D-00AA006002F3}";
				}	
				else if (std::wstring::npos != impl_->progId.find(L".ScrollBar."))
				{
					classId = L"{DFD181E0-5E2F-11CE-A449-00AA004A803D}";
				}	
				else if (std::wstring::npos != impl_->progId.find(L".SpinButton."))
				{
					classId = L"{79176FB0-B7F2-11CE-97EF-00AA006D2776}";
				}	
				else if (std::wstring::npos != impl_->progId.find(L".TabStrip."))
				{
					classId = L"{EAE50EB0-4A62-11CE-BED6-00AA00611080}";
				}	
				else if (std::wstring::npos != impl_->progId.find(L".TextBox."))
				{
					classId = L"{8BD21D10-EC42-11CE-9E0D-00AA006002F3}";
				}	
			}
			else if (std::wstring::npos != impl_->progId.find(L"ShockwaveFlash.")) 
			{
					classId = L"{D27CDB6E-AE6D-11CF-96B8-444553540000}";
			}
#if defined(_WIN32) || defined(_WIN64)
			if (classId.empty())
			{
				//std::wstring test = L"AVSAudioEditor4.EditorFileInfo.1";
				//LPOLESTR str =(wchar_t*) test.c_str();
				LPOLESTR str =(wchar_t*) impl_->progId.c_str();
				CLSID clsid; 
				HRESULT hr = CLSIDFromProgID(str, &clsid);
				if (S_OK == hr)
				{
					LPOLESTR className;
					if (S_OK == StringFromCLSID(clsid, &className))
					{
						classId = className;
						CoTaskMemFree(className);
					}
				}
			}
#endif
			if (classId.empty())
			{
				classId = L"{00024500-0000-0000-C000-000000000046}";
				badClassId = true;
			}
			CP_XML_ATTR(L"ax:classid", classId);

			CP_XML_ATTR(L"ax:persistence", L"persistStreamInit");
			CP_XML_ATTR(L"r:id", impl_->dataBinRid);
		
			if (!impl_->license.empty())
			{
				CP_XML_ATTR(L"ax:license", impl_->license);
			}	
			
			CP_XML_ATTR(L"xmlns:ax", L"http://schemas.microsoft.com/office/2006/activeX");
			CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");			

			//if (!impl_->progId.empty() && badClassId)
			//{		
			//	CP_XML_NODE(L"ax:ocxPr")
			//	{
			//		CP_XML_ATTR(L"ax:name", L"ProgId");
			//		CP_XML_ATTR(L"ax:value", impl_->progId);
			//	}
			//}
		}
	}
}



}

