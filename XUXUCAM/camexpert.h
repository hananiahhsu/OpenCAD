#ifndef CAMEXPERT_H
#define CAMEXPERT_H
/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */
#include <QMainWindow>
#include <QMdiArea>
#include <QSplitter>
#include <QTreeView>
#include "cam_importmat.h"
#include "cam_newmaterial.h"
#include "cam_newprt.h"
#include "cam_importprt.h"
#include <QStandardItem>
#include <QtGui>
#include <QWidget>
#include <QFileDialog>
#include <QVector>
#include <QDesktopWidget>

#include "CamDxf/camdxf.h"
#include "CamDxf/campart.h"
#include "CamDxf/camscene.h"
#include "StringOperation/stringoperation.h"

namespace Ui {
class CAMExpert;
}

//value of row
class RowData
{
 public:
    RowData(){}
    ~RowData(){}
 public:
    RowData(QString qs_0,QString qs_1,QString qs_2,QString qs_3,QString qs_4,QString qs_5,QString qs_6,QString qs_7)
    {
        row_v_0 = qs_0;
        row_v_1 = qs_1;
        row_v_2 = qs_2;
        row_v_3 = qs_3;
        row_v_4 = qs_4;
        row_v_5 = qs_5;
        row_v_6 = qs_6;
        row_v_7 = qs_7;
    }
public:
    void RowData::operator=(const RowData& rd)
    {
        row_v_0 = rd.row_v_0;
        row_v_1 = rd.row_v_1;
        row_v_2 = rd.row_v_2;
        row_v_3 = rd.row_v_3;
        row_v_4 = rd.row_v_4;
        row_v_5 = rd.row_v_5;
        row_v_6 = rd.row_v_6;
        row_v_7 = rd.row_v_7;
    }
    bool operator ==(const RowData& rd)
    {
       if(row_v_0 == rd.row_v_0 &&
          row_v_1 == rd.row_v_1 &&
          row_v_2 == rd.row_v_2 &&
          row_v_3 == rd.row_v_3 &&
          row_v_4 == rd.row_v_4 &&
          row_v_5 == rd.row_v_5 &&
          row_v_6 == rd.row_v_6 &&
          row_v_7 == rd.row_v_7)
       {
           return true;
       }
       else
       {
           return false;
       }
    }

 public:
    QString row_v_0;
    QString row_v_1;
    QString row_v_2;
    QString row_v_3;
    QString row_v_4;
    QString row_v_5;
    QString row_v_6;
    QString row_v_7;
};

//value of col
class ColData
{
    QString col_v_0;
    QString col_v_1;
    QString col_v_2;
    QString col_v_3;
    QString col_v_4;
    QString col_v_5;
    QString col_v_6;
    QString col_v_7;
};




class CAMExpert : public QMainWindow
{
    Q_OBJECT

public:
    explicit CAMExpert(QWidget *parent = 0);
    ~CAMExpert();

private:
    Ui::CAMExpert *ui;
public:
    //screen info params
    int screen_w;
    int screen_h;
    //functions of scn
    void GetScreenInfos();
    //operation of strings
    StringOperations st_op;
public:
    QGraphicsView *plate_graphic_view;
    QGraphicsView *plate_graphics_preview;
    QGraphicsView *prt_view;//for preview of prt
    Part *prt;
    CamScene *plate_preView_scene;
    CamScene *plate_scene;
    CamScene *prt_preview_scene;
    DL_Dxf *dl_dxf;
    CamDxf cam_dxf;

public:
    CAM_ImportMat   *CAM_IMPORT_WIN;
    CAM_NewMaterial *CAM_NEWMAT_WIN;
    CAM_NewPrt      *CAM_NEWPRT_WIN;
    CAM_ImportPrt   *CAM_IMPORTPRT_WIN;
    QFileDialog     *DXF_WIN;//USELESS
    QList<WId>      p_cam_impmat_win_list;
    QList<WId>      p_cam_newmat_win_list;
    int             cam_impmat_show_times=0;
    int             cam_newmat_show_times=0;
    int             cam_newprt_show_times = 0;
    int             cam_impprt_show_times = 0;

