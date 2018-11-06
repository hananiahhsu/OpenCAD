/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */
#include "camexpert.h"
#include "ui_camexpert.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QtCore/qtextcodec.h>
#include <QTextCodec>

/// margin used to properly show piece in preview sheet @todo Add in option dialog
const int rectMarg=20;
CAMExpert::CAMExpert(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CAMExpert)
{
    ui->setupUi(this);
    //Get infos of screen
    GetScreenInfos();
    this->showMaximized();
    QSize s(screen_w,screen_h);
    this->resize(s);
    //Management of signal and slots
    CamSigSlotsManage();
    //Manage the Dock win
    CamDockWinManage();
    //Create scene for plate(materials and part)
    CreateScene();
    //Layout many docks by myself
    CamLayoutDocks();
    //ini palette
    errorText = QPalette(Qt::red,Qt::red,Qt::red,Qt::red,Qt::red,Qt::red,Qt::red,Qt::red,Qt::red);
    successText = QPalette(Qt::darkBlue,Qt::darkBlue,Qt::darkBlue,Qt::darkBlue,Qt::darkBlue,Qt::darkBlue,Qt::darkBlue,Qt::darkBlue,Qt::darkBlue);
    //right click signal for tableView of plate
    connect(ui->Plate_tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_cam_right_menu_tableView(QPoint)));
    //click the item of TableView
    connect(ui->Plate_tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(on_tableView_item_clicked()));
}

CAMExpert::~CAMExpert()
{
    delete ui;
}


//===========================================
//Layout many docks-2017.6.2
//===========================================
void CAMExpert::CamLayoutDocks()
{
   //tabifiedDockWidgets(ui->dockWidget_new_part);
   tabifyDockWidget(ui->dockWidget_new_plate,ui->dockWidget_new_part);
   tabifyDockWidget(ui->dockWidget_new_part,ui->dockWidget_property);
   //tabPosition(Qt::TopDockWidgetArea);
}




//===========================================
//Get screen infos-2017.6.2
//===========================================
void CAMExpert::GetScreenInfos()
{
    //Not the real dimension of screen but are the set value of display precision
     QDesktopWidget * desk_widget = QApplication::desktop();
//   QRect scr_rect = desk_widget->screenGeometry();

   QRect desk_rect = desk_widget->availableGeometry();
   screen_h = desk_rect.height();
   screen_w = desk_rect.width();
}




//===========================================
//Create the scene-2017.6.1
//===========================================
void CAMExpert::CreateScene()
{
   //********************New plate***************************************//
   //(Here donnot use at all)
   plate_scene = new CamScene(false);//meaning of false or true
   plate_graphic_view = new QGraphicsView(plate_scene);
   plate_graphic_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   plate_graphic_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

   //This is for previewing  of plate(material)
   plate_preView_scene = new CamScene();
   plate_preView_scene->setSceneRect(0,0,300,200);
   plate_graphics_preview = new QGraphicsView(plate_preView_scene);
   plate_graphics_preview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   plate_graphics_preview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

   ui->gridLayout_3->addWidget(plate_graphics_preview);
   //previewVS->addWidget(plate_graphics_preview);
   //ui->dockWidget_3->setWidget(previewVS);

   //*******************New Part**********************************************//
    QSplitter *previewVS = new QSplitter(this);
    previewVS->setOrientation(Qt::Vertical);//horizontal
    //**TableView
    QTableView *prt_tableView = new QTableView();
    QRect prt_rect(1,300,276,232);
    prt_tableView->setGeometry(prt_rect);

    //Layout--no use
    QGridLayout *prt_grid_layout = new QGridLayout;
    prt_grid_layout->addWidget(prt_tableView);
    //add widget to splitter
    previewVS->addWidget(prt_tableView);
    ui->dockWidget_new_part->setWidget(previewVS);

    //ui->dockWidget_new_part->setLayout(prt_grid_layout);

    //add buttons and spacers to docks--2017.6.2
    QPushButton *btn_add_prt,*btn_new_prt;
    btn_new_prt = new QPushButton(ui->dockWidget_new_part);
    btn_new_prt->setText(QString::fromLocal8Bit("新建零件"));
    btn_new_prt->setFixedHeight(25);
    btn_new_prt->setGeometry(QRect(10,200,30,10));

    btn_add_prt = new QPushButton(ui->dockWidget_new_part);
    btn_add_prt->setText(QString::fromLocal8Bit("导入零件"));
    btn_add_prt->setGeometry(QRect(50,200,30,10));
    btn_add_prt->setFixedHeight(25);

    //spcaers
    QSpacerItem *prt_btn_spacer_1,*prt_btn_spacer_2,*prt_btn_spacer_3;
    prt_btn_spacer_1 = new QSpacerItem(20,10,QSizePolicy::Expanding);
    prt_btn_spacer_2 = new QSpacerItem(20,10,QSizePolicy::Expanding);
    prt_btn_spacer_3 = new QSpacerItem(20,10,QSizePolicy::Expanding);


    QSplitter *h_splitter = new QSplitter(ui->dockWidget_new_part);

    h_splitter->addWidget(btn_new_prt);
    h_splitter->addWidget(btn_add_prt);

    previewVS->addWidget(h_splitter);

//    QHBoxLayout * h_prt_layout = new QHBoxLayout();
//    h_prt_layout->addWidget(btn_add_prt);
//    ui->dockWidget_new_part->setLayout(h_prt_layout);


    QObject::disconnect(prt_tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_tablewidget_double_clicked(QModelIndex)));
    //Add header acccording to model
    model_prt_tableView = new QStandardItemModel();
    model_prt_tableView->setColumnCount(8);
    model_prt_tableView->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("序号"));//QString::fromUtf8("序号")
    model_prt_tableView->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("长度"));
    model_prt_tableView->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("宽度"));
    model_prt_tableView->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("厚度"));
    model_prt_tableView->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("数量"));
    model_prt_tableView->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("材料"));
    model_prt_tableView->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("编码"));
    model_prt_tableView->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("旋转"));

    //set the above model
    prt_tableView->setModel(model_prt_tableView);
    //Set the assignment of header information display mode as middle
    prt_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //Initialize the header of QTableWidget
    prt_tableView->horizontalHeader()->setDefaultSectionSize(10);

    //set the width of each column
    prt_tableView->setColumnWidth(0,20);
    prt_tableView->setColumnWidth(1,20);
    prt_tableView->setColumnWidth(2,20);
    prt_tableView->setColumnWidth(3,20);
    prt_tableView->setColumnWidth(4,20);
    prt_tableView->setColumnWidth(5,20);
    prt_tableView->setColumnWidth(6,20);
    prt_tableView->setColumnWidth(7,20);
    //Resize the column to contents
    prt_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    prt_tableView->resizeRowsToContents();
    //Grid style
    prt_tableView->gridStyle();
    //Hide the first col
    QHeaderView *headerview = prt_tableView->verticalHeader();
    headerview->setHidden(true);

    //Display the lines
    //Set the behavior of selection as row(you can set single selection or multi-row selection)
    prt_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    prt_tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    //Right menu
    prt_tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    //policy of scrollBar
    prt_tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    prt_tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //=============================================================================================

    //Above widgets are QTableView and 2 QPushButtons
    //Now under which i add a QGraphicsView
    prt_preview_scene = new CamScene();
    prt_preview_scene->setSceneRect(0,0,300,200);
    prt_view = new QGraphicsView(prt_preview_scene);
    prt_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    prt_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    previewVS->addWidget(prt_view);


    //================================================================================================
    //Connection for pushbutton of newprt
    connect(btn_new_prt,SIGNAL(clicked(bool)),this,SLOT(on_new_prt_btn_newprt_clicked()));
    connect(btn_add_prt,SIGNAL(clicked(bool)),this,SLOT(on_new_prt_btn_addprt_clicked()));





}


