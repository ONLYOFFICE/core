#include "VbaReader.h"
#include "StreamObjects.h"
#include "../Utils/simple_xml_writer.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../OOXML/Base/Unit.h"

CVbaReader::CVbaReader(const std::wstring & vbaFileName, const std::wstring & vbaExtractFile)
{
	vbaExtractFile_ = vbaExtractFile;

	vbaProject_file_ = boost::shared_ptr<CVbaFile>(new CVbaFile(vbaFileName));

	if (vbaProject_file_->isError()) return;
}
const std::wstring CVbaReader::convert()
{
	_UINT32 code_page_ = 0;

	if (false == vbaProject_file_->isDirectory(L"VBA")) return L"";

	CVbaFileStreamPtr strmDir = vbaProject_file_->getNamedStream(L"VBA/dir");
	VBA::DirStreamObjectPtr DirStreamObject = VBA::DirStreamObjectPtr(new VBA::DirStreamObject(strmDir));

	if (false == DirStreamObject->loadContent()) return L"";

	code_page_ = strmDir->CodePage;
//------------------------------------------------------------------------------------------
	CVbaFileStreamPtr strmPROJECT = vbaProject_file_->getNamedStream(L"PROJECT");
	VBA::ProjectStreamObjectPtr ProjectStreamObject = VBA::ProjectStreamObjectPtr(new VBA::ProjectStreamObject(strmPROJECT));
	strmPROJECT->CodePage = code_page_;

	ProjectStreamObject->loadContent();

//------------------------------------------------------------------------------------------
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
				VBA::REFERENCEPtr & reference = DirStreamObject->ReferencesRecord->ReferenceArray[i];
				if (!reference)
					continue;
				CP_XML_NODE(L"Reference")
				{
					if (reference->NameRecord)
						CP_XML_ATTR(L"Name", reference->NameRecord->uName.value.empty() ?
												reference->NameRecord->aName.value : reference->NameRecord->uName.value);

					VBA::REFERENCEORIGINAL* original = dynamic_cast<VBA::REFERENCEORIGINAL*>(reference->ReferenceRecord.get());
					VBA::REFERENCECONTROL* control = dynamic_cast<VBA::REFERENCECONTROL*>(reference->ReferenceRecord.get());
					VBA::REFERENCEREGISTERED* registered = dynamic_cast<VBA::REFERENCEREGISTERED*>(reference->ReferenceRecord.get());
					VBA::REFERENCEPROJECT* project = dynamic_cast<VBA::REFERENCEPROJECT*>(reference->ReferenceRecord.get());

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
				VBA::MODULEPtr &module = DirStreamObject->ModulesRecord->modules[i];
				
				if (!module)
					continue;

				CP_XML_NODE(L"Module")
				{
					if (module->StreamNameRecord)
					{
						if (module->NameUnicodeRecord)
							CP_XML_ATTR(L"Name", module->NameUnicodeRecord->ModuleNameUnicode.value);
						else if (module->NameRecord)
							CP_XML_ATTR(L"Name", module->NameRecord->ModuleName.value);

						CP_XML_ATTR(L"Type", module->bProceduralModule ? L"Procedural" : L"Class");
						CP_XML_ATTR(L"ReadOnly", module->bReadOnly);
						CP_XML_ATTR(L"Private", module->bPrivate);

						std::wstring moduleName = (module->StreamNameRecord->StreamNameUnicode.value.empty() ?
													module->StreamNameRecord->StreamName.value : module->StreamNameRecord->StreamNameUnicode.value);
						
						std::wstring streamName = L"VBA/" + moduleName;

						_UINT32 offset = module->OffsetRecord ?	module->OffsetRecord->TextOffset : 0xffffffff;//skip cache

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

		if (false == ProjectStreamObject->DesignerModules.empty())
		{
			CP_XML_NODE(L"DesignerModules")
			{
				for (size_t i = 0; i < ProjectStreamObject->DesignerModules.size(); ++i)
				{
					std::wstring streamName = ProjectStreamObject->DesignerModules[i] + L"/VBFrame";
					CVbaFileStreamPtr strmVBFrame = vbaProject_file_->getNamedStream(streamName);
					strmVBFrame->CodePage = code_page_;
					
					VBA::VBFrameObjectPtr VBFrameObject = VBA::VBFrameObjectPtr(new VBA::VBFrameObject(strmVBFrame));
					VBFrameObject->loadContent();

					CP_XML_NODE(L"DesignerModule")
					{
						CP_XML_ATTR(L"Name", XmlUtils::EncodeXmlString(ProjectStreamObject->DesignerModules[i]));
						CP_XML_NODE(L"Properties")
						{
							for (size_t j = 0; j < VBFrameObject->Props.size(); ++j)
							{
								CP_XML_ATTR(VBFrameObject->Props[j].first.c_str(), XmlUtils::EncodeXmlString(VBFrameObject->Props[j].second));
							}
						}
						CP_XML_STREAM() << convertObject(ProjectStreamObject->DesignerModules[i], code_page_);
					}
				}
			}
		}
	}
	return strm.str();
}
std::wstring CVbaReader::convertObject(const std::wstring & name, unsigned int code_page)
{
	VBA::FormControlStreamPtr FormControlStream;

	std::wstring streamName;

	streamName = name + L"/f";
	CVbaFileStreamPtr strmForm = vbaProject_file_->getNamedStream(streamName);
	if (strmForm)
	{
		strmForm->CodePage = code_page;

		FormControlStream = VBA::FormControlStreamPtr(new VBA::FormControlStream(strmForm));
		FormControlStream->loadContent();
	}
	streamName = name + L"/o";
	CVbaFileStreamPtr strmObject = vbaProject_file_->getNamedStream(streamName);
	
	_UINT32 nextStreamPositionEmbedded = 0;
	if (strmObject)
	{
		strmObject->CodePage = code_page;
	}

	std::wstringstream strm;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"FormControl")
		{
			if (FormControlStream)
			{
				if (FormControlStream->Control)
				{
					CP_XML_NODE(L"Data")
					{
						CP_XML_ATTR_STR(L"Caption", FormControlStream->Control->Caption);
						CP_XML_ATTR_OPT(L"BackColor", FormControlStream->Control->BackColor);
						CP_XML_ATTR_OPT(L"ForeColor", FormControlStream->Control->ForeColor);
						CP_XML_ATTR_OPT(L"NextAvailableID", FormControlStream->Control->NextAvailableID);
						CP_XML_ATTR_OPT(L"BorderStyle", FormControlStream->Control->BorderStyle);
						CP_XML_ATTR_OPT(L"MousePointer", FormControlStream->Control->MousePointer);
						CP_XML_ATTR_OPT(L"ScrollBars", FormControlStream->Control->ScrollBars);
						CP_XML_ATTR_OPT(L"GroupCnt", FormControlStream->Control->GroupCnt);
						CP_XML_ATTR_OPT(L"Cycle", FormControlStream->Control->Cycle);
						CP_XML_ATTR_OPT(L"SpecialEffect", FormControlStream->Control->SpecialEffect);
						CP_XML_ATTR_OPT(L"BorderColor", FormControlStream->Control->BorderColor);
						CP_XML_ATTR_OPT(L"Zoom", FormControlStream->Control->Zoom);
						CP_XML_ATTR_OPT(L"PictureAlignment", FormControlStream->Control->PictureAlignment);
						CP_XML_ATTR_OPT(L"PictureSizeMode", FormControlStream->Control->PictureSizeMode);
						CP_XML_ATTR_OPT(L"ShapeCookie", FormControlStream->Control->ShapeCookie);
						CP_XML_ATTR_OPT(L"DrawBuffer", FormControlStream->Control->DrawBuffer);

						if (FormControlStream->Control->DisplayedSize)
						{
							CP_XML_NODE(L"DisplayedSize")
							{
								CP_XML_ATTR(L"Width", FormControlStream->Control->DisplayedSize->Width);
								CP_XML_ATTR(L"Height", FormControlStream->Control->DisplayedSize->Height);
							}
						}
						if (FormControlStream->Control->LogicalSize)
						{
							CP_XML_NODE(L"LogicalSize")
							{
								CP_XML_ATTR(L"Width", FormControlStream->Control->LogicalSize->Width);
								CP_XML_ATTR(L"Height", FormControlStream->Control->LogicalSize->Height);
							}
						}
						if (FormControlStream->Control->ScrollPosition)
						{
							CP_XML_NODE(L"ScrollPosition")
							{
								CP_XML_ATTR(L"Left", FormControlStream->Control->ScrollPosition->Left);
								CP_XML_ATTR(L"Top", FormControlStream->Control->ScrollPosition->Top);
							}
						}
						if (FormControlStream->Control->Font)
						{
							CP_XML_NODE(L"Font")
							{
								VBA::StdFont* stdFont = dynamic_cast<VBA::StdFont*>(FormControlStream->Control->Font.get());
								if (stdFont)
								{
									CP_XML_ATTR_STR(L"Name", stdFont->sFontName);
									CP_XML_ATTR_OPT(L"Height", stdFont->FontHeight);
									CP_XML_ATTR_OPT(L"Weight", stdFont->FontWeight);
									CP_XML_ATTR_OPT(L"CharSet", stdFont->FontCharSet);
									CP_XML_ATTR_OPT(L"PitchAndFamily", stdFont->FontPitchAndFamily);
									CP_XML_ATTR_OPT(L"ParagraphAlign", stdFont->ParagraphAlign);
									CP_XML_ATTR_OPT(L"Bold", stdFont->bFontBold);
									CP_XML_ATTR_OPT(L"Italic", stdFont->bFontItalic);
									CP_XML_ATTR_OPT(L"Underline", stdFont->bFontUnderline);
									CP_XML_ATTR_OPT(L"Strikeout", stdFont->bFontStrikeout);
									CP_XML_ATTR_OPT(L"AutoColor", stdFont->bFontAutoColor);
								}
							}
						}
					}
				}
				if (FormControlStream->DesignExData)
				{
				}
				if (FormControlStream->SiteData)
				{
					CP_XML_NODE(L"Sites")
					{
						for (size_t i = 0; i < FormControlStream->SiteData->Sites.size(); ++i)
						{
							VBA::OleSiteConcreteControlPtr & site = FormControlStream->SiteData->Sites[i];
							if (site->ObjectStreamSize && strmObject)
							{
								site->Object = VBA::ActiveXObjectPtr(OOX::ActiveXObject::Create(*site->ClsidCacheIndex));

								site->Object->Parse(strmObject->getData() + nextStreamPositionEmbedded, *site->ObjectStreamSize);
								nextStreamPositionEmbedded += *site->ObjectStreamSize;
							}

							CP_XML_NODE(L"Site")
							{
								CP_XML_ATTR_OPT(L"ID", site->ID);
								CP_XML_ATTR_OPT(L"HelpContextID", site->HelpContextID);
								CP_XML_ATTR_OPT(L"TabIndex", site->TabIndex);
								CP_XML_ATTR_OPT(L"ClsidCacheIndex", site->ClsidCacheIndex);
								//CP_XML_ATTR_OPT(L"ObjectStreamSize", site->ObjectStreamSize);
								CP_XML_ATTR_OPT(L"GroupID", site->GroupID);

								if (site->BitFlags)
								{
									CP_XML_ATTR(L"fTabStop", site->BitFlags->fTabStop);
									CP_XML_ATTR(L"fVisible", site->BitFlags->fVisible);
									CP_XML_ATTR(L"fDefault", site->BitFlags->fDefault);
									CP_XML_ATTR(L"fCancel", site->BitFlags->fCancel);
									CP_XML_ATTR(L"fAutoSize", site->BitFlags->fAutoSize);
									CP_XML_ATTR(L"fPreserveHeight", site->BitFlags->fPreserveHeight);
									CP_XML_ATTR(L"fFitToParent", site->BitFlags->fFitToParent);
									CP_XML_ATTR(L"fSelectChild", site->BitFlags->fSelectChild);
								}

								CP_XML_ATTR_STR(L"Name", site->Name);
								CP_XML_ATTR_STR(L"Tag", site->Tag);
								CP_XML_ATTR_STR(L"ControlTipText", site->ControlTipText);
								CP_XML_ATTR_STR(L"RuntimeLicKey", site->RuntimeLicKey);
								CP_XML_ATTR_STR(L"ControlSource", site->ControlSource);
								CP_XML_ATTR_STR(L"RowSource", site->RowSource);

								if (site->SitePosition)
								{
									CP_XML_NODE(L"SitePosition")
									{
										CP_XML_ATTR(L"Left", site->SitePosition->Left);
										CP_XML_ATTR(L"Top", site->SitePosition->Top);
									}
								}

								if (site->Object)
								{
									CP_XML_NODE(L"EmbeddedObject")
									{
										CP_XML_STREAM() << site->Object->toXml();
									}
								}

								if ((site->BitFlags) && (site->BitFlags->fPromoteControls) && (site->ID))
								{
									//read child objects
									std::wstring name_child = name + L"/i" + (*site->ID < 10 ? L"0" : L"") + std::to_wstring(*site->ID);

									CP_XML_STREAM() << convertObject(name_child, code_page);
								}
							}
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
