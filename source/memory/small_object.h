//------------------------------------------------------------------------------
// small_object.h
// 
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 7.08.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------

#pragma once

#include "small_object_allocator.h"

namespace hope::memory {

    /**
     * \brief thin useful wrapper of small_object_allocator
     * just inherit your class from this, and u get height quality boost of performance :)
     * take safe and fasten seat belts, the rocket starts now!
     */
    class small_object {
    public:
        virtual ~small_object() = default;

        static void* operator new(std::size_t size);
        static void operator delete(void* ptr, std::size_t size);
    };

}
