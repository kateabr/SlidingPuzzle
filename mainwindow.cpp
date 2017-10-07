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
}

MainWindow::~MainWindow() { delete ui; }
