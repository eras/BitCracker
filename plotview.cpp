#include <iostream>
#include <cassert>
#include <algorithm>

#include <QWheelEvent>
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
  QGraphicsView(parent), m_grid(new QGraphicsItemGroup), m_signalUs(0),
  m_zoomNumerator(1), m_zoomDenominator(1)
{
  m_scene = new QGraphicsScene(this);
  setScene(m_scene);

  m_scene->addItem(m_grid);

  connect(this, SIGNAL(signalUpdated()), this, SLOT(redraw()));
}

PlotView::~PlotView()
{
  // nothing
}

Signal*
PlotView::getSignalByIndex(int a_index)
{
  if (a_index < m_signals.size()) {
    return m_signals[a_index]->signal.get();
  } else {
    return 0;
  }
}

Signal*
PlotView::getSignalByName(QString a_name)
{
  auto it = std::find_if(m_signals.begin(), m_signals.end(), [&a_name](SignalWithInfo* x){ return x->signalName == a_name; });
  if (it != m_signals.end()) {
    return (*it)->signal.get();
  } else {
    return 0;
  }
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
    std::cerr << "Destroying group" << std::endl;
    delete si->itemGroup;
  }
  si->itemGroup = new QGraphicsItemGroup();
  m_scene->addItem(si->itemGroup);

  auto addLine = [&](unsigned t0, unsigned t1, bool y0, bool y1) {
    new QGraphicsLineItem(horizPosAt(t0), si->position + y0 * mc_heightScale, 
                          horizPosAt(t1), si->position + y1 * mc_heightScale,
                          si->itemGroup);
  };

  for (auto curTime: signal.tds()) {
    addLine(prevTime, curTime, !state, !state);
    addLine(curTime, curTime, 0, 1);
    prevTime = curTime;
    state = !state;
  }
}

void PlotView::redraw()
{
  redrawScene();
  redrawGrid();
}

unsigned PlotView::horizPosAt(TDS time)
{
  return time * mc_widthScale * m_zoomNumerator / m_zoomDenominator;
}

unsigned PlotView::vertBegin(unsigned index)
{
  return index * (mc_heightScale + 10) + 10;
}

unsigned PlotView::vertEnd(unsigned index)
{
  return (index + 1) * (mc_heightScale + 10);
}

void PlotView::wheelEvent(QWheelEvent* event)
{
  if (event->delta() > 0) {
    zoomIn();
  } else if (event->delta() < 0) {
    zoomOut();
  }
}

void PlotView::zoomIn()
{
  m_zoomNumerator *= 2;
  redraw();
}

void PlotView::zoomOut()
{
  m_zoomDenominator *= 2;
  redraw();
}

void PlotView::redrawGrid()
{
  delete m_grid;
  m_grid = new QGraphicsItemGroup();
  m_scene->addItem(m_grid);
  m_grid->setZValue(-10);

  QRectF sceneRect = m_scene->sceneRect();

  for (TDS t = 0; t < m_signalUs; t += 1000) {
    new QGraphicsLineItem(horizPosAt(t), 0,
                          horizPosAt(t), sceneRect.height(),
                          m_grid);
  }
}

void PlotView::redrawScene()
{
  if (m_signals.begin() != m_signals.end()) {
    m_signalUs = *(m_signals[0]->signal->tds_end() - 1);

    int width = horizPosAt(m_signalUs);
    int height = (mc_heightScale + 10) * m_signals.size() + 10;

    // QGraphicsScene* newScene = new QGraphicsScene(0, 0, width, height, this);
    // QGraphicsScene* oldScene = m_scene;
    // m_scene = newScene;
    m_scene->setSceneRect(0, 0, width, height);

    int index = 0;
    for (auto si: m_signals) {
      si->position = vertBegin(index);
      si->height = mc_heightScale;
      drawSignal(si);
      ++index;
    }

    // setScene(newScene);
    // delete oldScene;

    setSceneRect(0, 0, width, height);
  } else {
    // hmm..
  }
}
