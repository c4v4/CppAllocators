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
    explicit stack_allocator()
        : m_begin(nullptr), m_end(nullptr), m_stack_pointer(nullptr) {}

    explicit stack_allocator(void *buffer, std::size_t buffer_size)
        : m_begin((T *)buffer),
          m_end((T *)buffer + buffer_size / sizeof(T)),
          m_stack_pointer((T *)buffer) {}

    template <class U>
    stack_allocator(const stack_allocator<U> &other)
        : m_begin(reinterpret_cast<pointer>(other.m_begin)),
          m_end(reinterpret_cast<pointer>(other.m_end)),
          m_stack_pointer(reinterpret_cast<pointer>(other.m_stack_pointer)) {}

    constexpr size_type capacity() { return m_end - m_begin; }

    pointer allocate(size_type n, const_pointer hint = const_pointer())
    {
        return std::exchange(m_stack_pointer, m_stack_pointer + n);
    }

    void deallocate(pointer p, size_type n) {}

    size_type max_size() const noexcept { return m_end - m_stack_pointer; }

    template <class U, class... Args>
    void construct(U *p, Args &&...args)
    {
        new ((void *)p) U(std::forward<Args>(args)...);
    }

    template <class U>
    void destroy(U *p)
    {
        p->~U();
    }

    pointer address(reference x) const noexcept { return std::addressof(x); }

    const_pointer address(const_reference x) const noexcept
    {
        return std::addressof(x);
    }

    template <class U>
    struct rebind
    {
        typedef stack_allocator<U> other;
    };

    pointer buffer() const noexcept { return m_begin; }

public:
    bool pointer_to_internal_buffer(const_pointer p) const
    {
        return (!(std::less<const_pointer>()(p, m_begin)) &&
                (std::less<const_pointer>()(p, m_end)));
    }

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