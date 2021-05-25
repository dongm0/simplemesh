#ifndef SIMPLEMESH_H
#define SIMPLEMESH_H

#include <Eigen/Dense>
#include <array>
#include <set>
#include <vector>

namespace SimpleMesh {

enum class SEntityType { Vertex, HalfEdge, Edge, Cell };

class SEntityId {
private:
  int32_t m_id = -1;

public:
  bool Valid();
};

class SVertexId : SEntityId {
private:
  const SEntityType m_type = SEntityType::Vertex;
};

class SHalfEdgeId : SEntityId {
private:
  const SEntityType m_type = SEntityType::HalfEdge;
};

class SEdgeId : SEntityId {
private:
  const SEntityType m_type = SEntityType::Edge;
};

class SCellId : SEntityId {
private:
  const SEntityType m_type = SEntityType::Cell;
};

class SQuadCell {
private:
  std::array<SHalfEdgeId, 4> m_half_edges;
};
class STriCell {
private:
  std::array<SHalfEdgeId, 3> m_half_edges;
};
class SPolyCell {
private:
  std::vector<SHalfEdgeId> m_half_edges;
};
class SHalfEdge {
private:
  SVertexId m_st_v;
  SVertexId m_ed_v;
  SHalfEdgeId m_opposite;
  SCellId m_instance_cell;
  SEdgeId m_instance_edge;
};
class SEdge {
private:
  // each one won't be special and they don't have sequence
  std::array<SHalfEdgeId, 2> m_half_edges;
};
class SVertex {
private:
  // in counter-clockwise direction
  std::vector<int32_t> m_out_he;
};

class SimpleMesh {
private:
  std::vector<SCell> m_c;
  std::vector<SHalfEdge> m_he;
  std::vector<SEdge> m_e;
  std::vector<SVertex> m_v;

public:
  void ReadFromFile(const std::string &filename);
  // std::pair<Eigen::MatrixXd, Eigen::MatrixXi> GetMatrixView() const;
  // void AssignGeomFromMatrix(const Eigen::MatrixXd &v);
  // void Subdivide(const std::vector<int32_t> &tagged);
  void WriteToFile(const std::string &filename) const;
  // std::pair<Eigen::Matrix<double, Eigen::Dynamic, 2>,
  //          Eigen::Matrix<int, Eigen::Dynamic, 4>>
  // GetMatrixView();
  // std::pair<Eigen::Matrix<double, Eigen::Dynamic, 2>,
  //          Eigen::Matrix<int, Eigen::Dynamic, 4>>
  // GetMatrixView_Exp();

  // temp
  // void WriteToFile_Exp(const std::string &filename) const;

public:
  uint32_t NumVertices() const;
  uint32_t NumCells() const;
  uint32_t NumHalfEdges() const;
  uint32_t NumEdges() const;

  std::array<int32_t, 4> CellVertices(int32_t cell_id) const;
  std::array<std::array<int32_t, 3>, 4> CellCorners(int32_t cell_id) const;
  Eigen::Vector2d GetVertexCoord(int32_t vertex_id) const;
  int32_t AdjecentCell(int32_t cell_id, int32_t half_edge_id) const;
  int32_t OppositeHalfEdgeInCell(int32_t cell_id, int32_t half_edge_id) const;
  int32_t OppositeHalfEdge(int32_t half_edge_id) const;
  int32_t HalfEdgeInstanceEdge(int32_t half_edge_id) const;
  int32_t HalfEdgeInstanceCell(int32_t half_edge_id) const;
  /*
  const SMeshCell Cell(int32_t cell_id) const;
  const SMeshEdge Edge(int32_t edge_id) const;
  const SMeshHalfEdge HalfEdge(int32_t half_edge_id) const;
  const SMeshVertex Vertex(int32_t vertex_id) const;
  */
public:
  // 先用着，再改，已有的addcell逻辑不对换成这个
  int32_t AddCellFromVertex(int32_t vid1, int32_t vid2, int32_t vid3,
                            int32_t vid4);

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
  void clear();

private:
  // 可以有，但现在用不到也不会写
  bool CheckTopologyLegal() const;

private:
};
} // namespace SimpleMesh

#endif