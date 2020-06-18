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
// TypeList like tuple
// Copyright (c) 2020 glensand
// All rights reserved.
//
// Date: 17.06.2020
// Author: Bezborodoff Gleb
//------------------------------------------------------------------------------
#pragma once

#include "tuple/FlatTuple.h"
#include "tuple/DetectFieldsCount.h"
namespace Detail
{

    template<std::size_t I>
    using Int = std::integral_constant<std::size_t, I>;

    namespace Generated
    {\n
'''

generated = open('generated.h', 'w')
generated.write(header)

constantBegin = '''
        template <typename T>
        constexpr auto TupleFromStruct(const T& object, Int<'''

payload = str("")

for i in range (1, 100):
    generated.write(constantBegin)
    generated.write(str(i))
    generated.write(">)\n\t\t{\n")
    generated.write("\t\t\tauto& [")

    argList = ""
    for j in range (0, i):
        if j != 0: 
            argList += ", "

        argList += "_"
        argList += str(j)

    generated.write(argList)
    generated.write("] = object;\n")
    generated.write("\t\t\treturn MakeFlatTuple(")
    generated.write(argList)
    generated.write(");\n\t\t}\n")

constantEnd = '''
        }
   }

template <typename T>
constexpr auto TupleFromStruct(const T& object)
{
    constexpr auto fieldsCount = DetectFieldsCount(T{});
    return Detail::Generated::TupleFromStruct(object, Detail::Int<fieldsCount>{});
}
'''

generated.write(constantEnd)
generated.close()
