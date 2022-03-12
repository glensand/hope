/* Copyright (C) 2021 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

namespace hope {

    /**
     * Represents level of log writing
     */
    enum class log_level : unsigned {
        /**
         * Only messages marked as error writes to the stream
         */
        minimal,

        /**
         * Write errors and display callie functions name for the LOG_TRACE calls
         */
        debug,
    };

    /**
     * Message priority, controls visibility of the message on each log_level
     */
    enum class log_priority : unsigned{
        /**
         * Just information
         */
        info,

        /**
         * The application has encountered an error. This message will be visible at all logging levels.
         */
        error,
    };

}
