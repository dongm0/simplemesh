#include "simplemesh.h"
#include <algorithm>
#include <fstream>

void SimpleMesh::ReadFromFile(const std::string &filename) {
  std::ifstream fin;
  fin.open(filename);

  fin.close();
}

std::array<int32_t, 4> SimpleMesh::CellVertices(int32_t cell_id) {
  std::array<int32_t, 4> res;
  for (int i = 0; i < 4; ++i) {
    res.at(i) = c.at(cell_id).half_edges.at(i);
  }
  return res;
}

void SimpleMesh::AssignGeomFromMatrix(const Eigen::MatrixXd &V) {
  assert(V.rows() == coords.rows());
  assert(V.cols() == 2);
  coords = V;
}

std::array<std::array<int32_t, 3>, 4> SimpleMesh::CellCorners(int32_t cell_id) {
  std::array<std::array<int32_t, 3>, 4> res;
  for (uint8_t i = 0; i < 4; ++i) {
    std::array<int32_t, 3> _tmp{
        HalfEdge(Cell(cell_id).half_edges.at(i)).st,
        HalfEdge(Cell(cell_id).half_edges.at((i + 1) % 4)).st,
        HalfEdge(Cell(cell_id).half_edges.at((i + 3) % 4)).st};
    res.at(i) = _tmp;
  }
  return res;
}

Eigen::Vector2d SimpleMesh::GetVertexCoord(int32_t vertex_id) {
  return coords.row(vertex_id);
}

int32_t SimpleMesh::AdjecentCell(int32_t cell_id, int32_t half_edge_id) {
  return he.at(he.at(half_edge_id).opposite).instance_cell;
}

int32_t SimpleMesh::OppositeHalfEdgeInCell(int32_t cell_id,
                                           int32_t half_edge_id) {
  uint8_t _posid = 0;
  for (uint8_t i = 0; i < 4; ++i) {
    if (c.at(cell_id).half_edges.at(i) == half_edge_id) {
      _posid = i;
      break;
    }
  }
  return c.at(cell_id).half_edges.at((half_edge_id + 2) % 4);
}
int32_t SimpleMesh::OppositeHalfEdge(int32_t half_edge_id) {
  return he.at(half_edge_id).opposite;
}
int32_t SimpleMesh::HalfEdgeInstanceEdge(int32_t half_edge_id) {
  return he.at(half_edge_id).instance_edge;
}
int32_t SimpleMesh::HalfEdgeInstanceCell(int32_t half_edge_id) {
  return he.at(half_edge_id).instance_cell;
}

// 返回新vertex的id
int32_t SimpleMesh::AddVertex() {
  v.push_back(v.size());
  return int32_t(v.size() - 1);
}
int32_t SimpleMesh::AddHalfEdge(int32_t st_v_id, int32_t ed_v_id) {
  he.push_back({st_v_id, ed_v_id, -1, 0, -1, -1, -1});
  return int32_t(he.size() - 1);
}
int32_t SimpleMesh::AddEdge(int32_t he1_id, int32_t he2_id) {
  e.push_back({std::array<int32_t, 2>{he1_id, he2_id}});
  he.at(he1_id).opposite = he2_id;
  int32_t ne_id = e.size() - 1;
  he.at(he1_id).instance_edge = ne_id;
  he.at(he2_id).opposite = he1_id;
  he.at(he2_id).instance_edge = ne_id;
  return ne_id;
}
int32_t SimpleMesh::AddCell(int32_t heid1, int32_t heid2, int32_t heid3,
                            int32_t heid4) {
  c.push_back({std::array<int32_t, 4>{heid1, heid2, heid3, heid4}});
  int32_t cid = c.size() - 1;
  he.at(heid1).instance_cell = cid;
  he.at(heid2).instance_cell = cid;
  he.at(heid3).instance_cell = cid;
  he.at(heid4).instance_cell = cid;
  return cid;
}
int32_t SimpleMesh::SetHalfEdge(int32_t half_edge_id, int32_t st_v_id,
                                int32_t ed_v_id) {
  auto &_he_entity = he.at(half_edge_id);
  _he_entity.st = st_v_id;
  _he_entity.ed = ed_v_id;
  return half_edge_id;
}
int32_t SimpleMesh::SetEdge(int32_t edge_id, int32_t he1_id, int32_t he2_id) {
  auto &_e_entity = e.at(edge_id);
  _e_entity.half_edges[0] = he1_id;
  _e_entity.half_edges[1] = he2_id;
  he.at(he1_id).opposite = he2_id;
  he.at(he1_id).instance_edge = edge_id;
  he.at(he2_id).opposite = he1_id;
  he.at(he2_id).instance_edge = edge_id;
  return edge_id;
}
int32_t SimpleMesh::SetCell(int32_t cell_id, int32_t heid1, int32_t heid2,
                            int32_t heid3, int32_t heid4) {
  auto &_c_entity = c.at(cell_id);
  _c_entity.half_edges[0] = heid1;
  _c_entity.half_edges[1] = heid2;
  _c_entity.half_edges[2] = heid3;
  _c_entity.half_edges[3] = heid4;
  he.at(heid1).instance_cell = cell_id;
  he.at(heid2).instance_cell = cell_id;
  he.at(heid3).instance_cell = cell_id;
  he.at(heid4).instance_cell = cell_id;
  return cell_id;
}

