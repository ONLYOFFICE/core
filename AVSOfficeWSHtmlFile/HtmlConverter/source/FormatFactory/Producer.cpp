#include "stdafx.h"
#include "Producer.h"
#include <Serializer/HTMLSerializer.h>
#include <Serializer/XLSXSerializer.h>
#include <Serializer/XMLSerializer.h>
#include <Converter/HTMLConverter.h>
#include <Converter/XLSXConverter.h>


Producer::~Producer()
{
}


Serializer& Producer::getSerializer(const EWS_FORMAT format)
{
	switch(format)
	{
		case ef_HTML:
		{
			static HTML::HTMLSerializer prod;
			return prod;	
		}
		case ef_XML:
		{
			static XLS::XMLSerializer prod;
			return prod;	
		}
		case ef_XLSX:
		default:
		{
			static XLSX::XLSXSerializer prod;
			return prod;	
		}
	}
}


Converter& Producer::getConverter(const EWS_FORMAT format)
{
	switch(format)
	{
	case ef_HTML:
		{
			static HTML::HTMLConverter prod;
			return prod;	
		}
	case ef_XLSX:
	default:
		{
			static XLSX::XLSXConverter prod;
			return prod;	
		}
	}
}



