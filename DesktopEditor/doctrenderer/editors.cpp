#include "./editors.h"

#include "../common/StringBuilder.h"

namespace NSDoctRenderer
{
	namespace
	{
		bool AppendScript(NSStringUtils::CStringBuilderA* builder, const std::wstring& path, const std::string& header = "", const std::string& footer = "")
		{
			NSFile::CFileBinary oFile;

			if (!oFile.OpenFile(path))
				return false;

			int size = (int)oFile.GetFileSize();
			if (size < 3)
				return false;

			BYTE* pData = new BYTE[size];
			DWORD dwReadSize = 0;
			oFile.ReadFile(pData, (DWORD)size, dwReadSize);
			oFile.CloseFile();

			int nOffset = 0;
			if (pData[0] == 0xEF && pData[1] == 0xBB && pData[2] == 0xBF)
				nOffset = 3;

			if (!header.empty())
				builder->WriteString(header);

			builder->WriteString((char*)(pData + nOffset), size - nOffset);

			if (!footer.empty())
				builder->WriteString(footer);

			builder->WriteString("\n\n");
			RELEASEARRAYOBJECTS(pData);

			return true;
		}

		bool RunScript(JSSmart<NSJSBase::CJSContext>& context, JSSmart<NSJSBase::CJSTryCatch>& try_catch, const std::wstring& path)
		{
			NSFile::CFileBinary oFile;

			if (!oFile.OpenFile(path))
				return false;

			int size = (int)oFile.GetFileSize();
			if (size < 3)
				return false;

			BYTE* pData = new BYTE[size];
			DWORD dwReadSize = 0;
			oFile.ReadFile(pData, (DWORD)size, dwReadSize);
			oFile.CloseFile();

			int nOffset = 0;
			if (pData[0] == 0xEF && pData[1] == 0xBB && pData[2] == 0xBF)
				nOffset = 3;

			context->runScript(std::string((char*)(pData + nOffset), size - nOffset), try_catch);
			RELEASEARRAYOBJECTS(pData);

			return !try_catch->Check();
		}

		std::wstring GetAllScript(NSStringUtils::CStringBuilderA* builder, const DoctRendererEditorType& type, CDoctRendererConfig* config, const bool& isSnapshot = false)
		{
			for (std::vector<std::wstring>::const_iterator i = config->m_arrFiles.cbegin(); i != config->m_arrFiles.cend(); i++)
				AppendScript(builder, *i);

			std::wstring sFontsPath = config->m_strSdkPath + L"/common/libfont/engine";
#ifdef SUPPORT_HARFBUZZ_SHAPER
			sFontsPath += L"/fonts_native.js";
#else
			sFontsPath += L"/fonts_ie.js";
#endif

			std::wstring sCachePath = L"";

			switch (type)
			{
			case DoctRendererEditorType::WORD:
			{
				AppendScript(builder, config->m_strSdkPath + L"/word/sdk-all-min.js");
				AppendScript(builder, sFontsPath);
				AppendScript(builder, config->m_strSdkPath + L"/word/sdk-all.js");
				sCachePath = config->m_strSdkPath + L"/word/sdk-all";
				break;
			}
			case DoctRendererEditorType::SLIDE:
			{
				AppendScript(builder, config->m_strSdkPath + L"/slide/sdk-all-min.js");
				AppendScript(builder, sFontsPath);
				AppendScript(builder, config->m_strSdkPath + L"/slide/sdk-all.js");
				sCachePath = config->m_strSdkPath + L"/slide/sdk-all";
				break;
			}
			case DoctRendererEditorType::CELL:
			{
				AppendScript(builder, config->m_strSdkPath + L"/cell/sdk-all-min.js");
				AppendScript(builder, sFontsPath);
				AppendScript(builder, config->m_strSdkPath + L"/cell/sdk-all.js");
				builder->WriteString("\n$.ready();", 11);
				sCachePath = config->m_strSdkPath + L"/cell/sdk-all";
				break;
			}
			case DoctRendererEditorType::VISIO:
			{
				if (!NSFile::CFileBinary::Exists(config->m_strSdkPath + L"/visio/sdk-all-min.js"))
					return L"";
				AppendScript(builder, config->m_strSdkPath + L"/visio/sdk-all-min.js");
				AppendScript(builder, sFontsPath);
				AppendScript(builder, config->m_strSdkPath + L"/visio/sdk-all.js");
				sCachePath = config->m_strSdkPath + L"/visio/sdk-all";
				break;
			}
			case DoctRendererEditorType::PDF:
			{
				AppendScript(builder, config->m_strSdkPath + L"/word/sdk-all-min.js");
				AppendScript(builder, sFontsPath);
				AppendScript(builder, config->m_strSdkPath + L"/word/sdk-all.js");
				AppendScript(builder, config->m_strSdkPath + L"/pdf/src/engine/drawingfile_native.js");
				AppendScript(builder, config->m_strSdkPath + L"/pdf/src/annotations/stamps.json", "window[\"native_pdf_stamps\"]=", ";");
				sCachePath = config->m_strSdkPath + L"/pdf/sdk-all";
				break;
			}
			default:
				break;
			}

			if (!sCachePath.empty())
				sCachePath += (isSnapshot ? L".bin" : L".cache");

			return sCachePath;
		}

