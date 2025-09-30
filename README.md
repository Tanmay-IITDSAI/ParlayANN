# ParlayANN — Deterministic Parallel Graph-Based ANNS (ParlayHCNNG & ParlayPyNNDescent)

> Implementation of deterministic, batch-parallel graph-based Approximate Nearest Neighbor Search (ANNS) inspired by the ParlayANN framework.

## Overview

This repository contains an implementation of two graph-based ANNS algorithms adapted for deterministic parallel execution using the ParlayANN ideas: **HCNNG** and **PyNNDescent**. The implementation focuses on:

* Batch-parallel graph construction with prefix doubling
* Lock-free edge updates using snapshot isolation and atomic operations
* Deterministic task scheduling via work-stealing
* Practical optimizations for multi-core CPUs

The original project report includes experimental evaluation on SIFT1M, Deep1B and BIGANN-1B datasets and reports strong speedups while maintaining high recall.

---

## Key Features

* **Batch-Parallel Construction** (prefix doubling)
* **Deterministic Scheduling** (work-stealing deques with bounded stealing)
* **Lock-free Edge Updates** (atomic CAS for edge lists + bidirectional edge maintenance)
* **Versioned Snapshots & Epoch-based Reclamation** for safe concurrent queries
* **Algorithm Variants**:

  * ParlayHCNNG — HCNNG adapted for batch processing and deterministic updates
  * ParlayPyNNDescent — Deterministic, batch-parallel variant with progressive sparsification

---

## Repository Structure

```
/ (repo root)
├── README.md
├── src/                 # source code (C++ / ParlayLib bindings)
├── include/             # headers
├── tools/               # helper scripts (data convertors, evaluation scripts)
├── experiments/         # experiment configs & result logs
├── datasets/            # dataset download / preprocessing scripts
├── cmake/               # cmake helper files (if using cmake)
└── docs/                # plots, additional writeups
```

---

## Requirements

* C++17 or newer (or language used by the original implementation)
* A parallel runtime (e.g. ParlayLib) or a compatible task-stealing library
* CMake (recommended) or preferred build system
* `make` / `ninja` and a modern compiler (g++ / clang)
* Optional: GPU (NVIDIA) for baseline comparisons

---

## Build (example)

If your repository contains **CMakeLists** (preferred)

```bash
# from repo root
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j$(nproc)
```

After a successful build, look in `build/` (or `bin/`) for executables. Common targets for this project might be `parlay_hcnng_build`, `parlay_pynnd_build` or `hcnng_index` depending on how the CMake targets are named in `CMakeLists.txt`.

If your repository contains a top-level **Makefile**

```bash
# from repo root
# build with parallel jobs
make -j$(nproc)
# or to specify CXX/CFLAGS
make CXX=g++ CXXFLAGS='-O3 -march=native' -j$(nproc)
```

Common Makefile targets: `all` (default), `clean`, `install`. If the Makefile defines named targets like `hcnng_index`, run `make hcnng_index`.

### Quick checks

* If you see source/header names like `hcnng_index`, `clusterEdge`, `neighbors` (as in your project bundle), search the root `CMakeLists.txt` or `Makefile` for those names to identify the corresponding targets and output executables.
* After building, run `ls -l build/` and `file build/*` to inspect which files are executables.

### Example minimal CMakeLists (adapt to your code)

```cmake
cmake_minimum_required(VERSION 3.10)
project(ParlayANN LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Release)

# Add an option for Parlay/Threading libs
# find_package(Parlay REQUIRED)  # uncomment if you have a Parlay package

file(GLOB SRC_FILES src/*.cpp)
add_executable(parlay_hcnng_build ${SRC_FILES})
# target_include_directories(parlay_hcnng_build PRIVATE include/)
# target_link_libraries(parlay_hcnng_build PRIVATE parlay pthread)
```

### Example Makefile fragment (adapt to your code)

```makefile
CXX := g++
CXXFLAGS := -O3 -std=c++17 -march=native -Iinclude
SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)
TARGET := parlay_hcnng_build

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

clean:
	rm -f $(OBJS) $(TARGET)
```

---

## Usage (example)

### Construct an index

```bash
# Construct ParlayHCNNG index
./bin/parlay_hcnng_build --dataset /path/to/sift1m --out index_hcnng.bin --k 40 --batch 256

# Construct ParlayPyNNDescent index
./bin/parlay_pynnd_build --dataset /path/to/sift1m --out index_pynnd.bin --k 40 --batch 256
```

### Run queries

```bash
./bin/parlay_query --index index_hcnng.bin --queries /path/to/queries.npy --k 10 --threads 48
```

(Adjust CLI flags based on actual implementation.)

---

## Datasets

This project uses standard ANNS benchmarks for evaluation — SIFT1M, BIGANN-1B, and Deep1B. Provide dataset download & preprocessing scripts in `datasets/`.

---

## Evaluation & Results (summary)

The project evaluation demonstrates:

* **Construction speedup:** ~3.7–3.8× speedup on 48-core machines versus sequential baselines
* **Recall:** Maintained ≈99% recall@10 on large-scale datasets
* **Memory efficiency:** lower memory footprint compared to baseline HCNNG / PyNNDescent variants

Detailed tables, plots and terminal outputs are available in `experiments/` and the project report.

---

## Implementation Notes

* Batch prefix-doubling: start from `θ = 256`, double on success, with an upper bound (e.g. 65,536)
* Beam-search parameters: entry points, beam width (example: 3 entry starts, beam=200), converge on stable top-50 neighbors
* Edge update: connect to top-K (e.g. K=40), use atomic CAS for lock-free writes, maintain bidirectional edges
* Determinism: use fixed random seeds, deterministic scheduling policy and limited-steal work-stealing deques
* Memory: use versioned snapshots + epoch reclamation to allow lock-free reads during construction

---

## References

Add full BibTeX / reference list in `docs/` or cite directly in the paper/report. Relevant papers include ParlayANN and algorithm sources such as HCNNG, PyNNDescent, HNSW and DiskANN.

---

## Contributing

Contributions, issues and feature requests are welcome. Please open issues for bugs or feature proposals and submit pull requests for fixes.

---

## License

Add your chosen license (e.g., `MIT`, `Apache-2.0`).

*This README was generated from the project proposal.*
