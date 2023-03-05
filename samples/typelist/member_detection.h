/* Copyright (C) 2021 - 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/components/detector.h"

namespace hope::sample {

    template<typename TLambda, typename TFunctor = std::decay_t<TLambda>>
    std::enable_if_t<>void create_function(TLambda&& f){

    }

    void run_detector_test(){

    }
}
