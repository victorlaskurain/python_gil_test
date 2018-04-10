# Python GIL test #

This miniproject is  a test of the  abbysal multithreading performance
of Python  for CPU bound  tasks. It contains three  implementations of
the same simple algorithm:

1. In C++ as baseline.
2. In Python, using multithreading.
3. In Python, using multiprocessing.

The goal  is to show how  badly Pythons multithreading scales  for CPU
bound  tasks and  how  the  situation can  be  somehow remedied  using
multiprocessing instead.

Compile  and run  with `make`.  The output  should be  something along
these lines:

```
C++,1,10000000,649
C++,2,10000000,360
C++,4,10000000,314
Python (mt),1,10000000,5934
Python (mt),2,10000000,10429
Python (mt),4,10000000,13316
Python (mp),1,10000000,7386
Python (mp),2,10000000,4109
Python (mp),4,10000000,3934
```

The first column identifies the  implementation, the second the number
of threads, the third is the  result of the calculation and the fourth
and last is the run time in milliseconds.

The machine has two cores. As expected,  in C++ and in Python with the
multiprocessing   library,  using   two   tasks   almost  halves   the
runtime. Using more  tasks is of no  help as the cores  are already as
busy as  they can be.  Python with  multithreading on the  other hand,
instead of  reducing the runtime  takes almost  twice as much  time to
execute the same computation. The
[GIL](https://wiki.python.org/moin/GlobalInterpreterLock) is to  blame
for this.
