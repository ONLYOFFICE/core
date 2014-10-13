#pragma once

typedef enum {
	ef_XLS,
	ef_XLSX,
	ef_ODS,
	ef_SXC,
	ef_XML,
	ef_AUTO_DETECT,
	ef_END
} EWS_FORMAT;

class Document;

namespace EWSCORE
{;


class EWSDocument
{
public:

	EWSDocument();
	~EWSDocument();

public:
	bool loadFrom(const _bstr_t& from_filename, const EWS_FORMAT format);
	bool saveTo(const _bstr_t& to_folder, const EWS_FORMAT format);

private:
	Document* doc_;

};




}

