#include <iostream>
#include "condition.h"

ConditionSession::ConditionSession()
{
  // nothing
}

ConditionSession::~ConditionSession()
{
  // nothing
}

ConditionCheckFactory::ConditionCheckFactory()
{
  // nothing
}

ConditionCheckFactory::~ConditionCheckFactory()
{
  // nothing
}

namespace {
  struct StateLengthCondition: public ConditionSession {
    StateLengthCondition(bool a_state, unsigned a_min, unsigned a_max) :
      m_state(a_state), m_min(a_min), m_max(a_max), m_prev(0)
    {
      // nothing
    }
    ~StateLengthCondition() {}

    bool operator()(TDS a_sample, bool a_state) override {
      unsigned delta = a_sample - m_prev;
      m_prev = a_sample;
      return (a_state == m_state && delta >= m_min && delta <= m_max);
    }

    bool     m_state;
    unsigned m_min;
    unsigned m_max;
    TDS      m_prev;
  };
}

StateLengthCheckFactory::StateLengthCheckFactory(bool a_state, unsigned a_min, unsigned a_max) :
  m_state(a_state), m_min(a_min), m_max(a_max)
{
  // nothing
}

StateLengthCheckFactory::~StateLengthCheckFactory()
{
  // nothing
}

ConditionSession*
StateLengthCheckFactory::start() const
{
  return new StateLengthCondition(m_state, m_min, m_max);
}
