/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _BUILD_BIGINTEGER_CROSSPLATFORM_H_
#define _BUILD_BIGINTEGER_CROSSPLATFORM_H_

#include <string>

class CBigInteger
{
protected:
    std::string m_value;
    bool        m_isNegative;

public:

    CBigInteger()
    {
        m_value = "0";
        m_isNegative = false;
    }

    CBigInteger(unsigned char* data, int size)
    {
        m_isNegative = false;
        m_value = "0";

        for (int i = 0; i < size; i++)
        {
            CBigInteger tmp((int)data[i]);

            for (int j = size - 1 - i; j > 0; --j)
                tmp *= 256;

            *this += tmp;
        }
    }

    CBigInteger(std::string data, int nBase = 10)
    {
        m_isNegative = false;

        if (10 == nBase)
        {
            m_value = data;
        }
        else
        {
            m_value = "0";
            int len = (int)data.length();
            for (int i = 0; i < len; i++)
            {
                int val = 0;
                char _c = data[i];
                if (_c >= '0' && _c <= '9')
                    val = _c - '0';
                else if (_c >= 'A' && _c <= 'F')
                    val = 10 + _c - 'A';
                else if (_c >= 'a' && _c <= 'f')
                    val = 10 + _c - 'a';

                CBigInteger tmp(val);
                for (int j = len - 1 - i; j > 0; --j)
                    tmp *= nBase;

                *this += tmp;
            }
        }
    }

    CBigInteger(int value)
    {
        if (value < 0)
        {
            m_isNegative = true;
            m_value = std::to_string(-value);
        }
        else
        {
            m_isNegative = false;
            m_value = std::to_string(value);
        }
    }

    std::string GetValue()
    {
        return m_value;
    }
    bool IsNegative()
    {
        return m_isNegative;
    }

    void operator = (CBigInteger b)
    {
        m_value = b.m_value;
        m_isNegative = b.m_isNegative;
    }

    bool operator == (CBigInteger b)
    {
        return equals((*this) , b);
    }

    bool operator != (CBigInteger b)
    {
        return !equals((*this) , b);
    }

    bool operator > (CBigInteger b)
    {
        return greater((*this) , b);
    }

    bool operator < (CBigInteger b)
    {
        return less((*this) , b);
    }

    bool operator >= (CBigInteger b)
    {
        return equals((*this) , b) || greater((*this), b);
    }

    bool operator <= (CBigInteger b)
    {
        return equals((*this) , b) || less((*this) , b);
    }

    CBigInteger absolute()
    {
        return CBigInteger(m_value); // +ve by default
    }

    CBigInteger& operator ++()
    {
        // prefix
        (*this) = (*this) + 1;
        return (*this);
    }

    CBigInteger operator ++(int)
    {
        // postfix
        CBigInteger before = (*this);
        (*this) = (*this) + 1;
        return before;
    }

    CBigInteger& operator --()
    {
        // prefix
        (*this) = (*this) - 1;
        return (*this);
    }

    CBigInteger operator --(int)
    {
        // postfix
        CBigInteger before = (*this);
        (*this) = (*this) - 1;
        return before;
    }

    CBigInteger operator + (CBigInteger b)
    {
        CBigInteger addition;
        if (m_isNegative == b.m_isNegative)
        {
            // both +ve or -ve
            addition.m_value = (add(m_value, b.m_value));
            addition.m_isNegative = m_isNegative;
        }
        else
        {
            // sign different
            if (absolute() > b.absolute())
            {
                addition.m_value = subtract(m_value, b.m_value);
                addition.m_isNegative = m_isNegative;
            }
            else
            {
                addition.m_value = subtract(b.m_value, m_value);
                addition.m_isNegative = b.m_isNegative;
            }
        }

        if (addition.m_value == "0") // avoid (-0) problem
            addition.m_isNegative = false;

        return addition;
    }

    CBigInteger operator - (CBigInteger b)
    {
        b.m_isNegative = !b.m_isNegative; // x - y = x + (-y)
        return (*this) + b;
    }

    CBigInteger operator * (CBigInteger b)
    {
        CBigInteger mul;

        mul.m_value = multiply(m_value, b.m_value);
        mul.m_isNegative = (m_isNegative != b.m_isNegative);

        if (mul.m_value == "0") // avoid (-0) problem
            mul.m_isNegative = false;

        return mul;
    }

    CBigInteger& operator += (CBigInteger b)
    {
        (*this) = (*this) + b;
        return (*this);
    }

    CBigInteger& operator -= (CBigInteger b)
    {
        (*this) = (*this) - b;
        return (*this);
    }

