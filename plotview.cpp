#include <iostream>
#include <cassert>

#include "plotview.h"
#include "signal.h"

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

void PlotView::setSignal(Signal* a_signal)
{
  m_signal.reset(a_signal);
  emit signalUpdated();
}

void PlotView::redrawScene()
{
  double widthScale = 1.0 / 100.0;
  double heightScale = 100;

  int width = *(m_signal->tds_end() - 1) * widthScale;
  int height = heightScale;

  std::cerr << "width: " << width << std::endl;
  std::cerr << "height: " << height << std::endl;

  QGraphicsScene* newScene = new QGraphicsScene(0, 0, width, height, this);
  QGraphicsScene* oldScene = m_scene;
  m_scene = newScene;

  if (m_signal) {
    bool     state    = true;
    unsigned prevTime = 0;

    for (auto curTime: m_signal->tds()) {
      m_scene->addLine(prevTime * widthScale, !state * heightScale, curTime * widthScale, !state * heightScale);
      m_scene->addLine(curTime * widthScale, !state * heightScale, curTime * widthScale, state * heightScale);
      prevTime = curTime;
      state = !state;
    }
  }

  setScene(newScene);
  delete oldScene;

  setSceneRect(0, 0, width, height);
}
