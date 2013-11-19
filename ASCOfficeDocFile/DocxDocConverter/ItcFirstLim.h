#pragma once

namespace AVSDocFileFormat
{
	union ItcFirstLim
	{
	private:
		struct
		{
			unsigned short itcFirst:8;
			unsigned short itcLim:8;
		} ItcFirstLimStruct;
		unsigned short ItcFirstLimShort;

	public:

		explicit ItcFirstLim( byte _itcFirst, byte _itcLim ) : ItcFirstLimShort(0)
		{
			this->ItcFirstLimStruct.itcFirst = _itcFirst;

			if ( _itcLim < this->ItcFirstLimStruct.itcFirst )
			{
				this->ItcFirstLimStruct.itcLim = this->ItcFirstLimStruct.itcFirst;
			}
			else
			{
				this->ItcFirstLimStruct.itcLim = _itcLim;  
			}
		}

		operator short() const
		{
			return this->ItcFirstLimShort;
		}
	};
}