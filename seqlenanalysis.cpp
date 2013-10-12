#include <cstdio>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QTemporaryFile>
#include <QTableWidgetItem>

#include "seqlenanalysis.h"
#include "ui_seqlenanalysis.h"

SeqLenAnalysis::SeqLenAnalysis(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SeqLenAnalysis)
{
  ui->setupUi(this);

  connect(ui->seqLenTable, SIGNAL(itemClicked(QTableWidgetItem*)),
          this, SLOT(chooseItem(QTableWidgetItem*)));
  connect(ui->mark, SIGNAL(clicked(bool)),
          this, SLOT(markDone()));
}

void
SeqLenAnalysis::chooseItem(QTableWidgetItem* a_item)
{
  const Data& d = *m_widgetData[a_item];
  ui->high->setChecked(d.bit);
  ui->low->setChecked(!d.bit);
  ui->minimum->setText(QString::number(d.min));
  ui->maximum->setText(QString::number(d.max));
}

void
SeqLenAnalysis::markDone()
{
  emit mark(ui->identifier->text(),
            ui->high->isChecked(),
            ui->minimum->text().toInt(),
            ui->maximum->text().toInt());
}

SeqLenAnalysis::~SeqLenAnalysis()
{
  delete ui;
}

namespace {
  double readNanOrDouble(std::istream& stream)
  {
    double d = 0.0;
    if (stream) {
      stream >> d;
      if (!stream) {
        //std::cerr << "Resetting stream\n";
        stream.clear();
        stream.ignore(100, ' ');

        d = 0.0/0.0;
      }
    }
    return d;
  }
}

void SeqLenAnalysis::setupOnData(const TDSVector& a_data, SeqLenAnalysis& a_instance)
{
  QTemporaryFile tmpFileOut;
  QTemporaryFile tmpFileIn;
  tmpFileOut.setAutoRemove(true);
  tmpFileOut.open();
  tmpFileIn.setAutoRemove(true);
  tmpFileIn.open();
  {
    std::ofstream outputFile(tmpFileOut.fileName().toStdString().c_str());
    TDS prev = 0;
    for (auto sample: a_data) {
      outputFile << sample - prev << "\n";
      prev = sample;
    }
  }

  a_instance.ui->seqLenTable->setHorizontalHeaderItem(0, new QTableWidgetItem("value"));
  a_instance.ui->seqLenTable->setHorizontalHeaderItem(1, new QTableWidgetItem("n"));
  a_instance.ui->seqLenTable->setHorizontalHeaderItem(2, new QTableWidgetItem("mean"));
  a_instance.ui->seqLenTable->setHorizontalHeaderItem(3, new QTableWidgetItem("stddev"));
  a_instance.ui->seqLenTable->setHorizontalHeaderItem(4, new QTableWidgetItem("min"));
  a_instance.ui->seqLenTable->setHorizontalHeaderItem(5, new QTableWidgetItem("max"));


  std::cerr << (QString("../tool/bitcracker.native seqlen ") + tmpFileOut.fileName() + " >" + tmpFileIn.fileName()).toStdString().c_str() << std::endl;
  system((QString("../tool/bitcracker.native seqlen ") + tmpFileOut.fileName() + " >" + tmpFileIn.fileName()).toStdString().c_str());
  std::ifstream inputFile(tmpFileIn.fileName().toStdString().c_str());
  while (inputFile) {
    std::string str;
    if (std::getline(inputFile, str)) {
      std::stringstream st(str);
      int bit;
      int n;
      double centroid;
      double mean;
      double stddev;
      double min;
      double max;
      st >> bit >> centroid >> n >> mean;
      stddev = readNanOrDouble(st);
      st >> min >> max;
      if (0 && !st.good()) {
        std::cerr << "Failed to read\n";
      } else {
        Data d;
        d.bit = bit;
        d.n = n;
        d.mean = mean;
        d.stddev = stddev;
        d.min = min;
        d.max = max;
        a_instance.add(d);
      }
    }
  }
}

void
SeqLenAnalysis::add(Data a_data)
{
  ui->seqLenTable->insertRow(0);
  m_data.push_front(a_data);
  Data& data = *m_data.begin();

  auto add = [&](int x, QString value) {
    QTableWidgetItem* w = new QTableWidgetItem(value);
    ui->seqLenTable->setItem(0, x, w);
    m_widgetData[w] = &data;
  };

  add(0, QString::number(data.bit));
  add(1, QString::number(data.n));
  add(2, QString::number(data.mean));
  add(3, QString::number(data.stddev));
  add(4, QString::number(data.min));
  add(5, QString::number(data.max));
}
