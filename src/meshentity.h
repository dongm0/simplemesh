
namespace SimpleMesh {
struct SMeshCell {
  std::array<int32_t, 4> half_edges{-1, -1, -1, -1};
};
struct SMeshHalfEdge {
  int32_t st = -1;
  int32_t ed = -1;
  int32_t opposite = -1;
  //是否处理
  int32_t split = 0; // 0好的1分裂前半段2分裂
  int32_t split_relevancy = -1;
  int32_t instance_cell = -1;
  int32_t instance_edge = -1;
};
struct SMeshEdge {
  std::array<int32_t, 2> half_edges{-1, -1};
};
using SMeshVertex = int32_t;
}
}