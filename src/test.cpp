
#include "2Dmesh/meshentity.hpp"
#include "core/quicklist.hpp"
#include <iostream>

int main() {
  SimpleMesh::S2D::SCellId id1(0);
  SimpleMesh::S2D::SCellId id2(1);
  SimpleMesh::S2D::SVertexId id444(0);
  QuickList<int> list;
  list.PushBack(0);
  list.PushBack(1);
  list.PushBack(2);
  list.PushBack(3);
  list.PushBack(4);
  list.PopBack();
  list.PopFront();
  list.PushFront(988);
  list.PutAfter(988, 2);
  list.Erase(list.Find(2));

  for (auto it = list.begin(); it != list.end(); ++it) {
    std::cout << *it << std::endl;
  }
  return 0;
}
