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

template <typename T> class Board {
public:
  static Board MakeBoard(int x, int y);
  int zeroPos() const;
  bool final() const;
  int heuristic() const;
  static bool validLayout(const Board &b);
  const T *getLayout() const;
  const T &getElem(int i, int j) const;
  Board<T> moveUp() const;
  bool canMoveUp();
  Board<T> moveDown() const;
  bool canMoveDown();
  Board<T> moveLeft() const;
  bool canMoveLeft();
  Board<T> moveRight() const;
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
  T *layout;
  int gridSizeX;
  int gridSizeY;
  size_t boardHash;
  void generateHash();
};

template <typename T> Board<T> Board<T>::MakeBoard(int x, int y) {
  Board<T> res;
  res.layout = new T[x * y];
  res.gridSizeX = x;
  res.gridSizeY = y;

  std::iota(res.layout, res.layout + x * y, 0);

  do {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(res.layout, res.layout + x * y,
                 std::default_random_engine(seed));
  } while (!Board::validLayout(res));

  res.generateHash();

  return res;
}

template <typename T> bool Board<T>::validLayout(const Board &b) {
  /*
   * source: http://www.cs.mcgill.ca/~newborn/nxp_puzzleOct9.htm
   * An NxP-puzzle with an odd number of columns can be
   * solved only if NI is an even number.
   *
   * An NxP-puzzle with an even number of columns
   * and an even number of rows can be solved only
   * if (NI + Row of the blank) is an even number.
   *
   * An NxP-puzzle with an even number of columns
   * and an odd number of rows can be solved only
   * if (NI + Row of the blank) is an odd number.
   */

  int inv = 0; // inversions

  int k = b.zeroPos() / b.gridSizeY + 1; // row of zero tile

  for (int i = 1; i < b.gridSizeX * b.gridSizeY; ++i)
    if (b.layout[i])
      for (int j = 0; j < i; ++j)
        if (b.layout[j]) {
          if (b.layout[j] > b.layout[i])
            ++inv;
        }

  if (b.gridSizeY % 2)
    return !(inv % 2);
  else if (!(b.gridSizeX % 2))
    return !((k + inv) % 2);
  else
    return (k + inv) % 2;
}

template <typename T> const T *Board<T>::getLayout() const { return layout; }

template <typename T> const T &Board<T>::getElem(int i, int j) const {
  return layout[j * gridSizeX + i];
}

template <typename T> Board<T> Board<T>::moveUp() const {
  Board<T> res(*this);
  std::swap(res.layout[zeroPos()], res.layout[zeroPos() + gridSizeX]);
  res.generateHash();
  return res;
}

template <typename T> Board<T> Board<T>::moveDown() const {
  Board<T> res(*this);
  std::swap(res.layout[zeroPos()], res.layout[zeroPos() - gridSizeX]);
  res.generateHash();
  return res;
}

template <typename T> bool Board<T>::canMoveDown() {
  return zeroPos() >= gridSizeX;
}

template <typename T> bool Board<T>::canMoveUp() {
  return zeroPos() <= gridSizeX * (gridSizeY - 1) - 1;
}

template <typename T> bool Board<T>::canMoveLeft() {
  return (zeroPos() + 1) % gridSizeX;
}

template <typename T> bool Board<T>::canMoveRight() {
  return zeroPos() % gridSizeX;
}

template <typename T> size_t Board<T>::getHash() const { return boardHash; }

template <typename T> void Board<T>::print() const {
  QDebug deb = qDebug();
  for (int i = 0; i < gridSizeX * gridSizeY; ++i)
    deb << layout[i] << " ";
  deb << endl;
}

template <typename T> Board<T> Board<T>::moveLeft() const {
  Board<T> res(*this);
  std::swap(res.layout[zeroPos()], res.layout[zeroPos() + 1]);
  res.generateHash();
  return res;
}

template <typename T> Board<T> Board<T>::moveRight() const {
  Board<T> res(*this);
  std::swap(res.layout[zeroPos()], res.layout[zeroPos() - 1]);
  res.generateHash();
  return res;
}

template <typename T> int Board<T>::zeroPos() const {
  return std::distance(layout,
                       std::find(layout, layout + gridSizeX * gridSizeY, 0));
}

template <typename T> bool Board<T>::final() const {
  for (int i = 0; i < gridSizeX * gridSizeY - 1; ++i)
    if (layout[i] != (i + 1))
      return false;
  return true;
}

template <typename T> int Board<T>::heuristic() const {}

template <typename T> Board<T>::~Board() { delete[] layout; }

template <typename T>
Board<T>::Board(const Board &other)
    : gridSizeX(other.gridSizeX), gridSizeY(other.gridSizeY) {
  layout = new T[gridSizeX * gridSizeY];
  boardHash = other.boardHash;
  std::copy(other.layout, other.layout + gridSizeX * gridSizeY, layout);
}

template <typename T>
Board<T>::Board(Board &&other)
    : gridSizeX(other.gridSizeX), gridSizeY(other.gridSizeY),
      layout(other.layout), boardHash(other.boardHash) {
  other.layout = nullptr;
}

template <typename T> Board<T> &Board<T>::operator=(const Board &other) {
  gridSizeX = other.gridSizeX;
  gridSizeY = other.gridSizeY;
  boardHash = other.boardHash;
  std::copy(other.layout, other.layout + gridSizeX * gridSizeY, layout);
  return *this;
}

template <typename T> Board<T> &Board<T>::operator=(Board &&other) {
  gridSizeX = other.gridSizeX;
  gridSizeY = other.gridSizeY;
  delete[] layout;
  layout = other.layout;
  other.layout = nullptr;
  boardHash = other.boardHash;
  return *this;
}

template <typename T> void Board<T>::generateHash() {
  boardHash = 0;
  std::hash<std::string> strHash;
  std::string tempHash = "";
  for (int i = 0; i < gridSizeX * gridSizeY; ++i) {
    tempHash += std::to_string(layout[i]);
  }
  boardHash = strHash(tempHash);
}

template <typename T>
Board<T>::Board() : layout(nullptr), gridSizeX(0), gridSizeY(0) {}

#endif // BOARD_H
