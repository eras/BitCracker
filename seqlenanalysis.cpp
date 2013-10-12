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

int SeqLenAnalysis::runOnData(const TDSVector& a_data)
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
  SeqLenAnalysis instance;

  instance.ui->seqLenTable->setHorizontalHeaderItem(0, new QTableWidgetItem("value"));
  instance.ui->seqLenTable->setHorizontalHeaderItem(1, new QTableWidgetItem("n"));
  instance.ui->seqLenTable->setHorizontalHeaderItem(2, new QTableWidgetItem("mean"));
  instance.ui->seqLenTable->setHorizontalHeaderItem(3, new QTableWidgetItem("stddev"));
  instance.ui->seqLenTable->setHorizontalHeaderItem(4, new QTableWidgetItem("min"));
  instance.ui->seqLenTable->setHorizontalHeaderItem(5, new QTableWidgetItem("max"));


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
        instance.ui->seqLenTable->insertRow(0);
        instance.ui->seqLenTable->setItem(0, 0, new QTableWidgetItem(QString::number(bit)));
        instance.ui->seqLenTable->setItem(0, 1, new QTableWidgetItem(QString::number(n)));
        instance.ui->seqLenTable->setItem(0, 2, new QTableWidgetItem(QString::number(mean)));
        instance.ui->seqLenTable->setItem(0, 3, new QTableWidgetItem(QString::number(stddev)));
        instance.ui->seqLenTable->setItem(0, 4, new QTableWidgetItem(QString::number(min)));
        instance.ui->seqLenTable->setItem(0, 5, new QTableWidgetItem(QString::number(max)));
      }
    }
  }

  return instance.exec();
}
