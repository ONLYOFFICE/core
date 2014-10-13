#pragma once
#ifndef PPTX_COMMENTS_COMMENTS_H_
#define PPTX_COMMENTS_COMMENTS_H_

#include "WrapperFile.h"
#include "FileContainer.h"

namespace PPTX
{
	namespace Logic
	{
		class Comment : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Comment)
		private:
			nullable_int	authorId;
			nullable_int	idx;
			nullable_string dt;

			nullable_int	pos_x;
			nullable_int	pos_y;

			nullable_string	text;
			
			nullable_int	parentAuthorId;
			nullable_int	parentCommentId;

			nullable_string additional_data; // teamlab editor information!!!

		public:
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"authorId", authorId);
				node.ReadAttributeBase(L"dt", dt);
				node.ReadAttributeBase(L"idx", idx);

				XmlUtils::CXmlNode oNodePos = node.ReadNode(_T("p:pos"));
				if (oNodePos.IsValid())
				{
					oNodePos.ReadAttributeBase(L"x", pos_x);
					oNodePos.ReadAttributeBase(L"y", pos_y);
				}
				
				XmlUtils::CXmlNode oNodeText = node.ReadNode(_T("p:text"));
				if (oNodeText.IsValid())
					text = oNodeText.GetTextExt();
				
				XmlUtils::CXmlNode oNodeExtLst = node.ReadNode(_T("p:extLst"));

				bool bIsFound1 = false;
				bool bIsFound2 = false;
				if (oNodeExtLst.IsValid())
				{
					XmlUtils::CXmlNodes oNodesExt;
					if (oNodeExtLst.GetNodes(_T("p:ext"), oNodesExt))
					{
						int nCountExts = oNodesExt.GetCount();
						for (int nIndex = 0; nIndex < nCountExts; ++nIndex)
						{
							XmlUtils::CXmlNode oNodeExt;
							oNodesExt.GetAt(nIndex, oNodeExt);

							// parent
							if (!bIsFound1)
							{
								XmlUtils::CXmlNode oNodeTI = oNodeExt.ReadNode(_T("p15:threadingInfo"));
								if (oNodeTI.IsValid())
								{
									XmlUtils::CXmlNode oNodeParent = oNodeTI.ReadNode(_T("p15:parentCm"));

									oNodeParent.ReadAttributeBase(L"authorId", parentAuthorId);
									oNodeParent.ReadAttributeBase(L"idx", parentCommentId);

									bIsFound1 = true;
								}
							}
							if (!bIsFound2)
							{
								XmlUtils::CXmlNode oNodeAU = oNodeExt.ReadNode(_T("p15:presenceInfo"));
								if (oNodeAU.IsValid())
								{
									CString strData = oNodeAU.GetAttribute(_T("userId"));

									strData.Replace(_T("&amp;"),	_T("&"));
									strData.Replace(_T("&apos;"),	_T("'"));
									strData.Replace(_T("&lt;"),		_T("<"));
									strData.Replace(_T("&gt;"),		_T(">"));
									strData.Replace(_T("&quot;"),	_T("\""));

									if (_T("") != strData)
										additional_data = strData;

									bIsFound2 = true;
								}
							}
						}
					}
					
					XmlUtils::CXmlNode oNodeExt = oNodeExtLst.ReadNode(_T("p:ext"));
					if (oNodeExt.IsValid())
					{
						XmlUtils::CXmlNode oNodeTI = oNodeExt.ReadNode(_T("p15:threadingInfo"));
						if (oNodeTI.IsValid())
						{
							XmlUtils::CXmlNode oNodeParent = oNodeTI.ReadNode(_T("p15:parentCm"));

							oNodeParent.ReadAttributeBase(L"authorId", parentAuthorId);
							oNodeParent.ReadAttributeBase(L"idx", parentCommentId);
						}
					}
				}
			}

			virtual CString toXML() const
			{
				return _T("");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:cm"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("authorId"), authorId);
				pWriter->WriteAttribute(_T("dt"), dt);
				pWriter->WriteAttribute(_T("idx"), idx);
				pWriter->EndAttributes();

				if (pos_x.is_init() && pos_y.is_init())
				{
					CString sPos = _T("");
					sPos.Format(_T("<p:pos x=\"%d\" y=\"%d\"/>"), *pos_x, *pos_y);
					pWriter->WriteString(sPos);
				}
				if (text.is_init())
				{
					pWriter->WriteString(_T("<p:text>"));
					pWriter->WriteString(*text);
					pWriter->WriteString(_T("</p:text>"));
				}

				bool bIsExtLst = false;
				if ((parentAuthorId.is_init() && parentCommentId.is_init()) || additional_data.is_init())
					bIsExtLst = true;

				if (bIsExtLst)
					pWriter->WriteString(_T("<p:extLst>"));

				if (parentAuthorId.is_init() && parentCommentId.is_init())
				{
					pWriter->WriteString(_T("<p:ext uri=\"{C676402C-5697-4E1C-873F-D02D1690AC5C}\">\
<p15:threadingInfo xmlns:p15=\"http://schemas.microsoft.com/office/powerpoint/2012/main\" timeZoneBias=\"-240\">"));

					CString sPos = _T("");
					sPos.Format(_T("<p15:parentCm authorId=\"%d\" idx=\"%d\"/>"), *parentAuthorId, *parentCommentId);
					pWriter->WriteString(sPos);

					pWriter->WriteString(_T("</p15:threadingInfo></p:ext>"));
				}

				if (additional_data.is_init())
				{
					pWriter->WriteString(_T("<p:ext uri=\"{19B8F6BF-5375-455C-9EA6-DF929625EA0E}\">\
<p15:presenceInfo xmlns:p15=\"http://schemas.microsoft.com/office/powerpoint/2012/main\" userId=\""));

					CString strData = additional_data.get();

					strData.Replace ( _T("&"),  _T("&amp;") );
					strData.Replace ( _T("'"),  _T("&apos;") );
					strData.Replace ( _T("<"),  _T("&lt;") );
					strData.Replace ( _T(">"),  _T("&gt;") );
					strData.Replace ( _T("\""), _T("&quot;") );

					pWriter->WriteString(strData);

					pWriter->WriteString(_T("\" providerId=\"AD\"/></p:ext>"));
				}

				if (bIsExtLst)
					pWriter->WriteString(_T("</p:extLst>"));

				pWriter->EndNode(_T("p:cm"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, authorId);
				pWriter->WriteString2(1, dt);
				pWriter->WriteInt2(2, idx);

				pWriter->WriteInt2(3, pos_x);
				pWriter->WriteInt2(4, pos_y);

				pWriter->WriteString2(5, text);

				pWriter->WriteInt2(6, parentAuthorId);
				pWriter->WriteInt2(7, parentCommentId);

				pWriter->WriteString2(8, additional_data);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);	
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
					case 0:
						authorId = pReader->GetLong();
						break;
					case 1:
						dt = pReader->GetString2();
						break;
					case 2:
						idx = pReader->GetLong();
						break;
					case 3:
						pos_x = pReader->GetLong();
						break;
					case 4:
						pos_y = pReader->GetLong();
						break;
					case 5:
						text = pReader->GetString2();
						break;
					case 6:
						parentAuthorId = pReader->GetLong();
						break;
					case 7:
						parentCommentId = pReader->GetLong();
						break;
					case 8:
						additional_data = pReader->GetString2();
					default:
						break;
					}
				}

				pReader->Seek(_end_rec);
			}

			virtual void FillParentPointersForChilds()
			{
			}
		};
	}
	
	class Comments : public WrapperFile
	{
	private:
		std::vector<PPTX::Logic::Comment> m_arComments;

	public:
		Comments()
		{
		}
		Comments(const OOX::CPath& filename, FileMap& map)
		{
			read(filename, map);
		}
		virtual ~Comments()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile2(filename.m_strFilename);

			XmlUtils::CXmlNodes oNodes;
			oNode.GetNodes(_T("p:cm"), oNodes);
			int nCount = oNodes.GetCount();
			for (int i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode oCm;
				oNodes.GetAt(i, oCm);

				PPTX::Logic::Comment elm;
				m_arComments.push_back(elm);
				m_arComments.back().fromXML(oCm);
			}
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, PPTX::ContentTypes::File& content)const
		{			
		}
		
	public:
		virtual const PPTX::FileType type() const
		{
			return PPTX::FileTypes::SlideComments;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

	public:
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecordArray(0, 0, m_arComments);
		}

		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:cmLst"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
			pWriter->EndAttributes();

			pWriter->WriteArray2(m_arComments);

			pWriter->EndNode(_T("p:cmLst"));
		}

		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetLong() + 4;

			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						pReader->Skip(4); // len
						ULONG lCount = pReader->GetULong();

						for (ULONG i = 0; i < lCount; ++i)
						{
							pReader->Skip(1);
							
							PPTX::Logic::Comment elm;
							m_arComments.push_back(elm);
							m_arComments.back().fromPPTY(pReader);
						}
						
						break;
					}
					default:
					{
						pReader->SkipRecord();
						break;
					}
				}
			}

			pReader->Seek(end);
		}
	};
} // namespace PPTX

#endif // PPTX_COMMENTS_COMMENTS_H_