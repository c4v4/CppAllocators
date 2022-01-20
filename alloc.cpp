#include <fmt/core.h>

#include "stack_allocator.hpp"
#include "stack_allocator_with_backup.hpp"
#include <array>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <memory_resource>

//#define INLINE __attribute__((noinline))
#define INLINE inline

#define RAND_SIZE 128
int *rand_vals;
int rand_index;
static inline int rand_val()
{
    return rand_vals[++rand_index % RAND_SIZE];
}

int size_glob;
std::byte *buffer_glob;

template <typename T>
class list_node_expose : private std::list<T>
{
public:
    using Node = typename std::list<T>::_Node;
    constexpr static std::size_t size = sizeof(Node);
};

class default_std_alloc
{
public:
    int INLINE operator()(int total_nodes)
    {
        std::list<int> list;
        for (int i = 0; i != total_nodes; ++i)
        {
            list.push_back(rand_val() & 15);
        }

        for (int val : list)
            if ((rand_val() & 127) == 0)
                return val;
        return list.back();
    };
};
class default_pmr_alloc
{
public:
    int INLINE operator()(int total_nodes)
    {
        std::pmr::list<int> list;
        for (int i = 0; i != total_nodes; ++i)
        {
            list.push_back(rand_val() & 15);
        }

        for (int val : list)
            if ((rand_val() & 127) == 0)
                return val;
        return list.back();
    };
};
class pmr_alloc_no_buf
{
public:
    int INLINE operator()(int total_nodes)
    {
        std::pmr::monotonic_buffer_resource mbr;
        std::pmr::polymorphic_allocator<int> pa{&mbr};
        std::pmr::list<int> list{pa};
        for (int i = 0; i != total_nodes; ++i)
        {
            list.push_back(rand_val() & 15);
        }

        for (int val : list)
            if ((rand_val() & 127) == 0)
                return val;
        return list.back();
    };
};
class pmr_alloc_and_buf
{
public:
    int INLINE operator()(int total_nodes)
    {
        std::size_t size = total_nodes * list_node_expose<int>::size;
        int *buffer = (int *)alloca(size);
        std::pmr::monotonic_buffer_resource mbr{buffer, size / sizeof(int)};
        std::pmr::polymorphic_allocator<int> pa{&mbr};
        std::pmr::list<int> list{pa};
        for (int i = 0; i != total_nodes; ++i)
        {
            list.push_back(rand_val() & 15);
        }

        for (int val : list)
            if ((rand_val() & 127) == 0)
                return val;
        return list.back();
    };
};
class custom_stack_allocator
{
public:
    int INLINE operator()(int total_nodes)
    {
        std::size_t size = total_nodes * list_node_expose<int>::size;
        std::byte *buffer = (std::byte *)alloca(size);
        std::list<int, stack_allocator<int>> list(
            stack_allocator<int>(buffer, size));
        for (int i = 0; i != total_nodes; ++i)
        {
            list.push_back(rand_val() & 15);
        }

        for (int val : list)
            if ((rand_val() & 127) == 0)
                return val;
        return list.back();
    }
};

class custom_stack_alloc_glob
{
public:
    int INLINE operator()(int total_nodes)
    {
        std::list<int, stack_allocator<int>> list(
            stack_allocator<int>(buffer_glob, size_glob));

        for (int i = 0; i != total_nodes; ++i)
        {
            list.push_back(rand_val() & 15);
        }

        for (int val : list)
            if ((rand_val() & 127) == 0)
                return val;
        return list.back();
    }
};

template <typename T>
struct identity
{
    T operator()(T val) { return val; }
};

template <typename ChooseSize = identity<int>>
class default_std_alloc_vec_reserved
{
public:
    int INLINE operator()(int nodes)
    {
        const auto total_nodes = ChooseSize()(nodes);
        std::vector<int> vec;
        vec.reserve(nodes);
        for (int i = 0; i != total_nodes; ++i)
        {
            vec.push_back(rand_val() & 15);
        }

        int sum = 0;
        for (int i = 0; i < 10; ++i)
            sum += vec[rand_val() % vec.size()];
        return sum;
    };
};

