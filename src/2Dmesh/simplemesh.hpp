#ifndef SIMPLEMESH_H
#define SIMPLEMESH_H

#include "../core/quicklist.hpp"
#include "meshentity.hpp"
#include <Eigen/Dense>
#include <array>
#include <set>
#include <vector>

namespace SimpleMesh {
namespace S2D {
template <SMeshType _Mty> class TopoMesh {
private:
  std::unordered_map<SVertexId, SVertex> m_v;
  std::unordered_map<SCellId, SCell<_Mty>> m_c;
  std::unordered_map<SHalfEdgeId, SHalfEdge> m_he;
  std::unordered_map<SEdgeId, SEdge> m_e;

private:
  SVertex VEntity(SVertexId v_id) const {
    if (auto _vit = m_v.find(v_id); _vit != m_v.end()) {
      return (*_vit).second;
    } else {
      assert(false, "vertexid not in mesh.");
    }
  }
  SCell CEntity(SCellId c_id) const {
    if (auto _cit = m_c.find(c_id); _cit != m_c.end()) {
      return (*_cit).second;
    } else {
      assert(false, "cellid not in mesh.");
    }
  }
  SEdge EEntity(SEdgeId e_id) const {
    if (auto _eit = m_e.find(e_id); _eit != m_e.end()) {
      return (*_eit).second;
    } else {
      assert(false, "edgeid not in mesh.");
    }
  }
  SHalfEdge HEEntity(SHalfEdgeId he_id) const {
    if (auto _heit = m_he.find(he_id); _heit != m_he.end()) {
      return (*_heit).second;
    } else {
      assert(false, "halfedgeid not in mesh.");
    }
  }

public:
  // void ReadFromFile(const std::string &filename);
  // void WriteToFile(const std::string &filename) const;

public:
  uint32_t NumVertices() const { return m_v.size(); }
  uint32_t NumCells() const { return m_c.size(); }
  uint32_t NumHalfEdges() const { return m_he.size(); }
  uint32_t NumEdges() const { return m_e.size(); }

  std::vector<SVertexId> CellHalfEdges(SCellId cell_id) const {
    return CEntity(cell_id).m_half_edges;
  }
  std::vector<SVertexId> CellVertices(SCellId cell_id) const {
    std::vector<SVertexId> _res;
    for (auto _heid : CEntity(cell_id).m_half_edges) {
      _res.push_back(HEEntity(_heid).m_st_v);
    }
    return _res;
  }
  std::vector<std::array<SVertexId, 3>> CellCorners(SCellId cell_id) const {
    std::vector<SVertexId> _tmp;
    for (auto _heid : CEntity(cell_id).m_half_edges) {
      _tmp.push_back(HEEntity(_heid).m_st_v);
    }
    std::vector<std::array<SVertexId, 3>> _res{{_tmp[3], tmp[0], tmp[1]},
                                               {_tmp[0], tmp[1], tmp[2]},
                                               {_tmp[1], tmp[2], tmp[3]},
                                               {_tmp[2], tmp[3], tmp[0]}};
    return _res;
  }
  // Eigen::Vector2d GetVertexCoord(int32_t vertex_id) const;
  SCellId AdjecentCell(SCellId cell_id, SHalfEdgeId half_edge_id) const {
    auto _centity = CEntity(cell_id);
    auto _heentity = HEEntity(half_edge_id);
    auto _opposite_heentity = HEEntity(_heentity.m_opposite);
    return _opposite_heentity.m_instance_cell;
  }
  template <SMeshType _Mty>
  SHalfEdgeId OppositeHalfEdgeInCell(SCellId cell_id,
                                     SHalfEdgeId half_edge_id) const {
    assert(false, "only support quad mesh.");
  }
  template <>
  SHalfEdgeId
  OppositeHalfEdgeInCell<SMeshType::Quad>(SCellId cell_id,
                                          SHalfEdgeId half_edge_id) const {
    int _pos = -1;
    for (int i = 0; i < 4; ++i) {
      if (m_c.at(cell_id).m_half_edges.at(i) == half_edge_id) {
        _pos = i;
        break;
      }
    }
    assert(_pos != -1, "halfedge not in cell!");
    return m_c.at(cell_id).m_half_edges.at((i + 2) % 4);
  }
  SHalfEdgeId OppositeHalfEdge(SHalfEdgeId half_edge_id) const {
    auto _heentity = HEEntity(half_edge_id);
    return _heentity.m_opposite;
  }
  SEdgeId HalfEdgeInstanceEdge(SHalfEdgeId half_edge_id) const {
    return HEEntity(half_edge_id).m_instance_edge;
  }
  SCellId HalfEdgeInstanceCell(SHalfEdgeId half_edge_id) const {
    return HEEntity(half_edge_id).m_instance_cell;
  }
  /*
  const SMeshCell Cell(int32_t cell_id) const;
  const SMeshEdge Edge(int32_t edge_id) const;
  const SMeshHalfEdge HalfEdge(int32_t half_edge_id) const;
  const SMeshVertex Vertex(int32_t vertex_id) const;
  */
public:
  // 先用着，再改，已有的addcell逻辑不对换成这个
  // int32_t AddCellFromVertex(int32_t vid1, int32_t vid2, int32_t vid3,
  //                          int32_t vid4);

public:
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
} // namespace S2D
} // namespace SimpleMesh

#endif
