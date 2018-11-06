#include "cam_fileio.h"

CAM_FileIO::CAM_FileIO(void)
{

}

CAM_FileIO::~CAM_FileIO()
{

}

//read text by line
void CAM_FileIO::CAM_ReadText(QFile& file,QList<QString>& str_list)
{
  //2017.5.22-iF the file is already opened,donnot open it again that will fail
  if(file.atEnd())
  {
    file.close();
  }
  if(!file.open(QIODevice::Text | QIODevice::ReadOnly))
  {
     QMessageBox::information(NULL,"info","Failed to open text!",QMessageBox::Yes | QMessageBox::No);
     return;
  }
  //open success
  while(!file.atEnd())
  {
     QByteArray line = file.readLine();
     QString str(line);
     str_list.append(str);
  }
  file.close();
}

//read text by QStreamText
void CAM_FileIO::CAM_ReadText_ByStream(QFile& file,QString& lines)
{
    //2017.5.22-iF the file is already opened,donnot open it again that will fail
    if(file.atEnd())
    {
      file.close();
    }
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       QMessageBox::information(NULL,"info","Failed to open text!");
       return;
    }
    QTextStream in(&file);
    lines = in.readLine();
    while(!lines.isNull())
    {
       lines = in.readLine();
    }
    file.close();
}

//write text line by line
void CAM_FileIO::CAM_WriteText(QFile& file,QString& strs)
{
    //2017.5.22-iF the file is already opened,donnot open it again that will fail
    if(file.atEnd())
    {
      file.close();
    }
    //append to the end of txt  and move to next line
    if(!file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append))
    {
       QMessageBox::information(NULL,"info","Failed to open text,please check the path of file!");
       return;
    }
    QTextStream stream(&file);
    //move the pointer to the end of file
    stream.seek(file.size());
    stream << '\n';
    //write the contents
    stream << QObject::trUtf8(strs.toLocal8Bit().data());
}
