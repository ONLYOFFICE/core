#pragma once

#include <Document/EWSDocument.h> // EWS_FORMAT

class Serializer;
class Converter;

class Producer
{
	~Producer();
public:
	static Serializer& getSerializer(const EWS_FORMAT format);
	static Converter& getConverter(const EWS_FORMAT format);

};

