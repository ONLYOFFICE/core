#pragma once

#include ".\XMLWrapper.h"
#include "..\common\utils.h"
#include <map>
#include <sstream>
#include <string>
#include "comutil.h"

namespace AVS
{
	namespace Image
	{
		namespace XML
		{
			class MetaDataXMLTransform
			{
			private:
				static std::wstring PrintCommonXMLElement(const std::wstring & TagName, std::wstring & TagValue, std::wstring & desc, std::wstring category)
				{
					std::wstringstream resultXMLString;
					resultXMLString << L"<CommonTag>";
					PrintCommonXMLElementInternal(resultXMLString, TagName, TagValue, desc, category);
					resultXMLString << L"</CommonTag>";
					return resultXMLString.str();
				}

			public:
				static std::wstringstream & PrintCommonXMLElementInternal(std::wstringstream & resultXMLString, const std::wstring & TagName, std::wstring & TagValue, std::wstring & desc, std::wstring category)
				{
					resultXMLString <<  L"<Name>" << TagName << "</Name>"
						<< L"<Value>" << TagValue << L"</Value>";

					if (desc.length() > 0)
						resultXMLString << L"<Description>" << desc <<L"</Description>";

					if (category.length() > 0)
						resultXMLString << L"<Category>" << category <<L"</Category>";
						
					return resultXMLString;
				}

			public:

				static bool TransformIPTCToCommon(BSTR bstrIPTCXML, BSTR *bstrResult)
				{
					_ASSERTE(NULL != bstrResult);
					::XML::XMLDocumentWrapper IPTCXmlDocument;
					if (!IPTCXmlDocument.LoadXML(bstrIPTCXML))
						return false;

					std::wstring resultXMLString = L"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
											<AVSImageMetaDataCommon>";

					::XML::XMLNodeListWrapper IPTCXmlNodes = IPTCXmlDocument.GetRootNode().FindNodes(CComBSTR(L"IPTCTag"));
					const int nCount = IPTCXmlNodes.GetCount();
					for (int i = 0; i < nCount; i++)
					{
						BSTR bstrTagName = NULL;
						if (!IPTCXmlNodes.GetNode(i).GetAttributeValue(L"tagname", &bstrTagName))
							continue;
						_bstr_t _bstrTagName;
						_bstrTagName.Attach(bstrTagName);

						BSTR bstrText = NULL;
						if (!IPTCXmlNodes.GetNode(i).GetText(&bstrText))
							continue;
						_bstr_t _bstrText;
						_bstrText.Attach(bstrText);
						
						resultXMLString += PrintCommonXMLElement(std::wstring(bstrTagName), std::wstring(bstrText), std::wstring(L""), std::wstring(L""));
					}
					resultXMLString += L"</AVSImageMetaDataCommon>";
					*bstrResult = ::SysAllocString(resultXMLString.c_str());
					return true;
				}

				enum TransformFlags
				{
					TRANSFORM_USEUNKNOWN = 0x1,
					TRANSFORM_RAWMAKERNOTETAGS = 0x2
				};


