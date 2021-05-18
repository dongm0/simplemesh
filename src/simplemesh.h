#ifndef SIMPLEMESH_H
#define SIMPLEMESH_H

#include <Eigen/Dense>
#include <array>
#include <set>
#include <vector>

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

class SimpleMesh {
private:
  std::vector<SMeshCell> c;
  std::vector<SMeshHalfEdge> he;
  std::vector<SMeshEdge> e;
  std::vector<SMeshVertex> v;

  Eigen::Matrix<double, Eigen::Dynamic, 2> coords;

public:
  void ReadFromFile(const std::string &filename);
  std::pair<Eigen::MatrixXd, Eigen::MatrixXi> GetMatrixView();
  void AssignGeomFromMatrix(const Eigen::MatrixXd &v);
  void Subdivide(const std::vector<int32_t> &tagged);
  void WriteToFile(const std::string &filename);

private:
  // 可以public但是等写项目再说
  std::array<int32_t, 4> CellVertices(int32_t cell_id);
  std::array<std::array<int32_t, 3>, 4> CellCorners(int32_t cell_id);
  Eigen::Vector2d GetVertexCoord(int32_t vertex_id);
  int32_t AdjecentCell(int32_t cell_id, int32_t half_edge_id);
  int32_t OppositeHalfEdgeInCell(int32_t cell_id, int32_t half_edge_id);
  int32_t OppositeHalfEdge(int32_t half_edge_id);
  int32_t HalfEdgeInstanceEdge(int32_t half_edge_id);
  int32_t HalfEdgeInstanceCell(int32_t half_edge_id);

  SMeshCell Cell(int32_t cell_id);
  SMeshEdge Edge(int32_t edge_id);
  SMeshHalfEdge HalfEdge(int32_t half_edge_id);
  SMeshVertex Vertex(int32_t vertex_id);

private:
  // 可能功能性有问题，但是先写着
  int32_t AddVertex(); // 返回新vertex的id
  int32_t AddHalfEdge(int32_t st_v_id, int32_t ed_v_id);
  int32_t AddEdge(int32_t he1_id, int32_t he2_id);
  int32_t AddCell(int32_t heid1, int32_t heid2, int32_t heid3, int32_t heid4);
  int32_t SetHalfEdge(int32_t half_edge_id, int32_t st_v_id, int32_t ed_v_id);
  int32_t SetEdge(int32_t edge_id, int32_t he1_id, int32_t he2_id);
  int32_t SetCell(int32_t cell_id, int32_t heid1, int32_t heid2, int32_t heid3,
                  int32_t heid4);

private:
  // 可以有，但现在用不到也不会写
  bool CheckTopologyLegal();

private:
};

#endif
