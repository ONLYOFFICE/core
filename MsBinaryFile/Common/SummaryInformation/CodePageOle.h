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
#pragma once
#include "Property.h"

#include <string>
#include <vector>
#include <boost/smart_ptr/shared_array.hpp>
#include "../../../DesktopEditor/common/Types.h"
#include "../../../OOXML/Base/Base.h"
namespace OLEPS
{

class PropertyCodePage : public Property
{
public:
    PropertyCodePage(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type), code_page(0)
	{
	}
	virtual bool Read(XLS::CFStreamPtr stream);
	virtual std::wstring toString();

	static const unsigned short DefaultCodePage = 1250;	
	unsigned short code_page;
};
typedef boost::shared_ptr<PropertyCodePage> PropertyCodePagePtr;
//-----------------------------------------------------------------------------------------
class PropertyString : public Property
{
public:
	PropertyString(unsigned int prop_type, const unsigned short value_type, unsigned short code_page_) : Property(prop_type, value_type)
	{
		code_page = code_page_;
	}
	
	virtual bool Read(XLS::CFStreamPtr stream);
	virtual std::wstring toString();
	virtual bool IsEmpty();

	std::wstring value;
	unsigned short code_page;
};
typedef boost::shared_ptr<PropertyString> PropertyStringPtr;
//-----------------------------------------------------------------------------------------
class PropertyWString : public Property
{
public:
	PropertyWString(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type)
	{
	}

	virtual bool Read(XLS::CFStreamPtr stream);
	virtual std::wstring toString();

	std::wstring value;
};
typedef boost::shared_ptr<PropertyWString> PropertyWStringPtr;
//-----------------------------------------------------------------------------------------
class PropertyDTM : public Property
{
public:
    PropertyDTM(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type)
	{
	}
	virtual bool IsEmpty();

	virtual std::wstring toString();
	virtual bool Read(XLS::CFStreamPtr stream);
	
	_UINT32 dwLowDateTime;
	_UINT32 dwHighDateTime;
};
typedef boost::shared_ptr<PropertyDTM> PropertyDTMPtr;
//-----------------------------------------------------------------------------------------
class PropertyInt : public Property
{
public:
    PropertyInt(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type)
	{
	}
	virtual bool Read(XLS::CFStreamPtr stream);
	virtual std::wstring toString();

	_UINT32			value;
};
typedef boost::shared_ptr<PropertyInt> PropertyIntPtr;
//-----------------------------------------------------------------------------------------
class PropertyBool : public Property
{
public:
    PropertyBool(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type)
	{
	}
	virtual bool Read(XLS::CFStreamPtr stream);
	virtual std::wstring toString();

	bool value;
};
typedef boost::shared_ptr<PropertyDTM> PropertyDTMPtr;
//----------------------------------------------------------------------------------
class PropertyVecString : public Property
{
public:
	PropertyVecString(unsigned int prop_type, const unsigned short value_type, unsigned short code_page_) : Property(prop_type, value_type)
	{
		code_page = code_page_;
	}

	virtual bool Read(XLS::CFStreamPtr stream);
	virtual std::wstring toString();

	std::vector<std::wstring> values;
	unsigned short code_page;
};
typedef boost::shared_ptr<PropertyVecString> PropertyVecStringPtr;
//----------------------------------------------------------------------------------
class PropertyVecHeadingPair : public Property
{
public:
	struct HeadingPair
	{
		std::wstring headingString;
		_INT32 headerParts; // + TypeId(2) + Padding(2)
	};
	PropertyVecHeadingPair(unsigned int prop_type, const unsigned short value_type, unsigned short code_page_) : Property(prop_type, value_type)
	{
		code_page = code_page_;
	}

	virtual bool Read(XLS::CFStreamPtr stream);
	virtual std::wstring toString();

	std::vector<HeadingPair> values;
	unsigned short code_page;
};
typedef boost::shared_ptr<PropertyVecHeadingPair> PropertyVecHeadingPairPtr;
//----------------------------------------------------------------------------------
class PropertyDigSig : public Property
{
public:
	struct SerializedPropertyEntry
	{
		_UINT32 id = 0x00000020;
		_UINT32 encodingType;
		_UINT32 length;
		boost::shared_array<char> value; //ignored for read
		
		bool Read(XLS::CFStreamPtr stream);
	};
	struct SerializedCertificateEntry
	{
		_UINT32 id = 0x00000020;
		_UINT32 encodingType;
		_UINT32 length;
		boost::shared_array<char> certificate; //ASN.1 [ITUX680-1994] DER encoding of an X.509 certificate as specified by [RFC3280].
		
		bool Read(XLS::CFStreamPtr stream);
	};
	struct CertStoreCertificateGroup
	{
		std::vector<SerializedPropertyEntry> elementList; // until SerializedPropertyEntry.id == 0x00000020
		SerializedCertificateEntry certificateElement;
		
		bool Read(XLS::CFStreamPtr stream);
	};
	struct EndElementMarkerEntry
	{
		_UINT32 id = 0x00000000;
		_UINT64 marker = 0x0000000000000000;
	};
	struct VBASigSerializedCertStore
	{
		_UINT32 version;
		_UINT32 fileType = 0x54524543;
		CertStoreCertificateGroup certGroup;
		EndElementMarkerEntry endMarkerElement; 
		
		bool Read(XLS::CFStreamPtr stream);
	};
	struct SignatureInfo
	{
		_UINT32 cbSignature;
		_UINT32 signatureOffset;
		_UINT32 cbSigningCertStore;
		_UINT32 certStoreOffset;
		_UINT32 cbProjectName;
		_UINT32 projectNameOffset;
		_UINT32 fTimestamp = 0; //reserved
		_UINT32 cbTimestampUrl = 0; //reserved
		_UINT32 timestampUrlOffset;

		boost::shared_array<char> pbSignatureBuffer; //PKCS #7 SignedData (cbSigningCertStore)
		VBASigSerializedCertStore pbSigningCertStoreBuffer;

		std::wstring rgchProjectNameBuffer; //null-terminate
		std::wstring rgchTimestampBuffer;

		bool Read(XLS::CFStreamPtr stream);
	};
	struct DigSigBlob
	{
		_UINT32 cb;
		_UINT32 serializedPointer;
		SignatureInfo sigInfo;
		// padding; //skip for reading
	};
	PropertyDigSig(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type) {}

	virtual bool Read(XLS::CFStreamPtr stream);
	virtual std::wstring toString();

	DigSigBlob data;
};
typedef boost::shared_ptr<PropertyDigSig> PropertyDigSigPtr;
} // namespace OLEPS