				/*!	\brief	Преобразовать XML из представления Exif в общее представление 'TagName' - 'TagValue'.
				*			При перобразовании используется XML с описанием преобразования.
				*
				*	\param[in]	bstrExifXML		Входная XML с представлением Exif-данных (см. ImageMetaDataExifShema.xsd)	
				*	\param[in]	bstrExifDesc	XML с описанием тегов Exif для преобразования имен
				*	\param[out]	bstrResult		Результирующая XML - описание метаданных в общем представлении (см. ImageMetaDataCommonSchema.xsd)
				*/
				static bool TransformExifToCommon(BSTR bstrExifXML, BSTR bstrExifDesc, BSTR * bstrResult,
					DWORD dwFlags =  (TRANSFORM_USEUNKNOWN | TRANSFORM_RAWMAKERNOTETAGS))
				{
					//ATLTRACE2(L"%s", bstrExifXML);
					//std::wcout << bstrExifXML << std::flush << std::endl;
					_ASSERTE(NULL != bstrResult);
					bool bResult;
					::XML::XMLDocumentWrapper exifXMLDocument, _ExifDesc;
					if (!exifXMLDocument.LoadXML(bstrExifXML))
					{
						_ASSERT_EXPR(false, L"Bad Exif XML"); // эту XML мы создаем сами!
						return false;
					}

					if (!_ExifDesc.LoadXML(bstrExifDesc))
					{
						return false;
					}

					typedef std::pair<std::wstring, std::wstring> ExifTagNameDesc;
					typedef std::map<unsigned short, ExifTagNameDesc> ExifDescMap;

					ExifDescMap _ExifDescMap;
					::XML::XMLNodeListWrapper _ExifDescNodes = _ExifDesc.GetRootNode().FindNodes(CComBSTR(L"Tag"));
					const int nExifDescNodesCount = _ExifDescNodes.GetCount();
					for (int i = 0; i < nExifDescNodesCount; i++)
					{
						BSTR bstrValue = NULL;
						if (!_ExifDescNodes.GetNode(i).GetAttributeValue(CComBSTR(L"tagid"), &bstrValue))
							continue;
						_bstr_t _bstrValue;
						_bstrValue.Attach(bstrValue);
						unsigned short nTagId;
						try
						{
							nTagId = AVS::Utils::lexical_cast<unsigned short>(std::wstring(bstrValue));
						}catch(std::bad_cast)
						{
							continue;
						}
						::SysFreeString(_bstrValue.Detach());
						bstrValue = NULL;

						if (!_ExifDescNodes.GetNode(i).GetAttributeValue(CComBSTR(L"name"), &bstrValue))
							continue;
						_bstrValue.Attach(bstrValue);
						std::wstring TagName = std::wstring(bstrValue);						
						::SysFreeString(_bstrValue.Detach());
						bstrValue = NULL;

						std::wstring TagDesc; 
						if (_ExifDescNodes.GetNode(i).GetAttributeValue(CComBSTR(L"tagname"), &bstrValue))
						{
							TagDesc = std::wstring(bstrValue);
							::SysFreeString(bstrValue);
						}
						_ExifDescMap[nTagId] = ExifTagNameDesc(TagName, TagDesc);
						::SysFreeString(bstrValue);
					}


					std::wstring resultXMLString = L"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
											<AVSImageMetaDataCommon>";
					::XML::XMLNodeListWrapper _ExifXmlNodes = exifXMLDocument.GetRootNode().FindNodes(CComBSTR(L"Tag"));
					const int nCount = _ExifXmlNodes.GetCount();
					for (int i = 0; i < nCount; i++)
					{
						BSTR bstrValue = NULL;						
						unsigned short nMakerNoteId = 0;
						_bstr_t _bstrValue;

						::XML::XMLNodeWrapper currentNodeCommon = _ExifXmlNodes.GetNode(i).FindNode(CComBSTR(L"CommonTag"));
						if (currentNodeCommon)
						{
							currentNodeCommon.GetXML(&bstrValue);
							_bstrValue.Attach(bstrValue);
							resultXMLString += _bstrValue;
							::SysFreeString(_bstrValue.Detach());
							bstrValue = NULL;
							continue;
						}

						::XML::XMLNodeWrapper currentNodeExif = _ExifXmlNodes.GetNode(i).FindNode(CComBSTR(L"ExifTag"));
						
						if (currentNodeExif.GetAttributeValue(CComBSTR(L"categoryid"), &bstrValue))
						{
							_bstrValue.Attach(bstrValue);
							try
							{
								nMakerNoteId = AVS::Utils::lexical_cast<unsigned short>(std::wstring(bstrValue));
							}catch(std::bad_cast)
							{
								continue;
							}
							::SysFreeString(_bstrValue.Detach());
							if ((nMakerNoteId != 0) && (!(dwFlags & TRANSFORM_RAWMAKERNOTETAGS)))
								continue;
						}
						
						if (!currentNodeExif.GetAttributeValue(CComBSTR(L"tagid"), &bstrValue))
							continue;
						
						_bstrValue.Attach(bstrValue);
						unsigned short nTagId;
						try
						{
							nTagId = AVS::Utils::lexical_cast<unsigned short>(std::wstring(bstrValue));
						}catch(std::bad_cast)
						{
							continue;
						}
						::SysFreeString(_bstrValue.Detach());
						
						if (!currentNodeExif.GetText(&bstrValue))
							continue;
						std::wstring TagValue = std::wstring(bstrValue);
						::SysFreeString(bstrValue);

						std::wstring TagCategory;
						if (currentNodeExif.GetAttributeValue(CComBSTR(L"category"), &bstrValue))
						{
							TagCategory = std::wstring(bstrValue);
							::SysFreeString(bstrValue);
							bstrValue = NULL;
						}
						
						ExifDescMap::iterator _FindResult = _ExifDescMap.find(nTagId);
						if ((_FindResult != _ExifDescMap.end()) && (nMakerNoteId == 0))
							resultXMLString += PrintCommonXMLElement(_FindResult->second.first, TagValue, _FindResult->second.second, TagCategory);
						else
						{
							if ((dwFlags & TRANSFORM_USEUNKNOWN))
							{
								std::wstring UnknownTagname = std::wstring(L"Unwnown Tag #") + AVS::Utils::lexical_cast<std::wstring>(nTagId);
								resultXMLString += PrintCommonXMLElement(UnknownTagname, TagValue, std::wstring(L""), TagCategory);
							}
						}
					}
					resultXMLString += L"</AVSImageMetaDataCommon>";

					*bstrResult = ::SysAllocString(resultXMLString.c_str());
					return true;
				}

