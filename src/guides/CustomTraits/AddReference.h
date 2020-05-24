//------------------------------------------------------------------------------
// AddReference.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

template <typename T>
struct _AddLValueReference
{
	using Type = T&;
};

// TODO:: add remove qualifiers specialization
template <>
struct _AddLValueReference<void>
{
	using Type = void;
};

template <typename T>
using AddLValueReference = typename _AddLValueReference<T>::Type;

template <typename T>
struct _AddRValueReference
{
	using Type = T&&;
};

template <>
struct _AddRValueReference<void>
{
	using Type = void;
};

template <typename T>
using AddRValueReference = typename _AddRValueReference<T>::Type;
