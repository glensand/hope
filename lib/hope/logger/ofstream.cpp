#include "ofstream.h"

namespace hope {

    ofstream::ofstream(std::string_view file_name) {
        // assumed binary append mode is best choise for defaul stream
        m_stream_impl.open(file_name.data(), std::ios::binary | std::ios::app);
    }

    ofstream::~ofstream() {
        m_stream_impl.close();
    }

    void ofstream::write(const void *data, std::size_t size) {
        m_stream_impl.write((const char*)data, size);
    }

    void ofstream::flush() {
        m_stream_impl.flush();
    }

    bool ofstream::is_open() const noexcept {
        return m_stream_impl.is_open();
    }
    
}