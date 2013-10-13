#ifndef SEQLENANALYSIS_H
#define SEQLENANALYSIS_H

#include <QDialog>
#include <map>
#include <list>
#include <memory>

#include "signal.h"

class QTableWidgetItem;
class QRadioButton;

namespace Ui {
class SeqLenAnalysis;
}

class SeqLenAnalysis : public QDialog
{
  Q_OBJECT

public:
  explicit SeqLenAnalysis(QWidget *parent = 0);
  ~SeqLenAnalysis();

  static void setupOnData(const TDSVector& a_data, SeqLenAnalysis& a_instance);

signals:
  void mark(QString label, bool state, unsigned min, unsigned max, QColor color);

private slots:
  void chooseItem(QTableWidgetItem* item);
  void markDone();
  void changeColor();
  void redraw();
  void chooseK();

private:
  Ui::SeqLenAnalysis *ui;
  struct Data {
    int bit;
    int n;
    double centroid;
    double mean;
    double stddev;
    double min;
    double max;
  };

  void add(int k, Data data);

  std::map<QTableWidgetItem*, Data*> m_widgetData;
  std::map<int, std::list<Data>>     m_data;
  QColor                             m_color;
  std::map<QRadioButton*, int>       m_chooseK;
  int                                m_currentK;
};

#endif // SEQLENANALYSIS_H
