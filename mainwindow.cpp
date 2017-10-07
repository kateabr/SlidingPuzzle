#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->puzzleSolve, &QPushButton::clicked, ui->canvas,
          &Canvas::solvePuzzle);
  connect(
      ui->solvingMethod,
      static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
      ui->canvas, &Canvas::setSolvingMethod);

  connect(ui->canvas, &Canvas::updateTime,
          [&](int time) { ui->time->setText(QString::number(time) + " sec"); });

  connect(ui->canvas, &Canvas::updateOps,
          [&](int ops) { ui->ops->setText(QString::number(ops) + " ops"); });

  connect(ui->canvas, &Canvas::showControls, this, &MainWindow::showControls);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showControls(bool showControls) {
  ui->gridSizeX->setEnabled(showControls);
  ui->gridSizeY->setEnabled(showControls);
  ui->puzzleSolve->setEnabled(showControls);
  ui->solvingMethod->setEnabled(showControls);
}
