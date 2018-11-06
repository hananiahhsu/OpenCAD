/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */

#include "cam_newprt.h"

QFile mat_file("E:/X_HSU/xuxu_CAM/XUXUCAM/ConfigFiles/Materials.txt");
CAM_NewPrt::CAM_NewPrt(QWidget *parent) :
    QDialog(parent)
{
    //Init the win
    Init();

}

CAM_NewPrt::~CAM_NewPrt()
{

}


void CAM_NewPrt::Init()
{
   //pushbutton
   new_prt_new_prt = new QPushButton(this);
   //new_prt_add_prt = new QPushButton(this);
   new_prt_new_prt->setText(QString::fromLocal8Bit("新建零件"));
   //new_prt_add_prt->setText(QString::fromLocal8Bit("添加零件"));
   new_prt_new_prt->setFixedHeight(25);
   //new_prt_add_prt->setFixedHeight(25);
   new_prt_new_prt->setGeometry(QRect(145,280,80,25));
   //new_prt_add_prt->setGeometry(QRect(145,280,80,25));

   //label(initialization method)
   label_name = new QLabel(this);
   label_name->setText(QString::fromLocal8Bit("名称:"));
   label_name->setGeometry(QRect(20,10,80,25));

   label_length = new QLabel(this);
   label_length->setText(QString::fromLocal8Bit("长度:"));
   label_length->setGeometry(QRect(20,40,80,25));

   label_width = new QLabel(this);
   label_width->setText(QString::fromLocal8Bit("宽度:"));
   label_width->setGeometry(QRect(20,70,80,25));

   label_mat = new QLabel(this);
   label_mat->setText(QString::fromLocal8Bit("材料:"));
   label_mat->setGeometry(QRect(20,100,80,25));

   label_thk = new QLabel(this);
   label_thk->setText(QString::fromLocal8Bit("厚度:"));
   label_thk->setGeometry(QRect(20,130,80,25));

   label_quantity = new QLabel(this);
   label_quantity->setText(QString::fromLocal8Bit("数量:"));
   label_quantity->setGeometry(QRect(20,160,80,25));

   label_prt_no = new QLabel(this);
   label_prt_no->setText(QString::fromLocal8Bit("编码:"));
   label_prt_no->setGeometry(QRect(20,190,80,25));

   label_rot = new QLabel(this);
   label_rot->setText(QString::fromLocal8Bit("旋转:"));
   label_rot->setGeometry(QRect(20,220,80,25));

   //=====================================================
   lineE_name = new QLineEdit(this);
   lineE_name->setGeometry(QRect(60,10,150,22));

   lineE_len = new QLineEdit(this);
   lineE_len->setGeometry(QRect(60,40,150,22));

   lineE_width = new QLineEdit(this);
   lineE_width->setGeometry(QRect(60,70,150,22));

   combo_material = new QComboBox(this);
   combo_material->setGeometry(QRect(60,100,150,22));

   combo_mat_thicks = new QComboBox(this);
   combo_mat_thicks->setGeometry(QRect(60,130,150,22));

   lineE_Qty = new QLineEdit(this);
   lineE_Qty->setGeometry(QRect(60,160,150,22));

   lineE_prt_no = new QLineEdit(this);
   lineE_prt_no->setGeometry(QRect(60,192,150,22));

   chk_rot = new QCheckBox(this);
   chk_rot->setGeometry(QRect(60,222,30,22));

   //Get the path of material.txt(2017.6.7)
   QString str_path;QDir dir;
   str_path = dir.currentPath();
   str_path = str_path + "/Materials.txt";
   //Get the material from text
   StringOperations str_op;
   QList<QString> str_list;
   QList<QString> thick_list;
   QString tmp_str,name_str,pre_name_str;
   file_io.CAM_ReadText(mat_file,str_list);
   //***************************Init of combobox*********************//
   for(int i = 0;i < str_list.size();i++)
   {
       thick_list.clear();
       tmp_str = str_list[i];
       int pos = tmp_str.indexOf(",");
       //material eg:SUS304,0.8,1.0,1.2,2.0
       //name of the material
       name_str = tmp_str.left(pos);
       if(pre_name_str != name_str)
       {
          //combo_material->insertItem(0,QIcon(""),name_str);
          combo_material->insertItem(i,QIcon(" "),name_str);
          //2017.6.6--FILTER the repeated mats
          //Thickness of the materials
          str_op.DivideStrIntoList(tmp_str.right(tmp_str.length()-pos-1),",",thick_list);
          p_all_mat_list.append(thick_list);
       }

       pre_name_str = name_str;
   }
   //Here only show the thickness of the first material,other thick of mat will be shown after the mat choosed
   for(int j = 0;j < p_all_mat_list[0].size();j++)
   {
     combo_mat_thicks->insertItem(0,QIcon(" "),p_all_mat_list[0][j]);
   }
   //set editable for combo
   combo_material->setEditable(true);
   combo_mat_thicks->setEditable(true);

   //Connection of sig&slot
   //Note::You must put connect behind insertItem(),if not,index will be out of range--2017.6.6.Night(21:32)
   QObject::connect(combo_material,SIGNAL(currentIndexChanged(int)),this,SLOT(on_combo_mat_changed()));
   QObject::connect(combo_mat_thicks,SIGNAL(currentIndexChanged(int)),this,SLOT(on_combo_mat_thk_changed()));
   QObject::connect(new_prt_new_prt,SIGNAL(clicked()),this,SLOT(on_newprt_new_clicked()));

}

void CAM_NewPrt::closeEvent(QCloseEvent *event)
{
    event->accept();
    //2017.6.6--send signal of destroy(for sys signal not work)
    emit CAM_NewPrt_Destroy();
}

//bool CAM_NewPrt::event(QEvent *event)
//{

//    //return false;
//}


void CAM_NewPrt::paintEvent(QPaintEvent* event)
{
    line_painter = new QPainter(this);
    line_painter->setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap,Qt::RoundJoin));
    //line_painter->setPen(QColor(100,1,1));
    QPointF s_pnt(0.0,260),s_pnt1(0.0,265);
    QPointF d_pnt(249.0,260.0),d_pnt1(249.0,265);
    QLineF  v_line(s_pnt,d_pnt),v_line2(s_pnt1,d_pnt1);
    line_painter->drawLine(v_line);
    line_painter->drawLine(v_line2);
    //line_painter->drawLine(10,0,10,200);
}



//==================================================
//***slot of combobox
//***
//==================================================
void CAM_NewPrt::on_combo_mat_changed()
{
    //each time of item changed,refresh the contents of thickness combobox
    int cur_index = combo_material->currentIndex();
    combo_mat_thicks->clear();
    for(int i = 0;i < p_all_mat_list[cur_index].size();i++)
    {
        combo_mat_thicks->insertItem(0,QIcon(" "),p_all_mat_list[cur_index][i]);
    }
}


void CAM_NewPrt::on_combo_mat_thk_changed()
{

}


//for new prt
void CAM_NewPrt::on_newprt_new_clicked()
{
   emit CAM_NewPrt_New_Signal(lineE_name->text(),
                              lineE_len->text(),
                              lineE_width->text(),
                              combo_material->currentText(),
                              combo_mat_thicks->currentText(),
                              lineE_Qty->text(),
                              lineE_prt_no->text(),
                              chk_rot->isChecked()
                              );
}
