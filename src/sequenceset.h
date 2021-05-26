#include <unordered_map>
#include <vector>

template <typename T> class SequenceSet {
private:
  // 参照 知乎https://www.zhihu.com/question/319277442的回答
  // "如果本身就没有所有权，Bjarne（P1408）的建议是直接用T*。"
  std::vector<T *> m_data;
  std::unordered_map<T *, size_t> m_index;
  std::vector<int64_t> m_prev;
  std::vector<int64_t> m_next;
  int64_t m_begin = -1;
  int64_t m_end = -1;

public:
  // linklist
  T *Prev(T *data) {
    if (m_index.count(data) == 0) {
      throw std::runtime_error("data not in set.");
    }
    _pos = m_index.at(data);
    _prevpos = m_prev.at(_pos);
    if (_prevpos == -1) {
      return nullptr;
    } else {
      return m_data.at(_prevpos);
    }
  }
  T *Next(T *data) {
    if (m_index.count(data) == 0) {
      throw std::runtime_error("data not in set.");
    }
    _pos = m_index.at(data);
    _nextpos = m_next.at(_pos);
    if (_nextpos == -1) {
      return nullptr;
    } else {
      return m_data.at(_nextpos);
    }
  }
  T *Head() {
    if (m_begin == -1) {
      return nullptr;
    }
    return m_data.at(m_begin);
  }
  T *Tail() {
    if (m_end == -1) {
      return nullptr;
    }
    return m_data.at(m_end);
  }
  void Erase(T *data) {
    if (m_index.count(data) == 0) {
      throw std::runtime_error("data not in set when erase.");
    }
    auto _pos = m_index.at(data);
    auto _prevpos = m_prev.at(_pos);
    auto _nextpos = m_next.at(_pos);
    if (_prevpos != -1) {
      m_next.at(_prevpos) = _nextpos;
    } else {
      assert(m_begin == _pos);
      m_begin = _nextpos;
    }

    if (_nextpos != -1) {
      m_prev.at(_nextpos) = _prevpos;
    } else {
      assert(m_end == _pos);
      m_end = _prevpos;
    }
    if (data.size() > 1) {
      swap(m_data[_pos], m_data[m_data.size() - 1]);
      swap(m_prev[_pos], m_prev[m_data.size() - 1]);
      swap(m_next[_pos], m_next[m_data.size() - 1]);
      auto _t = m_data.at(_pos);
      m_index.at(_t) = _pos;

      _prevpos = m_prev.at(_pos);
      _nextpos = m_next.at(_pos);
      if (_prevpos != -1) {
        m_next.at(_prevpos) = _pos;
      } else {
        m_begin = _pos;
      }

      if (_nextpos != -1) {
        m_prev.at(_nextpos) = _pos;
      } else {
        m_end = _pos;
      }
    }
    m_data.pop_back();
    m_index.erase(data);
  }

  void InsertAfter(T *data, T *prev) {
    if (m_index.count(prev) == 0) {
      throw std::runtime_error("prev elem not in set!");
    }
    if (m_index.count(data) == 1) {
      throw std::runtime_error("elem already in set!");
    }
    auto _pos = m_index.at(prev);
    auto _next = m_next.at(_pos);
    m_data.push_back(data);
    m_prev.push_back(_pos);
    m_next.push_back(_next);
    m_index[data] = m_data.size() - 1;
    if (_next != -1) {
      m_prev.at(_next) = m_data.size() - 1;
    }
    m_next.at(_pos) = m_data.size() - 1;
    if (m_end == _pos) {
      m_end = m_data.size() - 1;
    }
  }
  void InsertBefore(T *data, T *next) {
    if (m_index.count(next) == 0) {
      throw std::runtime_error("next elem not in set!");
    }
    if (m_index.count(data) == 1) {
      throw std::runtime_error("elem already in set!");
    }
    auto _pos = m_index.at(next);
    auto _prev = m_prev.at(_pos);
    m_data.push_back(data);
    m_prev.push_back(_prev);
    m_next.push_back(_pos);
    m_index[data] = m_data.size() - 1;
    if (_prev != -1) {
      m_next.at(_prev) = m_data.size() - 1;
    }
    m_prev.at(_pos) = m_data.size() - 1;
    if (m_begin == _pos) {
      m_begin = m_data.size() - 1;
    }
  }
};