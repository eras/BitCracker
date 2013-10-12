#ifndef SEQLENANALYSIS_H
#define SEQLENANALYSIS_H

#include <QDialog>
#include <map>
#include <list>
#include <memory>

#include "signal.h"

class QTableWidgetItem;

namespace Ui {
class SeqLenAnalysis;
}

class SeqLenAnalysis : public QDialog
{
  Q_OBJECT

public:
  explicit SeqLenAnalysis(QWidget *parent = 0);
  ~SeqLenAnalysis();

  static int runOnData(const TDSVector& a_data);

signals:
  void mark(QString label, bool state, unsigned min, unsigned max);

private slots:
  void chooseItem(QTableWidgetItem* item);
  void markDone();

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

  void add(Data data);

  std::map<QTableWidgetItem*, Data*> m_widgetData;
  std::list<Data>                    m_data;
};

#endif // SEQLENANALYSIS_H
