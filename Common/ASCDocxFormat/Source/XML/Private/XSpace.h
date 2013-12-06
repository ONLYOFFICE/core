#pragma once
#ifndef XML_PRIVATE_XSPACE_INCLUDE_H_
#define XML_PRIVATE_XSPACE_INCLUDE_H_

namespace XML
{
	namespace Private
	{
		class XSpace
		{
		public:
			XSpace();
			XSpace(const bool space);

		public:
			const XSpace& operator =(const bool rhs);

		public:
			void setDefault();
			void setPreserve();

		public:
			const bool isDefault() const;
			const bool isPreserve() const;

		private:
			bool m_space;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XSPACE_INCLUDE_H_