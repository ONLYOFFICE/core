/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
#include "doctrenderer.h"
#include "docbuilder.h"

#ifdef BOOL
#undef BOOL
#endif

#include "embed/Default.h"
#include "embed/NativeControlEmbed.h"
#include "embed/GraphicsEmbed.h"
#include "embed/DrawingFileEmbed.h"

#include "./editors.h"
#include <iostream>

namespace NSDoctRenderer
{
	void ParseStringAsInts(const std::string& s, std::vector<int>& arr)
	{
		const char* data = s.c_str();
		int curOld = 0;
		int cur = 0;
		int valCur = 0;
		int len = (int)s.length();

		while (cur < len)
		{
			if (data[cur] == ',')
			{
				if (cur > curOld)
					arr.push_back(valCur);

				valCur = 0;
				curOld = cur + 1;
			}
			else
			{
				valCur *= 10;
				valCur += (data[cur] - '0');
			}
			++cur;
		}

		if (cur > curOld)
			arr.push_back(valCur);
	}

	class CExecuteParams
	{
	public:
		DoctRendererFormat::FormatFile m_eSrcFormat;
		DoctRendererFormat::FormatFile m_eDstFormat;

		std::wstring m_strFontsDirectory;
		std::wstring m_strImagesDirectory;
		std::wstring m_strThemesDirectory;

		std::wstring m_strSrcFilePath;
		std::wstring m_strDstFilePath;

		std::vector<std::wstring> m_arChanges;
		int m_nCountChangesItems;

		std::wstring m_strMailMergeDatabasePath;
		std::wstring m_strMailMergeField;
		int m_nMailMergeIndexStart;
		int m_nMailMergeIndexEnd;

		std::wstring m_sJsonParams;
		int m_nLcid;

		std::vector<int> m_arThemesThumbnailsParams;

	public:
		CExecuteParams() : m_arChanges()
		{
			m_eSrcFormat = DoctRendererFormat::INVALID;
			m_eDstFormat = DoctRendererFormat::INVALID;

			m_strFontsDirectory = L"";
			m_strImagesDirectory = L"";
			m_strThemesDirectory = L"";

			m_strSrcFilePath = L"";
			m_strDstFilePath = L"";

			m_nCountChangesItems = -1;

			m_strMailMergeDatabasePath = L"";
			m_strMailMergeField = L"";
			m_nMailMergeIndexStart = -1;
			m_nMailMergeIndexEnd = -1;

			m_nLcid = -1;
		}
		~CExecuteParams()
		{
			m_arChanges.clear();
		}

	public:
		bool FromXml(const std::wstring& strXml)
		{
			XmlUtils::CXmlNode oNode;
			if (!oNode.FromXmlString(strXml))
				return false;

			m_strSrcFilePath = oNode.ReadValueString(L"SrcFilePath");
			m_strDstFilePath = oNode.ReadValueString(L"DstFilePath");

			m_eSrcFormat = (DoctRendererFormat::FormatFile)(oNode.ReadValueInt(L"SrcFileType"));
			m_eDstFormat = (DoctRendererFormat::FormatFile)(oNode.ReadValueInt(L"DstFileType"));

			m_strFontsDirectory = oNode.ReadValueString(L"FontsDirectory");
			m_strImagesDirectory = oNode.ReadValueString(L"ImagesDirectory");
			m_strThemesDirectory = oNode.ReadValueString(L"ThemesDirectory");

			XmlUtils::CXmlNode oNodeChanges;
			if (oNode.GetNode(L"Changes", oNodeChanges))
			{
				m_nCountChangesItems = oNodeChanges.ReadAttributeInt(L"TopItem", -1);

				std::vector<XmlUtils::CXmlNode> oNodes;
				oNodeChanges.GetNodes(L"Change", oNodes);

				size_t nCount = oNodes.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& _node = oNodes[i];
					m_arChanges.push_back(_node.GetText());
				}
			}

			XmlUtils::CXmlNode oNodeMailMerge;
			if (oNode.GetNode(L"MailMergeData", oNodeMailMerge))
			{
				m_strMailMergeDatabasePath = oNodeMailMerge.ReadAttribute(L"DatabasePath");
				m_nMailMergeIndexStart = oNodeMailMerge.ReadAttributeInt(L"Start", -1);
				m_nMailMergeIndexEnd = oNodeMailMerge.ReadAttributeInt(L"End", -1);
				m_strMailMergeField = oNodeMailMerge.ReadAttribute(L"Field");
			}

			m_nLcid = oNode.ReadValueInt(L"Lcid", -1);
			m_sJsonParams = oNode.ReadValueString(L"JsonParams");

