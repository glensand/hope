# Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
# You may use, distribute and modify this code under the
# terms of the MIT license.
# 
# You should have received a copy of the MIT license with
# this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope

import random

HEADER = '''
/* Copyright (C) 2020 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */
 
#pragma once

#include "typelist/type_list.h"

'''

ALIGNMENT = 16
MAX_SM_OBJECT_SIZE = 64
FILENAME = "objects_aligned_to" + str(ALIGNMENT) + ".h"
ALIGNED_STORAGE_BEGIN = 'std::aligned_storage_t<'
ALIGNED_STORAGE_END = ", " + str(ALIGNMENT) + ">, \n"

def write_types(types_header, indeces, stream):
    generated.write(types_header)
    for i in indeces:
        if i % ALIGNMENT == 0:
            generated.write(ALIGNED_STORAGE_BEGIN)
            generated.write(str(i))
            generated.write(ALIGNED_STORAGE_END)
    generated.write('>;\n\n')

generated = open(FILENAME, 'w')
generated.write(HEADER)

ascending_types = "using RegisteredTypesAscending" + "AlignedTo" + str(ALIGNMENT) + " = hope::type_list\n<\n"
ascending_indeces = [i for i in range(1, MAX_SM_OBJECT_SIZE + 1)]
write_types(ascending_types, ascending_indeces, generated)


descending_types = "using RegisteredTypesDescending" + "AlignedTo" + str(ALIGNMENT) + " = hope::type_list\n<\n"
descending_indeces = [MAX_SM_OBJECT_SIZE + 1 - i for i in range(1, MAX_SM_OBJECT_SIZE + 1)]
write_types(descending_types, descending_indeces, generated)


shuffled = [i for i in range (1, MAX_SM_OBJECT_SIZE + 1)]
random.shuffle(shuffled)
shuffled_types = "using RegisteredTypesShuffled" + "AlignedTo" + str(ALIGNMENT) + " = hope::type_list\n<\n"
write_types(shuffled_types, shuffled, generated)

generated.close()
