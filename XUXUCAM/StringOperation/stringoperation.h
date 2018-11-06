#ifndef STRINGOPERATION_H
#define STRINGOPERATION_H
/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */

#include<QString>
#include<QList>
class StringOperations
{
public:
      StringOperations(void);
      virtual ~StringOperations();
public:
    //Divide a long str by comma
    void DivideStrIntoList(QString in_str,QString div_str,QList<QString>& str_list);
    //Int to char*
    QString IntToQstring(int in_i);
    //Int to QByteArray
    QByteArray IntToQByteArray(int in_i);
};


#endif // STRINGOPERATION_H