				static bool GetInfoByTagId(USHORT _nTagId, BSTR bstrExifXML, BSTR * bstrTagValue, unsigned int &nTagCount, unsigned short &nTagType)
				{
					_ASSERTE(NULL != bstrTagValue);
					if (!bstrTagValue)
						return false;
					*bstrTagValue = NULL;
					::XML::XMLDocumentWrapper exifXMLDocument;
					if (!exifXMLDocument.LoadXML(bstrExifXML))
						return false;

					::XML::XMLNodeListWrapper _ExifXmlNodes = exifXMLDocument.GetRootNode().FindNodes(CComBSTR(L"Tag"));
					const int nCount = _ExifXmlNodes.GetCount();
					for (int i = 0; i < nCount; i++)
					{
						::XML::XMLNodeWrapper currentNode = _ExifXmlNodes.GetNode(i).FindNode(CComBSTR(L"ExifTag"));
						BSTR bstrValue = NULL;						
						if (!currentNode.GetAttributeValue(CComBSTR(L"tagid"), &bstrValue))
							continue;
						_bstr_t _bstrValue;
						_bstrValue.Attach(bstrValue);
						unsigned short nTagId;
						try
						{
							nTagId = AVS::Utils::lexical_cast<unsigned short>(std::wstring(bstrValue));
						}catch(std::bad_cast)
						{
							continue;
						}
						_bstrValue.Detach();
						if (nTagId == _nTagId)
						{
							unsigned int tag_count;
							unsigned short tag_type;
							BSTR tag_value = NULL;
							_bstr_t _tag_value;
							if (currentNode.GetText(&tag_value))
								_tag_value.Attach(tag_value);

							currentNode.GetAttributeValue(L"type",&bstrValue);
							_bstrValue.Attach(bstrValue);
							try
							{
								tag_type = AVS::Utils::lexical_cast<unsigned short>(std::wstring(_bstrValue.Detach()));
							}catch(std::bad_cast)
							{
								continue;		
							}
							currentNode.GetAttributeValue(L"count",&bstrValue);
							_bstrValue.Attach(bstrValue);
							try
							{
								tag_count = AVS::Utils::lexical_cast<unsigned int>(std::wstring(_bstrValue.Detach()));
							}catch(std::bad_cast)
							{
								continue;		
							}
							*bstrTagValue = _tag_value.Detach();
							nTagCount = tag_count;
							nTagType = tag_type;
							return true;
						}
					}
					return false;
				}
			};
		}
	}
}