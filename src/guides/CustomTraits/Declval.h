//------------------------------------------------------------------------------
// DeclVal.h

// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 24.05.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "AddReference.h"

template <typename T>
// ReSharper disable once CppFunctionIsNotImplemented
constexpr AddRValueReference<T> DeclVal() noexcept;
