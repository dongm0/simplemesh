
#include "2Dmesh/meshentity.hpp"
#include "core/quicklist.hpp"
#include <iostream>

int main() {
  SimpleMesh::S2D::SHalfEdgeId id0(0);
  SimpleMesh::S2D::SHalfEdgeId id1(1);
  SimpleMesh::S2D::SHalfEdgeId id2(2);
  SimpleMesh::S2D::SHalfEdgeId id3(3);
  SimpleMesh::S2D::SHalfEdgeId id4(4);
  SimpleMesh::S2D::SHalfEdgeId id5(5);
  std::vector<SimpleMesh::S2D::SHalfEdgeId> ids = {id0, id1, id2};
  SimpleMesh::S2D::SCell<SimpleMesh::S2D::SMeshType::Quad> c1(ids);
  
  
  return 0;
}