void SimpleMesh::Subdivide(const std::vector<int32_t> &tagged) {
  std::set<int32_t> splited_edges;
  for (auto _cid : tagged) {
    for (uint32_t i = 0; i < 4; ++i) {
      auto _heid = c.at(_cid).half_edges.at(i);
      auto _cid_c = _cid, _heid_c = _heid;
      while (AdjecentCell(_cid_c, _heid_c) != -1) {
        splited_edges.insert(HalfEdgeInstanceEdge(_heid_c));
        _cid_c = AdjecentCell(_cid_c, _heid_c);
        _heid_c = OppositeHalfEdgeInCell(_cid_c, OppositeHalfEdge(_heid_c));
      }
    }
  }

  for (auto _eid : splited_edges) {
    auto _newv = AddVertex();
    auto [_heid1, _heid2] = e.at(_eid).half_edges;
    auto _heid1_2 = AddHalfEdge(_newv, he.at(_heid1).ed);
    auto _heid2_2 = AddHalfEdge(he.at(_heid2).st, _newv);
    _heid1 = SetHalfEdge(_heid1, he.at(_heid1).st, _newv);
    _heid2 = SetHalfEdge(_heid2, _newv, he.at(_heid2).ed);
    SetEdge(_eid, _heid1, _heid2);
    AddEdge(_heid1_2, _heid2_2);
    // 下面一块考虑后面单独写操作，opposite放到edge里处理
    he.at(_heid1).split = 1;
    he.at(_heid1).split_relevancy = _heid1_2;
    he.at(_heid1_2).split = 2;
    he.at(_heid1).split_relevancy = _heid1;
    he.at(_heid2).split = 2;
    he.at(_heid2).split_relevancy = _heid2_2;
    he.at(_heid2_2).split = 1;
    he.at(_heid2_2).split_relevancy = _heid2;
    if (he.at(_heid1).instance_cell == -1) {
      he.at(_heid1).split = 0;
      he.at(_heid1_2).split = 0;
    }
    if (he.at(_heid2).instance_cell == -1) {
      he.at(_heid2).split = 0;
      he.at(_heid2_2).split = 0;
    }
  }

  for (auto _cid : tagged) {
    uint8_t _split_edge_n = 0;
    uint8_t _st_tmp = 0;
    for (uint8_t i = 0; i < 4; ++i) {
      auto _heid = c.at(_cid).half_edges.at(i);
      if (he.at(_heid).split != 0) {
        if (_split_edge_n == 0) {
          _st_tmp = i;
        }
        _split_edge_n += 1;
      }
    }
    if (_split_edge_n == 2) {
      /*
                he1
          nhe1 nhe5 nhe3
          nhe2 nhe6 nhe4
                he2
      */
      auto _heid1 = c.at(_cid).half_edges.at(_st_tmp);
      auto _nheid1 = c.at(_cid).half_edges.at(_st_tmp + 1);
      auto _nheid2 = he.at(_nheid1).split_relevancy;
      if (he.at(_nheid1).split != 1) {
#ifndef NDEBUG
        assert(he.at(_nheid1).split == 2);
#endif
        std::swap(_nheid1, _nheid2);
      }
      auto _heid2 = c.at(_cid).half_edges.at(_st_tmp + 2);
      auto _nheid4 = c.at(_cid).half_edges.at(_st_tmp + 3);
      auto _nheid3 = he.at(_nheid4).split_relevancy;
      if (he.at(_nheid4).split != 1) {
#ifndef NDEBUG
        assert(he.at(_nheid4).split == 2);
#endif
        std::swap(_nheid4, _nheid3);
      }

      auto _nheid5 = AddHalfEdge(he.at(_nheid1).ed, he.at(_nheid3).st);
      auto _nheid6 = AddHalfEdge(he.at(_nheid4).ed, he.at(_nheid2).st);
      AddEdge(_nheid5, _nheid6);
      SetCell(_cid, _heid1, _nheid1, _nheid5, _nheid3);
      AddCell(_heid2, _nheid4, _nheid6, _nheid2);
    } else if (_split_edge_n == 4) {
      /*
                nhe2 nhe1
          nhe3             nhe8
          nhe4             nhe7
                nhe5 nhe6
      */
      auto _nheid1 = c.at(_cid).half_edges.at(_st_tmp);
      auto _nheid2 = he.at(_nheid1).split_relevancy;
      if (he.at(_nheid1).split != 1) {
#ifndef NDEBUG
        assert(he.at(_nheid1).split == 2);
#endif
        std::swap(_nheid1, _nheid2);
      }
      auto _nheid3 = c.at(_cid).half_edges.at(_st_tmp + 1);
      auto _nheid4 = he.at(_nheid3).split_relevancy;
      if (he.at(_nheid3).split != 1) {
#ifndef NDEBUG
        assert(he.at(_nheid3).split == 2);
#endif
        std::swap(_nheid3, _nheid4);
      }
      auto _nheid5 = c.at(_cid).half_edges.at(_st_tmp + 2);
      auto _nheid6 = he.at(_nheid5).split_relevancy;
      if (he.at(_nheid5).split != 1) {
#ifndef NDEBUG
        assert(he.at(_nheid5).split == 2);
#endif
        std::swap(_nheid5, _nheid6);
      }
      auto _nheid7 = c.at(_cid).half_edges.at(_st_tmp + 3);
      auto _nheid8 = he.at(_nheid7).split_relevancy;
      if (he.at(_nheid7).split != 1) {
#ifndef NDEBUG
        assert(he.at(_nheid7).split == 2);
#endif
        std::swap(_nheid7, _nheid8);
      }

      auto _nvid = AddVertex();
      auto _nheid9 = AddHalfEdge(he.at(_nheid1).ed, _nvid);
      auto _nheid10 = AddHalfEdge(_nvid, he.at(_nheid2).st);
      auto _nheid11 = AddHalfEdge(he.at(_nheid3).ed, _nvid);
      auto _nheid12 = AddHalfEdge(_nvid, he.at(_nheid4).st);
      auto _nheid13 = AddHalfEdge(he.at(_nheid5).ed, _nvid);
      auto _nheid14 = AddHalfEdge(_nvid, he.at(_nheid6).st);
      auto _nheid15 = AddHalfEdge(he.at(_nheid7).ed, _nvid);
      auto _nheid16 = AddHalfEdge(_nvid, he.at(_nheid8).st);

      AddEdge(_nheid9, _nheid10);
      AddEdge(_nheid11, _nheid12);
      AddEdge(_nheid13, _nheid14);
      AddEdge(_nheid15, _nheid16);

      SetCell(_cid, _nheid1, _nheid9, _nheid16, _nheid8);
      AddCell(_nheid2, _nheid3, _nheid11, _nheid10);
      AddCell(_nheid4, _nheid5, _nheid13, _nheid12);
      AddCell(_nheid6, _nheid7, _nheid15, _nheid14);
    } else {
      assert(false);
    }
  }

  for (auto _eid : splited_edges) {
    auto _heid1 = e.at(_eid).half_edges[0];
    auto _heid2 = e.at(_eid).half_edges[1];
    auto _heid3 = he.at(_heid1).split_relevancy;
    auto _heid4 = he.at(_heid2).split_relevancy;
    he.at(_heid1).split = 0;
    he.at(_heid1).split_relevancy = -1;
    he.at(_heid2).split = 0;
    he.at(_heid2).split_relevancy = -1;
    he.at(_heid3).split = 0;
    he.at(_heid3).split_relevancy = -1;
    he.at(_heid4).split = 0;
    he.at(_heid4).split_relevancy = -1;
  }
}