//===========================================
//Create a right menu for TableView of plate
//===========================================
void CAMExpert::createRightMenu()
{
   right_menu = new QMenu;
   QA_del_row = new QAction(QString::fromLocal8Bit("删除行"),this);
   QA_copy_row = new QAction(QString::fromLocal8Bit("复制行"),this);
   QA_paste_row = new QAction(QString::fromLocal8Bit("粘贴行"),this);
   QA_cut_row = new QAction(QString::fromLocal8Bit("剪切行"),this);
   right_menu->addAction(QA_del_row);
   right_menu->addAction(QA_cut_row);
   right_menu->addAction(QA_copy_row);
   right_menu->addAction(QA_paste_row);
   //CONNECT the action with main window
   connect(QA_del_row,SIGNAL(triggered(bool)),this,SLOT(on_qa_del_row()));
   connect(QA_copy_row,SIGNAL(triggered(bool)),this,SLOT(on_qa_copy_row()));
   connect(QA_paste_row,SIGNAL(triggered(bool)),this,SLOT(on_qa_paste_row()));
   connect(QA_cut_row,SIGNAL(triggered(bool)),this,SLOT(on_qa_cut_row()));
}

//===========================================
//Remove repeated index from TB_index_list
//===========================================
void CAMExpert::RemoveRepeatedIndex()
{
   int row = -1,num = 0,data_num = 0;RowData data_row;
   for(int i = 0;i < TB_index_list.size();i++)
   {
       for(int j = i+1;j < TB_index_list.size();j++)
       {
           if(TB_index_list[i].row() == TB_index_list[j].row())
           {
               num++;
               row = TB_index_list[j].row();
           }
       }
   }
   if(num%2 == 1)
   {
      for(int i = 0;i < TB_index_list.size();i++)
      {
          if(row == TB_index_list[i].row())
          {
              //unselect the item of row ADN set the color to azure
              model_plate_tableView->item(row,0)->setBackground(QBrush(QColor(240,255,255)));
              model_plate_tableView->item(row,1)->setBackground(QBrush(QColor(240,255,255)));
              model_plate_tableView->item(row,2)->setBackground(QBrush(QColor(240,255,255)));
              model_plate_tableView->item(row,3)->setBackground(QBrush(QColor(240,255,255)));
              model_plate_tableView->item(row,4)->setBackground(QBrush(QColor(240,255,255)));
              model_plate_tableView->item(row,5)->setBackground(QBrush(QColor(240,255,255)));
              model_plate_tableView->item(row,6)->setBackground(QBrush(QColor(240,255,255)));
              model_plate_tableView->item(row,7)->setBackground(QBrush(QColor(240,255,255)));
              TB_index_list.removeAt(i);
              i--;
          }
      }
   }
   //For tb_data_list(2017.6.5)
   for(int i = 0;i < TB_row_data_list.size();i++)
   {
       for(int j = i+1;j < TB_row_data_list.size();j++)
       {
          if(TB_row_data_list[j] == TB_row_data_list[i])
          {
             data_row = TB_row_data_list[j];
             data_num++;
          }
       }
   }
   if(data_num%2 == 1)
   {
      for(int k = 0;k < TB_row_data_list.size();k++)
      {
         if(data_row == TB_row_data_list[k])
         {
            TB_row_data_list.removeAt(k); k--;
         }
      }
   }

}

//===========================================
//Refresh the tableview after editting
//===========================================
void CAMExpert::RefreshTBIndex()
{
    char *c_row;QByteArray arr_row; QString qs_row;
    //model_plate_tableView->setSortRole(0);
    model_plate_tableView->sort(0,Qt::AscendingOrder);
    int row_num = model_plate_tableView->rowCount();
    //reset the no of rows
    for(int i = 0;i < row_num;i++)
    {
        qs_row = QString::number(i,10);
        arr_row = qs_row.toLatin1();
        c_row = arr_row.data();
        model_plate_tableView->setItem(i,0,new QStandardItem(c_row));
        model_plate_tableView->item(i,0)->setTextAlignment(Qt::AlignCenter);

        //set the color
        model_plate_tableView->item(i,0)->setBackground(QBrush(QColor(240,255,255)));
        model_plate_tableView->item(i,1)->setBackground(QBrush(QColor(240,255,255)));
        model_plate_tableView->item(i,2)->setBackground(QBrush(QColor(240,255,255)));
        model_plate_tableView->item(i,3)->setBackground(QBrush(QColor(240,255,255)));
        model_plate_tableView->item(i,4)->setBackground(QBrush(QColor(240,255,255)));
        model_plate_tableView->item(i,5)->setBackground(QBrush(QColor(240,255,255)));
        model_plate_tableView->item(i,6)->setBackground(QBrush(QColor(240,255,255)));
        model_plate_tableView->item(i,7)->setBackground(QBrush(QColor(240,255,255)));
    }
}

