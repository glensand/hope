//------------------------------------------------------------------------------
// Decay.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "RemoveQualifiers.h"

template <typename T>
struct _Decay : _RemoveCV<T>
{
	
};

template <typename T>
struct _Decay<T[]>
{
	using Type = T*;
};

template <typename T, size_t N>
struct _Decay<T[N]>
{
	using Type = T*;
};

template<typename R, typename... Args>
struct _Decay<R(Args...)>
{
	using Type = R(*)(Args...);
};

template<typename R, typename... Args>
struct _Decay<R(Args..., ...)>
{
	using Type = R(*)(Args..., ...);
};

template <typename T>
using Decay = typename _Decay<T>::Type;