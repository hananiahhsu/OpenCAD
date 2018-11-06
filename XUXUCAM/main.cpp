#include "camexpert.h"
#include <QApplication>
/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing
 *** @Hananiahhsu@live.cn
 ************************************
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CAMExpert w;
    w.show();

    return a.exec();
}