//===========================================
//Del,cut,copy or paste a row of tableView
//===========================================
void CAMExpert::TableViewDelRow()
{
   for(int i = 0;i < TB_index_list.size();i++)
   {
      //the param should be row(),not i instead
      model_plate_tableView->removeRow(TB_index_list[i].row());
   }
   //After you delete the above rows,please remove the data in TB_index_list
   for(int j = 0;j < TB_index_list.size();j++)
   {
       TB_index_list.removeAt(j); j--;
   }
}

void CAMExpert::TableViewCutRow()
{
  //Cut-->copy the row to QList,then del the row
  for(int i = 0;i < TB_index_list.size();i++)
  {
     TB_cut_template.append(TB_index_list[i]);

  }
  //First clear TB_row_data_list
  TB_row_data_cut_list.clear();
  for(int i = 0;i < TB_row_data_list.size();i++)
  {
    TB_row_data_cut_list.append(TB_row_data_list[i]);
  }
  TableViewDelRow();
}

void CAMExpert::TableViewCopyRow()
{
   TB_row_data_cpy_list.clear();
   for(int i = 0;i < TB_row_data_list.size();i++)
   {
       TB_row_data_cpy_list.append(TB_row_data_list[i]);
   }

}

void CAMExpert::TableViewPasteRow()
{
   int row = 0;QString qs_row;char *c_row;QByteArray arr_row;
   //There are 2 cases for pasting,one from cutting,another from copy
   //case 1:From cutting
   if(!TB_row_data_cut_list.isEmpty())
   {
     //paste the items from TB_cut_template to current model
     for(int i = 0;i < TB_row_data_cut_list.size();i++)
     {
       row = model_plate_tableView->rowCount();
       qs_row = QString::number(row,10);
       arr_row = qs_row.toLatin1();
       c_row = arr_row.data();
       //paste datas from TB_cut_template to current model
       model_plate_tableView->setItem(row,0,new QStandardItem(c_row));
       model_plate_tableView->item(row,0)->setTextAlignment(Qt::AlignCenter);

       arr_row = TB_row_data_cut_list[i].row_v_1.toLatin1();
       c_row = arr_row.data();
       model_plate_tableView->setItem(row,1,new QStandardItem(c_row));
       model_plate_tableView->item(row,1)->setTextAlignment(Qt::AlignCenter);

       arr_row = TB_row_data_cut_list[i].row_v_2.toLatin1();
       c_row = arr_row.data();
       model_plate_tableView->setItem(row,2,new QStandardItem(c_row));
       model_plate_tableView->item(row,2)->setTextAlignment(Qt::AlignCenter);

       arr_row = TB_row_data_cut_list[i].row_v_3.toLatin1();
       c_row = arr_row.data();
       model_plate_tableView->setItem(row,3,new QStandardItem(c_row));
       model_plate_tableView->item(row,3)->setTextAlignment(Qt::AlignCenter);

       arr_row = TB_row_data_cut_list[i].row_v_4.toLatin1();
       c_row = arr_row.data();
       model_plate_tableView->setItem(row,4,new QStandardItem(c_row));
       model_plate_tableView->item(row,4)->setTextAlignment(Qt::AlignCenter);

       arr_row = TB_row_data_cut_list[i].row_v_5.toLatin1();
       c_row = arr_row.data();
       model_plate_tableView->setItem(row,5,new QStandardItem(c_row));
       model_plate_tableView->item(row,5)->setTextAlignment(Qt::AlignCenter);

       arr_row = TB_row_data_cut_list[i].row_v_6.toLatin1();
       c_row = arr_row.data();
       model_plate_tableView->setItem(row,6,new QStandardItem(c_row));
       model_plate_tableView->item(row,6)->setTextAlignment(Qt::AlignCenter);

       arr_row = TB_row_data_cut_list[i].row_v_7.toLatin1();
       c_row = arr_row.data();
       model_plate_tableView->setItem(row,7,new QStandardItem(c_row));
       model_plate_tableView->item(row,7)->setTextAlignment(Qt::AlignCenter);
     }
   }

   //case 2:from copying
   if(!TB_row_data_cpy_list.isEmpty())
   {
     for(int j = 0;j < TB_row_data_cpy_list.size();j++)
     {
         row = model_plate_tableView->rowCount();
         qs_row = QString::number(row,10);
         arr_row = qs_row.toLatin1();
         c_row = arr_row.data();
         //paste datas from TB_cut_template to current model
         model_plate_tableView->setItem(row,0,new QStandardItem(c_row));
         model_plate_tableView->item(row,0)->setTextAlignment(Qt::AlignCenter);

         arr_row = TB_row_data_cpy_list[j].row_v_1.toLatin1();
         c_row = arr_row.data();
         model_plate_tableView->setItem(row,1,new QStandardItem(c_row));
         model_plate_tableView->item(row,1)->setTextAlignment(Qt::AlignCenter);

         arr_row = TB_row_data_cpy_list[j].row_v_2.toLatin1();
         c_row = arr_row.data();
         model_plate_tableView->setItem(row,2,new QStandardItem(c_row));
         model_plate_tableView->item(row,2)->setTextAlignment(Qt::AlignCenter);

         arr_row = TB_row_data_cpy_list[j].row_v_3.toLatin1();
         c_row = arr_row.data();
         model_plate_tableView->setItem(row,3,new QStandardItem(c_row));
         model_plate_tableView->item(row,3)->setTextAlignment(Qt::AlignCenter);

         arr_row = TB_row_data_cpy_list[j].row_v_4.toLatin1();
         c_row = arr_row.data();
         model_plate_tableView->setItem(row,4,new QStandardItem(c_row));
         model_plate_tableView->item(row,4)->setTextAlignment(Qt::AlignCenter);

         arr_row = TB_row_data_cpy_list[j].row_v_5.toLatin1();
         c_row = arr_row.data();
         model_plate_tableView->setItem(row,5,new QStandardItem(c_row));
         model_plate_tableView->item(row,5)->setTextAlignment(Qt::AlignCenter);

         arr_row = TB_row_data_cpy_list[j].row_v_6.toLatin1();
         c_row = arr_row.data();
         model_plate_tableView->setItem(row,6,new QStandardItem(c_row));
         model_plate_tableView->item(row,6)->setTextAlignment(Qt::AlignCenter);

         arr_row = TB_row_data_cpy_list[j].row_v_7.toLatin1();
         c_row = arr_row.data();
         model_plate_tableView->setItem(row,7,new QStandardItem(c_row));
         model_plate_tableView->item(row,7)->setTextAlignment(Qt::AlignCenter);
     }
   }



}


