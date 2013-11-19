#include "Converter.h"
#include "AbstractConverter.h"
#include "Docx.h"
#include "Txt.h"
#include <boost/foreach.hpp>
#include "ToString.h"
#include "Encoding.h"
#include "TxtEvent.h"

namespace Docx2Txt
{

    class Converter_Impl : public AbstractConverter<Docx::Folder, Txt::File>
    {
    public:
        Converter_Impl();
        ~Converter_Impl();

    public:
        void convert(Txt::ITxtEvent& Event);

    public:
        void writeUtf8(const boost::filesystem::wpath& path) const;
        void writeUnicode(const boost::filesystem::wpath& path) const;
        void writeBigEndian(const boost::filesystem::wpath& path) const;
        void writeAnsi(const boost::filesystem::wpath& path) const;

    private:
        void convert(const std::vector<OOX::Logic::TextItem>& items, std::list<std::string>& text, Txt::ITxtEvent& Event, bool isFirstLevel,
            const OOX::Document& document, const OOX::Styles& styles);
        int m_lPercent;
        int m_lAddition;
        bool m_bCancelled;
        int ListCount;
        int* Lists;
        const std::string convert(const OOX::Logic::Paragraph& paragraph, Txt::ITxtEvent& Event,
            const OOX::Document& document, const OOX::Styles& styles);// const;

        size_t NoteCount;
        std::list<std::string> Notes;

        static std::string IntToLowerLetter(int number);
        static std::string IntToUpperLetter(int number);
        static std::string IntToLowerRoman(int number);
        static std::string IntToUpperRoman(int number);

        static const std::string m_letters;
    };

    Converter::Converter() : converter_(new Converter_Impl)
    {}

	Converter::~Converter()
    {
        delete converter_;
    }

    void Converter::convert(Txt::ITxtEvent& Event)
    {
        return converter_->convert(Event);
    }

    void Converter::read(const boost::filesystem::wpath& path)
    {
        return converter_->read(path);
    }

    void Converter::write(const boost::filesystem::wpath& path) const
    {
        return converter_->write(path);
    }

    void Converter::writeUtf8(const boost::filesystem::wpath& path) const
    {
        return converter_->writeUtf8(path);
    }

    void Converter::writeUnicode(const boost::filesystem::wpath& path) const
    {
        return converter_->writeUnicode(path);
    }

    void Converter::writeBigEndian(const boost::filesystem::wpath& path) const
    {
        return converter_->writeBigEndian(path);
    }

    void Converter::writeAnsi(const boost::filesystem::wpath& path) const
    {
        return converter_->writeAnsi(path);
    }


	const std::string Converter_Impl::m_letters = "abcdefghijklmnopqrstuvwxyz";

	Converter_Impl::Converter_Impl()
		: base(L""), m_lPercent(0), m_lAddition(0), m_bCancelled(false), ListCount(0), Lists(NULL), NoteCount(0)
	{
	}


	Converter_Impl::~Converter_Impl()
	{
		if(Lists != NULL)
		{
			delete []Lists;
			Lists = NULL;
		}
	}


	void Converter_Impl::convert(Txt::ITxtEvent& Event)
	{
		const OOX::Document& document = m_input.find<OOX::Document>();
		const OOX::Styles& styles = document.find<OOX::Styles>();
		m_lPercent = 100000;
		m_bCancelled = Event.Progress(0, m_lPercent);
		if(m_bCancelled)
			return;
		if(document.exist(OOX::FileTypes::Numbering))
		{
			const OOX::Numbering& numbering = document.find<OOX::Numbering>();
			ListCount = numbering.Nums->size();
			Lists = new int[9 * ListCount];
			if(Lists == NULL)
				return;
			for(int i = 0; i < 9 * ListCount; i++)
				Lists[i] = 0;
		}
		if(document.Items->size() > 0)
		{
			m_lAddition = 800000;
			Notes.clear();
			convert(*document.Items, *m_output.Content, Event, true, document, styles);

			if(NoteCount != 0)
			{
				m_output.Content->push_back("---------------------------");
				for(std::list<std::string>::const_iterator iter = Notes.begin(); iter != Notes.end(); iter++)
				{
					m_output.Content->push_back(*iter);
				}
			}
		}
		if(Lists != NULL)
		{
			delete []Lists;
			Lists = NULL;
		}
		Event.Progress(0, 900000);
	}


	void Converter_Impl::writeUtf8(const boost::filesystem::wpath& path) const
	{
		m_output.writeUtf8(path);
	}


