# Copyright (C) 2020  - 2021 Gleb Bezborodov - All Rights Reserved
# You may use, distribute and modify this code under the
# terms of the MIT license.
# 
# You should have received a copy of the MIT license with
# this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope

header = '''
/* Copyright (C) 2020- 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */
 
#pragma once

#include "hope/tuple/flat_tuple.h"
#include "hope/tuple/tuple_policy.h"

namespace hope::detail {

    template<std::size_t I>
    using Int = std::integral_constant<std::size_t, I>;

    namespace generated {
'''

generated = open('generated.h', 'w')
generated.write(header)

constantBegin = '''
        template <typename T, typename ValuePolicy = field_policy::value>
        constexpr auto tuple_from_struct(T&& object, Int<'''

payload = str("")

for i in range (1, 100):
    generated.write(constantBegin)
    generated.write(str(i))
    generated.write(">, ValuePolicy value_policy = ValuePolicy{}) {\n")
    generated.write("\t\t\tauto&& [")

    argList = ""
    for j in range (0, i):
        if j != 0: 
            argList += ", "

        argList += "_"
        argList += str(j)

    generated.write(argList)
    generated.write("] = object;\n")
    generated.write("\t\t\tif constexpr (std::is_same_v<ValuePolicy, field_policy::value>)\n")
    generated.write("\t\t\t\treturn make_flat_tuple(")
    generated.write(argList)
    generated.write(");\n")
    generated.write("\t\t\telse\n")
    generated.write("\t\t\t\treturn make_flat_ref_tuple(")
    generated.write(argList)
    generated.write(");\n")
    generated.write("\t\t}\n")
    
generated.write("\t}\n}")
generated.close()
