//------------------------------------------------------------------------------
// RemoveReference.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

template <typename T>
struct _RemoveReference
{
	using Type = T;
};

template <typename T>
struct _RemoveReference<T&>
{
	using Type = T;
};

template <typename T>
struct _RemoveReference<T&&>
{
	using Type = T;
};

template <typename T>
using RemoveReference = typename _RemoveReference<T>::Type;