	void Converter_Impl::writeUnicode(const boost::filesystem::wpath& path) const
	{
		m_output.writeUnicode(path);
	}


	void Converter_Impl::writeBigEndian(const boost::filesystem::wpath& path) const
	{
		m_output.writeBigEndian(path);
	}


	void Converter_Impl::writeAnsi(const boost::filesystem::wpath& path) const
	{
		m_output.writeAnsi(path);
	}


	void Converter_Impl::convert(const std::vector<OOX::Logic::TextItem>& items, std::list<std::string>& text, Txt::ITxtEvent& Event, bool isFirstLevel,
		const OOX::Document& document, const OOX::Styles& styles)
	{
		if(items.size() > 0)
		{
			if(isFirstLevel)
				m_lAddition = m_lAddition / items.size();
			BOOST_FOREACH(const OOX::Logic::TextItem& item, items)
			{
				if (item.is<OOX::Logic::Paragraph>())
				{
					//m_output.Content->push_back(convert(item.as<OOX::Logic::Paragraph>()));
					text.push_back(convert(item.as<OOX::Logic::Paragraph>(), Event, document, styles));
				}
				/*else if (item.is<OOX::Logic::List>())
				{
					BOOST_FOREACH(const OOX::Logic::ListItem& listItem, *item.as<OOX::Logic::List>().Items)
					{
						convert(listItem.Items);
					}
				}*/
				else if (item.is<OOX::Logic::Table>())
				{
					BOOST_FOREACH(const OOX::Logic::TableRow& row, *item.as<OOX::Logic::Table>().Rows)
					{
						BOOST_FOREACH(const OOX::Logic::TableCell& cell, *row.Cells)
						{
							convert(*cell.Items, text, Event, false, document, styles);
						}
					}
				}
				else if (item.is<OOX::Logic::Sdt>())
				{
					convert(item.as<OOX::Logic::Sdt>().Content->Items, text, Event, false, document, styles);
				}

				if(isFirstLevel)
				{
					m_lPercent += m_lAddition;
					m_bCancelled = Event.Progress(0, m_lPercent);
					if(m_bCancelled)
						return;
				}
			}
		}
	}


