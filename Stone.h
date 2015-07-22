// Stone.h
// 棋子类，存储了棋子的基础信息

#ifndef STONE_H
#define STONE_H

#include <QString>

class Stone
{
public:
    Stone();

    enum TYPE{JIANG, CHE, PAO, MA, BING, SHI, XIANG};
    int _row;
    int _col;
    TYPE _type;


    int _id;
    bool _dead;
    bool _red;

    // 棋子的初始化
    void init(int id);

    // 判断_type返回相应字符串
    QString getText();

};

#endif // STONE_H
