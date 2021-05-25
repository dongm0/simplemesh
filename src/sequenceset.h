#include <unordered_map>
#include <vector>

template <typename T> class SequenceSet {
private
  std::vector<T> m_data;
  std::unordered_map<T, size_t> m_index;
  std::vector<size_t> m_prev;
  std::vector<size_t> m_next;
  int64_t m_begin = -1;
  int64_t m_end = -1;

public:
  void push_back(T data) {
    assert(m_index.count(data) == 0);
    m_index[data] = m_data.size();
    m_data.push_back(data);
    m_prev.push_back(m_end);
    m_next.push_back(-1);
    if (m_end != -1) {
      m_next.at(m_end) = m_index[data];
    }
    m_end = m_index[data];
  }
  void erasr(T data) {
    if (m_index.count(data) == 0)) {
        return;
    }
  }
}