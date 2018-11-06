#ifndef CAM_DOCUMENT_H
#define CAM_DOCUMENT_H

/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */

#include "CamCores/CamEng/cam_pen.h"
#include <QString>
/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */
class  Cam_Document
{
public:
     Cam_Document();
     virtual ~Cam_Document();




     /********basic operation of docu*******/
     virtual bool cam_save();
     virtual bool cam_saveas(bool is_auto_save);
     virtual bool cam_open(const QString file_name);
     virtual bool cam_loadtemplate();
     virtual void cam_newdoc();



public:
     //record the state of doc
     bool is_modified;
     //name of auto saved
     QString auto_save_file_name;
     //Current filename(new)
     QString cur_file_name;
     //cur active cam pen
     Cam_Pen *cam_pen;
     //format of docu

     //graphicsView for reading or saving cur view





};

































#endif // CAM_DOCUMENT_H
