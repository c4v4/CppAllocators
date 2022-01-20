# CppAllocators
C++ benchmark for default, pmr and custom local allocator 

## DISCLAIMER
These test are fast and dirty, only to see if there's any potential benefit in what I usually do coding. Do not exepect code of any quality.

## Sample results on Ryzen 9 5959X

        Results of 1000 iterations for std::list<int> of size 250000
         w  (warming step)                        : 4.235 sec
         t1 (default std alloc no reserve)        : 3.423 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 4.093 sec; t1/t2: 0.836 
         t3 (pmr alloc no buffer no reserve)      : 2.018 sec; t1/t3: 1.696 
         t4 (pmr alloc with buffer with reserve)  : 1.603 sec; t1/t4: 2.135 
         t5 (custom stack alloc with reserve)     : 0.550 sec; t1/t5: 6.224 
         t6 (custom stackalloc allctd once, resvd): 0.604 sec; t1/t5: 5.667 

        Results of 268435456 iterations for std::vector<int> of size 4
         w  (warming step)                        : 17.327 sec
         t0 (default std alloc with reserve)      : 7.593 sec; t1/t0: 1.709 
         t1 (default std alloc no reserve)        : 12.977 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 15.632 sec; t1/t2: 0.830 
         t3 (pmr alloc no buffer no reserve)      : 15.754 sec; t1/t3: 0.824 
         t4 (pmr alloc with buffer with reserve)  : 7.680 sec; t1/t4: 1.690 
         t5 (custom stackalloc with reserve)      : 4.400 sec; t1/t5: 2.949 
         t6 (custom stackalloc allctd once, resvd): 4.293 sec; t1/t5: 3.023 

        Results of 67108864 iterations for std::vector<int> of size 16
         w  (warming step)                        : 7.234 sec
         t0 (default std alloc with reserve)      : 2.429 sec; t1/t0: 2.102 
         t1 (default std alloc no reserve)        : 5.107 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 6.301 sec; t1/t2: 0.811 
         t3 (pmr alloc no buffer no reserve)      : 5.254 sec; t1/t3: 0.972 
         t4 (pmr alloc with buffer with reserve)  : 2.410 sec; t1/t4: 2.119 
         t5 (custom stackalloc with reserve)      : 2.134 sec; t1/t5: 2.393 
         t6 (custom stackalloc allctd once, resvd): 2.094 sec; t1/t5: 2.439 

        Results of 16777216 iterations for std::vector<int> of size 64
         w  (warming step)                        : 3.387 sec
         t0 (default std alloc with reserve)      : 1.197 sec; t1/t0: 1.834 
         t1 (default std alloc no reserve)        : 2.196 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 2.685 sec; t1/t2: 0.818 
         t3 (pmr alloc no buffer no reserve)      : 2.133 sec; t1/t3: 1.029 
         t4 (pmr alloc with buffer with reserve)  : 1.238 sec; t1/t4: 1.773 
         t5 (custom stackalloc with reserve)      : 1.190 sec; t1/t5: 1.845 
         t6 (custom stackalloc allctd once, resvd): 1.193 sec; t1/t5: 1.840 

        Results of 4194304 iterations for std::vector<int> of size 256
         w  (warming step)                        : 2.196 sec
         t0 (default std alloc with reserve)      : 0.840 sec; t1/t0: 1.517 
         t1 (default std alloc no reserve)        : 1.275 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 1.426 sec; t1/t2: 0.894 
         t3 (pmr alloc no buffer no reserve)      : 1.329 sec; t1/t3: 0.959 
         t4 (pmr alloc with buffer with reserve)  : 0.916 sec; t1/t4: 1.391 
         t5 (custom stackalloc with reserve)      : 0.924 sec; t1/t5: 1.379 
         t6 (custom stackalloc allctd once, resvd): 0.920 sec; t1/t5: 1.386 

        Results of 1048576 iterations for std::vector<int> of size 1024
         w  (warming step)                        : 1.852 sec
         t0 (default std alloc with reserve)      : 0.753 sec; t1/t0: 1.323 
         t1 (default std alloc no reserve)        : 0.996 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 1.047 sec; t1/t2: 0.951 
         t3 (pmr alloc no buffer no reserve)      : 1.031 sec; t1/t3: 0.965 
         t4 (pmr alloc with buffer with reserve)  : 0.835 sec; t1/t4: 1.192 
         t5 (custom stackalloc with reserve)      : 0.857 sec; t1/t5: 1.162 
         t6 (custom stackalloc allctd once, resvd): 0.853 sec; t1/t5: 1.167 

        Results of 262144 iterations for std::vector<int> of size 4096
         w  (warming step)                        : 2.309 sec
         t0 (default std alloc with reserve)      : 0.727 sec; t1/t0: 2.011 
         t1 (default std alloc no reserve)        : 1.462 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 0.920 sec; t1/t2: 1.589 
         t3 (pmr alloc no buffer no reserve)      : 0.908 sec; t1/t3: 1.610 
         t4 (pmr alloc with buffer with reserve)  : 0.824 sec; t1/t4: 1.775 
         t5 (custom stackalloc with reserve)      : 0.847 sec; t1/t5: 1.725 
         t6 (custom stackalloc allctd once, resvd): 0.844 sec; t1/t5: 1.733 

        Results of 65536 iterations for std::vector<int> of size 16384
         w  (warming step)                        : 2.599 sec
         t0 (default std alloc with reserve)      : 0.731 sec; t1/t0: 2.410 
         t1 (default std alloc no reserve)        : 1.761 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 0.892 sec; t1/t2: 1.974 
         t3 (pmr alloc no buffer no reserve)      : 0.887 sec; t1/t3: 1.985 
         t4 (pmr alloc with buffer with reserve)  : 0.812 sec; t1/t4: 2.170 
         t5 (custom stackalloc with reserve)      : 0.838 sec; t1/t5: 2.102 
         t6 (custom stackalloc allctd once, resvd): 0.835 sec; t1/t5: 2.108 

        Results of 16384 iterations for std::vector<int> of size 65536
         w  (warming step)                        : 2.656 sec
         t0 (default std alloc with reserve)      : 0.726 sec; t1/t0: 2.500 
         t1 (default std alloc no reserve)        : 1.816 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 0.886 sec; t1/t2: 2.050 
         t3 (pmr alloc no buffer no reserve)      : 0.881 sec; t1/t3: 2.060 
         t4 (pmr alloc with buffer with reserve)  : 0.814 sec; t1/t4: 2.231 
         t5 (custom stackalloc with reserve)      : 0.839 sec; t1/t5: 2.165 
         t6 (custom stackalloc allctd once, resvd): 0.835 sec; t1/t5: 2.173 

        Results of 4096 iterations for std::vector<int> of size 262144
         w  (warming step)                        : 2.671 sec
         t0 (default std alloc with reserve)      : 0.728 sec; t1/t0: 2.517 
         t1 (default std alloc no reserve)        : 1.832 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 0.904 sec; t1/t2: 2.028 
         t3 (pmr alloc no buffer no reserve)      : 0.901 sec; t1/t3: 2.034 
         t4 (pmr alloc with buffer with reserve)  : 0.815 sec; t1/t4: 2.248 
         t5 (custom stackalloc with reserve)      : 0.841 sec; t1/t5: 2.178 
         t6 (custom stackalloc allctd once, resvd): 0.837 sec; t1/t5: 2.188 

        Results of 1024 iterations for std::vector<int> of size 1048576
         w  (warming step)                        : 3.791 sec
         t0 (default std alloc with reserve)      : 0.729 sec; t1/t0: 4.046 
         t1 (default std alloc no reserve)        : 2.950 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 1.944 sec; t1/t2: 1.518 
         t3 (pmr alloc no buffer no reserve)      : 1.927 sec; t1/t3: 1.531 
         t4 (pmr alloc with buffer with reserve)  : 0.815 sec; t1/t4: 3.619 
         t5 (custom stackalloc with reserve)      : 0.841 sec; t1/t5: 3.509 
         t6 (custom stackalloc allctd once, resvd): 0.838 sec; t1/t5: 3.521 

        Results of 1000000 iterations for std::vector<int> of max size 256
         w  (warming step)                        : 0.326 sec
         t0 (default std alloc with reserve)      : 0.136 sec; t1/t0: 1.499 
         t1 (default std alloc no reserve)        : 0.203 sec; t1/t1: 1.000 
         t2 (default pmr alloc no reserve)        : 0.241 sec; t1/t2: 0.843 
         t3 (pmr alloc no buffer no reserve)      : 0.216 sec; t1/t3: 0.940 
         t4 (pmr alloc with buffer with reserve)  : 0.152 sec; t1/t4: 1.335 
         t7 (custom stackalloc with backup, resvd): 0.150 sec; t1/t5: 1.359
        (in this test the final size is not known, "reserves" are made with 
        a guess, in average equal to half to the final size)    

### Quick comment:
The simple stack allocator (which, in the best case scenario, when finishes its buffer causes a segfault), is quite efficient when compared to all other general or local allocators.
This would be very interesting if the simple "reserve" wouldn't be almost always better (aside for very small vector sizes). 
Notice also that if we use this version of stack_allocator, we must know the max size beforehand, so we can call reserve with vector using the same size.
For the list case (which, as far as I know, has no reserve method (as one would expect)), it can be a useful way to implement a "reserve" functionality, but only with the general stack_allocator that call a general allocator when the buffer finishes.

Comparing t5 and t6, allocating a stack buffer in the parent function seems to be effective with very small sizes. It rapidly loosees any relevance (as the stack-allocator compared to the simple "reserve" call of std::vector).

Finally, when the final size is not known, making a guess with "reserve" still seems more efficient than allocating into the stack a small buffer for smaller sizes.