template <typename ChooseSize = identity<int>>
class default_std_alloc_vec
{
public:
    int INLINE operator()(int nodes)
    {
        const auto total_nodes = ChooseSize()(nodes);
        std::vector<int> vec;
        for (int i = 0; i != total_nodes; ++i)
        {
            vec.push_back(rand_val() & 15);
        }

        int sum = 0;
        for (int i = 0; i < 10; ++i)
            sum += vec[rand_val() % vec.size()];
        return sum;
    };
};
template <typename ChooseSize = identity<int>>
class default_pmr_alloc_vec
{
public:
    int INLINE operator()(int nodes)
    {
        const auto total_nodes = ChooseSize()(nodes);
        std::pmr::vector<int> vec;
        for (int i = 0; i != total_nodes; ++i)
        {
            vec.push_back(rand_val() & 15);
        }

        int sum = 0;
        for (int i = 0; i < 10; ++i)
            sum += vec[rand_val() % vec.size()];
        return sum;
    };
};
template <typename ChooseSize = identity<int>>
class pmr_alloc_no_buf_vec
{
public:
    int INLINE operator()(int nodes)
    {
        const auto total_nodes = ChooseSize()(nodes);
        std::pmr::monotonic_buffer_resource mbr;
        std::pmr::polymorphic_allocator<int> pa{&mbr};
        std::pmr::vector<int> vec{pa};
        for (int i = 0; i != total_nodes; ++i)
        {
            vec.push_back(rand_val() & 15);
        }

        int sum = 0;
        for (int i = 0; i < 10; ++i)
            sum += vec[rand_val() % vec.size()];
        return sum;
    };
};
template <typename ChooseSize = identity<int>>
class pmr_alloc_and_buf_vec
{
public:
    int INLINE operator()(int nodes)
    {
        const auto total_nodes = ChooseSize()(nodes);
        std::size_t size = nodes * sizeof(int) * 2; // consider vector growth factor
        std::byte *buffer = (std::byte *)alloca(size);
        std::pmr::monotonic_buffer_resource mbr{buffer, size};
        std::pmr::polymorphic_allocator<int> pa{&mbr};
        std::pmr::vector<int> vec{pa};
        vec.reserve(nodes);
        for (int i = 0; i != total_nodes; ++i)
        {
            vec.push_back(rand_val() & 15);
        }

        int sum = 0;
        for (int i = 0; i < 10; ++i)
            sum += vec[rand_val() % vec.size()];
        return sum;
    };
};
class custom_stack_alloc_vec
{
public:
    int INLINE operator()(int total_nodes)
    {
        std::size_t size = total_nodes * sizeof(int) * 2; // consider vector growth factor
        std::byte *buffer = (std::byte *)alloca(size);
        std::vector<int, stack_allocator<int>> vec(stack_allocator<int>(buffer, size));

        vec.reserve(total_nodes);
        for (int i = 0; i != total_nodes; ++i)
        {
            vec.push_back(rand_val() & 15);
        }

        int sum = 0;
        for (int i = 0; i < 10; ++i)
            sum += vec[rand_val() % vec.size()];
        return sum;
    }
};

class custom_stack_alloc_vec_glob
{
public:
    int INLINE operator()(int total_nodes)
    {
        std::vector<int, stack_allocator<int>> vec(stack_allocator<int>(buffer_glob, size_glob));
        vec.reserve(total_nodes);
        for (int i = 0; i != total_nodes; ++i)
        {
            vec.push_back(rand_val() & 15);
        }

        int sum = 0;
        for (int i = 0; i < 10; ++i)
            sum += vec[rand_val() % vec.size()];
        return sum;
    }
};

