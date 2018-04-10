#include <algorithm>
#include <chrono>
#include <cstdint>
#include <future>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using std::accumulate;
using std::async;
using std::begin;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::end;
using std::future;
using std::min;
using std::vector;

static uint32_t count_binary_ones(uint32_t n)
{
    uint32_t result = 0;
    for (size_t i = 0; i < sizeof(n) * 8; ++i) {
        result += (n>>i) & 1;
    }
    return result;
}

static uint32_t count_ones(const uint32_t *begin, const uint32_t *end)
{
    return accumulate(begin, end, 0, [](uint32_t acc, uint32_t n) {
            return acc + count_binary_ones(n);
        });
}

uint32_t acc_future_uint32(uint32_t acc, future<uint32_t> &fut)
{
    return acc + fut.get();
}

const auto DATA_LENGTH = 10000000;

int main(int argc, char **argv)
{
    if (argc != 2) {
        exit(EXIT_FAILURE);
    }
    auto thread_count = strtol(argv[1], nullptr, 10);
    if (thread_count <= 0 || thread_count == LONG_MAX || thread_count > DATA_LENGTH) {
        exit(EXIT_FAILURE);
    }
    auto data       = std::vector<uint32_t>(DATA_LENGTH);
    auto data_begin = data.data();
    auto data_end   = data.data() + DATA_LENGTH;
    auto step       = DATA_LENGTH / thread_count;
    auto begin_ts   = high_resolution_clock::now();
    fill(begin(data), end(data), 0x01);
    vector<future<uint32_t>> futures(thread_count);
    for (auto i = 0; i < thread_count; ++i) {
        futures[i] = async(
            std::launch::async,
            count_ones,
            data_begin +  i * step,
            min(data_begin + (i + 1) * step, data_end));
    }
    auto zero_count   = accumulate(begin(futures), end(futures), 0, acc_future_uint32);
    auto end_ts       = high_resolution_clock::now();
    auto work_time_ms = duration_cast<milliseconds>(end_ts - begin_ts).count();
    printf("C++,%ld,%u,%ld\n", thread_count, zero_count, work_time_ms);
    exit(EXIT_SUCCESS);
}
