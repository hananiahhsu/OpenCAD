/*
 ************************************
 *** @Xu Xu
 *** @2017.05.18
 *** @NanJing
 *** @Hananiahhsu@live.cn
 ************************************
 */
#include "stringoperation.h"


StringOperations::StringOperations(void)
{

}

StringOperations::~StringOperations()
{

}


//Divide a long str by comma
void StringOperations::DivideStrIntoList(QString in_str,QString div_str,QList<QString>& str_list)
{
   if(in_str.isEmpty()) return;
   int pos = in_str.indexOf(div_str);
   if(pos < 0 && !in_str.isEmpty())
   {
       str_list.append(in_str.left(in_str.indexOf("\n")));
       return;
   }
   QString left_str = in_str.left(pos);
   str_list.append(left_str);
   QString right_str = in_str.right(in_str.length()-pos-1);
   if(!right_str.isEmpty())
   StringOperations::DivideStrIntoList(right_str,div_str,str_list);
}



//Int to char*
QString StringOperations::IntToQstring(int in_i)
{
    QString qs_row = QString::number(in_i,10);
    QByteArray arr_row = qs_row.toLatin1();
    return qs_row;
}

//Int to QByteArray
QByteArray StringOperations::IntToQByteArray(int in_i)
{
    QString qs_row = QString::number(in_i,10);
    QByteArray arr_row = qs_row.toLatin1();
    return arr_row;
}
