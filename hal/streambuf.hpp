#pragma once

#include "../target/target.hpp"

namespace hwstl {
    template <size_t t_buffer_len>
    class buffer_size {
    public:
        size_t size() {
            return t_buffer_len;
        }
    };

    class streambuf {
    public:
        using BufferValue = uint8_t;

        streambuf() { }

        virtual BufferValue* Buffer() = 0;
    };

    template <size_t t_buffer_len>
    class streambuf_backend : public streambuf {
    public:
        using BufferValue = uint8_t;

    private:
        BufferValue m_buffer[t_buffer_len];

    public:
        streambuf_backend() { }

        BufferValue* Buffer() override {
            return m_buffer;
        }
    };

    template <size_t t_buffer_len>
    constexpr auto make_streambuf() {
        return streambuf_backend<t_buffer_len>();
    }
} // namespace hwstl