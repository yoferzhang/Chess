// Board.h
// Board类实现了棋盘的绘制以及显示
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
    int _selectid;

    // 返回棋盘行列对应的像素坐标
    QPoint center(int row, int col);
    QPoint center(int id);

    bool getRowCol(QPoint pt, int &row, int &col);

    void drawStone(QPainter &painter, int id); // 绘制棋子

    void DrawBackground(); // 设置背景颜色

    virtual void paintEvent(QPaintEvent *);

    virtual void mouseReleaseEvent(QMouseEvent *ev); // 鼠标点击象棋并释放鼠标时候触发

signals:

public slots:
};

#endif // BOARD_H
