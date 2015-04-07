#pragma once

namespace DocFileFormat
{
	class EmuValue
	{
	public:
		// Creates a new EmuValue for the given value.
		EmuValue (int value = 0)
		{
			m_Value = value;
		}

		// Converts the EMU to pt
		double ToPoints() const
		{
			return (double) m_Value / 12700.0;
		}

		// Converts the EMU to twips
		inline double ToTwips () const
		{
			return (double) m_Value / 635.0;
		}

		inline double ToCm () const
		{
			return m_Value / 36000.0;
		}

		inline double ToMm () const
		{
			return ToCm () * 10.0;
		}

		inline operator int () const
		{
			return m_Value;
		}

	private:

		int m_Value;
	};
}