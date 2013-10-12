#ifndef RANGE_H
#define RANGE_H

template <typename T>
class Range {
public:
  Range(T a_begin, T a_end) : m_begin(a_begin), m_end(a_end) {}

  T begin() { return m_begin; };
  T end() { return m_end; };

private:
  T m_begin;
  T m_end;
};

template <typename T> Range<T> mkRange(T a_begin, T a_end)
{
  return Range<T>(a_begin, a_end);
}

#endif // RANGE_H
