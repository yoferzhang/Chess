// Board.h
//

#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "Stone.h"

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);

    Stone _s[32]; // 定义32个棋子
    int _r; // 棋子的半径

    // 返回棋盘行列对应的像素坐标
    QPoint center(int row, int col);
    QPoint center(int id);

    void drawStone(QPainter &painter, int id); // 绘制棋子

    virtual void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // BOARD_H
