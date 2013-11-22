#pragma once

using XmlUtils::CXmlNode;
using XmlUtils::CXmlNodes;

class BaseNumbering {
public:
	enum NumberingFormat {
		DecimalNumbering,
		BulletNumbering,
		LowerLetterNumbering,
		UpperLetterNumbering,
		LowerRomanNumbering,
		UpperRomanNumbering,
		NumberInDashNumbering
	};

protected:
	CString getNumber(const int& number, const NumberingFormat& format) const {
		switch (format) {
				case DecimalNumbering:		return getNum(number);
				case LowerLetterNumbering:	return getAlpha(number);
				case UpperLetterNumbering:	return getAlpha(number, true);
				case LowerRomanNumbering:	return getRoman(number);
				case UpperRomanNumbering:	return getRoman(number, true);
				case NumberInDashNumbering:	return getNumInDash(number);
				default:					return getNum(number);
		}
	}

private:
	CString getNum(const int& number) const {
		CString num;
		num.Format(_T("%i"), number);

		return num;
	}
	CString getNumInDash(const int& number) const {
		CString num;
		num.Format(_T("-%i-"), number);

		return num;
	}
	CString getAlpha(const int& number, const bool& upper = false) const {
		static CString alphas = _T(" abcdefghijklmnopqrstuvwxyz");

		CString alphaNumber;
		if (number < alphas.GetLength())
			alphaNumber = alphas[number];
		else
			alphaNumber = alphas[alphas.GetLength() - 1];

		if (upper)
			alphaNumber.MakeUpper();

		return alphaNumber;
	}
	CString getRoman(int number, const bool& upper = false) const {
		static CString romans[] = {_T(""), _T("i"), _T("ii"), _T("iii"), _T("iv"), 
			_T("v"), _T("vi"), _T("vii"), _T("iix"), _T("ix")};

		CString romanNumber = _T("");
		while (number >= 10) {
			romanNumber += _T("x");
			number -= 10;
		}
		romanNumber += romans[number];

		if (upper)
			romanNumber.MakeUpper();

		return romanNumber;
	}

protected:
	NumberingFormat getNumberingFormat(const CString& format) {
		if (_T("decimal") == format) return DecimalNumbering;
		else if (_T("bullet") == format) return BulletNumbering;
		else if (_T("lowerLetter") == format) return LowerLetterNumbering;
		else if (_T("upperLetter") == format) return UpperLetterNumbering;
		else if (_T("lowerRoman") == format) return LowerRomanNumbering;
		else if (_T("upperRoman") == format) return UpperRomanNumbering;
		else if (_T("numberInDash") == format) return NumberInDashNumbering;
		
		return DecimalNumbering;
	}
};
class ListNumeration {
private:
	class Numbering : public BaseNumbering {
	private:
		class Level {
		public:
			int _start;
			CString _text;
			NumberingFormat _format;

		public:
			Level()
				: _start(1), _format(DecimalNumbering) {
			}

		};

	private:
		CArray<Level> _levels;
		CArray<int> _numbers;

	public:
		Numbering()
			: _numbers() {
		}
		Numbering(const Numbering& src) {
			_levels.Copy(src._levels);
			_numbers.Copy(src._numbers);
		}

		Numbering(CXmlNode& numberingXml) {
			loadFromXml(numberingXml);
		}

	public:
		void loadFromXml(CXmlNode& numberingXml) {
			CXmlNodes levelXmls;
			if (numberingXml.GetNodes(_T("lvl"), levelXmls)) {
				int count = levelXmls.GetCount();
				for (int i = 0; i < count; ++i) {
					CXmlNode levelXml;
					levelXmls.GetAt(i, levelXml);

					_levels.Add();
					Level& level = _levels[i];
					level._start = levelXml.ReadAttributeInt(_T("start"), 1);
					level._text = levelXml.GetAttribute(_T("text"));
					level._format = getNumberingFormat(levelXml.GetAttribute(_T("format")));
				}
			}
		}

	public:
		CString getElement(const int& level, const Numbering& numOverride) {
			int overrideLevelCount = (int) numOverride._levels.GetCount();
			if (level >= overrideLevelCount)
				return getElement(level);

			Level tmpLevel = _levels[level];
			tmpLevel._format = numOverride._levels[level]._format;
			tmpLevel._text = numOverride._levels[level]._text;

			processElementLevel(level);
			return getLevelElement(tmpLevel);
		}
		CString getElement(const int& level) {
			processElementLevel(level);
			return getLevelElement(_levels[level]);
		}
		
	private:
		CString	getFormattedText(const CString& text, const NumberingFormat& format) const {
			CString formattedText;
			int i = 0;
			while (i < text.GetLength()) {
				if (text[i] != _T('%'))
					formattedText += text[i++];
				else if (isdigit(text[++i])) {
					wchar_t symbolLevel = text[i++];
					int level = _wtoi(&symbolLevel) - 1;
					if (level < (int)_numbers.GetCount())
						formattedText += getNumber(_numbers[level], format);
				}
			}

			return formattedText;
		}

