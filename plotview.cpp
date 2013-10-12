#include <iostream>
#include <cassert>
#include <algorithm>

#include <QGraphicsItemGroup>

#include "plotview.h"
#include "signal.h"

struct PlotView::SignalWithInfo {
  QString                 signalName;
  std::unique_ptr<Signal> signal;
  QGraphicsItemGroup*     itemGroup;
  int                     position;
  int                     height;

  SignalWithInfo(QString signalName,
                 Signal* signal) : 
    signalName(signalName),
    signal(signal),
    itemGroup(0)
  {
    // nothing
  }

  ~SignalWithInfo()
  {
    delete itemGroup;
  }
};

PlotView::PlotView(QWidget *parent) :
  QGraphicsView(parent)
{
  m_scene = new QGraphicsScene(this);
  setScene(m_scene);

  connect(this, SIGNAL(signalUpdated()), this, SLOT(redrawScene()));
}

PlotView::~PlotView()
{
  // nothing
}

void PlotView::removeSignal(QString a_name)
{
  auto it = std::find_if(m_signals.begin(), m_signals.end(), [&a_name](SignalWithInfo* x){ return x->signalName == a_name; });
  if (it != m_signals.end()) {
    delete *it;
    m_signals.erase(it);
  }
  emit signalUpdated();
}

void PlotView::setSignal(QString a_name, Signal* a_signal)
{
  removeSignal(a_name);
  m_signals.push_back(new SignalWithInfo(a_name, a_signal));
  emit signalUpdated();
}

void PlotView::drawSignal(SignalWithInfo* si)
{
  const Signal& signal = *si->signal.get();
  bool     state    = true;
  unsigned prevTime = 0;

  if (si->itemGroup) {
    m_scene->destroyItemGroup(si->itemGroup);
  }
  si->itemGroup = new QGraphicsItemGroup();
  m_scene->addItem(si->itemGroup);

  auto addLine = [&](unsigned t0, unsigned t1, bool y0, bool y1) {
    new QGraphicsLineItem(t0 * mc_widthScale, si->position + y0 * mc_heightScale, 
                          t1 * mc_widthScale, si->position + y1 * mc_heightScale,
                          si->itemGroup);
  };

  for (auto curTime: signal.tds()) {
    addLine(prevTime, curTime, !state, !state);
    addLine(curTime, curTime, 0, 1);
    prevTime = curTime;
    state = !state;
  }
}


void PlotView::redrawScene()
{
  if (m_signals.begin() != m_signals.end()) {
    int width = *(m_signals[0]->signal->tds_end() - 1) * mc_widthScale;
    int height = mc_heightScale * m_signals.size();

    std::cerr << "width: " << width << std::endl;
    std::cerr << "height: " << height << std::endl;

    // QGraphicsScene* newScene = new QGraphicsScene(0, 0, width, height, this);
    // QGraphicsScene* oldScene = m_scene;
    // m_scene = newScene;
    m_scene->setSceneRect(0, 0, width, height);

    int yOfs = 0;
    for (auto si: m_signals) {
      si->position = yOfs;
      si->height = mc_heightScale;
      drawSignal(si);
      yOfs += mc_heightScale + 10;
    }

    // setScene(newScene);
    // delete oldScene;

    setSceneRect(0, 0, width, height);
  } else {
    // hmm..
  }
}