			m_arThemesThumbnailsParams.clear();
			std::wstring sThemesThumbnailsParams = oNode.ReadValueString(L"ThemesThumbnailsParams");
			if (!sThemesThumbnailsParams.empty())
			{
				std::string sThemesThumbnailsParamsA = U_TO_UTF8(sThemesThumbnailsParams);

				if (!sThemesThumbnailsParamsA.empty())
					ParseStringAsInts(sThemesThumbnailsParamsA, m_arThemesThumbnailsParams);
			}

			return true;
		}
	};
} // namespace NSDoctRenderer

namespace NSDoctRenderer
{
	std::wstring string_replaceAll(std::wstring str, const std::wstring& from, const std::wstring& to)
	{
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::wstring::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
		return str;
	}
} // namespace NSDoctRenderer

namespace NSDoctRenderer
{
	static void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo)
	{
		size_t posn = 0;
		while (std::wstring::npos != (posn = text.find(replaceFrom, posn)))
		{
			text.replace(posn, replaceFrom.length(), replaceTo);
			posn += replaceTo.length();
		}
	}
	static void replace_for_xml(std::wstring& text)
	{
		string_replace(text, L"&", L"&amp;");
		string_replace(text, L"'", L"&apos;");
		string_replace(text, L"<", L"&lt;");
		string_replace(text, L">", L"&gt;");
		string_replace(text, L"\"", L"&quot;");
	}

	class CDoctRenderer_Private : public CDoctRendererConfig
	{
	public:
		CExecuteParams m_oParams;

		std::wstring m_strEditorType;
		std::wstring m_strFilePath;

		std::vector<std::wstring> m_arImagesInChanges;

		IOfficeDrawingFile* m_pDrawingFile;

	public:
		CDoctRenderer_Private(const std::wstring& sAllFontsPath = L"") : CDoctRendererConfig()
		{
			LoadConfig(NSFile::GetProcessDirectory(), sAllFontsPath);
			m_pDrawingFile = NULL;
		}
		~CDoctRenderer_Private()
		{
		}
		void LoadConfig(const std::wstring& sConfigDir, const std::wstring& sAllFontsPath = L"")
		{
			if (!sAllFontsPath.empty())
			{
				SetAllFontsExternal(sAllFontsPath);
			}

			CDoctRendererConfig::Parse(sConfigDir);
		}

	public:
		static void _LOGGING_ERROR_(const std::wstring& strType, const std::wstring& strError)
		{
#if 0
			if (m_sErrorsLogFile.empty())
				return;

			FILE* f = NSFile::CFileBinary::OpenFileNative(m_sErrorsLogFile, L"a+");

			std::string sT = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strType);
			std::string sE = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strError);

			fprintf(f, sT.c_str());
			fprintf(f, ": ");
			fprintf(f, sE.c_str());
			fprintf(f, "\n");
			fclose(f);
