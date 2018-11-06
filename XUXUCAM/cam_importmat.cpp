/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */

#include "cam_importmat.h"
#include "ui_cam_importmat.h"
#include "camexpert.h"//???
#include <QCloseEvent>
#include "FileIO/cam_fileio.h"

CAM_ImportMat::CAM_ImportMat(QWidget *parent) :
    /*QDialog(parent),*/
    QFileDialog(parent),
    ui(new Ui::CAM_ImportMat)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(clicked()),this,SLOT(on_buttonBox_clicked()));
    connect(this,SIGNAL(filesSelected(QStringList)),this,SLOT(on_FilesSelected()));
    connect(this,SIGNAL(destroyed(QObject *)),this,SLOT(on_destroyed()));
}

CAM_ImportMat::~CAM_ImportMat()
{
    delete ui;
}


void CAM_ImportMat::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit CAM_ImportMat_Close_Signal(dxf_list);
}


//==============================================================//
//**Slots
//**
//==============================================================//
//press the OKAY or Cancel,emit the signal too(this 2 buttons are disabled-2017/05/26)
void CAM_ImportMat::on_buttonBox_clicked(QAbstractButton *button)
{
     emit CAM_ImportMat_Close_Signal(dxf_list);
}

void CAM_ImportMat::on_FilesSelected()
{
  //choose the dxf file
  dxf_list = this->selectedFiles();
  //Just for test here
  if(dxf_list.size() > 0)
  {
    dxf_name = dxf_list[0];
  }
}
//Failed to emit-17.05.26
void CAM_ImportMat::on_destroyed()
{
    emit CAM_ImportMat_Close_Signal(dxf_list);
}