//======================================
//Management of sig and slots
//======================================
void CAMExpert::CamSigSlotsManage()
{
  QObject::connect(ui->pushButton_NewMat,SIGNAL(clicked()),this,SLOT(on_pushButton_NewMat_clicked()));
  QObject::connect(ui->pushButton_importMat,SIGNAL(clicked()),this,SLOT(on_pushButton_importMat_clicked()));



  //The connect action must be behind the instance of CAM_NEWMAT_WIN or CAM_IMPORT_WIN
  if(1 == cam_impmat_show_times)
  {
    QObject::connect(CAM_IMPORT_WIN,SIGNAL(destroyed()),this,SLOT(on_cam_importmat_win_destroy()));
  }
  if(1 == cam_newmat_show_times)
  {
    QObject::connect(CAM_NEWMAT_WIN,SIGNAL(destroyed(CAM_NEWMAT_WIN)),this,SLOT(on_cam_newmat_win_destroy()));
  }
}


//======================================
//Management of dock windows
//======================================
void CAMExpert::CamDockWinManage()
{
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    //************************Init of QTableView***************************//
    //Note:TableView a little different with Table Widget
    QObject::disconnect(ui->Plate_tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_tablewidget_double_clicked(QModelIndex)));
    //Add header acccording to model
    model_plate_tableView = new QStandardItemModel();
    model_plate_tableView->setColumnCount(8);
    model_plate_tableView->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("序号"));//QString::fromUtf8("序号")
    model_plate_tableView->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("长度"));
    model_plate_tableView->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("宽度"));
    model_plate_tableView->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("厚度"));
    model_plate_tableView->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("数量"));
    model_plate_tableView->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("材料"));
    model_plate_tableView->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("编码"));
    model_plate_tableView->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("消耗量"));

    //set the above model
    ui->Plate_tableView->setModel(model_plate_tableView);
    //Set the assignment of header information display mode as middle
    ui->Plate_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //Initialize the header of QTableWidget
    ui->Plate_tableView->horizontalHeader()->setDefaultSectionSize(10);
    QRect plate_rect(0,0,298,200);
    ui->Plate_tableView->setGeometry(plate_rect);
    //set the width of each column
    ui->Plate_tableView->setColumnWidth(0,20);
    ui->Plate_tableView->setColumnWidth(1,20);
    ui->Plate_tableView->setColumnWidth(2,20);
    ui->Plate_tableView->setColumnWidth(3,20);
    ui->Plate_tableView->setColumnWidth(4,20);
    ui->Plate_tableView->setColumnWidth(5,20);
    ui->Plate_tableView->setColumnWidth(6,20);
    ui->Plate_tableView->setColumnWidth(7,20);
    //Resize the column to contents
    //ui->Plate_tableView->horizontalHeader()->setStretchLastSection(true);
    ui->Plate_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->Plate_tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
//    ui->Plate_tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
//    ui->Plate_tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
//    ui->Plate_tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
//    ui->Plate_tableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
//    ui->Plate_tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
//    ui->Plate_tableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeToContents);
//    ui->Plate_tableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);
    ui->Plate_tableView->resizeRowsToContents();
    //ui->Plate_tableView->resizeColumnsToContents();

    //Grid style
    ui->Plate_tableView->gridStyle();

    //Hide the first col
    QHeaderView *headerview = ui->Plate_tableView->verticalHeader();
    headerview->setHidden(true);

    //Display the lines

    //Set the behavior of selection as row(you can set single selection or multi-row selection)
    ui->Plate_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Plate_tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    //Disable the editing of tableWidget(if not set,then it's editable by default)
    //ui->Plate_tableView->setEditTriggers(QAbstractItemView::EditTriggers);

    //Right menu
    ui->Plate_tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    createRightMenu();
    //QStringList header;
    //header << "序号(No.)" << "长(L)" << "宽(W)" << "厚度(T)" << "数量(Q)" << "材料(M)" << "编码(C)" << "消耗量(CP)";
    //ui->Plate_tableView->setHorizontalHeaderLabels(header);
    //ui->Plate_tableView->horizontalHeader()->setStretchLastSection(true);//fill the whole table

    //After select multi-rows,right click and delete

}


//======================================
//close event
//======================================
void CAMExpert::closeEvent(QCloseEvent *event)
{
    event->accept();
}

//======================================
//process event
//======================================
void CAMExpert::customEvent(QEvent * event,QObject * obj)
{
  if(event->type() == QEvent::Destroy)
  {
      cam_impmat_show_times = 0;
      cam_newmat_show_times = 0;
  }
}


//======================================
//Slot of Files
//======================================
void CAMExpert::on_new_order_triggered()
{
    QMessageBox::information(this,"title","New_order");
}

