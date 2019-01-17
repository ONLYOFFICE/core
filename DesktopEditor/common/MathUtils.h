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

#ifndef _MATH_UTILS_
#define _MATH_UTILS_

#include <cmath>
#include <algorithm>

namespace ASC
{
    template <class T> class vec2;
    template <class T> class vec3;
    template <class T> class vec4;
    
    /// Template class for 2-tuple vector.
    template <class T>
    class vec2 {
    public:
        
        typedef T value_type;
        int size() const { return 2;}
        
        /// Default/scalar constructor
        vec2(const T & t = T()) {
            for(int i = 0; i < size(); i++) _array[i] = t;
        }
        
        /// Construct from array
        vec2(const T * tp) {
            for(int i = 0; i < size(); i++) _array[i] = tp[i];
        }
        
        /// Construct from explicit values
        vec2( const T v0, const T v1) {
            x = v0;
            y = v1;
        }
        
        /// "Cropping" explicit constructor from vec3 to vec2
        explicit vec2( const vec3<T> &u) {
            for(int i = 0; i < size(); i++) _array[i] = u._array[i];
        }
        
        /// "Cropping" explicit constructor from vec4 to vec2
        explicit vec2( const vec4<T> &u) {
            for(int i = 0; i < size(); i++) _array[i] = u._array[i];
        }
        
        const T * get_value() const {
            return _array;
        }
        
        vec2<T> & set_value( const T * rhs ) {
            for(int i = 0; i < size(); i++) _array[i] = rhs[i];
            return *this;
        }
        
        // indexing operators
        T & operator [] ( int i ) {
            return _array[i];
        }
        
        const T & operator [] ( int i ) const {
            return _array[i];
        }
        
        // type-cast operators
        operator T * () {
            return _array;
        }
        
        operator const T * () const {
            return _array;
        }
        
        ////////////////////////////////////////////////////////
        //
        //  Math operators
        //
        ////////////////////////////////////////////////////////
        
        // scalar multiply assign
        friend vec2<T> & operator *= ( vec2<T> &lhs, T d ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= d;
            return lhs;
        }
        
        // component-wise vector multiply assign
        friend vec2<T> & operator *= ( vec2<T> &lhs, const vec2<T> &rhs ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= rhs[i];
            return lhs;
        }
        
        // scalar divide assign
        friend vec2<T> & operator /= ( vec2<T> &lhs, T d ) {
            if(d == 0) return lhs;
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= d;
            return lhs;
        }
        
        // component-wise vector divide assign
        //    friend vec2<T> & operator /= ( vec2<T> &lhs, const vec2<T> & rhs ) {
        //        for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= rhs._array[i];
        //        return *this;
        //    }
        
        // component-wise vector add assign
        friend vec2<T> & operator += ( vec2<T> &lhs, const vec2<T> & rhs ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] += rhs._array[i];
            return lhs;
        }
        
