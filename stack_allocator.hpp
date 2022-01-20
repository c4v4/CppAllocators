#ifndef _STACK_ALLOCATOR_HPP_
#define _STACK_ALLOCATOR_HPP_

#include <cstddef>
#include <utility>

template <class T>
class stack_allocator
{
public:
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;

public:
    explicit stack_allocator() noexcept
        : m_begin(nullptr), m_end(nullptr), m_stack_pointer(nullptr) {}

    stack_allocator(void *buffer, std::size_t buffer_size) noexcept
        : m_begin((T *)buffer),
          m_end((T *)buffer + buffer_size / sizeof(T)),
          m_stack_pointer((T *)buffer) {}

    template <class U>
    stack_allocator(const stack_allocator<U> &other) noexcept
        : m_begin(reinterpret_cast<T *>(other.m_begin)),
          m_end(reinterpret_cast<T *>(other.m_end)),
          m_stack_pointer(reinterpret_cast<T *>(other.m_stack_pointer)) {}

    constexpr size_type capacity() noexcept { return m_end - m_begin; }

    inline pointer allocate(size_type n, const_pointer hint = const_pointer()) noexcept
    {
        pointer other = m_stack_pointer;
        m_stack_pointer += n;
        return other;
    }

    inline void deallocate(pointer p, size_type n) const noexcept {}

    inline size_type max_size() const noexcept { return m_end - m_stack_pointer; }

    template <class U, class... Args>
    inline void construct(U *p, Args &&...args)
    {
        if constexpr (!std::is_fundamental<U>::value)
        {
            new ((void *)p) U(std::forward<Args>(args)...);
        }
        else
        {
            *p = U(std::forward<Args>(args)...);
        }
    }

    template <class U>
    inline void destroy(U *p)
    {
        if constexpr (!std::is_fundamental<U>::value)
        {
            p->~U();
        }
    }

    inline pointer address(reference x) const noexcept { return std::addressof(x); }

    inline const_pointer address(const_reference x) const noexcept
    {
        return std::addressof(x);
    }

    template <class U>
    struct rebind
    {
        typedef stack_allocator<U> other;
    };

    inline pointer buffer() const noexcept { return m_begin; }

public:
    pointer m_begin;
    pointer m_end;
    pointer m_stack_pointer;
};

template <class T1, std::size_t N, class Allocator, class T2>
bool operator==(const stack_allocator<T1> &lhs,
                const stack_allocator<T2> &rhs) noexcept
{
    return lhs.buffer() == rhs.buffer();
}

template <class T1, class Allocator, class T2>
bool operator!=(const stack_allocator<T1> &lhs,
                const stack_allocator<T2> &rhs) noexcept
{
    return !(lhs == rhs);
}

#endif