void CAMExpert::on_save_order_triggered()
{
    QMessageBox::information(this,"title","save_order");
}

void CAMExpert::on_history_order_triggered()
{
    QMessageBox::information(this,"title","history_order");
}

void CAMExpert::on_import_2_triggered()
{
     QMessageBox::information(this,"title","import");
}

void CAMExpert::on_export_2_triggered()
{
    QMessageBox::information(this,"title","export");
}

void CAMExpert::on_quit_triggered()
{
    QApplication::quit();
}

//======================================
//Slot of Nest
//======================================

void CAMExpert::on_new_mat_triggered()
{
    //QMessageBox::information(this,"title","New_mat");
    //Toggle to dock of new palte(2017.6.7)
    ui->dockWidget_new_plate->raise();
    on_pushButton_NewMat_clicked();
}

void CAMExpert::on_new_layout_triggered()
{
    QMessageBox::information(this,"title","New_layout");
}

void CAMExpert::on_new_prt_triggered()
{
    //QMessageBox::information(this,"title","New_prt");
    //Toggle to dock of new parts(2017.6.7)
    ui->dockWidget_new_part->raise();
    on_new_prt_btn_newprt_clicked();
}


void CAMExpert::on_run_triggered()
{
    QMessageBox::information(this,"title","run_nest");
}



void CAMExpert::on_print_preview_triggered()
{
     QMessageBox::information(this,"title","print preview");
}

void CAMExpert::on_print_triggered()
{
     QMessageBox::information(this,"title","print");
}


void CAMExpert::on_send_triggered()
{
    QMessageBox::information(this,"title","send to factory");
}

//======================================
//Slot of Layout
//======================================

void CAMExpert::on_open_layout_triggered()
{
     QMessageBox::information(this,"title","open layout");
}

void CAMExpert::on_print_layout_triggered()
{
     QMessageBox::information(this,"title","print layout");
}

void CAMExpert::on_open_all_layout_triggered()
{
     QMessageBox::information(this,"title","print all layout");
}



//======================================
//Slot of KBE
//======================================

void CAMExpert::on_equipments_triggered()
{
      QMessageBox::information(this,"title","equip lib");
}

void CAMExpert::on_materials_triggered()
{
      QMessageBox::information(this,"title","mat lib");
}

void CAMExpert::on_manufs_triggered()
{
      QMessageBox::information(this,"title","tech lib");
}


//======================================
//Slot of Tools
//======================================
void CAMExpert::on_cost_triggered()
{
      QMessageBox::information(this,"title","cost");
}

void CAMExpert::on_calculator_triggered()
{
      QMessageBox::information(this,"title","calculator");
}


//======================================
//Slot of Help
//======================================
void CAMExpert::on_about_triggered()
{
      QMessageBox::information(this,"title","about");
}

void CAMExpert::on_help_files_triggered()
{
      QMessageBox::information(this,"title","help files");
}


//======================================
//Slot of Drawing
//======================================
void CAMExpert::on_Line_triggered()
{
   QMessageBox::information(this,"title","line");
}

void CAMExpert::on_Arc_triggered()
{
   QMessageBox::information(this,"title","arc");
}

void CAMExpert::on_Text_triggered()
{
   QMessageBox::information(this,"title","text");
}

void CAMExpert::on_Circle_triggered()
{
   QMessageBox::information(this,"title","circle");
}

void CAMExpert::on_Ellipse_triggered()
{
    QMessageBox::information(this,"title","Ellipse");
}

void CAMExpert::on_Polygon_triggered()
{
    QMessageBox::information(this,"title","polygon");
}

void CAMExpert::on_Copy_triggered()
{
    QMessageBox::information(this,"title","copy");
}

void CAMExpert::on_Dimension_triggered()
{
    QMessageBox::information(this,"title","dimension");
}

void CAMExpert::on_Choose_triggered()
{
     QMessageBox::information(this,"title","choose");
}

void CAMExpert::on_Translation_triggered()
{
    QMessageBox::information(this,"title","translation");
}

void CAMExpert::on_Rotation_triggered()
{
    QMessageBox::information(this,"title","Rotation");
}

void CAMExpert::on_Delete_triggered()
{
    QMessageBox::information(this,"title","Delete");
}

void CAMExpert::on_Offset_triggered()
{
     QMessageBox::information(this,"title","offset");
}

void CAMExpert::on_Line_Color_triggered()
{
     QMessageBox::information(this,"title","Line color");
}

void CAMExpert::on_Line_Type_triggered()
{
     QMessageBox::information(this,"title","Line type");
}

void CAMExpert::on_Line_Width_triggered()
{
     QMessageBox::information(this,"title","Line width");
}



//======================================
//Slot of pushButtons
//======================================

