#include "CamCores/CamEng/cam_vector.h"



//with multi-params
Cam_Vector::Cam_Vector(double v_x,double v_y,double v_z)
    :x(v_x),y(v_y),z(v_z),is_valid(true)
{
 //...
}

//single param firbidden implicit calling
Cam_Vector::Cam_Vector(double angle)
    :x(cos(angle)),y(sin(angle)),is_valid(true)
{
 //...
}

Cam_Vector::Cam_Vector(bool valid)
    :is_valid(valid)
{
 //...
}


//Any primitive type to bool
Cam_Vector::operator bool()const
{
    return is_valid;
}
//coords of setting
void Cam_Vector::CamSetUnitVector(double angle)//by dir
{
   //sqrt(cos()^2+sin()^2) == 1
   x = cos(angle);
   y = sin(angle);
   z = 0.0;
   is_valid = true;
}

void Cam_Vector::CamSet(double x,double y,double z)
{
   x = x;
   y = y;
   z = z;
   is_valid = true;
}

void Cam_Vector::CamSetPolar(double radius,double angle)
{
    x = radius * cos(angle);
    y = radius * sin(angle);
    z = 0.0;
    is_valid = true;
}

Cam_Vector Cam_Vector::CamGetPolar(double radius,double angle)
{
   return {radius*cos(angle),radius*sin(angle),0.0};
}

//operators of basic maths computation
double Cam_Vector::CamTwoD3Dist(const Cam_Vector& v)const
{
   return sqrt((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y) + (z-v.z)*(z-v.z));
}
double Cam_Vector::CamGetCorrectAngle()//atan2
{


    return 0 ;
}

double Cam_Vector::CamGetAngleToVector(const Cam_Vector& v)const
{
   return 0;
}
double Cam_Vector::CamGetAngleBetweenVectors(const Cam_Vector& u,const Cam_Vector& v)const
{
   return 0;
}
double Cam_Vector::CamGetVectorLength(const Cam_Vector& v)
{
   return 0;
}

double Cam_Vector::CamGetMagnitudeSquared()
{
   return 0 ;
}

double Cam_Vector::CamGetMagnitudeSquaredToVector(const Cam_Vector& v)
{
  return 0;
}

double Cam_Vector::CamLerp(const Cam_Vector& v,double t)
{
   return 0;
}

//Position check
bool Cam_Vector::CamIsInWindow(const Cam_Vector& l_pnt,const Cam_Vector& r_pnt)
{

    return false;
}

bool Cam_Vector::CamIsInWindowOrdered(const Cam_Vector& u_pnt,const Cam_Vector& d_pnt)
{
  return false;
}





















Cam_Vector Cam_Vector::CamToInteger()
{
   x = rint(x);
   y = rint(y);
   return *this;
}






