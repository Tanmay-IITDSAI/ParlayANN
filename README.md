# Implementation of Deterministic Parallel Graph-Based ANNS Algorithms from ParlayANN
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

## Repository Structure (suggested)

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

> Replace `project-name` and commands with your repo's actual build targets.

```bash
# from repo root
mkdir -p build && cd build
cmake ..
cmake --build . -- -j$(nproc)
```

After building you should get binaries for the index construction and the query server, for example: `parlay_hcnng_build`, `parlay_pynnd_build`, `parlay_query` (names are placeholders — update according to actual targets).

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

-----

## License

Add your chosen license (e.g., `MIT`, `Apache-2.0`)

