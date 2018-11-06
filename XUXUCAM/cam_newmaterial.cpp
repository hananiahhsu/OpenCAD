/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */
#include "cam_newmaterial.h"
#include "ui_cam_newmaterial.h"
#include "StringOperation/stringoperation.h"

QFile file("E:/X_HSU/xuxu_CAM/XUXUCAM/ConfigFiles/Materials.txt");
CAM_NewMaterial::CAM_NewMaterial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAM_NewMaterial)
{
    ui->setupUi(this);
    //Init the win
    Init();

    //***********************Slots***********************************************************//
    //Material names
    connect(ui->comboBox_2,SIGNAL(currentIndexChanged(int)),this,SLOT(on_comboBox_2_choosed()));
    //Thickness of mat
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_comboBox_choosed()));
    //Get the input of name of materials(No need)
    //connect(ui->lineEdit_2,SIGNAL(returnPressed()),this,SLOT(on_lineEdit2_changed()));
    //Get the value of thick of mat
    connect(ui->lineEdit,SIGNAL(editingFinished()),this,SLOT(on_lineEdit_changed()));
    //New material(from library)
    connect(ui->pushButton_new,SIGNAL(clicked(bool)),this,SLOT(on_pushbutton_new_clicked()));
    //Add material to lib(or txt file)
    connect(ui->pushButton_add,SIGNAL(clicked(bool)),this,SLOT(on_pushbutton_add_clicked()));

}

CAM_NewMaterial::~CAM_NewMaterial()
{
    delete ui;
}
//Initialize the win
void CAM_NewMaterial::Init()
{
   //Get the path of file
   QString str_path;QDir dir;
   str_path = dir.currentPath();
   //Get the material from text
   StringOperations str_op;
   QList<QString> str_list;
   QList<QString> thick_list;
   QString tmp_str,name_str,pre_name_str;
   file_io.CAM_ReadText(file,str_list);
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
          ui->comboBox_2->insertItem(i,QIcon(""),name_str);

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
     ui->comboBox->insertItem(0,QIcon(""),p_all_mat_list[0][j]);
   }
   //set editable for combo
   ui->comboBox_2->setEditable(true);
   ui->comboBox->setEditable(true);




}

/**************************************
**Signals--2017.5.24
**************************************/



/**************************************
**Slots
**************************************/
//Choose the thickness from combo eg.1.0
void CAM_NewMaterial::on_comboBox_choosed()
{
   //After choose the value of thickness,you can use it to calculate
   //


}
//choose the materials from combo eg.sus304L
void CAM_NewMaterial::on_comboBox_2_choosed()
{
   //each time of item changed,refresh the contents of thickness combobox
   int cur_index = ui->comboBox_2->currentIndex();
   ui->comboBox->clear();
   for(int i = 0;i < p_all_mat_list[cur_index].size();i++)
   {
       ui->comboBox->insertItem(0,QIcon(""),p_all_mat_list[cur_index][i]);
   }
}


//input thick of mat
void CAM_NewMaterial::on_lineEdit_changed()
{
   qs_name_mat = ui->lineEdit_2->text();
   qs_thick_mat = ui->lineEdit->text();
   //check if qs_name_mat is empty
   if(!qs_name_mat.isEmpty() && !qs_thick_mat.isEmpty())
   {
      QString tmp_qs;
      tmp_qs = qs_name_mat + "," + qs_thick_mat;
      //write text to txt files of material
      file_io.CAM_WriteText(file,tmp_qs);
   }
}


void CAM_NewMaterial::on_pushbutton_new_clicked()
{
    //Send signal to dock
    emit CAM_NewMat_New_Signal(ui->comboBox_2->currentText(),ui->comboBox->currentText());
}

void CAM_NewMaterial::on_pushbutton_add_clicked()
{
   //Send signal to Dock
    emit CAM_AddMat_Add_Signal(ui->lineEdit_2->text(),ui->lineEdit->text());
}



/**************************************
**Events--2017.5.24
**************************************/

void CAM_NewMaterial::closeEvent(QCloseEvent *event)
{
    emit CAM_NewMat_Close_Signal(ui->comboBox_2->currentText(),ui->comboBox->currentText());
}

























































































































































