#ifndef CAM_ENTITY_H
#define CAM_ENTITY_H
#include "CamCores/CamEng/cam_entity_container.h"
#include "CamCores/CamEng/cam_vector.h"
/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */
//Abstr virtual Class(interfaces supplied)
class Cam_Entity
{
public:
    Cam_Entity();
    Cam_Entity(Cam_Entity_Container* father=nullptr);
    virtual~Cam_Entity()=default;

public:
   //All functions
    void Init();
    virtual void InitId();
    virtual Cam_Entity* Clone()const=0;
    virtual void DuplicateFather(Cam_Entity_Container* father);

    void CamResetBorders();
    void CamSetBorders();
    void CamScaleBorders(const Cam_Vector& bias);
    void CamMoveBorders(const Cam_Vector& ori,const Cam_Vector& ratio);



public:
    //all variables






protected:
    //Father or nullPtr(no father)
    Cam_Entity_Container *e_father;
    //Id of entity
    unsigned long int e_id;
    //




};






















#endif // CAM_ENTITY_H
