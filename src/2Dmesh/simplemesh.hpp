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
template <SMeshType _Mty, typename V, typename HE, typename E, typename C>
class SBaseMesh {
  using Vtype = V;
  using HEtype = HE;
  using Etype = E;
  using Ctype = C;
  using VIDtype = V::id_type;
  using HEIDtype = HE::id_type;
  using EIDtype = E::id_type;
  using CIDtype = C::id_type;

private:
  std::unordered_map<VIDtype, Vtype> m_v;
  std::unordered_map<CIDtype, Ctype<_Mty>> m_c;
  std::unordered_map<HEIDtype, HEtype> m_he;
  std::unordered_map<EIDtype, Etype> m_e;

private:
  const Vtype &_VEntity(VIDtype v_id) const {
    if (auto _vit = m_v.find(v_id); _vit != m_v.end()) {
      return (*_vit).second;
    } else {
      assert(false, "vertexid not in mesh.");
    }
  }
  const Ctype &_CEntity(CIDtype c_id) const {
    if (auto _cit = m_c.find(c_id); _cit != m_c.end()) {
      return (*_cit).second;
    } else {
      assert(false, "cellid not in mesh.");
    }
  }
  const Etype &_EEntity(EIDtype e_id) const {
    if (auto _eit = m_e.find(e_id); _eit != m_e.end()) {
      return (*_eit).second;
    } else {
      assert(false, "edgeid not in mesh.");
    }
  }
  const HEtype &_HEEntity(HEIDtype he_id) const {
    if (auto _heit = m_he.find(he_id); _heit != m_he.end()) {
      return (*_heit).second;
    } else {
      assert(false, "halfedgeid not in mesh.");
    }
  }

public:
  size_t NumVertices() const { return m_v.size(); }
  size_t NumCells() const { return m_c.size(); }
  size_t NumHalfEdges() const { return m_he.size(); }
  size_t NumEdges() const { return m_e.size(); }

public:
  std::vector<VIDtype> CellHalfEdges(const CIDtype &cell_id) const {
    return _CEntity(cell_id).m_half_edges;
  }
  std::vector<VIDtype> CellVertices(const CIDtype &cell_id) const {
    std::vector<VIDtype> _res;
    for (auto _heid : _CEntity(cell_id).m_half_edges) {
      _res.push_back(_HEEntity(_heid).m_st_v);
    }
    return _res;
  }
  std::vector<std::array<VIDtype, 3>>
  CellCorners(const CIDtype &cell_id) const {
    std::vector<VIDtype> _tmp;
    for (auto _heid : _CEntity(cell_id).m_half_edges) {
      _tmp.push_back(_HEEntity(_heid).m_st_v);
    }
    std::vector<std::array<VIDtype, 3>> _res{{_tmp[3], tmp[0], tmp[1]},
                                             {_tmp[0], tmp[1], tmp[2]},
                                             {_tmp[1], tmp[2], tmp[3]},
                                             {_tmp[2], tmp[3], tmp[0]}};
    return _res;
  }

  std::vector<VIDtype> VertexVertices(const VIDtype &vid) const {
    std::vector<VIDtype> res;
    for (auto _he : _VEntity(vid).m_out_he) {
      res.push_back(_HEEntity(_he).m_ed_v);
    }
    return res;
  }
  std::vector<HEIDtype> VertexOutHalfEdges(const VIDtype &vid) const {
    std::vector<HEIDtype> res;
    for (auto _he : _VEntity(vid).m_out_he) {
      res.push_back(_he);
    }
    return res;
  }
  std::vector<CIDtype> VertexCells(const VIDtype &vid) const {
    std::vector<CIDtype> res;
    for (auto _he : _VEntity(vid).m_out_he) {
      res.push_back(_HEEntity(_he).m_instance_cell);
    }
    return res;
  }

