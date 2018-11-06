#ifndef CAM_NEWPRT_H
#define CAM_NEWPRT_H
/*
 ************************************
 *** @Xu Xu
 *** @2017.06.6
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 *** HardWriting Ui
 ************************************
 */
#include <QDialog>
#include <QCloseEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QPainter>
#include <QCheckBox>
#include <QDir>
#include "FileIO/cam_fileio.h"
#include "StringOperation/stringoperation.h"
namespace Ui {

  class CAM_NewPrt;
}


class CAM_NewPrt : public QDialog
{
    Q_OBJECT

public:
    explicit CAM_NewPrt(QWidget *parent = 0);
    ~CAM_NewPrt();

private:
    Ui::CAM_NewPrt *ui;
    void Init();
    void closeEvent(QCloseEvent *event);
    //bool event(QEvent *event);
signals:
    void CAM_NewPrt_Destroy();
    void CAM_NewPrt_Close_Signal(QString name_mat,QString thick_mat);
    void CAM_NewPrt_New_Signal(QString name_mat,QString len,QString width,QString mat,QString thick_mat,QString qty,QString no,bool is_rot);
    void CAM_AddPrt_Add_Signal(QString name_mat,QString thick_mat);
private slots:
    void on_combo_mat_changed();
    void on_combo_mat_thk_changed();
    void on_newprt_new_clicked();
public:
    //File operations
    CAM_FileIO file_io;
    QList<QList<QString>> p_all_mat_list;
public:
   //All widgets of this window
    QPushButton *new_prt_new_prt;
    QPushButton *new_prt_add_prt;
    QLabel      *label_name;
    QLabel      *label_length;
    QLabel      *label_width;
    QLabel      *label_mat;
    QLabel      *label_thk;
    QLabel      *label_quantity;
    QLabel      *label_prt_no;
    QLabel      *label_rot;

    QComboBox   *combo_names;
    QComboBox   *combo_material;
    QComboBox   *combo_mat_thicks;

    QLineEdit   *lineE_name;
    QLineEdit   *lineE_len;
    QLineEdit   *lineE_width;
    QLineEdit   *lineE_Qty;
    QLineEdit   *lineE_prt_no;
    QRadioButton *radio_rot;
    QCheckBox    *chk_rot;


    QPainter    *line_painter;


public:
    void paintEvent(QPaintEvent* event);
};




































#endif // CAM_NEWPRT_H
