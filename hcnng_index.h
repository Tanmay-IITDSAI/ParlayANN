#pragma once
#include <math.h>
#include <algorithm>
#include <queue>
#include <random>
#include <set>
#include "../utils/graph.h"
#include "clusterEdge.h"
#include "parlay/parallel.h"
#include "parlay/primitives.h"
#include "parlay/random.h"

namespace parlayANN {
...
void build_index(GraphI &G, PR &Points, long cluster_rounds, long cluster_size, long MSTDeg) {
    cluster<Point, PointRange, indexType> C;
    C.multiple_clustertrees(G, Points, cluster_size, cluster_rounds, MSTk, MSTDeg);
    remove_all_duplicates(G);
    parlay::parallel_for(0, G.size(), [&](size_t i) { robustPrune(i, Points, G, 1.1); });
}
...
} // namespace parlayANN
