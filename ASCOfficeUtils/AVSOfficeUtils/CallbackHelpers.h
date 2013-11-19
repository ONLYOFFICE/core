#pragma once

__interface IExtractedFileEvent;

// This callback class serves as a separator for external COM interface and ZLibZipUtils namespace
class ExtractedFileCallback
{
public:
	ExtractedFileCallback(IExtractedFileEvent* data_receiver);
	void Invoke(_bstr_t file_name, SAFEARRAY** arr);

private:
	IExtractedFileEvent* invokable_object;
};


__interface IRequestFileEvent;

// This callback class serves as a separator for external COM interface and ZLibZipUtils namespace
class RequestFileCallback
{
public:
	RequestFileCallback(IRequestFileEvent* data_receiver);
	bool Invoke(BSTR* file_name, SAFEARRAY** arr);

private:
	IRequestFileEvent* invokable_object;
};