#endif
			std::string sT = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strType);
			std::string sE = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strError);

			std::cerr << sT << ": " << sE << std::endl;
		}

		static bool Doct_renderer_SaveFile(CExecuteParams* pParams,
										   NSNativeControl::CNativeControl* pNative, JSSmart<CJSContext> context,
										   JSSmart<CJSValue>* args, std::wstring& strError, JSSmart<CJSObject>& api_js_maybe_null)
		{
			JSSmart<CJSTryCatch> try_catch = context->GetExceptions();
			JSSmart<CJSObject> global_js = context->GetGlobal();

			JSSmart<CJSObject> js_objectApi = api_js_maybe_null;
			if (!js_objectApi.IsInit() || js_objectApi->isUndefined())
				js_objectApi = global_js->get("Api")->toObject();

			bool bIsBreak = false;
			if (js_objectApi->isUndefined() || !js_objectApi->isObject())
			{
				_LOGGING_ERROR_(L"save_code_api", L"");
				_LOGGING_ERROR_(L"save_api", L"");

				strError = L"code=\"save_api\"";
				bIsBreak = true;
				return bIsBreak;
			}

			switch (pParams->m_eDstFormat)
			{
			case DoctRendererFormat::DOCT:
			case DoctRendererFormat::PPTT:
			case DoctRendererFormat::XLST:
			{
				JSSmart<CJSValue> js_result2 = js_objectApi->call_func("asc_nativeGetFileData", 1, args);
				if (try_catch->Check())
				{
					strError = L"code=\"save\"";
					bIsBreak = true;
				}
				else
				{
					JSSmart<CJSTypedArray> typedArray = js_result2->toTypedArray();
					NSJSBase::CJSDataBuffer oBuffer = typedArray->getData();

					NSFile::CFileBinary oFile;
					if (true == oFile.CreateFileW(pParams->m_strDstFilePath))
					{
						if (pNative->m_sHeader.find(";v10;") == std::string::npos)
						{
							oFile.WriteFile((BYTE*)pNative->m_sHeader.c_str(), (DWORD)pNative->m_sHeader.length());

							char* pDst64 = NULL;
							int nDstLen = 0;
							NSFile::CBase64Converter::Encode(oBuffer.Data, pNative->m_nSaveBinaryLen, pDst64, nDstLen, NSBase64::B64_BASE64_FLAG_NOCRLF);

							oFile.WriteFile((BYTE*)pDst64, (DWORD)nDstLen);

							RELEASEARRAYOBJECTS(pDst64);
						}
						else
						{
							oFile.WriteFile(oBuffer.Data, (DWORD)pNative->m_nSaveBinaryLen);
						}
						oFile.CloseFile();
					}

					if (oBuffer.IsExternalize)
						oBuffer.Free();
				}
				break;
			}
			case DoctRendererFormat::HTML:
			{
				// CALCULATE
				if (pParams->m_sJsonParams.empty())
					args[0] = CJSContext::createNull();
				else
				{
					std::string sTmp = U_TO_UTF8((pParams->m_sJsonParams));
					args[0] = context->JSON_Parse(sTmp.c_str());
				}

				JSSmart<CJSValue> js_result1 = js_objectApi->call_func("asc_nativeCalculateFile", 1, args);
				if (try_catch->Check())
				{
					strError = L"code=\"calculate\"";
					bIsBreak = true;
				}

				JSSmart<CJSValue> js_result2 = js_objectApi->call_func("asc_nativeGetHtml", 1, args);
				if (try_catch->Check())
				{
					strError = L"code=\"save\"";
					bIsBreak = true;
				}
				else
				{
					std::string sHTML_Utf8 = js_result2->toStringA();

					NSFile::CFileBinary oFile;
					if (true == oFile.CreateFileW(pParams->m_strDstFilePath))
					{
						oFile.WriteFile((BYTE*)sHTML_Utf8.c_str(), (DWORD)sHTML_Utf8.length());
						oFile.CloseFile();
					}
				}
				break;
			}
			case DoctRendererFormat::PDF:
			case DoctRendererFormat::PPTX_THEME_THUMBNAIL:
			case DoctRendererFormat::IMAGE:
			{
				// CALCULATE
				if (pParams->m_sJsonParams.empty())
					args[0] = CJSContext::createNull();
				else
				{
					std::string sTmp = U_TO_UTF8((pParams->m_sJsonParams));
					args[0] = context->JSON_Parse(sTmp.c_str());
				}

				JSSmart<CJSValue> js_result2 = js_objectApi->call_func("asc_nativeCalculateFile", 1, args);
				if (try_catch->Check())
				{
					strError = L"code=\"calculate\"";
					bIsBreak = true;
				}

				// PAGESCOUNT
				LONG lPagesCount = 0;
				if (!bIsBreak)
				{
					JSSmart<CJSValue> js_result1 = js_objectApi->call_func("asc_nativePrintPagesCount", 1, args);
					if (try_catch->Check())
					{
						strError = L"code=\"calculate\"";
						bIsBreak = true;
					}
					else
						lPagesCount = (LONG)js_result1->toInt32();
				}

				// RENDER
				if (!bIsBreak && (DoctRendererFormat::PDF == pParams->m_eDstFormat || DoctRendererFormat::IMAGE == pParams->m_eDstFormat))
				{
					if (pParams->m_sJsonParams.empty())
					{
						if (DoctRendererFormat::IMAGE == pParams->m_eDstFormat)
						{
							args[0] = context->JSON_Parse("{ \"saveFormat\" : \"image\" }");
						}
						else
							args[0] = CJSContext::createNull();
					}
					else
					{
						std::string sTmp = U_TO_UTF8((pParams->m_sJsonParams));
						args[0] = context->JSON_Parse(sTmp.c_str());

						if (DoctRendererFormat::IMAGE == pParams->m_eDstFormat)
						{
							JSSmart<CJSObject> argObj = args[0]->toObject();
							argObj->set("saveFormat", CJSContext::createString("image"));
						}
					}

					JSSmart<CJSValue> js_result2 = js_objectApi->call_func("asc_nativeGetPDF", 1, args);
					if (try_catch->Check())
					{
						strError = L"code=\"save\"";
						bIsBreak = true;
					}
					else if (!js_result2->isNull())
					{
						JSSmart<CJSTypedArray> typedArray = js_result2->toTypedArray();
						NSJSBase::CJSDataBuffer oBuffer = typedArray->getData();

						DWORD bufferSize = (pNative->m_nSaveBinaryLen == 0) ? (DWORD)oBuffer.Len : (DWORD)pNative->m_nSaveBinaryLen;

						NSFile::CFileBinary oFile;
						if (true == oFile.CreateFileW(pParams->m_strDstFilePath))
						{
							oFile.WriteFile(oBuffer.Data, bufferSize);
							oFile.CloseFile();
						}

						if (oBuffer.IsExternalize)
							oBuffer.Free();
					}
				}
				if (!bIsBreak && DoctRendererFormat::PPTX_THEME_THUMBNAIL == pParams->m_eDstFormat)
				{
					JSSmart<CJSValue> js_result2;
					int nSizeArgs = (int)pParams->m_arThemesThumbnailsParams.size();
					if (0 == nSizeArgs)
					{
						js_result2 = js_objectApi->call_func("asc_nativeGetThemeThumbnail", 1, args);
					}
					else
					{
						JSSmart<CJSArray> arrArgs = CJSContext::createArray(nSizeArgs);
						int nCurArg = 0;
						for (std::vector<int>::iterator iter = pParams->m_arThemesThumbnailsParams.begin(); iter != pParams->m_arThemesThumbnailsParams.end(); iter++)
							arrArgs->set(nCurArg++, CJSContext::createInt(*iter));

						args[0] = arrArgs->toValue();
						js_result2 = js_objectApi->call_func("asc_nativeGetThemeThumbnail", 1, args);
					}

					if (try_catch->Check())
					{
						strError = L"code=\"save\"";
						bIsBreak = true;
					}
					else
					{
						if (!js_result2->isNull())
						{
							JSSmart<CJSObject> objNative = js_result2->toObject();

							JSSmart<CJSValue> oDataArray = objNative->get("data");
							JSSmart<CJSTypedArray> oTypedArray = oDataArray->toTypedArray();

							NSJSBase::CJSDataBuffer oBuffer = oTypedArray->getData();
							std::wstring sThemeName = objNative->get("name")->toStringW();
							int nDataLen = objNative->get("dataLen")->toInt32();

							if (sThemeName.empty())
								sThemeName = L"Default";

							NSFile::CFileBinary oFile;
							if (true == oFile.CreateFileW(pParams->m_strDstFilePath + L"/" + sThemeName + L".theme"))
							{
								oFile.WriteFile(oBuffer.Data, (DWORD)nDataLen);
								oFile.CloseFile();
							}

							if (oBuffer.IsExternalize)
								oBuffer.Free();
						}
						else
						{
							strError = L"code=\"save (invalid theme)\"";
							bIsBreak = true;
						}
					}
				}
				break;
			}
			case DoctRendererFormat::WATERMARK:
			{
				if (!pParams->m_sJsonParams.empty())
				{
					std::string sTmp = U_TO_UTF8((pParams->m_sJsonParams));
					args[0] = context->JSON_Parse(sTmp.c_str());

					JSSmart<CJSValue> js_watermark = js_objectApi->call_func("asc_nativeGetWatermark", 1, args);
					if (!try_catch->Check() && js_watermark->isString())
					{
						std::string sWatermark = js_watermark->toStringA();
						std::string::size_type pos = sWatermark.find(',');
						if (pos != std::string::npos)
						{
							int nInputSize = (int)(sWatermark.length() - pos) - 1;
							char* pInput = (char*)(sWatermark.c_str() + pos + 1);

							int nOutputSize = 0;
							BYTE* pOutput = NULL;

							if (NSFile::CBase64Converter::Decode(pInput, nInputSize, pOutput, nOutputSize))
							{
								NSFile::CFileBinary oFileWatermark;
								if (oFileWatermark.CreateFileW(pParams->m_strDstFilePath))
								{
									oFileWatermark.WriteFile(pOutput, nOutputSize);
									oFileWatermark.CloseFile();
								}

								RELEASEARRAYOBJECTS(pOutput);
							}
						}
					}
				}
				break;
			}
			default:
				break;
			}
			return bIsBreak;
		}

		bool ExecuteScript(NSDoctRenderer::DoctRendererEditorType& editorType, std::wstring& strError, std::wstring& strReturnParams)
		{
			LOGGER_SPEED_START();

			bool bIsBreak = false;
			JSSmart<CJSContext> context = NSDoctRenderer::CreateEditorContext(editorType, this);

			if (true)
			{
				CJSContextScope scope(context);
				CJSContext::Embed<CNativeControlEmbed>(false);
				CJSContext::Embed<CGraphicsEmbed>();
				NSJSBase::CreateDefaults();

				JSSmart<CJSTryCatch> try_catch = context->GetExceptions();

				JSSmart<CJSObject> global_js = context->GetGlobal();
				global_js->set("window", global_js);
				global_js->set("self", global_js);

				JSSmart<CJSObject> oNativeCtrl = CJSContext::createEmbedObject("CNativeControlEmbed");
				global_js->set("native", oNativeCtrl);

				NSNativeControl::CNativeControl* pNative = static_cast<NSNativeControl::CNativeControl*>(oNativeCtrl->getNative()->getObject());
				pNative->m_sConsoleLogFile = m_sConsoleLogFile;

				LOGGER_SPEED_LAP("compile");

				NSDoctRenderer::RunEditor(editorType, context, try_catch, this);

				if (editorType == DoctRendererEditorType::PDF && m_pDrawingFile)
					EmbedDrawingFile(context, m_pDrawingFile);

				if (try_catch->Check())
				{
					strError = L"code=\"run\"";
					bIsBreak = true;
				}

				LOGGER_SPEED_LAP("run");

				//---------------------------------------------------------------
				JSSmart<CJSValue> args[1];
				args[0] = CJSContext::createInt(0);

				// GET_NATIVE_ENGINE
				if (pNative != NULL)
				{
					pNative->m_pChanges = &m_oParams.m_arChanges;
					pNative->m_strFontsDirectory = m_oParams.m_strFontsDirectory;
					pNative->m_strImagesDirectory = m_oParams.m_strImagesDirectory;

					pNative->CheckFonts();

					pNative->m_strEditorType = m_strEditorType;
					pNative->SetFilePath(m_strFilePath);

					pNative->m_nMaxChangesNumber = m_oParams.m_nCountChangesItems;
				}

				// Api object
				JSSmart<CJSObject> js_objectApi;

				bool bIsWatermark = (m_oParams.m_eDstFormat == NSDoctRenderer::DoctRendererFormat::WATERMARK) ? true : false;
				JSSmart<CJSValue> openOptions;

				if (!bIsBreak)
				{
					if (!m_oParams.m_sJsonParams.empty())
					{
						std::string sTmp = U_TO_UTF8((m_oParams.m_sJsonParams));
						openOptions = context->JSON_Parse(sTmp.c_str());

						if (0 < m_oParams.m_nLcid)
						{
							if (openOptions->isObject())
								openOptions->toObject()->set("locale", CJSContext::createInt(m_oParams.m_nLcid));
						}
					}
					else
					{
						JSSmart<CJSObject> optionsParams = CJSContext::createObject();
						if (0 < m_oParams.m_nLcid)
							optionsParams->set("locale", CJSContext::createInt(m_oParams.m_nLcid));
						openOptions = optionsParams->toValue();
					}
				}

				// OPEN
				if (!bIsBreak)
				{
					if (!bIsWatermark)
					{
						CChangesWorker oWorkerLoader;
						int nVersion = 0;

						if (editorType == DoctRendererEditorType::PDF)
							nVersion = -1;
						else
							nVersion = oWorkerLoader.OpenNative(pNative->GetFilePath());

						JSSmart<CJSValue> args_open[4];
						args_open[0] = (nVersion != -1) ? oWorkerLoader.GetDataFull()->toValue() : CJSContext::createUndefined();
						args_open[1] = CJSContext::createInt(nVersion);
						std::wstring sXlsx = NSFile::GetDirectoryName(pNative->GetFilePath()) + L"/Editor.xlsx";
						args_open[2] = NSFile::CFileBinary::Exists(sXlsx) ? CJSContext::createString(sXlsx) : CJSContext::createUndefined();
						args_open[3] = openOptions;

						global_js->call_func("NativeOpenFileData", 4, args_open);
					}
					else
					{
						JSSmart<CJSValue> args_open[1];
						args_open[0] = openOptions;

						global_js->call_func("NativeCreateApi", 1, args_open);
					}

					if (try_catch->Check())
					{
						strError = L"code=\"open\"";
						bIsBreak = true;
					}

					js_objectApi = global_js->get("Api")->toObject();
					if (try_catch->Check())
					{
						strError = L"code=\"open\"";
						bIsBreak = true;
					}
				}

				LOGGER_SPEED_LAP("open");

				// CHANGES
				if (!bIsBreak && !bIsWatermark)
				{
					if (m_oParams.m_arChanges.size() != 0)
					{
						int nCurrentIndex = 0;
						CChangesWorker oWorker;

						int nFileType = 0;
						if (m_strEditorType == L"spreadsheet")
							nFileType = 1;

						oWorker.SetFormatChanges(nFileType);
						oWorker.CheckFiles(m_oParams.m_arChanges);

						while (!bIsBreak)
						{
							nCurrentIndex = oWorker.Open(m_oParams.m_arChanges, nCurrentIndex);
							bool bIsFull = (nCurrentIndex == m_oParams.m_arChanges.size()) ? true : false;

							JSSmart<CJSValue> args_changes[2];
							args_changes[0] = oWorker.GetData()->toValue();
							args_changes[1] = CJSContext::createBool(bIsFull);
							js_objectApi->call_func("asc_nativeApplyChanges2", 2, args_changes);
							if (try_catch->Check())
							{
								char buffer[50];
								sprintf(buffer, "index=\"%d\"", pNative->m_nCurrentChangesNumber - 1);
								std::string s(buffer);
								strError = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)s.c_str(), (LONG)s.length());
								bIsBreak = true;
							}

							if (bIsFull)
								break;
						}

						// images in changes
						if (NULL != pNative)
						{
							for (std::map<std::wstring, bool>::const_iterator iter = pNative->m_mapImagesInChanges.begin(); iter != pNative->m_mapImagesInChanges.end(); iter++)
							{
								m_arImagesInChanges.push_back(iter->first);
							}
						}
					}
				}

				LOGGER_SPEED_LAP("changes");

				bool bIsMailMerge = false;
				if (!m_oParams.m_strMailMergeDatabasePath.empty() && m_oParams.m_nMailMergeIndexEnd >= m_oParams.m_nMailMergeIndexStart && m_oParams.m_nMailMergeIndexEnd >= 0)
				{
					bIsMailMerge = true;
				}

				// MAIL MERGE
				if (!bIsBreak)
				{
					if (bIsMailMerge)
					{
						// OPEN DATABASE
						NSFile::CFileBinary oFileDataBase;
						if (oFileDataBase.OpenFile(m_oParams.m_strMailMergeDatabasePath))
						{
							DWORD dwSizeBase = (DWORD)oFileDataBase.GetFileSize();
							DWORD dwSizeRead = 0;
							BYTE* pBaseData = new BYTE[dwSizeBase + 1];
							oFileDataBase.ReadFile(pBaseData, dwSizeBase, dwSizeRead);

							if (dwSizeBase != dwSizeRead)
							{
								RELEASEARRAYOBJECTS(pBaseData);
								bIsBreak = true;

								strError = L"mailmerge=\"databaseopen\"";
							}
							else
							{
								int nStart = 0;
								if (dwSizeBase > 3)
								{
									if (pBaseData[0] == 0xEF && pBaseData[1] == 0xBB && pBaseData[2] == 0xBF)
									{
										nStart = 3;
									}
								}

								pBaseData[dwSizeBase] = 0;
								JSSmart<CJSValue> args_changes[1];
								args_changes[0] = context->JSON_Parse((char*)(pBaseData + nStart));

								js_objectApi->call_func("asc_StartMailMergeByList", 1, args_changes);
								if (try_catch->Check())
								{
									strError = L"mailmerge=\"databaseopenjs\"";
									bIsBreak = true;
								}
							}
						}
						else
						{
							strError = L"mailmerge=\"databaseopen\"";
							bIsBreak = true;
						}

						if (!bIsBreak)
						{
							std::string sFieldUtf8 = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(m_oParams.m_strMailMergeField.c_str(), (LONG)m_oParams.m_strMailMergeField.length());

							strReturnParams += L"<MailMergeFields>";
							for (int nIndexMM = m_oParams.m_nMailMergeIndexStart; nIndexMM <= m_oParams.m_nMailMergeIndexEnd && !bIsBreak; ++nIndexMM)
							{
								JSSmart<CJSValue> args_changes[1];
								args_changes[0] = CJSContext::createInt(nIndexMM);

								js_objectApi->call_func("asc_PreviewMailMergeResult", 1, args_changes);
								if (try_catch->Check())
								{
									strError = L"mailmerge=\"preview" + std::to_wstring(nIndexMM) + L"\"";
									bIsBreak = true;
								}

								std::wstring sSaveFile = L"";
								if (!bIsBreak)
								{
									// SAVE
									std::wstring sSaveOld = m_oParams.m_strDstFilePath;
									m_oParams.m_strDstFilePath += (L"/file" + std::to_wstring(nIndexMM));
									sSaveFile = m_oParams.m_strDstFilePath;
									bIsBreak = Doct_renderer_SaveFile(&m_oParams, pNative, context, args, strError, js_objectApi);

									m_oParams.m_strDstFilePath = sSaveOld;
								}

								if (!bIsBreak)
								{
									JSSmart<CJSValue> args_changes[2];
									args_changes[0] = CJSContext::createInt(nIndexMM);
									args_changes[1] = CJSContext::createString(sFieldUtf8);

									JSSmart<CJSValue> js_result2 = js_objectApi->call_func("asc_GetMailMergeFiledValue", 2, args_changes);
									if (try_catch->Check())
									{
										strError = L"mailmerge=\"field" + std::to_wstring(nIndexMM) + L"\"";
										bIsBreak = true;
									}

									std::wstring sField = js_result2->toStringW();
									NSDoctRenderer::replace_for_xml(sField);
									NSDoctRenderer::replace_for_xml(sSaveFile);

									strReturnParams += L"<file path=\"" + sSaveFile + L"\" field=\"" + sField + L"\" />";
								}
							}
							strReturnParams += L"</MailMergeFields>";
						}
					}
				}

				// SAVE
				if (!bIsBreak && !bIsMailMerge)
				{
					bIsBreak = Doct_renderer_SaveFile(&m_oParams, pNative, context, args, strError, js_objectApi);
				}

				LOGGER_SPEED_LAP("save");
			}

			context->Dispose();

			return bIsBreak ? false : true;
		}

		std::string ReadScriptFile(const std::wstring& strFile)
		{
			NSFile::CFileBinary oFile;

			if (!oFile.OpenFile(strFile))
				return "";

			int nSize = (int)oFile.GetFileSize();
			if (nSize < 3)
				return "";

			BYTE* pData = new BYTE[nSize];
			DWORD dwReadSize = 0;
			oFile.ReadFile(pData, (DWORD)nSize, dwReadSize);

			int nOffset = 0;
			if (pData[0] == 0xEF && pData[1] == 0xBB && pData[2] == 0xBF)
			{
				nOffset = 3;
			}

			std::string sReturn((char*)(pData + nOffset), nSize - nOffset);

			RELEASEARRAYOBJECTS(pData);
			return sReturn;
		}
	};

	CDoctrenderer::CDoctrenderer(const std::wstring& sAllFontsPath)
	{
		m_pInternal = new CDoctRenderer_Private(sAllFontsPath);
	}

	void CDoctrenderer::LoadConfig(const std::wstring& sConfigDir, const std::wstring& sAllFontsPath)
	{
		m_pInternal->LoadConfig(sConfigDir, sAllFontsPath);
	}

	CDoctrenderer::~CDoctrenderer()
	{
		RELEASEOBJECT(m_pInternal);
	}

	bool CDoctrenderer::Execute(const std::wstring& strXml, std::wstring& strError)
	{
		strError = L"";
		m_pInternal->m_oParams.FromXml(strXml);
		m_pInternal->m_arImagesInChanges.clear();

		NSDoctRenderer::DoctRendererEditorType editorType = NSDoctRenderer::DoctRendererEditorType::INVALID;
		switch (m_pInternal->m_oParams.m_eSrcFormat)
		{
		case DoctRendererFormat::DOCT:
		{
			switch (m_pInternal->m_oParams.m_eDstFormat)
			{
			case DoctRendererFormat::DOCT:
			case DoctRendererFormat::PDF:
			case DoctRendererFormat::IMAGE:
			case DoctRendererFormat::HTML:
			case DoctRendererFormat::WATERMARK:
			{
				editorType = NSDoctRenderer::DoctRendererEditorType::WORD;
				m_pInternal->m_strEditorType = L"document";
				break;
			}
			default:
				return false;
			}
			break;
		}
		case DoctRendererFormat::PPTT:
		{
			switch (m_pInternal->m_oParams.m_eDstFormat)
			{
			case DoctRendererFormat::PPTT:
			case DoctRendererFormat::PDF:
			case DoctRendererFormat::IMAGE:
			case DoctRendererFormat::PPTX_THEME_THUMBNAIL:
			{
				editorType = NSDoctRenderer::DoctRendererEditorType::SLIDE;
				m_pInternal->m_strEditorType = L"presentation";
				break;
			}
			default:
				return false;
			}
			break;
		}
		case DoctRendererFormat::XLST:
		{
			switch (m_pInternal->m_oParams.m_eDstFormat)
			{
			case DoctRendererFormat::XLST:
			case DoctRendererFormat::PDF:
			case DoctRendererFormat::IMAGE:
			{
				editorType = NSDoctRenderer::DoctRendererEditorType::CELL;
				m_pInternal->m_strEditorType = L"spreadsheet";
				break;
			}
			default:
				return false;
			}
			break;
		}
		case DoctRendererFormat::VSDT:
		{
			switch (m_pInternal->m_oParams.m_eDstFormat)
			{
			case DoctRendererFormat::VSDT:
			case DoctRendererFormat::PDF:
			case DoctRendererFormat::IMAGE:
			{
				editorType = NSDoctRenderer::DoctRendererEditorType::VISIO;
				m_pInternal->m_strEditorType = L"draw";
				break;
			}
			default:
				return false;
			}
			break;
		}
		case DoctRendererFormat::PDF:
		{
			switch (m_pInternal->m_oParams.m_eDstFormat)
			{
			case DoctRendererFormat::PDF:
			{
				editorType = NSDoctRenderer::DoctRendererEditorType::PDF;
				m_pInternal->m_strEditorType = L"pdf";
				break;
			}
			default:
				return false;
			}
			break;
		}

		default:
			return false;
		}

		std::wstring strFileName = m_pInternal->m_oParams.m_strSrcFilePath;
		if (!NSFile::CFileBinary::Exists(strFileName))
		{
			strFileName += L"/Editor.bin";
		}

		strFileName = string_replaceAll(strFileName, L"\\\\", L"\\");
		strFileName = string_replaceAll(strFileName, L"//", L"/");
		strFileName = string_replaceAll(strFileName, L"\\", L"/");

		m_pInternal->m_strFilePath = strFileName;

		std::wstring sReturnParams = L"";
		bool bResult = m_pInternal->ExecuteScript(editorType, strError, sReturnParams);

		if (strError.length() != 0)
		{
			strError = L"<result><error " + strError + L" />" + sReturnParams + L"</result>";
		}
		else if (sReturnParams.length() != 0)
		{
			strError = L"<result>" + sReturnParams + L"</result>";
		}

		return bResult ? true : false;
	}

	std::vector<std::wstring> CDoctrenderer::GetImagesInChanges()
	{
		return m_pInternal->m_arImagesInChanges;
	}

	void CDoctrenderer::CreateCache(const std::wstring& sAllFontsPath, const std::wstring& sCacheDir)
	{
#ifndef JS_ENGINE_JAVASCRIPTCORE
		LoadConfig(NSFile::GetProcessDirectory(), sAllFontsPath);

		std::vector<NSDoctRenderer::DoctRendererEditorType> editors;
		editors.push_back(NSDoctRenderer::DoctRendererEditorType::WORD);
		editors.push_back(NSDoctRenderer::DoctRendererEditorType::SLIDE);
		editors.push_back(NSDoctRenderer::DoctRendererEditorType::CELL);
		editors.push_back(NSDoctRenderer::DoctRendererEditorType::VISIO);
		editors.push_back(NSDoctRenderer::DoctRendererEditorType::PDF);

		for (std::vector<NSDoctRenderer::DoctRendererEditorType>::const_iterator i = editors.begin(); i != editors.end(); i++)
		{
			JSSmart<CJSContext> context = new CJSContext();

			if (true)
			{
				CJSContextScope scope(context);
				CJSContext::Embed<CNativeControlEmbed>();
				CJSContext::Embed<CGraphicsEmbed>();
				NSJSBase::CreateDefaults();

				JSSmart<CJSObject> global = context->GetGlobal();
				global->set("window", global);
				global->set("self", global);
				global->set("native", CJSContext::createEmbedObject("CNativeControlEmbed"));

				JSSmart<CJSTryCatch> try_catch = context->GetExceptions();

				NSDoctRenderer::RunEditor(*i, context, try_catch, m_pInternal);
			}

			context->Dispose();
		}
#endif
	}

	void CDoctrenderer::CreateSnapshots()
	{
#ifdef V8_SUPPORT_SNAPSHOTS
		std::vector<NSDoctRenderer::DoctRendererEditorType> editors;
		editors.push_back(NSDoctRenderer::DoctRendererEditorType::WORD);
		editors.push_back(NSDoctRenderer::DoctRendererEditorType::SLIDE);
		editors.push_back(NSDoctRenderer::DoctRendererEditorType::CELL);
		editors.push_back(NSDoctRenderer::DoctRendererEditorType::VISIO);
		editors.push_back(NSDoctRenderer::DoctRendererEditorType::PDF);

		// initialize v8
		JSSmart<CJSContext> context = new CJSContext();

		for (std::vector<NSDoctRenderer::DoctRendererEditorType>::const_iterator i = editors.begin(); i != editors.end(); i++)
		{
			NSDoctRenderer::GenerateEditorSnapshot(*i, m_pInternal);
		}
#endif
	}

	void CDoctrenderer::SetAdditionalParam(const AdditionalParamType& type, void* data)
	{
		switch (type)
		{
		case AdditionalParamType::DRAWINGFILE:
			m_pInternal->m_pDrawingFile = (IOfficeDrawingFile*)data;
			break;
		default:
			break;
		}
	}
} // namespace NSDoctRenderer

bool Doct_renderer_SaveFile_ForBuilder(
	int nFormat, const std::wstring& strDstFile, NSNativeControl::CNativeControl* pNative, JSSmart<CJSContext> context, JSSmart<CJSValue>* args, std::wstring& strError, const std::wstring& jsonParams)
{
	NSDoctRenderer::CExecuteParams oParams;
	oParams.m_eDstFormat = (NSDoctRenderer::DoctRendererFormat::FormatFile)nFormat;
	oParams.m_strDstFilePath = strDstFile;
	oParams.m_sJsonParams = jsonParams;

	JSSmart<CJSObject> js_objectApi; // empty
	return NSDoctRenderer::CDoctRenderer_Private::Doct_renderer_SaveFile(&oParams, pNative, context, args, strError, js_objectApi);
}
