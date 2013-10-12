#ifndef PLOTVIEW_H
#define PLOTVIEW_H

#include <QGraphicsView>
#include <QList>
#include <memory>

class Signal;

class PlotView : public QGraphicsView
{
  Q_OBJECT

public:
  explicit PlotView(QWidget *parent = 0);
  ~PlotView();

public slots:
  void setSignal(QString name, Signal* a_signal);
  void redrawScene();
  void removeSignal(QString name);

signals:
  void signalUpdated();

private:
  struct SignalWithInfo;

  void drawSignal(SignalWithInfo* si);

  QGraphicsScene*        m_scene;
  QList<SignalWithInfo*> m_signals;

  const double mc_widthScale = 1.0 / 100.0;
  const double mc_heightScale = 100;


};

#endif // PLOTVIEW_H