//New mat by choosing the mat from local text
//If mat not found,you may add it too
void CAMExpert::on_pushButton_NewMat_clicked()
{
    if(cam_newmat_show_times == 0)
    {
        CAM_NEWMAT_WIN = new CAM_NewMaterial;
        CAM_NEWMAT_WIN->setAttribute(Qt::WA_DeleteOnClose);
        CAM_NEWMAT_WIN->setGeometry(QRect(490,150,510,140));//6.7
        CAM_NEWMAT_WIN->show();
        //The instance of import win must be realized before connectting signal and slots
        QObject::connect(CAM_NEWMAT_WIN,SIGNAL(destroyed()),this,SLOT(on_cam_newmat_win_destroy()));
        //Get the name and thickness of mat from CAM_NEWMAT_WIN(before destroyed)
        //2017.6.5-NIGHT--close the Close Signal and use button to send signal from CAM_NEWMAT_WIN
        //QObject::connect(CAM_NEWMAT_WIN,SIGNAL(CAM_NewMat_Close_Signal(QString,QString)),this,SLOT(on_cam_newmat_win_receiveData(QString,QString)));
        QObject::connect(CAM_NEWMAT_WIN,SIGNAL(CAM_NewMat_New_Signal(QString,QString)),this,SLOT(on_cam_newmat_win_receiveData(QString,QString)));
        //2017.6.6-connect add pushbutton with dock win
        QObject::connect(CAM_NEWMAT_WIN,SIGNAL(CAM_AddMat_Add_Signal(QString,QString)),this,SLOT(on_cam_newmat_win_receiveData(QString,QString)));
    }
    cam_newmat_show_times++;
}
//Import mat from the DXF file(Here could be connected with database)
//Later other format of files may be imported too
void CAMExpert::on_pushButton_importMat_clicked()
{

    if(cam_impmat_show_times == 0)
    {

        CAM_IMPORT_WIN = new CAM_ImportMat;
        CAM_IMPORT_WIN->setAttribute(Qt::WA_DeleteOnClose);
        CAM_IMPORT_WIN->show();

        //The instance of import win must be realized before connectting signal and slots
        QObject::connect(CAM_IMPORT_WIN,SIGNAL(CAM_ImportMat_Close_Signal(QStringList)),this,SLOT(on_cam_importmat_win_destroy()));
        //After choosing a dxf file,reset the value of cam_impmat_show_times too
        QObject::connect(CAM_IMPORT_WIN,SIGNAL(fileSelected(QString)),this,SLOT(on_cam_importmat_win_destroy()));
        QObject::connect(CAM_IMPORT_WIN,SIGNAL(destroyed(QObject*)),this,SLOT(on_cam_importmat_win_destroy()));
/*
        //The follwing window closed,it's hard to get the sig,so use another way(also failed-2017.5.25)
        //CamOpenDxfFile(QFileDialog::getOpenFileName(CAM_IMPORT_WIN,"导入材料",".","DXF FILES (*.dxf)"));
        //Alternative way of open file
        DXF_WIN = new QFileDialog(this);
        DXF_WIN->setWindowTitle(tr("导入材料"));
        DXF_WIN->setDirectory("/");
        DXF_WIN->setFilter(QDir::Files|QDir::NoSymLinks|QDir::AllDirs);

        //As DXF_WIN destroyed
        QObject::connect(DXF_WIN,SIGNAL(directoryEntered(QString)),this,SLOT(on_cam_importmat_win_destroy()));

        if(DXF_WIN->exec() == QFileDialog::Accepted)
        {
            QString dxf_path = DXF_WIN->selectedFiles()[0];
        }
*/
        QObject::connect(CAM_IMPORT_WIN,SIGNAL(CAM_ImportMat_Close_Signal(QStringList)),this,SLOT(on_cam_importmat_win_receiveData(QStringList)));
        QObject::connect(CAM_IMPORT_WIN,SIGNAL(fileSelected(QString)),this,SLOT(on_cam_importmat_win_openfiles(QString)));
    }
    cam_impmat_show_times++;
}

//Set the value to 0 again after killing the child window
void CAMExpert::on_cam_importmat_win_destroy()
{
   cam_impmat_show_times = 0;
}

void CAMExpert::on_cam_newmat_win_destroy()
{
   cam_newmat_show_times = 0;
}

void CAMExpert::on_cam_newprt_win_destroyed()
{
    cam_newprt_show_times = 0;
}

void CAMExpert::on_cam_impprt_win_destroyed()
{
    cam_impprt_show_times = 0;
}

//slots of TableView of plate
void CAMExpert::on_cam_newmat_win_receiveData(QString qs_name,QString qs_thick)
{
    //int converted to QString
    int row = model_plate_tableView->rowCount();
    QString qs_row = QString::number(row,10);
    char *c_row;
    QByteArray arr_row = qs_row.toLatin1();
    c_row = arr_row.data();
    //receive datas from new_mat window to tableView
    model_plate_tableView->setItem(row,0,new QStandardItem(c_row));
    model_plate_tableView->item(row,0)->setTextAlignment(Qt::AlignCenter);


    model_plate_tableView->setItem(row,1,new QStandardItem("100"));
    model_plate_tableView->item(row,1)->setTextAlignment(Qt::AlignCenter);

    model_plate_tableView->setItem(row,2,new QStandardItem("50"));
    model_plate_tableView->item(row,2)->setTextAlignment(Qt::AlignCenter);

    model_plate_tableView->setItem(row,3,new QStandardItem(qs_thick));
    model_plate_tableView->item(row,3)->setTextAlignment(Qt::AlignCenter);

    model_plate_tableView->setItem(row,4,new QStandardItem("1"));
    model_plate_tableView->item(row,4)->setTextAlignment(Qt::AlignCenter);

    model_plate_tableView->setItem(row,5,new QStandardItem(qs_name));
    model_plate_tableView->item(row,5)->setTextAlignment(Qt::AlignCenter);

    model_plate_tableView->setItem(row,6,new QStandardItem(""));
    model_plate_tableView->item(row,6)->setTextAlignment(Qt::AlignCenter);

    model_plate_tableView->setItem(row,7,new QStandardItem("1"));
    model_plate_tableView->item(row,7)->setTextAlignment(Qt::AlignCenter);
}



/**************************************
 * Slot for new prt in dock of new prt
**************************************/

void CAMExpert::on_new_prt_btn_newprt_clicked()
{
    if(cam_newprt_show_times == 0)
    {
       //new dialog
       CAM_NEWPRT_WIN = new CAM_NewPrt();
       CAM_NEWPRT_WIN->setAttribute(Qt::WA_DeleteOnClose);//6.6
       CAM_NEWPRT_WIN->setGeometry(QRect(450,150,230,320));
       CAM_NEWPRT_WIN->setFixedHeight(320);
       CAM_NEWPRT_WIN->setFixedWidth(230);
       cam_newprt_win_icon.addFile(QStringLiteral(":/rec/Resource/company/wit.png"), QSize(), QIcon::Normal, QIcon::Off);
       CAM_NEWPRT_WIN->setWindowIcon(cam_newprt_win_icon);
       CAM_NEWPRT_WIN->setWindowTitle(QString::fromLocal8Bit("新建零件"));
       CAM_NEWPRT_WIN->setModal(true);//set before showing
       CAM_NEWPRT_WIN->show();



       //connect self defined signals
       connect(CAM_NEWPRT_WIN,SIGNAL(CAM_NewPrt_Destroy()),this,SLOT(on_cam_newprt_win_destroyed()));
       //receive data from CAM_NEWPRT_WIN
       connect(CAM_NEWPRT_WIN,SIGNAL(CAM_NewPrt_New_Signal(QString,
                                                           QString,
                                                           QString,
                                                           QString,
                                                           QString,
                                                           QString,
                                                           QString,
                                                           bool
                                                           )),
                                       this,
                                       SLOT(on_cam_newprt_rev_params(QString,
                                                                     QString,
                                                                     QString,
                                                                     QString,
                                                                     QString,
                                                                     QString,
                                                                     QString,
                                                                     bool
                                                                       )));
    }
    cam_newprt_show_times++;
}

