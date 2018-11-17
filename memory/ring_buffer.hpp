/*
 * ring_buffer.h
 *
 * Created: 17-11-2018 12:43:08
 *  Author: Wiebe
 */ 


#pragma once

#include <array>
#include <iterator>

namespace hwstl {
    template <class BUFFER_TYPE, size_t SIZE>
    class ring_buffer {
    private:
        std::array<BUFFER_TYPE, SIZE> buffer;

        volatile size_t head_index, tail_index;

        const uint32_t mask = SIZE - 1;

    public:
        typedef uint32_t ring_buffer_size_t;

        class iterator : public std::iterator<
            std::input_iterator_tag,
            BUFFER_TYPE,
            BUFFER_TYPE,
            BUFFER_TYPE*,
            BUFFER_TYPE>
        {
            size_t index;
            ring_buffer<BUFFER_TYPE, SIZE> &ring_buffer_;

        public:
            explicit iterator(ring_buffer<BUFFER_TYPE, SIZE> &_ring_buffer, size_t offset = 0) :
                ring_buffer_(_ring_buffer),
                index(offset)
            {};
        
            iterator& operator++() {
                return increase();
            }

            iterator operator++(int) {
                iterator retval = *this;
                ++(*this);
                return retval;
            }

            iterator& operator--() {
                return decrease();
            }

            iterator operator--(int) {
                iterator retval = *this;
                --(*this);
                return retval;
            }

            bool operator==(iterator rhs) const {
                return index == rhs.index;
            }

            bool operator!=(iterator rhs) const {
                //std::cout << index << " -> " << rhs.index;
                return index != rhs.index;
            }

            BUFFER_TYPE& operator*() const { return ring_buffer_.buffer[index]; }
            BUFFER_TYPE* operator->() { return (ring_buffer_.buffer.data + index); }
       private:
            iterator& increase() {
            
                if (index + 1 > ring_buffer_.head()) {  // Are we at the head?
                    // Roll back the index to the tail
                    index = ring_buffer_.tail();

                    // Call myself again
                    } else {
                    index++;
                }

                return *this;
            }

            iterator& decrease() {
                if (index - 1 < ring_buffer_.tail()) {  // Are we at the head?
                    // Roll back the index to the tail
                    index = ring_buffer_.head();
                    } else {
                    index--;
                }

                return *this;
            }


        };

        explicit ring_buffer(std::array<BUFFER_TYPE, SIZE> _buffer) :
            buffer(_buffer),
            head_index(0),
            tail_index(0)
        {
            static_assert(((SIZE & (SIZE - 1)) == 0), "Ring buffer size must be power of two");
        }

        explicit ring_buffer() :
            buffer(std::array<BUFFER_TYPE, SIZE>()),
            head_index(0),
            tail_index(0)
        {
           static_assert(((SIZE & (SIZE - 1)) == 0), "Ring buffer size must be power of two");
        }

        void enqueue(const BUFFER_TYPE data) {
            if (is_full()) {
                // Going to overwrite mode.
                tail_index = ((tail_index + 1) & mask);
            }

            buffer[head_index] = data;
            head_index = ((head_index + 1) & mask);
        }

        BUFFER_TYPE dequeue() {
            BUFFER_TYPE data = 0;

            dequeue(&data);

            return data;
        
        }

        bool dequeue(BUFFER_TYPE &data) {
            BUFFER_TYPE data_cp;

            if (dequeue(&data_cp)) {
                data = data_cp;
                return true;
            }

            return false;
        }

        bool dequeue(BUFFER_TYPE *data) {
            if (is_empty()) {
                return false;
            }

            *data = buffer[tail_index];
            tail_index = ((tail_index + 1) & mask);
            return true;
        }

        size_t dequeue(BUFFER_TYPE *data, size_t len) {
            if (is_empty()) {
                return 0;
            }


            BUFFER_TYPE *data_ptr = data;

            size_t counter = 0;

            while ((counter < len) && dequeue(data_ptr)) {
                data_ptr++;
                counter++;
            }

            return counter;
        }

        iterator begin() {
            return iterator(*this, tail_index); // head_index
        }

        iterator end() {
            return iterator(*this, head_index);
        }

        inline bool is_empty() const {
            return (head_index == tail_index);
        }

        inline bool is_full() const {
            return ((head_index - tail_index) & mask) == mask;
        }

        inline ring_buffer_size_t size() const {
            return ((head_index - tail_index) & mask);
        }

        constexpr inline ring_buffer_size_t max_size() const {
            return SIZE;
        }

        inline ring_buffer_size_t tail() const {
            return tail_index;
        }

        inline ring_buffer_size_t head() const {
            return head_index;
        }
    };
} // namespace hwstl