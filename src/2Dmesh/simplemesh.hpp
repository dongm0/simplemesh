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
  const SVertex &_VEntity(SVertexId v_id) const {
    if (auto _vit = m_v.find(v_id); _vit != m_v.end()) {
      return (*_vit).second;
    } else {
      assert(false, "vertexid not in mesh.");
    }
  }
  const SCell &_CEntity(SCellId c_id) const {
    if (auto _cit = m_c.find(c_id); _cit != m_c.end()) {
      return (*_cit).second;
    } else {
      assert(false, "cellid not in mesh.");
    }
  }
  const SEdge &_EEntity(SEdgeId e_id) const {
    if (auto _eit = m_e.find(e_id); _eit != m_e.end()) {
      return (*_eit).second;
    } else {
      assert(false, "edgeid not in mesh.");
    }
  }
  const SHalfEdge &_HEEntity(SHalfEdgeId he_id) const {
    if (auto _heit = m_he.find(he_id); _heit != m_he.end()) {
      return (*_heit).second;
    } else {
      assert(false, "halfedgeid not in mesh.");
    }
  }

public:
  // void ReadFromFile(const std::string &filename);
  // void WriteToFile(const std::string &filename) const;

  // 不写迭代器

public:
  uint32_t NumVertices() const { return m_v.size(); }
  uint32_t NumCells() const { return m_c.size(); }
  uint32_t NumHalfEdges() const { return m_he.size(); }
  uint32_t NumEdges() const { return m_e.size(); }

  std::vector<SVertexId> CellHalfEdges(SCellId cell_id) const {
    return _CEntity(cell_id).m_half_edges;
  }
  std::vector<SVertexId> CellVertices(SCellId cell_id) const {
    std::vector<SVertexId> _res;
    for (auto _heid : _CEntity(cell_id).m_half_edges) {
      _res.push_back(_HEEntity(_heid).m_st_v);
    }
    return _res;
  }
  std::vector<std::array<SVertexId, 3>> CellCorners(SCellId cell_id) const {
    std::vector<SVertexId> _tmp;
    for (auto _heid : _CEntity(cell_id).m_half_edges) {
      _tmp.push_back(_HEEntity(_heid).m_st_v);
    }
    std::vector<std::array<SVertexId, 3>> _res{{_tmp[3], tmp[0], tmp[1]},
                                               {_tmp[0], tmp[1], tmp[2]},
                                               {_tmp[1], tmp[2], tmp[3]},
                                               {_tmp[2], tmp[3], tmp[0]}};
    return _res;
  }
  // Eigen::Vector2d GetVertexCoord(int32_t vertex_id) const;
  SCellId AdjecentCell(SCellId cell_id, SHalfEdgeId half_edge_id) const {
    auto _centity = _CEntity(cell_id);
    auto _heentity = _HEEntity(half_edge_id);
    auto _opposite_heentity = _HEEntity(_heentity.m_opposite);
    return _opposite_heentity.m_instance_cell;
  }
  template <SMeshType _Mty>
  SHalfEdgeId OppositeHalfEdgeInCell(SCellId cell_id,
                                     SHalfEdgeId half_edge_id) const {
    throw std::runtime_error("only support quad mesh.");
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
    if (_pos != -1) {
      throw std::runtime_error("halfedge not in cell!");
    }
    return m_c.at(cell_id).m_half_edges.at((i + 2) % 4);
  }
  SHalfEdgeId OppositeHalfEdge(SHalfEdgeId half_edge_id) const {
    auto _heentity = _HEEntity(half_edge_id);
    return _heentity.m_opposite;
  }
  SEdgeId HalfEdgeInstanceEdge(SHalfEdgeId half_edge_id) const {
    return _HEEntity(half_edge_id).m_instance_edge;
  }
  SCellId HalfEdgeInstanceCell(SHalfEdgeId half_edge_id) const {
    return _HEEntity(half_edge_id).m_instance_cell;
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
  SVertexId AddVertex() {
    SVertexId res_id(m_v.size());
    m_v.insert(std::make_pair(res_id, SVertex()));
    return res_id;
  }
  SHalfEdgeId AddHalfEdge(SVertexId st_v_id, SVertexId ed_v_id) {
    for (auto _heid : _VEntity(st_v_id).m_out_he) {
      if (_HEEntity(_heid).m_ed_v == ed_v_id) {
        throw std::runtime_error("halfedge already exist!");
      }
    }
    SHalfEdgeId res_id(m_he.size());
    SHalfEdgeId res_oppo_id(m_he.size() + 1);
    SEdgeId edge_id(m_e.size());
    m_he.insert(std::make_pair(res_id, SHalfEdge(st, ed, res_id, edge_id)));
    m_he.insert(
        std::make_pair(res_oppo_id, SHalfEdge(ed, st, res_oppo_id, edge_id)));
    m_e.insert(std::make_pair(edge_id, SEdge(res_id, res_oppo_id)));
    m_v.at(st_v_id).m_out_he.PushBack(res_id);
    m_v.at(ed_v_id).m_out_he.PushBack(res_oppo_id);
    return res_id;
  }
  // todo:
  // - 添加id有效的检测（预想可以设置为宏）
  SCellId AddCell(int32_t heid1, int32_t heid2, int32_t heid3, int32_t heid4);
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
