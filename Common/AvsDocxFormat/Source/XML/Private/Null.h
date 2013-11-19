#pragma once
#ifndef XML_PRIVATE_NULL_INCLUDE_H_
#define XML_PRIVATE_NULL_INCLUDE_H_


namespace XML
{
	namespace Private
	{
		class NullText;

		class Null
		{
		public:
			static NullText& Text();
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_NULL_INCLUDE_H_