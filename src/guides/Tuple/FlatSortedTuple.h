//------------------------------------------------------------------------------
// FlatSortedTuple.h
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 19.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "typelist/TypeList.h"
#include "typelist/typelistsort.h"
#include "tuple/FlatTuple.h"

namespace Detail
{
    template<typename... Ts, typename... Vs>
    constexpr auto MakeFlatTupleFromTypeList(TypeList<Ts...>, Vs&&... args)
    {
        return FlatTuple<Ts...>();
    }
}

template<typename... Ts>
constexpr auto MakeSortedTuple(Ts... args)
{
    constexpr auto typeList = Sort(TypeList<Ts...>{});
    return Detail::MakeFlatTupleFromTypeList(typeList, std::forward<Ts>(args)...);
}