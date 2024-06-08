#ifndef SHARED_VECTOR_H
#define SHARED_VECTOR_H

#include "types.h"

namespace Util {
    template <typename Tp, uint32_t alignment = sizeof(unsigned long)>
    class Vector {
        static constexpr const uint32_t DEFAULT_CAPACITY = 64;
        
    public:
        Vector() = delete;
        Vector(const uint32_t capacity);
        
        ~Vector();
        
        void Create(const uint32_t capacity = DEFAULT_CAPACITY);
        void Destroy();
        
        void Clean();
        void Resize(const uint32_t new_capacity);
        void Append(const Tp& data);
        
        const T& AtIndex(const uint32_t index);
        const T& operator[](const uint32_t index);
        
        T* Data() const { return m_data; }
        uint32_t Size() const { return m_size; }
        
        // C++11 iterator/range-for loop utils
        T* begin() { return m_data; }
        T* end()   { return m_data + m_size; }
        
    private:
        Tp* m_data;
        uint32_t m_capacity;
        uint32_t m_size;
        uint32_t m_alignment;
    };
} // namespace Util    