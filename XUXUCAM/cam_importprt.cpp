/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */

#include "cam_importprt.h"
#include "FileIO/cam_fileio.h"

CAM_ImportPrt::CAM_ImportPrt(QWidget *parent) :
    /*QDialog(parent),*/
    QFileDialog(parent)
{
//    connect(ui->buttonBox,SIGNAL(clicked()),this,SLOT(on_buttonBox_clicked()));
      connect(this,SIGNAL(filesSelected(QStringList)),this,SLOT(on_FilesSelected()));
//    connect(this,SIGNAL(destroyed(QObject *)),this,SLOT(on_destroyed()));
}

CAM_ImportPrt::~CAM_ImportPrt()
{

}


void CAM_ImportPrt::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit CAM_ImportPrt_Destroyed();
    emit CAM_ImportPrt_Close_Signal(prt_list);
}
























/******************************************
 *   Slots of imp prt
 **
 ***
 ******************************************/

void CAM_ImportPrt::on_FilesSelected()
{
  //choose the dxf file
  prt_list = this->selectedFiles();

}