  CIDtype AdjecentCell(const CIDtype &cell_id,
                       const HEIDtype &half_edge_id) const {
    auto _centity = _CEntity(cell_id);
    auto _heentity = _HEEntity(half_edge_id);
    auto _opposite_heentity = _HEEntity(_heentity.m_opposite);
    return _opposite_heentity.m_instance_cell;
  }
  template <SMeshType _Mty>
  HEIDtype OppositeHalfEdgeInCell(const CIDtype &cell_id,
                                  const HEIDtype &half_edge_id) const {
    throw std::runtime_error("only support quad mesh.");
  }
  template <>
  HEIDtype
  OppositeHalfEdgeInCell<SMeshType::Quad>(const CIDtype &cell_id,
                                          const HEIDtype &half_edge_id) const {
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
  HEIDtype OppositeHalfEdge(const HEIDtype &half_edge_id) const {
    auto _heentity = _HEEntity(half_edge_id);
    return _heentity.m_opposite;
  }
  EIDtype HalfEdgeInstanceEdge(const HEIDtype &half_edge_id) const {
    return _HEEntity(half_edge_id).m_instance_edge;
  }
  CIDtype HalfEdgeInstanceCell(const HEIDtype &half_edge_id) const {
    return _HEEntity(half_edge_id).m_instance_cell;
  }

public:
  VIDtype AddVertex() {
    VIDtype res_id(m_v.size());
    m_v.insert(std::make_pair(res_id, Vtype()));
    return res_id;
  }
  EIDtype AddEdge(VIDtype vid1, VIDtypd vid2) {
    if (_VEntity(vid1).m_nbh_v.count(vid2) != 0) {
      auto _heid =
          *(_VEntity(vid1).m_out_he.Find(_VEntity(vid1).m_nbh_v[vid2]));
      return HalfEdgeInstanceEdge(_heid);
    }
    EIDtype res_id(m_e.size());
    HEIDtype he1_id(m_he.size());
    HEIDtype he2_id(m_he.size() + 1);
    m_he[he1_id] = HEtype(vid1, vid2, he1_id, res_id);
    m_he[he2_id] = HEtype(vid2, vid1, he2_id, res_id);
    m_e[res_id] = Etype(he1_id, he2_id);

    m_v.at(vid1).m_out_he.PushBack(he1_id);
    m_v.at(vid2).m_out_he.PushBack(he2_id);
    m_v.at(vid1).m_nbh_v.insert({vid2, he1_id});
    m_v.at(vid2).m_nbh_v.insert({vid1, he2_id});
    return res_id;
  }
  /*
  HEIDtype AddHalfEdge(VIDtype st_v_id, VIDtype ed_v_id) {
    if (_VEntity(st_v_id).m_nbh_v.count(ed_v_id) != 0) {
      throw std::runtime_error("halfedge already exist!");
    }
    HEIDtype res_id(m_he.size());
    HEIDtype res_oppo_id(m_he.size() + 1);
    EIDtype edge_id(m_e.size());
    m_he.insert(std::make_pair(res_id, HEtype(st, ed, res_id, edge_id)));
    m_he.insert(
        std::make_pair(res_oppo_id, HEtype(ed, st, res_oppo_id, edge_id)));
    m_e.insert(std::make_pair(edge_id, Etype(res_id, res_oppo_id)));
    m_v.at(st_v_id).m_out_he.PushBack(res_id);
    m_v.at(ed_v_id).m_out_he.PushBack(res_oppo_id);
    m_v.at(st_v_id).m_nbh_v.insert({ed_v_id, res_id});
    m_v.at(ed_v_id).m_nbh_v.insert({st_v_id, res_oppo_id});
    return res_id;
  }
  */
  // todo:
  // - 添加id有效的检测（预想可以设置为宏）

  // edge can be added freely, but cell needs check
  // halfedge和edge必须绑定，但是cell不一样
  CIDtype AddCell(const std::vector<HEIDtype> &heids) {
    for (size_t i = 0; i < heids.size(); ++i) {
      if (_HEEntity(heids[i]).m_ed_v !=
          _HEEntity(heids[(i + 1) % heids.size()]).m_st_v) {
        throw std::runtime_error("halfedges not form a circle!");
      }
      if (_HEEntity(heids[i]).m_instance_cell != -1)
    }
    Ctype _c(heids);
    CIDtype res_id(m_c.size());
    for (auto _heid : heids) {
      m_he[_heid].m_instance_cell = res_id;
    }
    m_c.insert(std::make_pair(res_id, _c));
    return res_id;
  }
  CIDtype AddCell(const std::vector<VIDtype> &vids) {
    std::vector<HEIDtype> heids;
    for (size_t i = 0; i < vids.size(); ++i) {
      if (_VEntity(vids[i]).m_nbh_v.count(vids[(i + 1) % vids.size()]) == 0) {
        heids.push_back(_EEntity(AddEdge(vids[i], vids[(i + 1) % vids.size()]))
                            .m_half_edges[0]);
      } else {
        heids.push_back(
            _VEntity(vids[i]).m_nbh_v.at(vids[(i + 1) % vids.size()]));
      }
    }
    Ctype _c(heids);
    CIDtype res_id(m_c.size());
    for (auto _heid : heids) {
      m_he[_heid].m_instance_cell = res_id;
    }
    m_c.insert(std::make_pair(res_id, _c));
    return res_id;
  }
  void clear();

private:
  // 可以有，但现在用不到也不会写
  bool CheckTopologyLegal() const;

private:
};
} // namespace S2D
} // namespace SimpleMesh

#endif
