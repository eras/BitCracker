#ifndef PLOTVIEW_H
#define PLOTVIEW_H

#include <memory>
#include <map>

#include <QGraphicsView>
#include <QList>

#include "signal.h"
#include "condition.h"

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

  Signal* getSignalByName(QString a_name);
  Signal* getSignalByIndex(int a_index);

  void mark(QString a_label, bool a_state, unsigned a_min, unsigned a_max);

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

  typedef bool ConditionMarker;

  typedef std::map<ConditionCheckFactory*, ConditionMarker> ConditionMap;

  ConditionMap m_conditions;
};

#endif // PLOTVIEW_H
