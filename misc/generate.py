#------------------------------------------------------------------------------
# generate.py
# Copyright (c) 2020 glensand
# All rights reserved.
#
# Date: 19.06.2020
# Author: Bezborodoff Gleb
#------------------------------------------------------------------------------

header = '''
//------------------------------------------------------------------------------
// generated.h
// type_list like tuple
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 17.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------
#pragma once

#include "tuple/FlatTuple.h"
#include "tuple/DetectFieldsCount.h"
namespace detail {

    template<std::size_t I>
    using Int = std::integral_constant<std::size_t, I>;

    namespace generated {\n
'''

generated = open('generated.h', 'w')
generated.write(header)

constantBegin = '''
        template <typename T>
        constexpr auto tuple_from_struct(const T& object, Int<'''

payload = str("")

for i in range (1, 100):
    generated.write(constantBegin)
    generated.write(str(i))
    generated.write(">) {\n")
    generated.write("\t\t\tauto& [")

    argList = ""
    for j in range (0, i):
        if j != 0: 
            argList += ", "

        argList += "_"
        argList += str(j)

    generated.write(argList)
    generated.write("] = object;\n")
    generated.write("\t\t\treturn make_flat_tuple(")
    generated.write(argList)
    generated.write(");\n\t\t}\n")

constantEnd = '''
        }
   }

namespace hope {
    template <typename T>
    constexpr auto tuple_from_struct(const T& object) {
        constexpr auto fields_count = detect_fields_count(T{});
        return detail::generated::tuple_from_struct(object, detail::Int<fields_count>{});
    }
}
'''

generated.write(constantEnd)
generated.close()
