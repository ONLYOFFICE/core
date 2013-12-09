#pragma once

#ifndef CALL_TRAITS_INCLUDE_H_
#define CALL_TRAITS_INCLUDE_H_

template <typename T>
struct NSCT_IMP
{
   typedef const T& param_type;
};

template <typename T>
struct NSCallTraits
{
public:
   typedef T value_type;
   typedef T& reference;
   typedef const T& const_reference;

   typedef typename NSCT_IMP<T>::param_type param_type;
};

#endif // CALL_TRAITS_INCLUDE_H_