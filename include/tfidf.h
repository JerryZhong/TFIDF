#ifndef _TFIDF_H
#define _TFIDF_H

struct feature_node
{
    feature_node():index(0),value(0){}
    feature_node(int _index,int _value):index(_index),value(_value){}
    int index;
    int value;
};

struct problem
{
    int l;//文档个数
    int n;//特征总个数
    int *y;//mediaid
    struct feature_node **x;
    struct feature_node *index2df;//多申请一个元素空间，实际(fnum+1)，计算从index=1开始
};

#endif
