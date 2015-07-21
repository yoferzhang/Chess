#ifndef STONE_H
#define STONE_H

#include <QString>

class Stone
{
public:
    Stone();

    enum TYPE{JIANG, CHE, PAO, MA, BING, SHI, XIANG};
    int _row; // 棋子所处的行
    int _col; // 棋子所处的列
    int _id; // 棋子的id
    bool _dead;
    bool _red;
    TYPE _type;

    void init(int id);

    // 判断_type返回相应字符串
    QString getText();

};

#endif // STONE_H
