#ifndef MESHENTITY_H
#define MESHENTITY_H

#include "../core/quicklist.hpp"
//#include <Eigen/Dense>
#include <array>
#include <set>
#include <vector>

namespace SimpleMesh {
namespace S2D {
enum class SEntityType { Vertex, HalfEdge, Edge, Cell };
enum class SMeshType { Tri, Quad, Poly };

template <SEntityType _Sty>
class SEntityId {
private:
  uint32_t m_id = 0;
  SEntityType m_type = _Sty;

public:
  SEntityId(uint32_t id) : m_id(id) {}
  bool operator==(const SEntityId<_Sty> &_rhs) { return _rhs.m_id == this->m_id; }
  bool Valid();
};
using SVertexId = SEntityId<SEntityType::Vertex>;
using SCellId = SEntityId<SEntityType::Cell>;
using SHalfEdgeId = SEntityId<SEntityType::HalfEdge>;
using SEdgeId = SEntityId<SEntityType::Edge>;

template <SMeshType _Ty> class SCell {
public:
  SCell() {
    if (_Ty == SMeshType::Tri) {
      m_half_edges.reserve(3);
    } else if (_Ty == SMeshType::Quad) {
      m_half_edges.reserve(4);
    }
  }

  // private:
public:
  std::vector<SHalfEdgeId> m_half_edges;
};

class SHalfEdge {
  // private:
public:
  SVertexId m_st_v;
  SVertexId m_ed_v;
  SHalfEdgeId m_opposite;
  SCellId m_instance_cell;
  SEdgeId m_instance_edge;
};
class SEdge {
  // private:
public:
  // each one won't be special and they don't have sequence
  std::array<SHalfEdgeId, 2> m_half_edges;
};
class SVertex {
  // private:
public:
  // in counter-clockwise direction
  QuickList<SHalfEdgeId> m_out_he;
};
// todo:
// mesh声明成各种实体的友元类
} // namespace S2D
} // namespace SimpleMesh

#endif