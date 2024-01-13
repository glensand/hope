/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/logger/logger.h"
#include "hope/logger/stack_buffer.h"
#include "hope/logger/log_helper.h"

#include "string_stream.h"

#include <gtest/gtest.h>

namespace hope::log::testing{

    class logger_test : public ::testing::Test {
    protected:

        void SetUp() override {
            m_stream = new string_stream;
            m_logger = new hope::logger;
            m_logger->enable(*m_stream, hope::log_level::debug);
        }

        void TearDown() override {
            delete m_logger;
            delete m_stream;
        }

        hope::log_helper LOG(hope::log_priority priority){
            return hope::log_helper(*m_logger, priority);
        }

    private:
        string_stream* m_stream;
        hope::logger* m_logger;
    };

}
