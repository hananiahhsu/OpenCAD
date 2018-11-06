#ifndef CAM_FILEIO_H
#define CAM_FILEIO_H
/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
class CAM_FileIO
{
public:
    CAM_FileIO(void);
    virtual ~CAM_FileIO();
public:
 //read text by line
 void CAM_ReadText(QFile& file,QList<QString>& str_list);
 //read text by QStreamText
 void CAM_ReadText_ByStream(QFile& file,QString& lines);
 //write text
 void CAM_WriteText(QFile& file,QString& strs);


};

#endif // CAM_FILEIO_H
