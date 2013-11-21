#pragma once

namespace MSXML2
{;

class MSXMLErrHandler
{
public:
	MSXMLErrHandler(const HRESULT hres, IXMLDOMNode* object, REFIID iid, const std::string& func_name);
	~MSXMLErrHandler();

	const std::string& toString();
	void logErrorAndThrow(bool force_throw = false);

private:
	HRESULT hres_;
	IXMLDOMNodePtr object_;
	IID iid_;
	std::string func_name_;
	std::string message;
};


} // namespace MSXML2
