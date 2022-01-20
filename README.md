# CppAllocators
C++ benchmark for default, pmr and custom local allocator 

## DISCLAIMER
These test are fast and dirty, only to see if there's any potential benefit in what I usually do coding. Do not exepect code of any quality.

## Sample results on Ryzen 9 5959X

        Results of 1000 iterations for std::list<int> of size 250000
        w  (warming step     )    : 4.258 sec
        t1 (default std alloc)    : 3.354 sec; t1/t1: 1.000 
        t2 (default pmr alloc)    : 3.910 sec; t1/t2: 0.858 
        t3 (pmr alloc no buffer)  : 2.079 sec; t1/t3: 1.613 
        t4 (pmr alloc with buffer): 1.530 sec; t1/t4: 2.191 
        t5 (custom stack alloc)   : 0.579 sec; t1/t5: 5.797 

        Results of 268435456 iterations for std::vector<int> of size 4
        w  (warming step     )    : 17.312 sec
        t0 (default std alloc res): 6.167 sec; t1/t0: 1.907 
        t1 (default std alloc)    : 11.758 sec; t1/t1: 1.000 
        t2 (default pmr alloc)    : 16.475 sec; t1/t2: 0.714 
        t3 (pmr alloc no buffer)  : 16.694 sec; t1/t3: 0.704 
        t4 (pmr alloc with buffer): 11.116 sec; t1/t4: 1.058 
        t5 (custom stack alloc)   : 5.662 sec; t1/t5: 2.077 

        Results of 67108864 iterations for std::vector<int> of size 16
        w  (warming step     )    : 7.367 sec
        t0 (default std alloc res): 2.144 sec; t1/t0: 2.315 
        t1 (default std alloc)    : 4.963 sec; t1/t1: 1.000 
        t2 (default pmr alloc)    : 7.323 sec; t1/t2: 0.678 
        t3 (pmr alloc no buffer)  : 6.472 sec; t1/t3: 0.767 
        t4 (pmr alloc with buffer): 4.732 sec; t1/t4: 1.049 
        t5 (custom stack alloc)   : 2.594 sec; t1/t5: 1.914 

        Results of 16777216 iterations for std::vector<int> of size 64
        w  (warming step     )    : 3.533 sec
        t0 (default std alloc res): 1.225 sec; t1/t0: 1.861 
        t1 (default std alloc)    : 2.280 sec; t1/t1: 1.000 
        t2 (default pmr alloc)    : 3.233 sec; t1/t2: 0.705 
        t3 (pmr alloc no buffer)  : 2.897 sec; t1/t3: 0.787 
        t4 (pmr alloc with buffer): 2.474 sec; t1/t4: 0.921 
        t5 (custom stack alloc)   : 1.349 sec; t1/t5: 1.690 

        Results of 4194304 iterations for std::vector<int> of size 256
        w  (warming step     )    : 2.333 sec
        t0 (default std alloc res): 0.866 sec; t1/t0: 1.477 
        t1 (default std alloc)    : 1.280 sec; t1/t1: 1.000 
        t2 (default pmr alloc)    : 1.884 sec; t1/t2: 0.679 
        t3 (pmr alloc no buffer)  : 1.897 sec; t1/t3: 0.675 
        t4 (pmr alloc with buffer): 1.629 sec; t1/t4: 0.786 
        t5 (custom stack alloc)   : 1.011 sec; t1/t5: 1.266 

        Results of 1048576 iterations for std::vector<int> of size 1024
        w  (warming step     )    : 1.927 sec
        t0 (default std alloc res): 0.766 sec; t1/t0: 1.301 
        t1 (default std alloc)    : 0.997 sec; t1/t1: 1.000 
        t2 (default pmr alloc)    : 1.457 sec; t1/t2: 0.684 
        t3 (pmr alloc no buffer)  : 1.482 sec; t1/t3: 0.673 
        t4 (pmr alloc with buffer): 1.358 sec; t1/t4: 0.734 
        t5 (custom stack alloc)   : 0.913 sec; t1/t5: 1.092 

        Results of 262144 iterations for std::vector<int> of size 4096
        w  (warming step     )    : 2.498 sec
        t0 (default std alloc res): 0.733 sec; t1/t0: 2.193 
        t1 (default std alloc)    : 1.608 sec; t1/t1: 1.000 
        t2 (default pmr alloc)    : 1.316 sec; t1/t2: 1.222 
        t3 (pmr alloc no buffer)  : 1.328 sec; t1/t3: 1.211 
        t4 (pmr alloc with buffer): 1.276 sec; t1/t4: 1.260 
        t5 (custom stack alloc)   : 0.889 sec; t1/t5: 1.809 

        Results of 65536 iterations for std::vector<int> of size 16384
        w  (warming step     )    : 2.682 sec
        t0 (default std alloc res): 0.734 sec; t1/t0: 2.433 
        t1 (default std alloc)    : 1.787 sec; t1/t1: 1.000 
        t2 (default pmr alloc)    : 1.273 sec; t1/t2: 1.403 
        t3 (pmr alloc no buffer)  : 1.272 sec; t1/t3: 1.405 
        t4 (pmr alloc with buffer): 1.271 sec; t1/t4: 1.406 
        t5 (custom stack alloc)   : 0.894 sec; t1/t5: 1.999 

        Results of 16384 iterations for std::vector<int> of size 65536
        w  (warming step     )    : 2.687 sec
        t0 (default std alloc res): 0.734 sec; t1/t0: 2.445 
        t1 (default std alloc)    : 1.794 sec; t1/t1: 1.000 
        t2 (default pmr alloc)    : 1.255 sec; t1/t2: 1.429 
        t3 (pmr alloc no buffer)  : 1.250 sec; t1/t3: 1.435 
        t4 (pmr alloc with buffer): 1.270 sec; t1/t4: 1.413 
        t5 (custom stack alloc)   : 0.895 sec; t1/t5: 2.005 

        Results of 4096 iterations for std::vector<int> of size 262144
        w  (warming step     )    : 2.734 sec
        t0 (default std alloc res): 0.736 sec; t1/t0: 2.488 
        t1 (default std alloc)    : 1.831 sec; t1/t1: 1.000 
        t2 (default pmr alloc)    : 1.272 sec; t1/t2: 1.440 
        t3 (pmr alloc no buffer)  : 1.268 sec; t1/t3: 1.444 
        t4 (pmr alloc with buffer): 1.278 sec; t1/t4: 1.432 
        t5 (custom stack alloc)   : 0.905 sec; t1/t5: 2.024 

        Results of 1024 iterations for std::vector<int> of size 1048576
        w  (warming step     )    : 3.876 sec
        t0 (default std alloc res): 0.735 sec; t1/t0: 4.109 
        t1 (default std alloc)    : 3.022 sec; t1/t1: 1.000 
        t2 (default pmr alloc)    : 2.401 sec; t1/t2: 1.259 
        t3 (pmr alloc no buffer)  : 2.375 sec; t1/t3: 1.272 
        t4 (pmr alloc with buffer): 1.283 sec; t1/t4: 2.356 
        t5 (custom stack alloc)   : 0.900 sec; t1/t5: 3.357 

### Quick comment:
The simple stack allocator (which when finishes its buffer causes a segfault in the best case scenario), is quite efficient compared to all other general or local allocators.
This would be very interesting if the simple "reserve" wouldn't be almost always better. Notice also that if we use this version of stack_allocator, we must know the max size beforehand, so we can always call reserve with vector.
For the list case (which as far as I know have no reserve method (as one would expect)), it can be a useful way to implement a "reserve" functionality, but with the general stack_allocator that call a general allocator when the buffer finishes.
