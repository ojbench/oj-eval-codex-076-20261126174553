# MyList

`MyList.hpp` implements the requested class using an owning doubly linked list.
Endpoint operations, size queries and moves take constant time. Indexed
operations walk from the nearer endpoint. Copying and linking take linear time
in the source size. Cutting locates the boundary then transfers nodes without
copying values. Each list owns its nodes exclusively. The shared per-type pool
retains storage proportional to the peak number of simultaneously live nodes.

The CMake target `code` runs the randomized local test driver. Build with
`cmake -S . -B build && cmake --build build`, then run `build/code`.

The live problem uses a class-only harness that includes submitted text as
`src.hpp`. Despite advertising Git submissions, it treats the repository URL
as source text. Submit `MyList.hpp` using the client's `--source-file` option.

## Validation and submissions

- 20,000 randomized operations checked against `std::vector`, including
  self-linking, copying, moving, and boundary cuts; ASan and UBSan passed.
- Submission 879524: compile error because the judge interpreted the Git URL
  as C++ source. No class implementation error was reported.
- Submission 879527: 75/100; the complex test exceeded its time limit.
  Added a cached traversal cursor to accelerate nearby indexed operations.
- Submission 879530: 75/100; traversal caching had little impact on runtime.
  Added a per-type block allocator with recycled node storage to reduce
  allocation overhead. Storage is released at program exit; values are
  destroyed immediately when their nodes are removed.
- Submission 879532: Accepted, 100/100 across all six groups. The complex test
  took 2,881 ms out of 3,000 ms. Four submissions were used in total.
