#include "VbaReader.h"
#include "StreamObjects.h"
#include "../Common/simple_xml_writer.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../Common/DocxFormat/Source/XML/Utils.h"

CVbaReader::CVbaReader(const std::wstring & vbaFileName, const std::wstring & vbaExtractFile)
{
	vbaExtractFile_ = vbaExtractFile;

	vbaProject_file_ = boost::shared_ptr<CVbaFile>(new CVbaFile(vbaFileName));

	if (vbaProject_file_->isError()) return;
}
const std::wstring CVbaReader::convert()
{
	_UINT32 code_page_ = 0;

	CVbaFileStreamPtr strmPROJECT = vbaProject_file_->getNamedStream(L"PROJECT");

	if (false == vbaProject_file_->isDirectory(L"VBA")) return L"";

	CVbaFileStreamPtr strmDir = vbaProject_file_->getNamedStream(L"VBA/dir");
	VBA::DirStreamObjectPtr DirStreamObject = VBA::DirStreamObjectPtr(new VBA::DirStreamObject(strmDir));

	if (false == DirStreamObject->loadContent()) return L"";

	code_page_ = strmDir->CodePage;

	std::wstringstream strm;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"Information")
		{
			if (DirStreamObject->InformationRecord->NameRecord)
				CP_XML_ATTR(L"Name", DirStreamObject->InformationRecord->NameRecord->ProjectName.value);

			if (DirStreamObject->InformationRecord->LcidRecord)
				CP_XML_ATTR(L"Lcid", DirStreamObject->InformationRecord->LcidRecord->Lcid);

			if (DirStreamObject->InformationRecord->ConstantsRecord)
				CP_XML_ATTR(L"Constants", DirStreamObject->InformationRecord->ConstantsRecord->uConstants.value.empty() ?
					DirStreamObject->InformationRecord->ConstantsRecord->aConstants.value :
					DirStreamObject->InformationRecord->ConstantsRecord->uConstants.value);

		}
		CP_XML_NODE(L"References")
		{
			for (size_t i = 0; i < DirStreamObject->ReferencesRecord->ReferenceArray.size(); ++i)
			{
				CP_XML_NODE(L"Reference")
				{
					if (DirStreamObject->ReferencesRecord->ReferenceArray[i]->NameRecord)
						CP_XML_ATTR(L"Name", DirStreamObject->ReferencesRecord->ReferenceArray[i]->NameRecord->uName.value.empty() ?
							DirStreamObject->ReferencesRecord->ReferenceArray[i]->NameRecord->aName.value :
							DirStreamObject->ReferencesRecord->ReferenceArray[i]->NameRecord->uName.value);

					VBA::REFERENCEORIGINAL* original = dynamic_cast<VBA::REFERENCEORIGINAL*>(DirStreamObject->ReferencesRecord->ReferenceArray[i]->ReferenceRecord.get());
					VBA::REFERENCECONTROL* control = dynamic_cast<VBA::REFERENCECONTROL*>(DirStreamObject->ReferencesRecord->ReferenceArray[i]->ReferenceRecord.get());
					VBA::REFERENCEREGISTERED* registered = dynamic_cast<VBA::REFERENCEREGISTERED*>(DirStreamObject->ReferencesRecord->ReferenceArray[i]->ReferenceRecord.get());
					VBA::REFERENCEPROJECT* project = dynamic_cast<VBA::REFERENCEPROJECT*>(DirStreamObject->ReferencesRecord->ReferenceArray[i]->ReferenceRecord.get());

					if (original)
					{
						CP_XML_NODE(L"Original")
						{
							CP_XML_ATTR(L"val", original->LibidOriginal.value);
						}
					}
					if (project)
					{
						CP_XML_NODE(L"Project")
						{
							if (!project->LibidAbsolute.value.empty())
								CP_XML_ATTR(L"LibidAbsolute", project->LibidAbsolute.value);
							if (!project->LibidRelative.value.empty())
								CP_XML_ATTR(L"LibidRelative", project->LibidRelative.value);
						}
					}
					if (control)
					{
						CP_XML_NODE(L"Control")
						{
							CP_XML_ATTR(L"TypeLib", VBA::guid2bstr(control->OriginalTypeLib));
							if (!control->LibidTwiddled.value.empty())
								CP_XML_ATTR(L"LibidTwiddled", control->LibidTwiddled.value);
							if (!control->LibidExtended.value.empty())
								CP_XML_ATTR(L"LibidExtended", control->LibidExtended.value);
						}
					}
					if (registered)
					{
						CP_XML_NODE(L"Registered")
						{
							CP_XML_ATTR(L"val", registered->Libid.value);
						}
					}
				}
			}
		}
		CP_XML_NODE(L"Modules")
		{
			for (size_t i = 0; i < DirStreamObject->ModulesRecord->modules.size(); ++i)
			{
				if (!DirStreamObject->ModulesRecord->modules[i])
					continue;

				CP_XML_NODE(L"Module")
				{
					if (DirStreamObject->ModulesRecord->modules[i]->StreamNameRecord)
					{
						if (DirStreamObject->ModulesRecord->modules[i]->NameUnicodeRecord)
							CP_XML_ATTR(L"Name", DirStreamObject->ModulesRecord->modules[i]->NameUnicodeRecord->ModuleNameUnicode.value);
						else if (DirStreamObject->ModulesRecord->modules[i]->NameRecord)
							CP_XML_ATTR(L"Name", DirStreamObject->ModulesRecord->modules[i]->NameRecord->ModuleName.value);

						CP_XML_ATTR(L"Type", DirStreamObject->ModulesRecord->modules[i]->bProceduralModule ? L"Procedural" : L"Class");
						CP_XML_ATTR(L"ReadOnly", DirStreamObject->ModulesRecord->modules[i]->bReadOnly);
						CP_XML_ATTR(L"Private", DirStreamObject->ModulesRecord->modules[i]->bPrivate);

						std::wstring streamName = L"VBA/" + (DirStreamObject->ModulesRecord->modules[i]->StreamNameRecord->StreamNameUnicode.value.empty() ?
							DirStreamObject->ModulesRecord->modules[i]->StreamNameRecord->StreamName.value :
							DirStreamObject->ModulesRecord->modules[i]->StreamNameRecord->StreamNameUnicode.value);

						_UINT32 offset = DirStreamObject->ModulesRecord->modules[i]->OffsetRecord ?
							DirStreamObject->ModulesRecord->modules[i]->OffsetRecord->TextOffset : 0xffffffff;//skip cache

						if (offset == 0xffffffff) 
							continue; // error record

						CVbaFileStreamPtr strmModule = vbaProject_file_->getNamedStream(streamName, offset);
						strmModule->CodePage = code_page_;

						VBA::ModuleStreamObjectPtr ModuleStreamObject = VBA::ModuleStreamObjectPtr(new VBA::ModuleStreamObject(strmModule));
						ModuleStreamObject->loadContent();
						CP_XML_NODE(L"SourceCode")
						{
							CP_XML_STREAM() << XmlUtils::EncodeXmlString(ModuleStreamObject->SourceCode);
						}
					}
				}
			}
		}
	}
	return strm.str();
}

bool CVbaReader::write()
{
	const std::wstring sXml = convert();
	
	NSFile::CFileBinary file;
	if (file.CreateFileW(vbaExtractFile_))
	{
		std::string root = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
        file.WriteFile((BYTE*)root.c_str(), (DWORD)root.length());
		
		file.WriteStringUTF8(sXml);
		file.CloseFile();

        return true;
    }
    return false;
}

CVbaReader::~CVbaReader()
{
	//if (vba_context)		delete vba_context;
	//if (output_document)	delete output_document;
}
