
#include "lr3/AVLTree.h"
#include "lr3/Array.h"
#include "lr3/DList.h"
#include "lr3/HashTable.h"
#include "lr3/List.h"
#include "lr3/Queue.h"
#include "lr3/Stack.h"

#include <benchmark/benchmark.h>
#include <cstddef>
#include <string>

using namespace lr3;

static void BM_ArrayPushBack(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));

    for (auto _ : state) {
        Array array;
        for (std::size_t i = 0; i < count; ++i) {
            array.pushBack(std::to_string(i));
        }
        benchmark::DoNotOptimize(array.size());
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

static void BM_ListPushBack(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));

    for (auto _ : state) {
        List list;
        for (std::size_t i = 0; i < count; ++i) {
            list.pushBack(std::to_string(i));
        }
        benchmark::DoNotOptimize(list.size());
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

static void BM_DListPushBack(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));

    for (auto _ : state) {
        DList list;
        for (std::size_t i = 0; i < count; ++i) {
            list.pushBack(std::to_string(i));
        }
        benchmark::DoNotOptimize(list.size());
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

static void BM_QueuePush(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));

    for (auto _ : state) {
        Queue queue;
        for (std::size_t i = 0; i < count; ++i) {
            queue.push(std::to_string(i));
        }
        benchmark::DoNotOptimize(queue.size());
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

static void BM_StackPush(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));

    for (auto _ : state) {
        Stack stack;
        for (std::size_t i = 0; i < count; ++i) {
            stack.push(std::to_string(i));
        }
        benchmark::DoNotOptimize(stack.size());
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

static void BM_HashTableInsert(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));

    for (auto _ : state) {
        HashTable table(count * 2 + 1);
        for (std::size_t i = 0; i < count; ++i) {
            table.insert(static_cast<int>(i), std::to_string(i));
        }
        benchmark::DoNotOptimize(table.size());
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

static void BM_AVLTreeInsert(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));

    for (auto _ : state) {
        AVLTree tree;
        for (std::size_t i = 0; i < count; ++i) {
            tree.insert(std::to_string(i));
        }
        benchmark::DoNotOptimize(tree.size());
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

#define REGISTER_BENCHMARK(function) BENCHMARK(function)->Arg(100)->Arg(1000)->Arg(5000)

REGISTER_BENCHMARK(BM_ArrayPushBack);
REGISTER_BENCHMARK(BM_ListPushBack);
REGISTER_BENCHMARK(BM_DListPushBack);
REGISTER_BENCHMARK(BM_QueuePush);
REGISTER_BENCHMARK(BM_StackPush);
REGISTER_BENCHMARK(BM_HashTableInsert);
REGISTER_BENCHMARK(BM_AVLTreeInsert);

BENCHMARK_MAIN();
