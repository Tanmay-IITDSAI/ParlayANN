#pragma once
#include <math.h>
#include <algorithm>
#include <functional>
#include <queue>
#include <random>
#include <set>
#include "../utils/graph.h"
#include "parlay/parallel.h"
#include "parlay/primitives.h"
#include "parlay/random.h"

namespace parlayANN {
...
// Random generator fixed seed
void random_clustering_wrapper(GraphI &G, PR &Points, size_t cluster_size, F f, long MSTDeg) {
    std::mt19937 rng(42);
    parlay::random rnd(rng());
    auto active_indices = parlay::tabulate(Points.size(), [&](size_t i) { return i; });
    random_clustering(G, Points, active_indices, rnd, cluster_size, f, MSTDeg);
}
...
} // namespace parlayANN
