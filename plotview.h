#ifndef PLOTVIEW_H
#define PLOTVIEW_H

#include <QGraphicsView>
#include <memory>

class Signal;

class PlotView : public QGraphicsView
{
  Q_OBJECT

public:
  explicit PlotView(QWidget *parent = 0);
  ~PlotView();

public slots:
  void setSignal(Signal* a_signal);
  void redrawScene();

signals:
  void signalUpdated();

private:

  QGraphicsScene*         m_scene;
  std::unique_ptr<Signal> m_signal;
};

#endif // PLOTVIEW_H
