#ifndef CAM_IMPORTMAT_H
#define CAM_IMPORTMAT_H
/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @LianYunGang,China
 *** @Hananiahhsu@live.cn
 ************************************
 */
#include <QDialog>
#include <QAbstractButton>
#include <QFileDialog>
namespace Ui {
class CAM_ImportMat;
}

class CAM_ImportMat : public /*QDialog*/QFileDialog
{
    Q_OBJECT

public:
    explicit CAM_ImportMat(QWidget *parent = 0);
    ~CAM_ImportMat();
public:
    QString dxf_name;
    QStringList dxf_list;
private:
    Ui::CAM_ImportMat *ui;
private:
    void closeEvent(QCloseEvent *event);

signals:
    void CAM_ImportMat_Close_Signal(QStringList dxf_list);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_destroyed();
    void on_FilesSelected();
};

#endif // CAM_IMPORTMAT_H