    CBigInteger& operator *= (CBigInteger b)
    {
        (*this) = (*this) * b;
        return (*this);
    }

    CBigInteger operator -() { // unary minus sign
        return (*this) * -1;
    }

    std::string ToString()
    {
        // for conversion from BigInteger to string
        std::string signedString = ( m_isNegative ) ? "-" : ""; // if +ve, don't print + sign
        signedString += m_value;
        return signedString;
    }

private:
    bool equals(CBigInteger n1, CBigInteger n2)
    {
        return ((n1.m_value == n2.m_value) && (n1.m_isNegative == n2.m_isNegative));
    }

    bool less(CBigInteger n1, CBigInteger n2)
    {
        bool sign1 = n1.m_isNegative;
        bool sign2 = n2.m_isNegative;

        if (sign1 && !sign2)
            return true;
        else if (!sign1 && sign2)
            return false;
        else if (!sign1)
        {
            std::string::size_type _size1 = n1.m_value.length();
            std::string::size_type _size2 = n2.m_value.length();
            if (_size1 < _size2)
                return true;
            if (_size1 > _size2)
                return false;
            return n1.m_value < n2.m_value;
        }
        else
        {
            std::string::size_type _size1 = n1.m_value.length();
            std::string::size_type _size2 = n2.m_value.length();

            if (_size1 > _size2)
                return true;
            if (_size1 < _size2)
                return false;
            return n1.m_value.compare(n2.m_value) > 0;
        }
    }

    bool greater(CBigInteger n1, CBigInteger n2)
    {
        return !equals(n1, n2) && !less(n1, n2);
    }

    std::string add(std::string number1, std::string number2)
    {
        std::string add = (number1.length() > number2.length()) ? number1 : number2;
        char carry = '0';
        int differenceInLength = (int)(number1.size() - number2.size());

        if (differenceInLength > 0)
            number2.insert(0, differenceInLength, '0');
        else
            number1.insert(0, -differenceInLength, '0');

        if (differenceInLength < 0)
            differenceInLength = -differenceInLength;

        for (int i = (int)(number1.size()) - 1; i >= 0; --i)
        {
            add[i] = ((carry-'0')+(number1[i]-'0')+(number2[i]-'0')) + '0';

            if(i != 0)
            {
                if(add[i] > '9')
                {
                    add[i] -= 10;
                    carry = '1';
                }
                else
                {
                    carry = '0';
                }
            }
        }
        if (add[0] > '9')
        {
            add[0] -= 10;
            add.insert(0, 1, '1');
        }
        return add;
    }

    std::string subtract(std::string number1, std::string number2)
    {
        std::string sub = (number1.length() > number2.length()) ? number1 : number2;
        int differenceInLength = (int)(number1.size() - number2.size());

        if (differenceInLength > 0)
            number2.insert(0, differenceInLength, '0');
        else
            number1.insert(0, -differenceInLength, '0');

        if (differenceInLength < 0)
            differenceInLength = -differenceInLength;

        for (int i = (int)(number1.length()) - 1; i >= 0; --i)
        {
            if (number1[i] < number2[i])
            {
                number1[i] += 10;
                number1[i-1]--;
            }
            sub[i] = ((number1[i]-'0')-(number2[i]-'0')) + '0';
        }

        while (sub[0]=='0' && sub.length() != 1) // erase leading zeros
            sub.erase(0, 1);

        return sub;
    }

    std::string multiply(std::string n1, std::string n2)
    {
        if (n1.length() > n2.length())
            n1.swap(n2);

        std::string res = "0";
        for (int i = (int)(n1.length()) - 1; i >= 0; --i)
        {
            std::string temp = n2;
            int currentDigit = n1[i] - '0';
            int carry = 0;

            for (int j = (int)(temp.length()) - 1; j >= 0; --j)
            {
                temp[j] = ((temp[j]-'0') * currentDigit) + carry;

                if (temp[j] > 9)
                {
                    carry = (temp[j] / 10);
                    temp[j] -= (carry * 10);
                }
                else
                {
                    carry = 0;
                }

                temp[j] += '0'; // back to string mood
            }

            if (carry > 0)
                temp.insert(0, 1, (char)(carry+'0'));

            temp.append((n1.length() - i - 1), '0'); // as like mult by 10, 100, 1000, 10000 and so on

            res = add(res, temp); // O(n)
        }

        while (res[0] == '0' && res.length() != 1) // erase leading zeros
            res.erase(0,1);

        return res;
    }
};

#endif //_BUILD_BIGINTEGER_CROSSPLATFORM_H_
