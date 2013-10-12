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
  Signal(const TDSVector& a_data);
  ~Signal();

  Range<TDSVector::const_iterator> tds() const;

  TDSVector::const_iterator tds_begin() const;
  TDSVector::const_iterator tds_end() const;

  const TDSVector& get();

 private:
  const TDSVector m_data;
};

#endif // SIGNAL_H
