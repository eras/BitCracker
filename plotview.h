#ifndef PLOTVIEW_H
#define PLOTVIEW_H

#include <QGraphicsView>
#include <QList>
#include <memory>

#include "signal.h"

class PlotView : public QGraphicsView
{
  Q_OBJECT

public:
  explicit PlotView(QWidget *parent = 0);
  ~PlotView();

public slots:
  void setSignal(QString name, Signal* a_signal);
  void redrawScene();
  void redraw();
  void removeSignal(QString name);
  void zoomIn();
  void zoomOut();

signals:
  void signalUpdated();

protected:
  void wheelEvent(QWheelEvent* event) override;

private:
  struct SignalWithInfo;

  void drawSignal(SignalWithInfo* si);
  void redrawGrid();

  QGraphicsScene*        m_scene;
  QList<SignalWithInfo*> m_signals;

  const double mc_widthScale = 1.0 / 100.0;
  const double mc_heightScale = 100;

  QGraphicsItemGroup* m_grid;
  TDS                 m_signalUs;

  unsigned            m_zoomNumerator;
  unsigned            m_zoomDenominator;

  unsigned horizPosAt(TDS time);
  unsigned vertBegin(unsigned index); // y-offset of the beginning of the nth plot
  unsigned vertEnd(unsigned index); // y-offset of the ending of the nth plot
};

#endif // PLOTVIEW_H
