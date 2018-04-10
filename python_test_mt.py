#!/usr/bin/python

import threading
import sys
import time
from Queue import Queue

DATA_LENGTH = 10000000

def count_binary_ones(n):
    result = 0
    while n:
        result += n % 2
        n = n / 2
    return result

def count_ones(q):
    q.put(sum([count_binary_ones(n) for n in q.get()]))

def main():
    thread_count = int(sys.argv[1])
    pool         = []
    queues       = []
    data         = [0x01 for i in range(DATA_LENGTH)]
    step         = len(data) / thread_count
    for i in range(thread_count):
        queue = Queue()
        queue.put(data[i*step:(i+1)*step])
        queues.append(queue)
        worker = threading.Thread(target=count_ones, args=(queue,))
        pool.append(worker)
    begin_ts = int(time.time() * 1000)
    [w.start() for w in pool]
    zero_count = sum([q.get() for q in queues])
    end_ts = int(time.time() * 1000)
    work_time_ms = end_ts - begin_ts
    print "Python (mt),%s,%s,%s"%(thread_count, zero_count, work_time_ms)

if __name__ == '__main__':
    main()
