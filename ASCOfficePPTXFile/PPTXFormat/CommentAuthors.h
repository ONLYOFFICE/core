#pragma once
#ifndef PPTX_COMMENTS_COMMENTSAUTHORS_H_
#define PPTX_COMMENTS_COMMENTSAUTHORS_H_

#include "WrapperFile.h"
#include "FileContainer.h"

namespace PPTX
{
	namespace Logic
	{
		class CommentAuthor : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CommentAuthor)
		private:
			nullable_int	id;
			nullable_int	last_idx;
			nullable_int	clr_idx;

			nullable_string	name;
			nullable_string	initials;			

		public:
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"id", id);
				node.ReadAttributeBase(L"lastIdx", last_idx);
				node.ReadAttributeBase(L"clrIdx", clr_idx);
				node.ReadAttributeBase(L"name", name);
				node.ReadAttributeBase(L"initials", initials);
			}

			virtual CString toXML() const
			{
				return _T("");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:cmAuthor"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("id"), id);
                pWriter->WriteAttribute2(_T("name"), name);
				pWriter->WriteAttribute(_T("initials"), initials);
				pWriter->WriteAttribute(_T("lastIdx"), last_idx);
				pWriter->WriteAttribute(_T("clrIdx"), clr_idx);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("p:cmAuthor"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, id);
				pWriter->WriteInt2(1, last_idx);
				pWriter->WriteInt2(2, clr_idx);
				pWriter->WriteString2(3, name);
				pWriter->WriteString2(4, initials);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);	
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
					case 0:
						id = pReader->GetLong();
						break;
					case 1:
						last_idx = pReader->GetLong();
						break;
					case 2:
						clr_idx = pReader->GetLong();
						break;
					case 3:
						name = pReader->GetString2();
						break;
					case 4:
						initials = pReader->GetString2();
						break;
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
	
	class Authors : public WrapperFile
	{
	private:
		std::vector<PPTX::Logic::CommentAuthor> m_arAuthors;

	public:
		Authors()
		{
		}
		Authors(const OOX::CPath& filename, FileMap& map)
		{
			read(filename, map);
		}
		virtual ~Authors()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile2(filename.m_strFilename);

			XmlUtils::CXmlNodes oNodes;
			oNode.GetNodes(_T("p:cmAuthor"), oNodes);
			int nCount = oNodes.GetCount();
			for (int i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode oCm;
				oNodes.GetAt(i, oCm);

				PPTX::Logic::CommentAuthor comm;

				m_arAuthors.push_back(comm);				
				m_arAuthors.back().fromXML(oCm);
			}		
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, PPTX::ContentTypes::File& content)const
		{			
		}
		
	public:
		virtual const PPTX::FileType type() const
		{
			return PPTX::FileTypes::CommentAuthors;
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
			pWriter->WriteRecordArray(0, 0, m_arAuthors);
		}

		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:cmAuthorLst"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
			pWriter->EndAttributes();

			pWriter->WriteArray2(m_arAuthors);

			pWriter->EndNode(_T("p:cmAuthorLst"));
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

							PPTX::Logic::CommentAuthor comm;

							m_arAuthors.push_back(comm);
							m_arAuthors.back().fromPPTY(pReader);
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

#endif // PPTX_COMMENTS_COMMENTSAUTHORS_H_