template <typename ChooseSize = identity<int>>
class custom_stack_alloc_vec_backup
{
public:
    int INLINE operator()(int nodes)
    {
        const auto total_nodes = ChooseSize()(nodes);
        std::size_t size = nodes * sizeof(int) * 2; // consider vector growth factor
        std::byte *buffer = (std::byte *)alloca(size);
        std::vector<int, stack_allocator_with_backup<int>> vec(stack_allocator_with_backup<int>(buffer, size));
        vec.reserve(nodes);
        for (int i = 0; i != total_nodes; ++i)
        {
            vec.push_back(rand_val() & 15);
        }

        int sum = 0;
        for (int i = 0; i < 10; ++i)
            sum += vec[rand_val() % vec.size()];
        return sum;
    };
};

template <typename Func>
auto INLINE benchmark(int iterations, int total_nodes)
{
    const auto start = std::chrono::system_clock::now();
    while (iterations-- > 0)
    {
        volatile int use_val = Func()(total_nodes);
    }
    const auto stop = std::chrono::system_clock::now();
    const auto secs = std::chrono::duration<double>(stop - start);
    return secs.count();
}

void list_test()
{
    int iterations{1000};
    int total_nodes{250'000};

    const double warm = benchmark<default_std_alloc>(iterations, total_nodes) +
                        benchmark<custom_stack_allocator>(iterations, total_nodes);
    const double t1 = benchmark<default_std_alloc>(iterations, total_nodes);
    const double t2 = benchmark<default_pmr_alloc>(iterations, total_nodes);
    const double t3 = benchmark<pmr_alloc_no_buf>(iterations, total_nodes);
    const double t4 = benchmark<pmr_alloc_and_buf>(iterations, total_nodes);
    const double t5 = benchmark<custom_stack_allocator>(iterations, total_nodes);
    size_glob = total_nodes * list_node_expose<int>::size;
    buffer_glob = (std::byte *)alloca(size_glob);
    const double t6 = benchmark<custom_stack_alloc_glob>(iterations, total_nodes);

    fmt::print("Results of {} iterations for std::list<int> of size {}\n ", iterations, total_nodes);
    fmt::print("w  (warming step)                        : {:.3f} sec\n ", warm);
    fmt::print("t1 (default std alloc no reserve)        : {:.3f} sec; t1/t1: {:.3f} \n ", t1, t1 / t1);
    fmt::print("t2 (default pmr alloc no reserve)        : {:.3f} sec; t1/t2: {:.3f} \n ", t2, t1 / t2);
    fmt::print("t3 (pmr alloc no buffer no reserve)      : {:.3f} sec; t1/t3: {:.3f} \n ", t3, t1 / t3);
    fmt::print("t4 (pmr alloc with buffer with reserve)  : {:.3f} sec; t1/t4: {:.3f} \n ", t4, t1 / t4);
    fmt::print("t5 (custom stack alloc with reserve)     : {:.3f} sec; t1/t5: {:.3f} \n ", t5, t1 / t5);
    fmt::print("t6 (custom stackalloc allctd once, resvd): {:.3f} sec; t1/t5: {:.3f} \n\n", t6, t1 / t6);
}

void INLINE vector_test(int total_nodes)
{
    int iterations = (1 << 30) / total_nodes;

    const double warm = benchmark<default_std_alloc_vec<>>(iterations, total_nodes) +
                        benchmark<custom_stack_alloc_vec>(iterations, total_nodes);
    const double t0 = benchmark<default_std_alloc_vec_reserved<>>(iterations, total_nodes);
    const double t1 = benchmark<default_std_alloc_vec<>>(iterations, total_nodes);
    const double t2 = benchmark<default_pmr_alloc_vec<>>(iterations, total_nodes);
    const double t3 = benchmark<pmr_alloc_no_buf_vec<>>(iterations, total_nodes);
    const double t4 = benchmark<pmr_alloc_and_buf_vec<>>(iterations, total_nodes);
    const double t5 = benchmark<custom_stack_alloc_vec>(iterations, total_nodes);
    size_glob = total_nodes * sizeof(int) * 2;
    buffer_glob = (std::byte *)alloca(size_glob);
    const double t6 = benchmark<custom_stack_alloc_vec_glob>(iterations, total_nodes);

    fmt::print(
        "Results of {} iterations for std::vector<int> of size {}\n ", iterations, total_nodes);
    fmt::print("w  (warming step)                        : {:.3f} sec\n ", warm);
    fmt::print("t0 (default std alloc with reserve)      : {:.3f} sec; t1/t0: {:.3f} \n ", t0, t1 / t0);
    fmt::print("t1 (default std alloc no reserve)        : {:.3f} sec; t1/t1: {:.3f} \n ", t1, t1 / t1);
    fmt::print("t2 (default pmr alloc no reserve)        : {:.3f} sec; t1/t2: {:.3f} \n ", t2, t1 / t2);
    fmt::print("t3 (pmr alloc no buffer no reserve)      : {:.3f} sec; t1/t3: {:.3f} \n ", t3, t1 / t3);
    fmt::print("t4 (pmr alloc with buffer with reserve)  : {:.3f} sec; t1/t4: {:.3f} \n ", t4, t1 / t4);
    fmt::print("t5 (custom stackalloc with reserve)      : {:.3f} sec; t1/t5: {:.3f} \n ", t5, t1 / t5);
    fmt::print("t6 (custom stackalloc allctd once, resvd): {:.3f} sec; t1/t5: {:.3f} \n\n", t6, t1 / t6);
}

struct RandSize
{
    unsigned operator()(unsigned val) { return 1 + rand() % (val * 2); }
};

void INLINE
unknown_size_vector_test(int max_nodes)
{
    int iterations = 1000000;

    const double warm = benchmark<default_std_alloc_vec<RandSize>>(iterations, max_nodes) +
                        benchmark<custom_stack_alloc_vec>(iterations, max_nodes);
    const double t1 = benchmark<default_std_alloc_vec<RandSize>>(iterations, max_nodes);
    const double t2 = benchmark<default_pmr_alloc_vec<RandSize>>(iterations, max_nodes);
    const double t3 = benchmark<pmr_alloc_no_buf_vec<RandSize>>(iterations, max_nodes);
    const double t4 = benchmark<pmr_alloc_and_buf_vec<RandSize>>(iterations, max_nodes);
    const double t7 = benchmark<custom_stack_alloc_vec_backup<RandSize>>(iterations, max_nodes);

    fmt::print(
        "Results of {} iterations for std::vector<int> of max size {}\n ", iterations, max_nodes * 2);
    fmt::print("w  (warming step)                    : {:.3f} sec\n ", warm);
    fmt::print("t1 (default std alloc no reserve)    : {:.3f} sec; t1/t1: {:.3f} \n ", t1, t1 / t1);
    fmt::print("t2 (default pmr alloc no reserve)    : {:.3f} sec; t1/t2: {:.3f} \n ", t2, t1 / t2);
    fmt::print("t3 (pmr alloc no buffer no reserve)  : {:.3f} sec; t1/t3: {:.3f} \n ", t3, t1 / t3);
    fmt::print("t4 (pmr alloc with buffer no reserve): {:.3f} sec; t1/t4: {:.3f} \n ", t4, t1 / t4);
    fmt::print("t7 (custom stackalloc with backup)   : {:.3f} sec; t1/t5: {:.3f} \n\n", t7, t1 / t7);
}

int main()
{
    int local_arr[RAND_SIZE];
    for (int &val : local_arr)
        val = rand();
    rand_vals = local_arr;

    list_test();
    for (int i = 4; i < (1 << 22); i <<= 2)
        vector_test(i);

    unknown_size_vector_test(128);

    return 0;
}