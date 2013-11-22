#pragma once

class BlockPr {
private:
	int _headingLevel;
	int _numberingLevel;

public:
	BlockPr() 
			: _headingLevel(0), _numberingLevel(0) {
	}

public:
	void setHeadingLevel(const int& headingLevel) {
		_headingLevel = headingLevel;
	}
	int getHeadingLevel() const {
		return _headingLevel;
	}

	void setNumberingLevel(const int& numberingLevel) {
		_numberingLevel = numberingLevel;
	}
	int getNumberingLevel() const {
		return _numberingLevel;
	}
};