void CAMExpert::on_new_prt_btn_addprt_clicked()
{
    if(cam_impprt_show_times == 0)
    {
       //new dialog
       CAM_IMPORTPRT_WIN = new CAM_ImportPrt;
       CAM_IMPORTPRT_WIN->setAttribute(Qt::WA_DeleteOnClose);//6.6
       CAM_IMPORTPRT_WIN->setGeometry(QRect(350,200,500,250));
       CAM_IMPORTPRT_WIN->setFixedHeight(250);
       CAM_IMPORTPRT_WIN->setFixedWidth(500);
       cam_impprt_win_icon.addFile(QStringLiteral(":/rec/Resource/company/wit.png"), QSize(), QIcon::Normal, QIcon::Off);
       CAM_IMPORTPRT_WIN->setWindowIcon(cam_impprt_win_icon);
       CAM_IMPORTPRT_WIN->setWindowTitle(QString::fromLocal8Bit("导入零件"));
       CAM_IMPORTPRT_WIN->setModal(true);//set before showing
       CAM_IMPORTPRT_WIN->show();


       //connect
       //Failed to send msg from prt win to dock(why?)
       QObject::connect(CAM_IMPORTPRT_WIN,SIGNAL(CAM_ImportPrt_Destroyed()),this,SLOT(on_cam_impprt_win_destroyed()));
       QObject::connect(CAM_IMPORTPRT_WIN,SIGNAL(destroyed(QObject*)),this,SLOT(on_cam_impprt_win_destroyed()));
       QObject::connect(CAM_IMPORTPRT_WIN,SIGNAL(fileSelected(QString)),this,SLOT(on_cam_impprt_win_destroyed()));
       //connect and receive data
       QObject::connect(CAM_IMPORTPRT_WIN,SIGNAL(fileSelected(QString)),this,SLOT(on_cam_importprt_win_openfiles(QString)));
    }
    cam_impprt_show_times++;
}



























/************************************
 * Open Dxf files
 *
***********************************/
//Slot for receiving QStringList from CAM_IMPORT_WIN
void CAMExpert::on_cam_importmat_win_receiveData(QStringList dxf_list)
{
   //Judge if enpty for the list
   if(dxf_list.isEmpty())
   {
      statusBar()->setPalette(errorText);
      statusBar()->showMessage(QString::fromLocal8Bit("请选择DXF文件"));
   }
   else
   {
      //Instance of DXF
      CamDxf *cam_dxf = new CamDxf();
      DL_Dxf *dl_dxf = new DL_Dxf();
      //open dxf file,if failed,give the msg
      if(!dl_dxf->in(dxf_list[0].toStdString(),cam_dxf))
      {
          statusBar()->setPalette(errorText);
          statusBar()->showMessage(QString::fromLocal8Bit("打开DXF文件失败"));
          //

          //delete ptr
          delete cam_dxf;
          delete dl_dxf;
          return;
      }
      else
      {
          statusBar()->setPalette(successText);
          statusBar()->showMessage(QString::fromLocal8Bit("打开DXF文件成功"));
      }
   }
}

//slot for open dxf file after choosing a dxf file
void CAMExpert::on_cam_importmat_win_openfiles(QString file)
{
    //Judge if empty for the list
    if(file.isEmpty() || file.indexOf(".dxf")<0)
    {
       statusBar()->setPalette(errorText);
       statusBar()->showMessage(QString::fromLocal8Bit("请选择DXF文件"));
    }
    else
    {
       //Instance of DXF
       //cam_dxf = new CamDxf();
       dl_dxf = new DL_Dxf();
       //open dxf file,if failed,give the msg
       if(!dl_dxf->in(file.toStdString(),&cam_dxf))
       {
           statusBar()->setPalette(errorText);
           statusBar()->showMessage(QString::fromLocal8Bit("打开DXF文件失败"));
           //

           //delete ptr
           //delete cam_dxf;
           delete dl_dxf;
           return;
       }
       else
       {
           statusBar()->setPalette(successText);
           statusBar()->showMessage(QString::fromLocal8Bit("打开DXF文件成功"));
       }

       plate_preView_scene->clear();
       //Open DXF files successfully,preview it in the widget
       QString partName = file.left(file.lastIndexOf("."));
       prt=new Part(cam_dxf.partPath,
                    cam_dxf.pointsPathVector,
                    cam_dxf.partPathsList,
                    cam_dxf.pointsCircleVector,
                    cam_dxf.circlePathsList);
       prt->setPartName(partName);
       //******Add little margin to show outline lead correctly       
       QRectF contRect=prt->boundingRect();    
       contRect.setHeight(contRect.height()+rectMarg);
       contRect.setWidth(contRect.width()+rectMarg);
       //ui->graphicsView_plate->setSceneRect(contRect);
       plate_graphics_preview->setSceneRect(contRect);



       prt->setPos(rectMarg/2,rectMarg/2);
       plate_preView_scene->addItem(prt);

       plate_preView_scene->zoomFit();
       //plate_graphics_preview->show();





       //delete cam_dxf;
       delete dl_dxf;
    }
}

