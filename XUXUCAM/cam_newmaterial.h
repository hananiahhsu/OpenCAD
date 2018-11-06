#ifndef CAM_NEWMATERIAL_H
#define CAM_NEWMATERIAL_H
/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */
#include <QDialog>
#include <QDir>
#include "FileIO/cam_fileio.h"

namespace Ui {
class CAM_NewMaterial;
}

class CAM_NewMaterial : public QDialog
{
    Q_OBJECT

public:
    explicit CAM_NewMaterial(QWidget *parent = 0);
    ~CAM_NewMaterial();

private:
    Ui::CAM_NewMaterial *ui;
    void Init();
    void closeEvent(QCloseEvent *event);
signals:
    void CAM_NewMat_Close_Signal(QString name_mat,QString thick_mat);
    void CAM_NewMat_New_Signal(QString name_mat,QString thick_mat);
    void CAM_AddMat_Add_Signal(QString name_mat,QString thick_mat);
private slots:
    void on_comboBox_2_choosed();//mat
    void on_comboBox_choosed();//thick of mat
    void on_lineEdit_changed();//input thick of mat
    void on_pushbutton_new_clicked();//
    void on_pushbutton_add_clicked();

public:
    CAM_FileIO file_io;
    QList<QList<QString>> p_all_mat_list;
    QString qs_name_mat,qs_thick_mat;//name and thickness of mat
};

#endif // CAM_NEWMATERIAL_H