    QString         mat_name_from_NEWMAT;
    QString         mat_thick_from_NEWMAT;
    QStandardItemModel * model_plate_tableView;
    QStandardItemModel * model_prt_tableView;
public:
    //All widgets and files
    QIcon cam_newprt_win_icon;
    QIcon cam_impprt_win_icon;
public:
    //palette
    QPalette errorText;
    QPalette successText;
    //Right menu for tableview of plate
    QMenu    *right_menu;
    QAction  *QA_del_row;
    QAction  *QA_copy_row;
    QAction  *QA_paste_row;
    QAction  *QA_cut_row;
    QList<QModelIndex> TB_index_list;//store the indexes choosed by clicking
    QList<QModelIndex> TB_cut_template;//for cut operation
    QList<QModelIndex> TB_copy_template;//for copy
    QList<RowData>     TB_row_data_list;//set of clicked items
    QList<RowData>     TB_row_data_cut_list;//set of cutted items
    QList<RowData>     TB_row_data_cpy_list;//set of copied items
private:
//    QSplitter *m_h_Spliter,*m_r_Splitter;
//    QTreeView *m_left_treeView;
//    QWidget   *child_dialog_new_prt;
//    QWidget   *child_dialog_new_layout;
//    QDialog   *child_dialog_new_prt_01;

      void CamSigSlotsManage();
      void CamDockWinManage();
      void CamEventMananger();
      void CamListener();
      void closeEvent(QCloseEvent *evnt);
      void createRightMenu();
      //delete,cut,copy and paste rows of tableView
      void TableViewDelRow();
      void TableViewCutRow();
      void TableViewCopyRow();
      void TableViewPasteRow();
      //remove repeated index item of TB_index_list
      void RemoveRepeatedIndex();
      //Refresh the tableview of plate after editting
      void RefreshTBIndex();
      //create the scene
      void CreateScene();
      //layout the docks
      void CamLayoutDocks();
public:
      //Functions related
      //open file with specific format such as Dxf...
      void CamOpenDxfFile(QString qs_dxf);


private slots:
     void on_new_order_triggered();
     void on_quit_triggered();
     void on_new_mat_triggered();
     void on_new_layout_triggered();
     void on_new_prt_triggered();
     void on_save_order_triggered();
     void on_history_order_triggered();
     void on_import_2_triggered();
     void on_export_2_triggered();
     void on_run_triggered();
     void on_send_triggered();
     void on_print_preview_triggered();
     void on_print_triggered();
     void on_open_layout_triggered();
     void on_print_layout_triggered();
     void on_open_all_layout_triggered();
     void on_equipments_triggered();
     void on_materials_triggered();
     void on_manufs_triggered();
     void on_cost_triggered();
     void on_calculator_triggered();
     void on_about_triggered();
     void on_help_files_triggered();
     void on_Line_triggered();
     void on_Arc_triggered();
     void on_Text_triggered();
     void on_Circle_triggered();
     void on_Ellipse_triggered();
     void on_Polygon_triggered();
     void on_Copy_triggered();
     void on_Dimension_triggered();
     void on_Choose_triggered();
     void on_Translation_triggered();
     void on_Rotation_triggered();
     void on_Delete_triggered();
     void on_Offset_triggered();
     void on_Line_Color_triggered();
     void on_Line_Type_triggered();
     void on_Line_Width_triggered();

//Dock of new material
     void on_pushButton_NewMat_clicked();
     void on_pushButton_importMat_clicked();
//Dock of new prt
     void on_new_prt_btn_newprt_clicked();
     void on_new_prt_btn_addprt_clicked();
//slot of Action for del,cut,copy and paste
     void on_qa_del_row();
     void on_qa_cut_row();
     void on_qa_copy_row();
     void on_qa_paste_row();
     void on_tableView_item_clicked();

//self-defined slots
public slots:
    //For Dock
    void on_cam_importmat_win_destroy();
    void on_cam_newmat_win_destroy();
    void on_cam_newprt_win_destroyed();//6.6
    void on_cam_impprt_win_destroyed();//6.6
    //For TableView of plate
    void on_cam_newmat_win_receiveData(QString qs_name,QString qs_thick);
    //For opening dxf files
    void on_cam_importmat_win_receiveData(QStringList dxf_list);
    void on_cam_importmat_win_openfiles(QString file);
    void on_cam_importprt_win_openfiles(QString file);

    //For right menu of TableView of plate
    void on_cam_right_menu_tableView(QPoint);
    void on_cam_newprt_rev_params(QString name_mat,
                                  QString len,
                                  QString width,
                                  QString mat,
                                  QString thick_mat,
                                  QString qty,
                                  QString no,
                                  bool    is_rot);
protected:
//Event management
virtual void customEvent(QEvent * event,QObject * obj);

};


#endif // CAMEXPERT_H