		std::wstring GetSnapshotPath(const DoctRendererEditorType& type, CDoctRendererConfig* config)
		{
			std::wstring sCachePath = L"";

			switch (type)
			{
			case DoctRendererEditorType::WORD:
			{
				return config->m_strSdkPath + L"/word/sdk-all.bin";
			}
			case DoctRendererEditorType::SLIDE:
			{
				return config->m_strSdkPath + L"/slide/sdk-all.bin";
			}
			case DoctRendererEditorType::CELL:
			{
				return config->m_strSdkPath + L"/cell/sdk-all.bin";
			}
			case DoctRendererEditorType::VISIO:
			{
				return config->m_strSdkPath + L"/visio/sdk-all.bin";
			}
			case DoctRendererEditorType::PDF:
			{
				return config->m_strSdkPath + L"/pdf/sdk-all.bin";
			}
			default:
				break;
			}
			return L"";
		}

		bool RunEditorFooter(JSSmart<NSJSBase::CJSContext>& context, JSSmart<NSJSBase::CJSTryCatch>& try_catch, CDoctRendererConfig* config)
		{
			if (!RunScript(context, try_catch, config->m_strAllFonts))
				return false;

			std::string sFooter = "window.InitNativeEditors();";

			if (context->isSnapshotUsed())
			{
				sFooter += "\
if (undefined === String.prototype.replaceAll)\
{\
String.prototype.replaceAll = function(str, newStr)\
{\
  if (Object.prototype.toString.call(str).toLowerCase() === '[object regexp]')\
	return this.replace(str, newStr);\
  return this.split(str).join(newStr);\
};\
}\n";
			}

			context->runScript(sFooter, try_catch);
			return !try_catch->Check();
		}
	} // namespace

	bool RunEditor(const DoctRendererEditorType& type, JSSmart<NSJSBase::CJSContext>& context, JSSmart<NSJSBase::CJSTryCatch>& try_catch, CDoctRendererConfig* config)
	{
		if (context->isSnapshotUsed())
			return RunEditorFooter(context, try_catch, config);

		NSStringUtils::CStringBuilderA builder;
		builder.AddSize(10 * 1024 * 1024);
		std::wstring sCachePath = GetAllScript(&builder, type, config);

		if (!sCachePath.empty())
		{
			context->runScript(builder.GetData(), try_catch, config->m_bIsUseCache ? sCachePath : L"");
			RunEditorFooter(context, try_catch, config);
		}

		return !try_catch->Check();
	}

	bool GenerateEditorSnapshot(const DoctRendererEditorType& type, CDoctRendererConfig* config)
	{
		NSStringUtils::CStringBuilderA builder;
		builder.AddSize(10 * 1024 * 1024);
		std::wstring sCachePath = GetAllScript(&builder, type, config, true);

		builder.WriteString("delete String.prototype.replaceAll;");

		if (sCachePath.empty())
			return false;

		return NSJSBase::CJSContext::generateSnapshot(builder.GetData(), sCachePath);
	}

	namespace
	{
		JSSmart<NSJSBase::CJSContext> RunEditorWithSnapshot(const DoctRendererEditorType& type, CDoctRendererConfig* config)
		{
	#ifndef V8_SUPPORT_SNAPSHOTS
			return NULL;
	#endif

			std::wstring sCachePath = GetSnapshotPath(type, config);
			if (!NSFile::CFileBinary::Exists(sCachePath))
				return NULL;

			if (sCachePath.empty())
				return NULL;

			JSSmart<NSJSBase::CJSContext> context = new NSJSBase::CJSContext(false);
			context->Initialize(sCachePath);

			return context;
		}
	} // namespace

	JSSmart<NSJSBase::CJSContext> CreateEditorContext(const DoctRendererEditorType& type, CDoctRendererConfig* config)
	{
		JSSmart<NSJSBase::CJSContext> context = RunEditorWithSnapshot(type, config);
		if (context.is_init())
			return context;
		return new NSJSBase::CJSContext();
	}
} // namespace NSDoctRenderer
