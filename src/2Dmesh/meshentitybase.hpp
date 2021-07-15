#ifndef MESHENTITYBASE_H
#define MESHENTITYBASE_H

#include "../core/quicklist.hpp"
#include <array>
#include <map>
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
  using id_type = SCellId;
  template <SMeshType _Ty> friend class SimpleMesh;

public:
  SCell(const std::vector<SHalfEdgeId> &heids) {
    size_t N = heids.size();
    if (_Ty == SMeshType::Tri) {
      if (N != 3) {
        throw std::runtime_error("three halfedges for tri cell!");
      }
    } else if (_Ty == SMeshType::Quad) {
      if (N != 4) {
        throw std::runtime_error("four halfedges for quad cell!");
      }
    } else {
      if (N < 3) {
        throw std::runtime_error("at least three halfedges for tri cell!");
      }
    }
    m_half_edges = std::vector<SHalfEdgeId>(heids);
  }

protected:
  std::vector<SHalfEdgeId> m_half_edges;
};

class SHalfEdge {
  using id_type = SHalfEdgeId;
  template <SMeshType _Ty> friend class SimpleMesh;

protected:
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
  using id_type = SEdgeId;
  template <SMeshType _Ty> friend class SimpleMesh;

protected:
  // each one won't be special and they don't have sequence
  std::array<SHalfEdgeId, 2> m_half_edges;

public:
  SEdge(const SHalfEdgeId &he1_id, const SHalfEdgeId &he2_id)
      : m_half_edges(std::array<SHalfEdgeId, 2>{he1_id, he2_id}) {}
};
class SVertex {
  using id_type = SVertexId;
  template <SMeshType _Ty> friend class SimpleMesh;

protected:
  // in counter-clockwise direction
  QuickList<SHalfEdgeId> m_out_he;
  std::map<SVertexId, SHalfEdgeId> m_nbh_v;
};
// todo:
} // namespace S2D
} // namespace SimpleMesh

#endif