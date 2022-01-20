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

class default_std_alloc_vec
{
public:
    int INLINE operator()(int total_nodes)
    {
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
class default_pmr_alloc_vec
{
public:
    int INLINE operator()(int total_nodes)
    {
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
class pmr_alloc_no_buf_vec
{
public:
    int INLINE operator()(int total_nodes)
    {
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
class pmr_alloc_and_buf_vec
{
public:
    int INLINE operator()(int total_nodes)
    {
        std::size_t size = total_nodes * sizeof(int) * 2; //consider vector growth factor
        std::byte *buffer = (std::byte *)alloca(size);
        std::pmr::monotonic_buffer_resource mbr{buffer, size};
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
class custom_stack_alloc_vec
{
public:
    int INLINE operator()(int total_nodes)
    {
        std::size_t size = total_nodes * sizeof(int) * 2; //consider vector growth factor
        std::byte *buffer = (std::byte *)alloca(size);
        std::vector<int, stack_allocator<int>> vec(
            stack_allocator<int>(buffer, size));

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

    const double t1 = benchmark<default_std_alloc>(iterations, total_nodes);
    const double t2 = benchmark<default_pmr_alloc>(iterations, total_nodes);
    const double t3 = benchmark<pmr_alloc_no_buf>(iterations, total_nodes);
    const double t4 = benchmark<pmr_alloc_and_buf>(iterations, total_nodes);
    const double t5 = benchmark<custom_stack_allocator>(iterations, total_nodes);

    fmt::print(
        "Results of {} iterations for std::list<int> of size {}\n "
        "t1 (default std alloc)    : {:.3f} sec; t1/t1: {:.3f} \n "
        "t2 (default pmr alloc)    : {:.3f} sec; t1/t2: {:.3f} \n "
        "t3 (pmr alloc no buffer)  : {:.3f} sec; t1/t3: {:.3f} \n "
        "t4 (pmr alloc with buffer): {:.3f} sec; t1/t4: {:.3f} \n "
        "t5 (custom stack alloc)   : {:.3f} sec; t1/t5: {:.3f} \n\n",
        iterations, total_nodes, t1, t1 / t1, t2, t1 / t2, t3, t1 / t3, t4,
        t1 / t4, t5, t1 / t5);
}

void INLINE vector_test(int total_nodes)
{
    int iterations = (1 << 28) / total_nodes;

    const double t1 = benchmark<default_std_alloc_vec>(iterations, total_nodes);
    const double t2 = benchmark<default_pmr_alloc_vec>(iterations, total_nodes);
    const double t3 = benchmark<pmr_alloc_no_buf_vec>(iterations, total_nodes);
    const double t4 = benchmark<pmr_alloc_and_buf_vec>(iterations, total_nodes);
    const double t5 = benchmark<custom_stack_alloc_vec>(iterations, total_nodes);

    fmt::print(
        "Results of {} iterations for std::vector<int> of size {}\n "
        "t1 (default std alloc)    : {:.3f} sec; t1/t1: {:.3f} \n "
        "t2 (default pmr alloc)    : {:.3f} sec; t1/t2: {:.3f} \n "
        "t3 (pmr alloc no buffer)  : {:.3f} sec; t1/t3: {:.3f} \n "
        "t4 (pmr alloc with buffer): {:.3f} sec; t1/t4: {:.3f} \n "
        "t5 (custom stack alloc)   : {:.3f} sec; t1/t5: {:.3f} \n\n",
        iterations, total_nodes, t1, t1 / t1, t2, t1 / t2, t3, t1 / t3, t4,
        t1 / t4, t5, t1 / t5);
}

int main()
{
    int local_arr[RAND_SIZE];
    for (int &val : local_arr)
        val = rand();
    rand_vals = local_arr;

    list_test();
    for (int i = 4; i < (1 << 18); i <<= 2)
        vector_test(i);

    return 0;
}