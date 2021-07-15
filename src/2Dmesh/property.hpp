#ifndef PROPERTY_HPP
#define PROPERTY_HPP

#include <string>
#include <tuple>

namespace SimpleMesh {
namespace S2D {
enum class SEntityType;
template <SEntityType _Sty> class SEntityId;

template <typename T> struct Field {
  using value_type = T;
  std::string_view name;
  T value;
  constexpr Field(std::string_view n, T v) : value{v}, name{n} {}
};

template <typename... Args> struct FieldList {
  std::tuple<Args...> fields;
  static constexpr size_t size = sizeof...(Args);
  constexpr FieldList(Args... args) : fields{args...} {}

  constexpr int innerFindProp(std::string_view n, std::index_sequence<>) const {
    return -1;
  }
  template <size_t N0, size_t... Ns>
  constexpr int innerFindProp(std::string_view n,
                              std::index_sequence<N0, Ns...>) const {
    return (std::get<N0>(fields).name == n
                ? N0
                : innerFindProp(n, std::index_sequence<Ns...>{}));
  }
  constexpr int FindProp(std::string_view n) const {
    return innerFindProp(n, std::make_index_sequence<size>{});
  }

  template <typename T, int pos> constexpr auto GetProp() const {
    return std::get<pos>(fields).value;
  }
};

template <typename T> struct SEntityPropInfoBase { using type = T; };

template <typename T> struct SEntityPropInfo;

template <typename E, typename V> class SEntityProperty {
private:
  E::V *m_val;
  SMesh *m_mesh;

public:
  template <typename E, typename V>
  constexpr SEntityProperty(std::string_view n) {
    constexpr int pos = SEntityPropInfo<E>::props.FindProp(n);
    val = SEntityPropInfo<E>::props.GetProp<V, pos>();
  }
  template <typename E, typename V> V &operator[](const E::id_type &id) {
    return m_mesh->GetEntity(id).*val;
  }
};

}; // namespace S2D
}; // namespace SimpleMesh

#endif