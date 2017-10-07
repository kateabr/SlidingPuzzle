#ifndef BOARD_H
#define BOARD_H

#include <QDebug>
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using byte = int;

class Board {
public:
  static Board MakeBoard(int x, int y);
  int zeroPos() const;
  bool final() const;
  int heuristic() const;
  static bool validLayout(const Board &b);
  const byte *getLayout() const;
  const byte &getElem(int i, int j) const;
  Board moveUp() const;
  bool canMoveUp();
  Board moveDown() const;
  bool canMoveDown();
  Board moveLeft() const;
  bool canMoveLeft();
  Board moveRight() const;
  bool canMoveRight();
  size_t getHash() const;
  void print() const;

  Board();
  ~Board();
  Board(const Board &other);
  Board(Board &&other);
  Board &operator=(const Board &other);
  Board &operator=(Board &&other);

private:
  byte *layout;
  int gridSizeX;
  int gridSizeY;
  size_t boardHash;
  void generateHash();
};

#endif // BOARD_H