	const std::string Converter_Impl::convert(const OOX::Logic::Paragraph& paragraph, Txt::ITxtEvent& Event,
		const OOX::Document& document, const OOX::Styles& styles)// const
	{
		std::string line = "";
		//const OOX::Document& document = m_input.find<OOX::Document>();
		//const OOX::Styles& styles = document.find<OOX::Styles>();
		
		/*BOOST_FOREACH(const OOX::Logic::ParagraphItem& item, *paragraph.Items)
		{
			line += item.toTxt();
		}*/

		int level = paragraph.GetLevel();
		int listNum = paragraph.GetListNum();
		if((listNum == 0) || (level == 0))
		{
			if(paragraph.Property.is_init())
			{
				std::string styleName = paragraph.Property->PStyle.get_value_or("");
				if(styleName != "")
				{
					const OOX::Styles::Style& style = styles.GetStyleById(styleName);
					if(style.ParagraphProperty.is_init())
					{
						if(style.ParagraphProperty->NumPr.is_init())
						{
							listNum = style.ParagraphProperty->NumPr->NumId.get_value_or(0);
							if(listNum > 0)
								level = style.ParagraphProperty->NumPr->Ilvl.get_value_or(0) + 1;
							else
								level = 0;
						}
					}
				}
			}
		}
		if((listNum > 0) && (level > 0) && document.exist(OOX::FileTypes::Numbering))
		{
			level--;
			int start = (listNum - 1) * 9;
			for(int i = level + 1; i < 9; i++)
			{
				Lists[start + i] = 0;
			}

			const OOX::Numbering& numbering = document.find<OOX::Numbering>();
			//for(int i = 0; i < level; i++)
			//	line += "  ";
			int abstractNumId = numbering.getAbstractNumId(listNum);
			const OOX::Numbering::AbstractNum& abstractNum = numbering.getAbstractNum(abstractNumId);
			Lists[start + level]++;

			std::string numType = abstractNum.getLevel(level).NumFmt->ToString();
			if(numType == "bullet")
				line += *abstractNum.getLevel(level).Text + " ";
			else
			{
				std::string example = *abstractNum.getLevel(level).Text;
				for(int i = 0; i < 9; i++)
				{
					std::string num = "%" + ToString(i + 1);
					while(example.find(num) != example.npos)
					{
						if(numType == "lowerLetter")
							example.replace(example.find(num), 2, IntToLowerLetter(Lists[start + i]));
						else if(numType == "upperLetter")
							example.replace(example.find(num), 2, IntToUpperLetter(Lists[start + i]));
						else if(numType == "lowerRoman")
							example.replace(example.find(num), 2, IntToLowerRoman(Lists[start + i]));
						else if(numType == "upperRoman")
							example.replace(example.find(num), 2, IntToUpperRoman(Lists[start + i]));
						else
							example.replace(example.find(num), 2, ToString(Lists[start + i]));
					}
				}
				line += example + " ";
			}
		}

		bool inField = false;
		BOOST_FOREACH(const OOX::Logic::ParagraphItem pItem, *paragraph.Items)
		{
			if (pItem.is<OOX::Logic::Run>())
			{
				const OOX::Logic::Run run = pItem.as<OOX::Logic::Run>();
				BOOST_FOREACH(const OOX::Logic::RunItem rItem, *run.Items)
				{
					if (rItem.is<OOX::Logic::FldChar>())
					{
						const OOX::Logic::FldChar fldChar = rItem.as<OOX::Logic::FldChar>();
						if (fldChar.FldCharType == "begin")
						{
							inField = true;
						}
						else 
						{
							if (fldChar.FldCharType == "separate")	
								inField = false;
						}
					}
					else if (inField == false)
					{
						bool caps = false;
						if(run.Property.is_init())
						{
							caps = run.Property->Caps.get_value_or(false);
						}

						std::string str = "";
						if(caps)
							str = Encoding::unicode2utf8(ToUpper(Encoding::utf82unicode(rItem.toTxt())));
						else
							str = rItem.toTxt();
						if((rItem.is<OOX::Logic::FootnoteReference>()) || (rItem.is<OOX::Logic::EndnoteReference>()))
						{
							NoteCount++;
							Notes.push_back("[" + ToString(NoteCount) + "]");

							if(rItem.is<OOX::Logic::FootnoteReference>())
							{
								const OOX::FootNote& footnote = document.find<OOX::FootNote>();
								convert(*footnote.find(rItem.as<OOX::Logic::FootnoteReference>()).Items, Notes, Event, false, document, styles);
							}
							else if(rItem.is<OOX::Logic::EndnoteReference>())
							{
								const OOX::EndNote& endnote = document.find<OOX::EndNote>();
								convert(*endnote.find(rItem.as<OOX::Logic::EndnoteReference>()).Items, Notes, Event, false, document, styles);
							}

							str.replace(str.find("_"), 1 , ToString(NoteCount));
						}
						line += str;
					}
				}
			}
			else
				line += pItem.toTxt();
		}

		return line;
	}

	std::string Converter_Impl::IntToLowerLetter(int number)
	{
		std::string result(number / 26 + 1, m_letters[(number % 26) - 1]);
		return result;
	}

	std::string Converter_Impl::IntToUpperLetter(int number)
	{
		std::string result(number / 26 + 1, m_letters[(number % 26) - 1]);
		result = ToUpper(result);
		return result;
	}

	std::string Converter_Impl::IntToLowerRoman(int number)
	{
		std::string result = "";
		const std::string ixcm = "ixcm";
		const std::string vxlcdm = "vxlcdm";
		const std::string vld = "vld";

		std::string str_num = ToString(number);
		int len = str_num.size();
		int digit;
		for(int i = 0; i < len; i++)
		{
			digit = str_num[len - i - 1] - '0';
			if(i > 2)
			{
				std::string m(digit + 10 * (i - 3), 'm');
				result = m + result;
			}
			else
			{
				if(((digit + 1) % 5) == 0)
				{
					result.insert(0, 1, vxlcdm[i * 2 + (digit + 1) / 5 - 1]);
					result.insert(0, 1, ixcm[i]);
					//result = ixcm[i] + vxlcdm[i * 2 + (digit + 1) / 5 - 1] + result;
				}
				else
				{
					std::string m(digit % 5, ixcm[i]);
					if(digit >= 5)
						result = vld[i] + m + result;
					else
						result = m + result;
				}
			}
		}
		return result;
	}

	std::string Converter_Impl::IntToUpperRoman(int number)
	{
		std::string result = IntToLowerRoman(number);
		result = ToUpper(result);
		return result;
	}

} // namespace Docx2Txt