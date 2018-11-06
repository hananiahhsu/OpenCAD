#include "CamCores/CamEng/cam_entity.h"


//Default construction
Cam_Entity::Cam_Entity()
{

}


//constr
Cam_Entity::Cam_Entity(Cam_Entity_Container* father)
{
   this->e_father = father;
   Init();
}



/*************************************\
 * @2017.6.8
 * @All implementations of functions
 * @Xu.Hsu
\*************************************/
//Pure vir functions forbidden imp
//................................
//-Init
void Cam_Entity::Init()
{
   //regular setting s.a.archives

   //init id of entity
   InitId();

}

//-init id of entity
void Cam_Entity::InitId()
{
   static unsigned long int id_counter = 0;
   e_id = id_counter++;
}


//-noneed to explain
void Cam_Entity::DuplicateFather(Cam_Entity_Container* father)
{
   this->e_father = father;
}

void Cam_Entity::CamResetBorders()
{

}

void Cam_Entity::CamSetBorders()
{

}

void Cam_Entity::CamScaleBorders(const Cam_Vector& bias)
{

}

void Cam_Entity::CamMoveBorders(const Cam_Vector& ori,const Cam_Vector& ratio)
{

}



















