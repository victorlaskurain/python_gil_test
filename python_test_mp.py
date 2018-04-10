#!/usr/bin/python

from multiprocessing import Pool
import sys
import time

DATA_LENGTH = 10000000

def count_binary_ones(n):
    result = 0
    while n:
        result += n % 2
        n = n / 2
    return result

def count_ones(l):
    return sum([count_binary_ones(n) for n in l])

def main():
    thread_count = int(sys.argv[1])
    pool = Pool(thread_count)
    data = [0x01 for i in range(DATA_LENGTH)]
    step = len(data) / thread_count
    futures = []
    begin_ts = int(time.time() * 1000)
    for i in range(thread_count):
        future = pool.apply_async(count_ones, (data[i*step:(i+1)*step],))
        futures.append(future)
    zero_count = sum([f.get() for f in futures])
    end_ts = int(time.time() * 1000)
    work_time_ms = end_ts - begin_ts
    print "Python (mp),%s,%s,%s"%(thread_count, zero_count, work_time_ms)

if __name__ == '__main__':
    main()