        // component-wise vector subtract assign
        friend vec2<T> & operator -= ( vec2<T> &lhs, const vec2<T> & rhs ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] -= rhs._array[i];
            return lhs;
        }
        
        // unary negate
        friend vec2<T> operator - ( const vec2<T> &rhs) {
            vec2<T> rv;
            for(int i = 0; i < rhs.size(); i++) rv._array[i] = -rhs._array[i];
            return rv;
        }
        
        // vector add
        friend vec2<T> operator + ( const vec2<T> & lhs, const vec2<T> & rhs) {
            vec2<T> rt(lhs);
            return rt += rhs;
        }
        
        // vector subtract
        friend vec2<T> operator - ( const vec2<T> & lhs, const vec2<T> & rhs) {
            vec2<T> rt(lhs);
            return rt -= rhs;
        }
        
        // scalar multiply
        friend vec2<T> operator * ( const vec2<T> & lhs, T rhs) {
            vec2<T> rt(lhs);
            return rt *= rhs;
        }
        
        // scalar multiply
        friend vec2<T> operator * ( T lhs, const vec2<T> & rhs) {
            vec2<T> rt(lhs);
            return rt *= rhs;
        }
        
        // vector component-wise multiply
        friend vec2<T> operator * ( const vec2<T> & lhs, const vec2<T> & rhs){
            vec2<T> rt(lhs);
            return rt *= rhs;
        }
        
        // scalar multiply
        friend vec2<T> operator / ( const vec2<T> & lhs, T rhs) {
            vec2<T> rt(lhs);
            return rt /= rhs;
        }
        
        // vector component-wise multiply
        friend vec2<T> operator / ( const vec2<T> & lhs, const vec2<T> & rhs){
            vec2<T> rt(lhs);
            return rt /= rhs;
        }
        
        ////////////////////////////////////////////////////////
        //
        //  Comparison operators
        //
        ////////////////////////////////////////////////////////
        
        // equality
        friend bool operator == ( const vec2<T> &lhs, const vec2<T> &rhs ) {
            bool r = true;
            for (int i = 0; i < lhs.size(); i++)
                r &= lhs._array[i] == rhs._array[i];
            return r;
        }
        
        // inequality
        friend bool operator != ( const vec2<T> &lhs, const vec2<T> &rhs ) {
            bool r = true;
            for (int i = 0; i < lhs.size(); i++)
                r &= lhs._array[i] != rhs._array[i];
            return r;
        }
        
        //data intentionally left public to allow vec2.x
        union {
            struct {
                T x,y;          // standard names for components
            };
            struct {
                T s,t;          // standard names for components
            };
            T _array[2];     // array access
        };
    };
    
    //////////////////////////////////////////////////////////////////////
    //
    // vec3 - template class for 3-tuple vector
    //
    //////////////////////////////////////////////////////////////////////
    template <class T>
    class vec3 {
    public:
        
        typedef T value_type;
        int size() const { return 3;}
        
        ////////////////////////////////////////////////////////
        //
        //  Constructors
        //
        ////////////////////////////////////////////////////////
        
        // Default/scalar constructor
        vec3(const T & t = T()) {
            for(int i = 0; i < size(); i++) _array[i] = t;
        }
        
        // Construct from array
        vec3(const T * tp) {
            for(int i = 0; i < size(); i++) _array[i] = tp[i];
        }
        
        // Construct from explicit values
        vec3( const T v0, const T v1, const T v2) {
            x = v0;
            y = v1;
            z = v2;
        }
        
        explicit vec3( const vec4<T> &u) {
            for(int i = 0; i < size(); i++) _array[i] = u._array[i];
        }
        
        explicit vec3( const vec2<T> &u, T v0) {
            x = u.x;
            y = u.y;
            z = v0;
        }
        
        const T * get_value() const {
            return _array;
        }
        
        vec3<T> & set_value( const T * rhs ) {
            for(int i = 0; i < size(); i++) _array[i] = rhs[i];
            return *this;
        }
        
        // indexing operators
        T & operator [] ( int i ) {
            return _array[i];
        }
        
        const T & operator [] ( int i ) const {
            return _array[i];
        }
        
        // type-cast operators
        operator T * () {
            return _array;
        }
        
        operator const T * () const {
            return _array;
        }
        
        ////////////////////////////////////////////////////////
        //
        //  Math operators
        //
        ////////////////////////////////////////////////////////
        
        // scalar multiply assign
        friend vec3<T> & operator *= ( vec3<T> &lhs, T d ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= d;
            return lhs;
        }
        
        // component-wise vector multiply assign
        friend vec3<T> & operator *= ( vec3<T> &lhs, const vec3<T> &rhs ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= rhs[i];
            return lhs;
        }
        
        // scalar divide assign
        friend vec3<T> & operator /= ( vec3<T> &lhs, T d ) {
            if(d == 0) return lhs;
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= d;
            return lhs;
        }
        
        // component-wise vector divide assign
        friend vec3<T> & operator /= ( vec3<T> &lhs, const vec3<T> & rhs ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= rhs._array[i];
            return lhs;
        }
        
        // component-wise vector add assign
        friend vec3<T> & operator += ( vec3<T> &lhs, const vec3<T> & rhs ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] += rhs._array[i];
            return lhs;
        }
        
        // component-wise vector subtract assign
        friend vec3<T> & operator -= ( vec3<T> &lhs, const vec3<T> & rhs ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] -= rhs._array[i];
            return lhs;
        }
        
        // unary negate
        friend vec3<T> operator - ( const vec3<T> &rhs) {
            vec3<T> rv;
            for(int i = 0; i < rhs.size(); i++) rv._array[i] = -rhs._array[i];
            return rv;
        }
        
        // vector add
        friend vec3<T> operator + ( const vec3<T> & lhs, const vec3<T> & rhs) {
            vec3<T> rt(lhs);
            return rt += rhs;
        }
        
        // vector subtract
        friend vec3<T> operator - ( const vec3<T> & lhs, const vec3<T> & rhs) {
            vec3<T> rt(lhs);
            return rt -= rhs;
        }
        
        // scalar multiply
        friend vec3<T> operator * ( const vec3<T> & lhs, T rhs) {
            vec3<T> rt(lhs);
            return rt *= rhs;
        }
        
        // scalar multiply
        friend vec3<T> operator * ( T lhs, const vec3<T> & rhs) {
            vec3<T> rt(lhs);
            return rt *= rhs;
        }
        
        // vector component-wise multiply
        friend vec3<T> operator * ( const vec3<T> & lhs, const vec3<T> & rhs){
            vec3<T> rt(lhs);
            return rt *= rhs;
        }
        
        // scalar multiply
        friend vec3<T> operator / ( const vec3<T> & lhs, T rhs) {
            vec3<T> rt(lhs);
            return rt /= rhs;
        }
        
        // vector component-wise multiply
        friend vec3<T> operator / ( const vec3<T> & lhs, const vec3<T> & rhs){
            vec3<T> rt(lhs);
            return rt /= rhs;
        }
        
        // vector - axis rotation //harish
        void rotate(float angle, T vx, T vy, T vz) //harish
        {
            vec3<T> NewPosition;
            
            // Calculate the sine and cosine of the angle once
            float cosTheta = (float)cos(angle);
            float sinTheta = (float)sin(angle);
            
            
            NewPosition. _array[0]  = (cosTheta + (1 - cosTheta) * vx * vx)            * _array[0];
            NewPosition. _array[0] += ((1 - cosTheta) * vx * vy - vz * sinTheta)    * _array[1];
            NewPosition. _array[0] += ((1 - cosTheta) * vx * vz + vy * sinTheta)    * _array[2];
            
            
            NewPosition._array[1]  = ((1 - cosTheta) * vx * vy + vz * sinTheta)    * _array[0];
            NewPosition._array[1] += (cosTheta + (1 - cosTheta) * vy * vy)        * _array[1];
            NewPosition._array[1] += ((1 - cosTheta) * vy * vz - vx * sinTheta)    * _array[2];
            
            NewPosition._array[2]  = ((1 - cosTheta) * vx * vz - vy * sinTheta)    *  _array[0];
            NewPosition._array[2] += ((1 - cosTheta) * vy * vz + vx * sinTheta)    *  _array[1];
            NewPosition._array[2] += (cosTheta + (1 - cosTheta) * vz * vz)        *  _array[2];
            
            _array[0]=NewPosition._array[0];
            _array[1]=NewPosition._array[1];
            _array[2]=NewPosition._array[2];
        }
        
        //Calculate the cross product
        vec3<T> cross(vec3<T> vec) //harish
        {
            vec3<T> vNormal;                                    // The vector to hold the cross product
            
            vNormal._array[0] = ((_array[1] * vec._array[2]) - (_array[2] * vec._array[1]));
            vNormal._array[1] = ((_array[2] * vec._array[0]) - (_array[0] * vec._array[2]));
            vNormal._array[2] = ((_array[0] * vec._array[1]) - (_array[1] * vec._array[0]));
            
            return vNormal;
        }
        
        ////////////////////////////////////////////////////////
        //
        //  Comparison operators
        //
        ////////////////////////////////////////////////////////
        
        // equality
        friend bool operator == ( const vec3<T> &lhs, const vec3<T> &rhs ) {
            bool r = true;
            for (int i = 0; i < lhs.size(); i++)
                r &= lhs._array[i] == rhs._array[i];
            return r;
        }
        
        // inequality
        friend bool operator != ( const vec3<T> &lhs, const vec3<T> &rhs ) {
            bool r = true;
            for (int i = 0; i < lhs.size(); i++)
                r &= lhs._array[i] != rhs._array[i];
            return r;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        //
        // dimension specific operations
        //
        ////////////////////////////////////////////////////////////////////////////////
        
        // cross product
        friend vec3<T> cross( const vec3<T> & lhs, const vec3<T> & rhs) {
            vec3<T> r;
            
            r.x = lhs.y * rhs.z - lhs.z * rhs.y;
            r.y = lhs.z * rhs.x - lhs.x * rhs.z;
            r.z = lhs.x * rhs.y - lhs.y * rhs.x;
            
            return r;
        }
        
        //data intentionally left public to allow vec2.x
        union {
            struct {
                T x, y, z;          // standard names for components
            };
            struct {
                T s, t, r;          // standard names for components
            };
            T _array[3];     // array access
        };
    };
    
    //////////////////////////////////////////////////////////////////////
    //
    // vec4 - template class for 4-tuple vector
    //
    //////////////////////////////////////////////////////////////////////
    template <class T>
    class vec4 {
    public:
        
        typedef T value_type;
        int size() const { return 4;}
        
        ////////////////////////////////////////////////////////
        //
        //  Constructors
        //
        ////////////////////////////////////////////////////////
        
        // Default/scalar constructor
        vec4(const T & t = T()) {
            for(int i = 0; i < size(); i++) _array[i] = t;
        }
        
        // Construct from array
        vec4(const T * tp) {
            for(int i = 0; i < size(); i++) _array[i] = tp[i];
        }
        
        // Construct from explicit values
        vec4( const T v0, const T v1, const T v2, const T v3) {
            x = v0;
            y = v1;
            z = v2;
            w = v3;
        }
        
        explicit vec4( const vec3<T> &u, T v0) {
            x = u.x;
            y = u.y;
            z = u.z;
            w = v0;
        }
        
        explicit vec4( const vec2<T> &u, T v0, T v1) {
            x = u.x;
            y = u.y;
            z = v0;
            w = v1;
        }
        
        const T * get_value() const {
            return _array;
        }
        
        vec4<T> & set_value( const T * rhs ) {
            for(int i = 0; i < size(); i++) _array[i] = rhs[i];
            return *this;
        }
        
        // indexing operators
        T & operator [] ( int i ) {
            return _array[i];
        }
        
        const T & operator [] ( int i ) const {
            return _array[i];
        }
        
        // type-cast operators
        operator T * () {
            return _array;
        }
        
        operator const T * () const {
            return _array;
        }
        
        ////////////////////////////////////////////////////////
        //
        //  Math operators
        //
        ////////////////////////////////////////////////////////
        
        // scalar multiply assign
        friend vec4<T> & operator *= ( vec4<T> &lhs, T d ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= d;
            return lhs;
        }
        
        // component-wise vector multiply assign
        friend vec4<T> & operator *= ( vec4<T> &lhs, const vec4<T> &rhs ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= rhs[i];
            return lhs;
        }
        
        // scalar divide assign
        friend vec4<T> & operator /= ( vec4<T> &lhs, T d ) {
            if(d == 0) return lhs;
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= d;
            return lhs;
        }
        
        // component-wise vector divide assign
        friend vec4<T> & operator /= ( vec4<T> &lhs, const vec4<T> & rhs ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= rhs._array[i];
            return lhs;
        }
        
        // component-wise vector add assign
        friend vec4<T> & operator += ( vec4<T> &lhs, const vec4<T> & rhs ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] += rhs._array[i];
            return lhs;
        }
        
        // component-wise vector subtract assign
        friend vec4<T> & operator -= ( vec4<T> &lhs, const vec4<T> & rhs ) {
            for(int i = 0; i < lhs.size(); i++) lhs._array[i] -= rhs._array[i];
            return lhs;
        }
        
        // unary negate
        friend vec4<T> operator - ( const vec4<T> &rhs) {
            vec4<T> rv;
            for(int i = 0; i < rhs.size(); i++) rv._array[i] = -rhs._array[i];
            return rv;
        }
        
        // vector add
        friend vec4<T> operator + ( const vec4<T> & lhs, const vec4<T> & rhs) {
            vec4<T> rt(lhs);
            return rt += rhs;
        }
        
        // vector subtract
        friend vec4<T> operator - ( const vec4<T> & lhs, const vec4<T> & rhs) {
            vec4<T> rt(lhs);
            return rt -= rhs;
        }
        
        // scalar multiply
        friend vec4<T> operator * ( const vec4<T> & lhs, T rhs) {
            vec4<T> rt(lhs);
            return rt *= rhs;
        }
        
        // scalar multiply
        friend vec4<T> operator * ( T lhs, const vec4<T> & rhs) {
            vec4<T> rt(lhs);
            return rt *= rhs;
        }
        
        // vector component-wise multiply
        friend vec4<T> operator * ( const vec4<T> & lhs, const vec4<T> & rhs){
            vec4<T> rt(lhs);
            return rt *= rhs;
        }
        
        // scalar multiply
        friend vec4<T> operator / ( const vec4<T> & lhs, T rhs) {
            vec4<T> rt(lhs);
            return rt /= rhs;
        }
        
        // vector component-wise multiply
        friend vec4<T> operator / ( const vec4<T> & lhs, const vec4<T> & rhs){
            vec4<T> rt(lhs);
            return rt /= rhs;
        }
        
        ////////////////////////////////////////////////////////
        //
        //  Comparison operators
        //
        ////////////////////////////////////////////////////////
        
        // equality
        friend bool operator == ( const vec4<T> &lhs, const vec4<T> &rhs ) {
            bool r = true;
            for (int i = 0; i < lhs.size(); i++)
                r &= lhs._array[i] == rhs._array[i];
            return r;
        }
        
        // inequality
        friend bool operator != ( const vec4<T> &lhs, const vec4<T> &rhs ) {
            bool r = true;
            for (int i = 0; i < lhs.size(); i++)
                r &= lhs._array[i] != rhs._array[i];
            return r;
        }
        
        //data intentionally left public to allow vec2.x
        union {
            struct {
                T x, y, z, w;          // standard names for components
            };
            struct {
                T s, t, r, q;          // standard names for components
            };
            T _array[4];     // array access
        };
    };
    
    ////////////////////////////////////////////////////////////////////////////////
    //
    // Generic vector operations
    //
    ////////////////////////////////////////////////////////////////////////////////
    
    // compute the dot product of two vectors
    //template<class T>
    //inline typename T::value_type dot( const T & lhs, const T & rhs ) {
    //    T::value_type r = 0;
    //    for(int i = 0; i < lhs.size(); i++) r += lhs._array[i] * rhs._array[i];
    //    return r;
    //}
    
    template<class T>
    inline T dot(const vec2<T> & lhs, const vec2<T> & rhs ) {
        T r = 0;
        for(int i=0; i < lhs.size(); i++) r += lhs._array[i] * rhs._array[i];
        return r;
    }
    
    template<class T>
    inline T dot(const vec3<T> & lhs, const vec3<T> & rhs ) {
        T r = 0;
        for(int i=0; i < lhs.size(); i++) r += lhs._array[i] * rhs._array[i];
        return r;
    }
    
    template<class T>
    inline T dot(const vec4<T> & lhs, const vec4<T> & rhs ) {
        T r = 0;
        for(int i=0; i < lhs.size(); i++) r += lhs._array[i] * rhs._array[i];
        return r;
    }
    
    // return the length of the provided vector
    //template< class T>
    //inline typename T::value_type length( const T & vec) {
    //    T::value_type r = 0;
    //    for(int i = 0; i < vec.size(); i++) r += vec._array[i]*vec._array[i];
    //    return T::value_type(sqrt(r));
    //}
    
    template<class T>
    inline T length( const vec2<T> & vec) {
        T r = 0;
        for(int i = 0; i < vec.size(); i++) r += vec._array[i]*vec._array[i];
        return T(sqrt(r));
    }
    
    template<class T>
    inline T length( const vec3<T> & vec) {
        T r = 0;
        for(int i = 0; i < vec.size(); i++) r += vec._array[i]*vec._array[i];
        return T(sqrt(r));
    }
    
    template<class T>
    inline T length( const vec4<T> & vec) {
        T r = 0;
        for(int i = 0; i < vec.size(); i++) r += vec._array[i]*vec._array[i];
        return T(sqrt(r));
    }
    
    // return the squared norm
    //template< class T>
    //inline typename T::value_type square_norm( const T & vec) {
    //    T::value_type r = 0;
    //    for(int i = 0; i < vec.size(); i++) r += vec._array[i]*vec._array[i];
    //    return r;
    //}
    
    template< class T>
    inline T square_norm( const vec2<T> & vec) {
        T r = 0;
        for(int i = 0; i < vec.size(); i++) r += vec._array[i]*vec._array[i];
        return r;
    }
    
    template< class T>
    inline T square_norm( const vec3<T> & vec) {
        T r = 0;
        for(int i = 0; i < vec.size(); i++) r += vec._array[i]*vec._array[i];
        return r;
    }
    
    template< class T>
    inline T square_norm( const vec4<T> & vec) {
        T r = 0;
        for(int i = 0; i < vec.size(); i++) r += vec._array[i]*vec._array[i];
        return r;
    }
    
    // return the normalized version of the vector
    //template< class T>
    //inline T normalize( const T & vec) {
    //    T::value_type sum(0);
    //    T r;
    //    for(int i = 0; i < vec.size(); i++)
    //        sum += vec._array[i] * vec._array[i];
    //    sum = T::value_type(sqrt(sum));
    //    if (sum > 0)
    //        for(int i = 0; i < vec.size(); i++)
    //            r._array[i] = vec._array[i] / sum;
    //    return r;
    //}
    
    template< class T>
    inline vec2<T> normalize( const vec2<T> & vec) {
        T sum(0);
        vec2<T> r;
        for(int i = 0; i < vec.size(); i++)
            sum += vec._array[i] * vec._array[i];
        sum = T(sqrt(sum));
        if (sum > 0)
            for(int i = 0; i < vec.size(); i++)
                r._array[i] = vec._array[i] / sum;
        return r;
    }
    
    template< class T>
    inline vec3<T> normalize( const vec3<T> & vec) {
        T sum(0);
        vec3<T> r;
        for(int i = 0; i < vec.size(); i++)
            sum += vec._array[i] * vec._array[i];
        sum = T(sqrt(sum));
        if (sum > 0)
            for(int i = 0; i < vec.size(); i++)
                r._array[i] = vec._array[i] / sum;
        return r;
    }
    
    template< class T>
    inline vec4<T> normalize( const vec4<T> & vec) {
        T sum(0);
        vec4<T> r;
        for(int i = 0; i < vec.size(); i++)
            sum += vec._array[i] * vec._array[i];
        sum = T(sqrt(sum));
        if (sum > 0)
            for(int i = 0; i < vec.size(); i++)
                r._array[i] = vec._array[i] / sum;
        return r;
    }
    
    // In VC8 : min and max are already defined by a #define...
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
    //componentwise min
    template< class T>
    inline T min( const T & lhs, const T & rhs ) {
        T rt;
        for (int i = 0; i < lhs.size(); i++) rt._array[i] = std::min( lhs._array[i], rhs._array[i]);
        return rt;
    }
    
    // componentwise max
    template< class T>
    inline T max( const T & lhs, const T & rhs ) {
        T rt;
        for (int i = 0; i < lhs.size(); i++) rt._array[i] = std::max( lhs._array[i], rhs._array[i]);
        return rt;
    }
    
    //  Matrix
    
    template<class T>
    class matrix4
    {
        
    public:
        
        matrix4() { make_identity(); }
        
        matrix4( T t )
        { set_value(t); }
        
        matrix4( const T * m )
        { set_value(m); }
        
        matrix4( T a00, T a01, T a02, T a03,
                T a10, T a11, T a12, T a13,
                T a20, T a21, T a22, T a23,
                T a30, T a31, T a32, T a33 ) :
        _11(a00), _12(a01), _13(a02), _14(a03),
        _21(a10), _22(a11), _23(a12), _24(a13),
        _31(a20), _32(a21), _33(a22), _34(a23),
        _41(a30), _42(a31), _43(a32), _44(a33)
        {}
        
        
        void get_value( T * mp ) const {
            int c = 0;
            for(int j=0; j < 4; j++)
                for(int i=0; i < 4; i++)
                    mp[c++] = element(i,j);
        }
        
        const T * get_value() const {
            return _array;
        }
        
        void set_value( T * mp) {
            int c = 0;
            for(int j=0; j < 4; j++)
                for(int i=0; i < 4; i++)
                    element(i,j) = mp[c++];
        }
        
        void set_value( T r ) {
            for(int i=0; i < 4; i++)
                for(int j=0; j < 4; j++)
                    element(i,j) = r;
        }
        
        void make_identity() {
            element(0,0) = 1.0;
            element(0,1) = 0.0;
            element(0,2) = 0.0;
            element(0,3) = 0.0;
            
            element(1,0) = 0.0;
            element(1,1) = 1.0;
            element(1,2) = 0.0;
            element(1,3) = 0.0;
            
            element(2,0) = 0.0;
            element(2,1) = 0.0;
            element(2,2) = 1.0;
            element(2,3) = 0.0;
            
            element(3,0) = 0.0;
            element(3,1) = 0.0;
            element(3,2) = 0.0;
            element(3,3) = 1.0;
        }
        
        // set a uniform scale
        void set_scale( T s ) {
            element(0,0) = s;
            element(1,1) = s;
            element(2,2) = s;
        }
        
        void set_scale( const vec3<T> & s ) {
            for (int i = 0; i < 3; i++) element(i,i) = s[i];
        }
        
        
        void set_translate( const vec3<T> & t ) {
            for (int i = 0; i < 3; i++) element(i,3) = t[i];
        }
        
        void set_row(int r, const vec4<T> & t) {
            for (int i = 0; i < 4; i++) element(r,i) = t[i];
        }
        
        void set_column(int c, const vec4<T> & t) {
            for (int i = 0; i < 4; i++) element(i,c) = t[i];
        }
        
        vec4<T> get_row(int r) const {
            vec4<T> v;
            for (int i = 0; i < 4; i++) v[i] = element(r,i);
            return v;
        }
        
        vec4<T> get_column(int c) const {
            vec4<T> v;
            for (int i = 0; i < 4; i++) v[i] = element(i,c);
            return v;
        }
        
        matrix4 & operator *= ( const matrix4 & rhs ) {
            matrix4 mt(*this);
            set_value(T(0));
            
            for(int i=0; i < 4; i++)
                for(int j=0; j < 4; j++)
                    for(int c=0; c < 4; c++)
                        element(i,j) += mt(i,c) * rhs(c,j);
            return *this;
        }
        
        friend matrix4 operator * ( const matrix4 & lhs, const matrix4 & rhs ) {
            matrix4 r(T(0));
            
            for(int i=0; i < 4; i++)
                for(int j=0; j < 4; j++)
                    for(int c=0; c < 4; c++)
                        r.element(i,j) += lhs(i,c) * rhs(c,j);
            return r;
        }
        
        // dst = M * src
        vec4<T> operator *( const vec4<T> &src) const {
            vec4<T> r;
            for ( int i = 0; i < 4; i++)
                r[i]  = ( src[0] * element(i,0) + src[1] * element(i,1) +
                         src[2] * element(i,2) + src[3] * element(i,3));
            return r;
        }
        
        // dst = src * M
        friend vec4<T> operator *( const vec4<T> &lhs, const matrix4 &rhs) {
            vec4<T> r;
            for ( int i = 0; i < 4; i++)
                r[i]  = ( lhs[0] * rhs.element(0,i) + lhs[1] * rhs.element(1,i) +
                         lhs[2] * rhs.element(2,i) + lhs[3] * rhs.element(3,i));
            return r;
        }
        
        T & operator () (int row, int col) {
            return element(row,col);
        }
        
        const T & operator () (int row, int col) const {
            return element(row,col);
        }
        
        T & element (int row, int col) {
            return _array[row | (col<<2)];
        }
        
        const T & element (int row, int col) const {
            return _array[row | (col<<2)];
        }
        
        matrix4 & operator *= ( const T & r ) {
            for (int i = 0; i < 4; ++i) {
                element(0,i) *= r;
                element(1,i) *= r;
                element(2,i) *= r;
                element(3,i) *= r;
            }
            return *this;
        }
        
        matrix4 & operator += ( const matrix4 & mat ) {
            for (int i = 0; i < 4; ++i) {
                element(0,i) += mat.element(0,i);
                element(1,i) += mat.element(1,i);
                element(2,i) += mat.element(2,i);
                element(3,i) += mat.element(3,i);
            }
            return *this;
        }
        
        
        friend bool operator == ( const matrix4 & lhs, const matrix4 & rhs ) {
            bool r = true;
            for (int i = 0; i < 16; i++)
                r &= lhs._array[i] == rhs._array[i];
            return r;
        }
        
        friend bool operator != ( const matrix4 & lhs, const matrix4 & rhs )  {
            bool r = true;
            for (int i = 0; i < 16; i++)
                r &= lhs._array[i] != rhs._array[i];
            return r;
        }
        
        union {
            struct {
                T _11, _12, _13, _14;   // standard names for components
                T _21, _22, _23, _24;   // standard names for components
                T _31, _32, _33, _34;   // standard names for components
                T _41, _42, _43, _44;   // standard names for components
            };
            T _array[16];     // array access
        };
    };
    
    
    //////////////////////////////////////////////////////////////////////
    //
    //  Friend functions
    //
    //   Not actually declared friends due to VC's template handling
    //
    //////////////////////////////////////////////////////////////////////
    template<class T>
    matrix4<T> inverse( const matrix4<T> & m) {
        matrix4<T> minv;
        
        T r1[8], r2[8], r3[8], r4[8];
        T *s[4], *tmprow;
        
        s[0] = &r1[0];
        s[1] = &r2[0];
        s[2] = &r3[0];
        s[3] = &r4[0];
        
        int i,j,p,jj;
        for(i=0;i<4;i++) {
            for(j=0;j<4;j++) {
                s[i][j] = m.element(i,j);
                if(i==j) s[i][j+4] = 1.0;
                else     s[i][j+4] = 0.0;
            }
        }
        T scp[4];
        for(i=0;i<4;i++) {
            scp[i] = T(fabs(s[i][0]));
            for(j=1;j<4;j++)
                if(T(fabs(s[i][j])) > scp[i]) scp[i] = T(fabs(s[i][j]));
            if(scp[i] == 0.0) return minv; // singular matrix!
        }
        
        int pivot_to;
        T scp_max;
        for(i=0;i<4;i++) {
            // select pivot row
            pivot_to = i;
            scp_max = T(fabs(s[i][i]/scp[i]));
            // find out which row should be on top
            for(p=i+1;p<4;p++)
                if (T(fabs(s[p][i]/scp[p])) > scp_max) {
                    scp_max = T(fabs(s[p][i]/scp[p]));
                    pivot_to = p;
                }
            // Pivot if necessary
            if(pivot_to != i) {
                tmprow = s[i];
                s[i] = s[pivot_to];
                s[pivot_to] = tmprow;
                T tmpscp;
                tmpscp = scp[i];
                scp[i] = scp[pivot_to];
                scp[pivot_to] = tmpscp;
            }
            
            T mji;
            // perform gaussian elimination
            for(j=i+1;j<4;j++) {
                mji = s[j][i]/s[i][i];
                s[j][i] = 0.0;
                for(jj=i+1;jj<8;jj++)
                    s[j][jj] -= mji*s[i][jj];
            }
        }
        if(s[3][3] == 0.0) return minv; // singular matrix!
        
        //
        // Now we have an upper triangular matrix.
        //
        //  x x x x | y y y y
        //  0 x x x | y y y y
        //  0 0 x x | y y y y
        //  0 0 0 x | y y y y
        //
        //  we'll back substitute to get the inverse
        //
        //  1 0 0 0 | z z z z
        //  0 1 0 0 | z z z z
        //  0 0 1 0 | z z z z
        //  0 0 0 1 | z z z z
        //
        
        T mij;
        for(i=3;i>0;i--) {
            for(j=i-1;j > -1; j--) {
                mij = s[j][i]/s[i][i];
                for(jj=j+1;jj<8;jj++)
                    s[j][jj] -= mij*s[i][jj];
            }
        }
        
        for(i=0;i<4;i++)
            for(j=0;j<4;j++)
                minv(i,j) = s[i][j+4] / s[i][i];
        
        return minv;
    }
    
    
    //
    // transpose
    //
    //   return the transposed matrix
    ////////////////////////////////////////////////////////////
    template<class T>
    matrix4<T> transpose( const matrix4<T> & m) {
        matrix4<T> mtrans;
        
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                mtrans(i,j) = m.element(j,i);
        return mtrans;
    }
    
    //
    // Rotation matrix creation
    // From rotation angle around X axis [radians]
    //
    //   return the rotation matrix
    ////////////////////////////////////////////////////////////
    template<class T>
    matrix4<T>& rotationX( matrix4<T> & M, const T angle )
    {
        T cosa = cos(angle);
        T sina = sin(angle);
        
        M.element(0,0) = 1.0;
        M.element(0,1) = 0.0;
        M.element(0,2) = 0.0;
        M.element(0,3) = 0.0;
        
        M.element(1,0) = 0.0;
        M.element(1,1) = cosa;
        M.element(1,2) = -sina;
        M.element(1,3) = 0.0;
        
        M.element(2,0) = 0.0;
        M.element(2,1) = sina;
        M.element(2,2) = cosa;
        M.element(2,3) = 0.0;
        
        M.element(3,0) = 0.0;
        M.element(3,1) = 0.0;
        M.element(3,2) = 0.0;
        M.element(3,3) = 1.0;
        
        return M;
    }
    
    //
    // Rotation matrix creation
    // From rotation angle around Y axis [radians]
    //
    //   return the rotation matrix
    ////////////////////////////////////////////////////////////
    template<class T>
    matrix4<T>& rotationY( matrix4<T> & M, const T angle )
    {
        T cosa = cos(angle);
        T sina = sin(angle);
        
        M.element(0,0) = cosa;
        M.element(0,1) = 0.0;
        M.element(0,2) = sina;
        M.element(0,3) = 0.0;
        
        M.element(1,0) = 0.0;
        M.element(1,1) = 1.0;
        M.element(1,2) = 0.0;
        M.element(1,3) = 0.0;
        
        M.element(2,0) = -sina;
        M.element(2,1) = 0.0;
        M.element(2,2) = cosa;
        M.element(2,3) = 0.0;
        
        M.element(3,0) = 0.0;
        M.element(3,1) = 0.0;
        M.element(3,2) = 0.0;
        M.element(3,3) = 1.0;
        
        return M;
    }
    
    //
    // Rotation matrix creation
    // From rotation angle around Y axis [radians]
    //
    //   return the rotation matrix
    ////////////////////////////////////////////////////////////
    template<class T>
    matrix4<T>& rotationZ( matrix4<T> & M, const T angle )
    {
        T cosa = cos(angle);
        T sina = sin(angle);
        
        M.element(0,0) = cosa;
        M.element(0,1) = -sina;
        M.element(0,2) = 0.0;
        M.element(0,3) = 0.0;
        
        M.element(1,0) = sina;
        M.element(1,1) = cosa;
        M.element(1,2) = 0.0;
        M.element(1,3) = 0.0;
        
        M.element(2,0) = 0.0;
        M.element(2,1) = 0.0;
        M.element(2,2) = 1.0;
        M.element(2,3) = 0.0;
        
        M.element(3,0) = 0.0;
        M.element(3,1) = 0.0;
        M.element(3,2) = 0.0;
        M.element(3,3) = 1.0;
        
        return M;
    }
    
    //
    // Rotation matrix creation
    // From euler angles
    //      1/ Yaw around Y axis in radians
    //      2/ Pitch around X axis in radians
    //      3/ Roll around Z axis in radians
    //
    //   return the rotation matrix [R] = [Roll].{Pitch].[Yaw]
    ////////////////////////////////////////////////////////////
    template<class T>
    matrix4<T>& rotationYawPitchRoll( matrix4<T> & M, const T yaw , const T pitch , const T roll )
    {
        M.make_identity();
        matrix4<T> rot;
        
        if (roll)
        {
            ASC::rotationZ(M, roll );
        }
        if (pitch)
        {
            M *= ASC::rotationX(rot, pitch );
        }
        if (yaw)
        {
            M *= ASC::rotationY(rot, yaw );
        }
        
        return M;
    }
    
    //
    // Translation matrix creation
    // From absolute translation values along X, Y and Z axis
    //
    //   return the translation matrix
    ////////////////////////////////////////////////////////////
    template<class T>
    matrix4<T>& translation( matrix4<T> & M, const T tx , const T ty , const T tz )
    {
        M.element(0,0) = 1.0;
        M.element(1,0) = 0.0;
        M.element(2,0) = 0.0;
        M.element(3,0) = 0.0;
        
        M.element(0,1) = 0.0;
        M.element(1,1) = 1.0;
        M.element(2,1) = 0.0;
        M.element(3,1) = 0.0;
        
        M.element(0,2) = 0.0;
        M.element(1,2) = 0.0;
        M.element(2,2) = 1.0;
        M.element(3,2) = 0.0;
        
        M.element(0,3) = tx;
        M.element(1,3) = ty;
        M.element(2,3) = tz;
        M.element(3,3) = 1.0;
        
        return M;
    }
    
    //
    // Look At matrix creation
    //
    //   return the inverse view matrix
    ////////////////////////////////////////////////////////////
    template<class T>
    matrix4<T> & lookAt( matrix4<T>& M, const vec3<T>& eye, const vec3<T>& center, const vec3<T>& up)
    {
        vec3<T> x, y, z;
        
        // make rotation matrix
        
        // Z vector
        z.x = eye.x - center.x;
        z.y = eye.y - center.y;
        z.z = eye.z - center.z;
        z = normalize(z);
        
        // Y vector
        y.x = up.x;
        y.y = up.y;
        y.z = up.z;
        
        // X vector = Y cross Z
        x = cross(y,z);
        
        // Recompute Y = Z cross X
        y = cross(z,x);
        
        // cross product gives area of parallelogram, which is < 1.0 for
        // non-perpendicular unit-length vectors; so normalize x, y here
        x = normalize(x);
        y = normalize(y);
        
        M._11 = x.x; M._21 = x.y; M._31 = x.z; M._41 = -x.x * eye.x - x.y * eye.y - x.z*eye.z;
        M._12 = y.x; M._22 = y.y; M._32 = y.z; M._42 = -y.x * eye.x - y.y * eye.y - y.z*eye.z;
        M._13 = z.x; M._23 = z.y; M._33 = z.z; M._43 = -z.x * eye.x - z.y * eye.y - z.z*eye.z;
        M._14 = 0.0; M._24 = 0.0; M._34 = 0.0; M._44 = 1.0;
        return M;
    }
    
    //
    // Projection matrix creation (Right Handed, OpenGL standard)
    // From the frustum definition
    //
    //   return the projection matrix
    ////////////////////////////////////////////////////////////
    template<class T>
    matrix4<T>& frustum( matrix4<T> & M, const T l, const T r, const T b, const T t, const T n, const T f)
    {
        M.element(0,0) = ((T)(2.0))*n / (r-l);
        M.element(1,0) = 0.0;
        M.element(2,0) = 0.0;
        M.element(3,0) = 0.0;
        
        M.element(0,1) = 0.0;
        M.element(1,1) = ((T)(2.0))*n / (t-b);
        M.element(2,1) = 0.0;
        M.element(3,1) = 0.0;
        
        M.element(0,2) = (r+l) / (r-l);
        M.element(1,2) = (t+b) / (t-b);
        M.element(2,2) = -(f+n) / (f-n);
        M.element(3,2) = -1.0;
        
        M.element(0,3) = 0.0;
        M.element(1,3) = 0.0;
        M.element(2,3) = -(((T)(2.0))*f*n) / (f-n);
        M.element(3,3) = 0.0;
        
        return M;
    }
    
    //
    // Projection matrix creation (Right Handed, OpenGL standard)
    // From the fovy in radians, aspect ratio and near far definition
    //
    //   return the projection matrix
    ////////////////////////////////////////////////////////////
    template<class T>
    matrix4<T>& perspective( matrix4<T> & M, const T fovy, const T aspect, const T n, const T f)
    {
        T xmin, xmax, ymin, ymax;
        
        ymax = n * (T)tan(fovy * 0.5);
        ymin = -ymax;
        
        xmin = ymin * aspect;
        xmax = ymax * aspect;
        
        return frustum(M, xmin, xmax, ymin, ymax, n, f);
    }
    
    template<class T>
    matrix4<T>& perspectivex( matrix4<T> & M, const T fovx, const T aspect, const T near, const T far)
    {
        float e = 1.0f / tanf(fovx / 2.0f);
        float aspectInv = 1.0f / aspect;
        float fovy = 2.0f * atanf(aspectInv / e);
        float xScale = 1.0f / tanf(0.5f * fovy);
        float yScale = xScale / aspectInv;
        
        M._array[0] = xScale;
        M._array[1] = 0.0f;
        M._array[2] = 0.0f;
        M._array[3] = 0.0f;
        
        M._array[4] = 0.0f;
        M._array[5] = yScale;
        M._array[6] = 0.0f;
        M._array[7] = 0.0f;
        
        M._array[8] = 0.0f;
        M._array[9] = 0.0f;
        M._array[10] = (far + near) / (near - far);
        M._array[11] = -1.0f;
        
        M._array[12] = 0.0f;
        M._array[13] = 0.0f;
        M._array[14] = (2.0f * far * near) / (near - far);
        M._array[15] = 0.0f;
        
        return M;
    }
    
    template<class T>
    matrix4<T>& ortho2D(matrix4<T> & M, T left, T right, T bottom, T top)
    {
        float zNear=-1.0; float zFar=1.0;
        
        float sx = 2.0f / (right - left);
        float sy = 2.0f / (top - bottom);
        float sz = 2.0f / (zFar - zNear);
        
        float tx = -(right + left) / (right - left);
        float ty = -(top + bottom) / (top - bottom);
        float tz = -(zFar + zNear) / (zFar - zNear);
        
        //matrix is stored column major
        M._array[0] = sx,    M._array[4] = 0.0f, M._array[ 8] = 0.0f,  M._array[12] = tx;
        M._array[1] = 0.0f,  M._array[5] = sy,   M._array[ 9] = 0.0f,  M._array[13] = ty;
        M._array[2] = 0.0f,  M._array[6] = 0.0f, M._array[10] = sz,    M._array[14] = tz;
        M._array[3] = 0.0f,  M._array[7] = 0.0f, M._array[11] = 0.0f,  M._array[15] = 1.0f;
        
        return M;
    }
    
    template<class T>
    matrix4<T>& ortho3D(matrix4<T> & M, T left, T right, T bottom, T top, T zNear, T zFar)
    {
        float sx = 2.0f / (right - left);
        float sy = 2.0f / (top - bottom);
        float sz = 2.0f / (zFar - zNear);
        
        float tx = -(right + left) / (right - left);
        float ty = -(top + bottom) / (top - bottom);
        float tz = -(zFar + zNear) / (zFar - zNear);
        
        //matrix is stored column major
        M._array[0] = sx,     M._array[4] = 0.0f, M._array[ 8] = 0.0f,  M._array[12] = tx;
        M._array[1] = 0.0f,  M._array[5] = sy,     M._array[ 9] = 0.0f,  M._array[13] = ty;
        M._array[2] = 0.0f,  M._array[6] = 0.0f, M._array[10] = sz,       M._array[14] = tz;
        M._array[3] = 0.0f,  M._array[7] = 0.0f, M._array[11] = 0.0f,  M._array[15] = 1.0f;
        
        return M;
    }
    
    inline unsigned int pow2(unsigned int x)
    {
        unsigned int y = 1;
        if (!x) return 0;
        
        while (y < x)  {
            y += y;
        }
        
        return y;
    }
};


#endif /* defined(_MATH_UTILS_) */
