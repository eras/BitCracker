#ifndef SIGNAL_H
#define SIGNAL_H

#include <QObject>
#include <vector>

#include "range.h"

typedef unsigned TDS;
typedef std::vector<TDS> TDSVector;

class Signal: public QObject {
  Q_OBJECT
public:
  typedef TDSVector::const_iterator tds_iterator;

  Signal(const TDSVector& a_data);
  ~Signal();

  Range<tds_iterator> tds() const;

  tds_iterator tds_begin() const;
  tds_iterator tds_end() const;

  tds_iterator findTime(TDS a_at) const;

  const TDSVector& get();

 private:
  const TDSVector m_data;
};

#endif // SIGNAL_H
