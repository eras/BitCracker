#include "signal.h"

Signal::Signal(const TDSVector &a_data) :
  m_data(a_data)
{
  // nothing
}

Signal::~Signal()
{
  // nothing
}

const TDSVector&
Signal::get()
{
  return m_data;
}

TDSVector::const_iterator
Signal::tds_begin() const
{
  return m_data.begin();
}

TDSVector::const_iterator
Signal::tds_end() const
{
  return m_data.end();
}

Range<TDSVector::const_iterator>
Signal::tds() const
{
  return mkRange(tds_begin(), tds_end());
}