//2017.6.6--Tue--For preview of imp prt
//slot for open prt-dxf file after choosing a prt-dxf file
void CAMExpert::on_cam_importprt_win_openfiles(QString file)
{
    //Judge if empty for the list
    if(file.isEmpty() || file.indexOf(".dxf")<0)
    {
       statusBar()->setPalette(errorText);
       statusBar()->showMessage(QString::fromLocal8Bit("请选择DXF文件"));
    }
    else
    {
       //Instance of DXF
       //cam_dxf = new CamDxf();
       dl_dxf = new DL_Dxf();
       //open dxf file,if failed,give the msg
       if(!dl_dxf->in(file.toStdString(),&cam_dxf))
       {
           statusBar()->setPalette(errorText);
           statusBar()->showMessage(QString::fromLocal8Bit("打开DXF文件失败"));
           //

           //delete ptr
           //delete cam_dxf;
           delete dl_dxf;
           return;
       }
       else
       {
           statusBar()->setPalette(successText);
           statusBar()->showMessage(QString::fromLocal8Bit("打开DXF文件成功"));
       }

       prt_preview_scene->clear();
       //Open DXF files successfully,preview it in the widget
       QString partName = file.left(file.lastIndexOf("."));
       prt=new Part(cam_dxf.partPath,
                    cam_dxf.pointsPathVector,
                    cam_dxf.partPathsList,
                    cam_dxf.pointsCircleVector,
                    cam_dxf.circlePathsList);
       prt->setPartName(partName);
       //******Add little margin to show outline lead correctly
       QRectF contRect=prt->boundingRect();
       contRect.setHeight(contRect.height()+rectMarg);
       contRect.setWidth(contRect.width()+rectMarg);
       //ui->graphicsView_plate->setSceneRect(contRect);
       prt_view->setSceneRect(contRect);

       prt->setPos(rectMarg/2,rectMarg/2);
       prt_preview_scene->addItem(prt);

       prt_preview_scene->zoomFit();
       //plate_graphics_preview->show();

       //delete cam_dxf;
       delete dl_dxf;
    }
}


void CAMExpert::on_cam_newprt_rev_params(QString name_mat,
                              QString len,
                              QString width,
                              QString mat,
                              QString thick_mat,
                              QString qty,
                              QString no,
                              bool    is_rot)
{
    //int converted to QString
    int row = model_prt_tableView->rowCount();
    QString qs_row = QString::number(row,10);
    char *c_row;
    QByteArray arr_row = qs_row.toLatin1();
    c_row = arr_row.data();
    //receive datas from new_mat window to tableView
    model_prt_tableView->setItem(row,0,new QStandardItem(c_row));
    model_prt_tableView->item(row,0)->setTextAlignment(Qt::AlignCenter);


    model_prt_tableView->setItem(row,1,new QStandardItem(len));
    model_prt_tableView->item(row,1)->setTextAlignment(Qt::AlignCenter);

    model_prt_tableView->setItem(row,2,new QStandardItem(width));
    model_prt_tableView->item(row,2)->setTextAlignment(Qt::AlignCenter);

    model_prt_tableView->setItem(row,3,new QStandardItem(thick_mat));
    model_prt_tableView->item(row,3)->setTextAlignment(Qt::AlignCenter);

    model_prt_tableView->setItem(row,4,new QStandardItem(qty));
    model_prt_tableView->item(row,4)->setTextAlignment(Qt::AlignCenter);

    model_prt_tableView->setItem(row,5,new QStandardItem(mat));
    model_prt_tableView->item(row,5)->setTextAlignment(Qt::AlignCenter);

    model_prt_tableView->setItem(row,6,new QStandardItem(no));
    model_prt_tableView->item(row,6)->setTextAlignment(Qt::AlignCenter);
    QString qs_rot;
    if(true == is_rot) qs_rot = "true";
    else qs_rot = "false";
    model_prt_tableView->setItem(row,7,new QStandardItem(qs_rot));
    model_prt_tableView->item(row,7)->setTextAlignment(Qt::AlignCenter);




}







void CAMExpert::CamOpenDxfFile(QString qs_dxf)
{

}


/*************************************
 * Slot of right menu for tableView
**************************************/
void CAMExpert::on_cam_right_menu_tableView(QPoint)
{
    right_menu->exec(QCursor::pos());
}

/********************************************
 * Slot of right menu's action for tableView
*********************************************/
void CAMExpert::on_qa_del_row()
{
  TableViewDelRow();
  RefreshTBIndex();
}

void CAMExpert::on_qa_cut_row()
{
   //2017.6.5--am
   TableViewCutRow();
   RefreshTBIndex();
}

void CAMExpert::on_qa_copy_row()
{
   //2017.6.5-am
   TableViewCopyRow();
   RefreshTBIndex();
}

void CAMExpert::on_qa_paste_row()
{
   //2017.6.5-am
   TableViewPasteRow();
   RefreshTBIndex();
}

void CAMExpert::on_tableView_item_clicked()
{
    //store model index clicked in TB_index_list,so you can make operation of copy del or paste
    QModelIndex tmp_index;
    tmp_index = ui->Plate_tableView->currentIndex();
    if(tmp_index.isValid())
    {
        TB_index_list.append(tmp_index);
        //Add the data of row to QList<RowData>
        RowData rd(model_plate_tableView->data(model_plate_tableView->index(tmp_index.row(),0)).toString(),
                   model_plate_tableView->data(model_plate_tableView->index(tmp_index.row(),1)).toString(),
                   model_plate_tableView->data(model_plate_tableView->index(tmp_index.row(),2)).toString(),
                   model_plate_tableView->data(model_plate_tableView->index(tmp_index.row(),3)).toString(),
                   model_plate_tableView->data(model_plate_tableView->index(tmp_index.row(),4)).toString(),
                   model_plate_tableView->data(model_plate_tableView->index(tmp_index.row(),5)).toString(),
                   model_plate_tableView->data(model_plate_tableView->index(tmp_index.row(),6)).toString(),
                   model_plate_tableView->data(model_plate_tableView->index(tmp_index.row(),7)).toString());
        TB_row_data_list.append(rd);
    }
    RemoveRepeatedIndex();
}































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































