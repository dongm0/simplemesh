#ifndef MESHENTITY_H
#define MESHENTITY_H

#include "meshentitybase.hpp"
#include "property.hpp"

namespace SimpleMesh {
namespace S2D {
class SiVertex : public SVertex {
public:
  int a;
};
template <> struct SEntityPropInfo<SiVertex> : SEntityPropInfoBase<SiVertex> {
  static constexpr FieldList props{Field{"a", &SiVertex::a}};
};
} // namespace S2D
} // namespace SimpleMesh

#endif