#ifndef CONDITION_H
#define CONDITION_H

#include "signal.h"

class ConditionSession {
public:
  ConditionSession();
  virtual ~ConditionSession();

  virtual bool operator()(TDS a_sample, bool state) = 0;
};

class ConditionCheckFactory {
public:
  ConditionCheckFactory();
  virtual ~ConditionCheckFactory();
 
  virtual ConditionSession* start() const = 0;
};

class StateLengthCheckFactory: public ConditionCheckFactory {
public:
  StateLengthCheckFactory(bool a_state, unsigned a_min, unsigned a_max);
  ~StateLengthCheckFactory();

  ConditionSession* start() const override;

private:
  bool     m_state;
  unsigned m_min;
  unsigned m_max;
};


#endif // CONDITION_H
