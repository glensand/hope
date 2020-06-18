//------------------------------------------------------------------------------
// RemoveQualifiers.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

template <typename T>
struct _RemoveConst
{
	using Type = T;
};

template <typename T>
struct _RemoveConst<T const>
{
	using Type = T;
};

template <typename T>
using RemoveConst = typename _RemoveConst<T>::Type;

// TODO:: is it appropriate implementation? do we remove const qv?
template <typename T>
struct _RemoveVolatile
{
	using Type = T;
};

template <typename T>
struct _RemoveVolatile<T volatile>
{
	using Type = T;
};

template <typename T>
using RemoveVolatile = typename _RemoveVolatile<T>::Type;

template <typename T>
struct _RemoveCV : _RemoveConst<RemoveVolatile<T>>
{
	
};

template <typename T>
using RemoveCV = typename _RemoveCV<T>::Type;