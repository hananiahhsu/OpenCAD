#ifndef CAM_IMPORTPRT_H
#define CAM_IMPORTPRT_H
/*
 ************************************
 *** @Xu Xu
 *** @2017.06.06
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */
#include <QDialog>
#include <QAbstractButton>
#include <QFileDialog>
#include <QCloseEvent>

//namespace Ui {
//class CAM_ImportPrt;
//}



class CAM_ImportPrt : public /*QDialog*/QFileDialog
{
    Q_OBJECT

public:
    explicit CAM_ImportPrt(QWidget *parent = 0);
    ~CAM_ImportPrt();
public:
    QStringList prt_list;

private:
    void closeEvent(QCloseEvent *event);

signals:
    void CAM_ImportPrt_Destroyed();
    void CAM_ImportPrt_Close_Signal(QStringList prt_list);

private slots:
    void on_FilesSelected();//for prt selected
};


#endif // CAM_IMPORTPRT_H
