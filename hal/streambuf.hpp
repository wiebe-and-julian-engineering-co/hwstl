//    Copyright Julian van Doorn and Wiebe van Breukelen.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

//#include "../target/target.hpp"

#include <cstddef>

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
        virtual void add(BufferValue val) = 0;
        //virtual BufferValue get(unsigned int index) = 0;
        virtual size_t size() = 0;
    };

    template <size_t t_buffer_len>
    class streambuf_backend : public streambuf {
    public:
        using BufferValue = uint8_t;
        volatile unsigned int head = 0, tail = 0;

    private:
        BufferValue m_buffer[t_buffer_len];

    public:
        streambuf_backend() { }

        BufferValue* Buffer() override {
            return m_buffer;
        }

        void add(BufferValue val) override {
            unsigned int i = (uint32_t)(head + 1) % t_buffer_len;

            if (i != tail) {
                m_buffer[head] = val;
                head = i;
            }
        }

        //BufferValue get(unsigned int index) override {
        //    return m_buffer[index];  
        //}

        constexpr size_t size() override {
            return t_buffer_len;
        }
    };

    template <size_t t_buffer_len>
    constexpr auto make_streambuf() {
        return streambuf_backend<t_buffer_len>();
    }
} // namespace hwstl