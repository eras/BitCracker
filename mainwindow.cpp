#include <QFileDialog>
#include <QTextStream>
#include <QTemporaryFile>

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

#include <cassert>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signal.h"
#include "seqlenanalysis.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  //setCentralWidget(ui->plotView);
  //ui->plotView->setMinimumSize(QSize(1000, 1000));
  connect(ui->action_Load, SIGNAL(activated()), this, SLOT(openLoadWaveform()));
  connect(ui->actionSignal_durations, SIGNAL(activated()), this, SLOT(openSequenceLengthAnalysis()));
  connect(ui->actionCapture, SIGNAL(activated()), this, SLOT(capture()));

  ui->cursorInfo->addItem("Signal 1");
  ui->cursorInfo->addItem("  Width");
  ui->cursorInfo->addItem("  Frequency");
  ui->cursorInfo->addItem("  Cycle length");
}

void MainWindow::openSequenceLengthAnalysis()
{
  Signal* signal = ui->plotView->getSignalByIndex(0);
  if (signal) {
    SeqLenAnalysis analysis;
    SeqLenAnalysis::setupOnData(signal->get(), analysis);
    connect(&analysis, SIGNAL(mark(QString, bool, unsigned, unsigned, QColor)),
            ui->plotView, SLOT(mark(QString, bool, unsigned, unsigned, QColor)));
    analysis.exec();
  }
}

void MainWindow::capture()
{
  QTemporaryFile tmpFile;
  tmpFile.setAutoRemove(true);
  tmpFile.open();

  setenv("TMPFILE", tmpFile.fileName().toStdString().c_str(), 1);
  system("../../lämpöpumppu/genericrx/receiveGenericRx.byte > $TMPFILE");

  loadWaveform(tmpFile.fileName());
}

void MainWindow::openLoadWaveform()
{
  QFileDialog fileDialog(this, Qt::Dialog);
  int code = fileDialog.exec();
  if (code == QDialog::Accepted) {
    QStringList files = fileDialog.selectedFiles();
    loadWaveform(files[0]);
  }
}

void MainWindow::loadWaveform(QString a_filename)
{
  QFile file(a_filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }

  QTextStream in(&file);

  TDSVector samples;

  unsigned time = 0;
  while (!in.atEnd()) {
    QString line = in.readLine();
    std::stringstream st(line.toStdString());
    int value;
    st >> value;
    if (st) {
      time += value;
      samples.push_back(time);
    }
  }

  Signal* signal = new Signal(samples);
  ui->plotView->setSignal(a_filename, signal);
}

MainWindow::~MainWindow()
{
  delete ui;
}
