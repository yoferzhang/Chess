// Board.h
// Board类实现了棋盘的绘制以及显示
//

#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include "Stone.h"

class Board : public QFrame
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);

    Stone _s[32]; // 定义32个棋子
    int _r; // 棋子的半径
    int _selectid;
    bool _bRedTurn;
    bool _bSide;

    void init(bool bRedSide);

    // 返回棋盘行列对应的像素坐标
    QPoint center(int row, int col);
    QPoint center(int id);

    bool getRowCol(QPoint pt, int &row, int &col);

    void drawStone(QPainter &painter, int id); // 绘制棋子

    void DrawBackground(); // 设置背景颜色

    virtual void paintEvent(QPaintEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *ev); // 鼠标点击象棋并释放鼠标时候触发

    bool canMove(int moveid, int killid, int row, int col);
    bool canMoveJiang(int moveid, int killid, int row, int col);
    bool canMoveShi(int moveid, int, int row, int col);
    bool canMoveXiang(int moveid, int, int row, int col);
    bool canMoveChe(int moveid, int, int row, int col);
    bool canMoveMa(int moveid, int killid, int row, int col);
    bool canMovePao(int moveid, int killid, int row, int col);
    bool canMoveBing(int moveid, int killid, int row, int col);

    int relation(int row1, int col1, int row, int col); // 得到两点之间的关系值

    int getStoneId(int row, int col); // 获取行row列col上的棋子id

    bool isDead(int id); // 判断id棋子是否死亡

    bool isBottomSide(int id); // 判断id棋子是否在棋盘下方

    // 判断两个点是否在同一条直线上，炮和车走棋的时候需要用到
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);


signals:

public slots:
};

#endif // BOARD_H
