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

template <SEntityType _Sty> class SEntityId {
  // template <SMeshType _Mty> friend class SimpleMesh;

private:
  int32_t m_id;
  SEntityType m_type = _Sty;

public:
  SEntityId() : m_id(-1) {}
  SEntityId(uint32_t id) : m_id(id) {}
  SEntityId(const SEntityId &rhs) : m_id(rhs.m_id) {}
  bool operator==(const SEntityId<_Sty> &_rhs) {
    return _rhs.m_id == this->m_id;
  }
  bool Valid();
};
using SVertexId = SEntityId<SEntityType::Vertex>;
using SCellId = SEntityId<SEntityType::Cell>;
using SHalfEdgeId = SEntityId<SEntityType::HalfEdge>;
using SEdgeId = SEntityId<SEntityType::Edge>;

template <SMeshType _Mty> class SimpleMesh;

template <SMeshType _Ty> class SCell {
  template <SMeshType _Ty> friend class SimpleMesh;

public:
  SCell() {
    if (_Ty == SMeshType::Tri) {
      m_half_edges.reserve(3);
    } else if (_Ty == SMeshType::Quad) {
      m_half_edges.reserve(4);
    }
  }

private:
  std::vector<SHalfEdgeId> m_half_edges;
};

class SHalfEdge {
  template <SMeshType _Ty> friend class SimpleMesh;

private:
  SVertexId m_st_v;
  SVertexId m_ed_v;
  SHalfEdgeId m_opposite;
  SCellId m_instance_cell;
  SEdgeId m_instance_edge;

public:
  SHalfEdge(const SVertexId &st, const SVertexId &ed,
            const SHalfEdgeId &opposite, const SEdgeId &edge)
      : m_st_v(st), m_ed_v(ed), m_opposite(opposite), m_instance_edge(edge) {}
};
class SEdge {
  template <SMeshType _Ty> friend class SimpleMesh;

private:
  // each one won't be special and they don't have sequence
  std::array<SHalfEdgeId, 2> m_half_edges;

public:
  SEdge(const SHalfEdgeId &he1_id, const SHalfEdgeId &he2_id)
      : m_half_edges(std::array<SHalfEdgeId, 2>{he1_id, he2_id}) {}
};
class SVertex {
  template <SMeshType _Ty> friend class SimpleMesh;

private:
  // in counter-clockwise direction
  QuickList<SHalfEdgeId> m_out_he;
};
// todo:
// mesh声明成各种实体的友元类
} // namespace S2D
} // namespace SimpleMesh

#endif