	private:
		CString getLevelElement(const Level& level) {
			if (level._format == BulletNumbering)
				return level._text;
			else
				return getFormattedText(level._text, level._format);
		}
		void processElementLevel(const int& level) {
			addMissingLevels(level);

			int count = (int)_numbers.GetCount();
			if (level >= count)
				for (int i = count; i <= level; ++i)
					_numbers.Add(_levels[i]._start);
			else {
				for (int i = level + 1; i < count; ++i)
					_numbers[i] = 0;
				++_numbers[level];
			}
		}
		void addMissingLevels(const int& level) {
			int levelsCount = (int) _levels.GetCount();
			if (level < levelsCount)
				return;

			for (int i = levelsCount; i < level + 1; ++i) {
				_levels.Add();
				_levels[i]._text.Format(_T("%%i"), i);
			}
		}
	};

private:
	std::map<CString, Numbering> _numberings;
	std::map<CString, Numbering> _numOverrides;

public:
	ListNumeration() {
	}

public:
	void loadFromXml(CXmlNode& listSettingsXml) {
		CXmlNodes numXmls;
		if (listSettingsXml.GetNodes(_T("num"), numXmls)) {
			int count = numXmls.GetCount();
			for (int i = 0; i < count; ++i) {
				CXmlNode numXml;
				numXmls.GetAt(i, numXml);

				CString id = numXml.GetAttribute(_T("id"));
				_numberings[id].loadFromXml(numXml);
			}
		}
		CXmlNodes numOverrideXmls;
		if (listSettingsXml.GetNodes(_T("numOverride"), numOverrideXmls)) {
			int count = numOverrideXmls.GetCount();
			for (int i = 0; i < count; ++i) {
				CXmlNode numOverrideXml;
				numOverrideXmls.GetAt(i, numOverrideXml);

				CString id = numOverrideXml.GetAttribute(_T("id"));
				_numOverrides[id].loadFromXml(numOverrideXml);
			}
		}
	}
	void clear() {
		_numberings.clear();
		_numOverrides.clear();
	}

public:
	CString getElement(const CString& id, const CString& numId, const int& level) {
		std::map<CString, Numbering>::iterator numbering = _numberings.find(id);
		if (numbering == _numberings.end())
			numbering = _numberings.insert(std::pair<CString, Numbering>(id, Numbering())).first;

		std::map<CString, Numbering>::iterator numOverride = _numOverrides.find(numId);
		if (numOverride != _numOverrides.end())
			return (*numbering).second.getElement(level, (*numOverride).second);
		else
			return (*numbering).second.getElement(level);
	}
};

class PageNumbering : public BaseNumbering {
private:
	NumberingFormat _format;
	
	int _start;
	int _index;

public:
	PageNumbering()
		: _start(1), _index(-1) {
	}

public:
	PageNumbering& operator=(const PageNumbering& src) {
		_format = src._format;
		
		_start = src._start;
		_index = src._index;

		return *this;
	}

public:
	void loadFromXml(CXmlNode& numberingXml) {
		_format = getNumberingFormat(numberingXml.GetAttribute(_T("format")));

		_start = numberingXml.ReadAttributeInt(_T("start"), -1);
		_index = -1;
	}
	void clear() {
		_format = DecimalNumbering;

		_start = 1;
		_index = -1;
	}

public:
	void next() {
		++_index;
	}

	void setAfter(const PageNumbering& numbering) {
		if (_start == -1) {
			_start = numbering.getIndex();
			_index = 0;
		}
		else
			_index = -1;
	}
public:
	int getIndex() const {
		return _start + _index;
	}
	CString getIndexString( ) const {
		return getNumber(getIndex(), _format);
	}
};

class PageNumeration {
private:
	PageNumbering _numbering;

	int _pageCount;

public:
	PageNumeration() 
		: _pageCount(0) {
	}

public:
	void loadFromXml(CXmlNode& numSettingsXml) {
		_numbering.clear();

		_pageCount = numSettingsXml.ReadAttributeInt(_T("pageCount"), 1);
	}
	void clear() {
		_numbering.clear();

		_pageCount = 0;
	}

public:
	void setNumbering(PageNumbering numbering) {
		numbering.setAfter(_numbering);
		_numbering = numbering;
	}

public:
	void nextPage() {
		_numbering.next();
	}
	int getPageIndex() const {
		return _numbering.getIndex();
	}
	CString getPageIndexElement() const {
		return _numbering.getIndexString();
	}
	int getPageCount() const {
		return _pageCount;
	}
	CString getPageCountElement() const {
		CString pageCount;
		pageCount.Format(_T("%i"), _pageCount);

		return pageCount;
	}
};