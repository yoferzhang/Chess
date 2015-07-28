// Board.h
// Board类实现了棋盘的绘制以及显示
//

#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include "Stone.h"
#include "Step.h"
#include <QVector>

class Board : public QFrame
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();


    /*===> 游戏数据 <===*/
    Stone _s[32]; // 定义32个棋子
    int _r; // 棋子的半径
    QPoint _off;
    bool _bSide;

    QVector<Step*> _steps; // 悔棋时使用


    /*===> 游戏状态 <===*/
    int _selectid;
    bool _bRedTurn;
    void init(bool bRedSide);


    /*===> 绘图函数 <===*/
    virtual void paintEvent(QPaintEvent *);
    void DrawBackground(); // 设置背景颜色
    void drawPlate(QPainter& p); // 绘制棋盘
    void drawPlace(QPainter& p); // 绘制九宫格
    void drawInitPosition(QPainter& p); // 绘制炮兵位置上的十字
    void drawInitPosition(QPainter& p, int row, int col);
    void drawStone(QPainter &p); // 绘制棋子
    void drawStone(QPainter &p, int id);


    /*===> 坐标转换相关函数 <===*/
    QPoint center(int row, int col); // 返回棋盘行列对应的像素坐标
    QPoint center(int id);
    QPoint topLeft(int row, int col);
    QPoint topLeft(int id);
    QRect cell(int row, int col);
    QRect cell(int id);

    bool getClickRowCol(QPoint pt, int &row, int &col);


    /*===> 帮助函数 <===*/
    QString name(int id);
    bool red(int id);
    bool sameColor(int id1, int id2);
    int getStoneId(int row, int col); // 获取行row列col上的棋子id
    void killStone(int id);
    void reliveStone(int id);
    void moveStone(int moveid, int row, int col);
    bool isDead(int id); // 判断id棋子是否死亡


    /*===> 移动相关函数 <===*/
    virtual void mouseReleaseEvent(QMouseEvent *ev); // 鼠标点击象棋并释放鼠标时候触发
    void click(QPoint pt);
    virtual void click(int id, int row, int col);
    void trySelectStone(int id);
    void tryMoveStone(int killid, int row, int col);
    void moveStone(int moveid, int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);
    void backOne();
    void back(Step* step);
    virtual void back();


    /*===> 移动规则 <===*/
    bool canMove(int moveid, int killid, int row, int col);
    bool canMoveJiang(int moveid, int killid, int row, int col);
    bool canMoveShi(int moveid, int, int row, int col);
    bool canMoveXiang(int moveid, int, int row, int col);
    bool canMoveChe(int moveid, int, int row, int col);
    bool canMoveMa(int moveid, int killid, int row, int col);
    bool canMovePao(int moveid, int killid, int row, int col);
    bool canMoveBing(int moveid, int killid, int row, int col);

    bool canSelect(int id);


    /*===> 移动规则相关的几个帮助函数 <===*/
    int relation(int row1, int col1, int row, int col); // 得到两点之间的关系值
    bool isBottomSide(int id); // 判断id棋子是否在棋盘下方
    // 判断两个点是否在同一条直线上，炮和车走棋的时候需要用到
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);


signals:

public slots:
    void slotBack();
};

#endif // BOARD_H
