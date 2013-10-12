#ifndef SEQLENANALYSIS_H
#define SEQLENANALYSIS_H

#include <QDialog>

#include "signal.h"

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

private:
  Ui::SeqLenAnalysis *ui;
};

#endif // SEQLENANALYSIS_H
