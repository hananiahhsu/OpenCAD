#ifndef CAM_VECTOR_H
#define CAM_VECTOR_H
/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */

#include <math.h>
#include <algorithm>

class Cam_Vector
{
public:
    //default
    Cam_Vector() {}
    //with multi-params
    Cam_Vector(double v_x,double v_y,double v_z=0.0);
    //single param firbidden implicit calling
    explicit Cam_Vector(double angle);
    explicit Cam_Vector(bool valid);
    //de-construc(no-vir)
    ~Cam_Vector()=default;
    //Any primitive type to bool
    explicit operator bool()const;
    //coords of setting
    void CamSetUnitVector(double angle);//by dir
    void CamSet(double x,double y,double z=0.0);
    void CamSetPolar(double radius,double angle);
    Cam_Vector CamGetPolar(double radius,double angle);
    //operators of basic maths computation
    double CamTwoD3Dist(const Cam_Vector& v)const;
    double CamGetCorrectAngle();//atan2
    double CamGetAngleToVector(const Cam_Vector& v)const;
    double CamGetAngleBetweenVectors(const Cam_Vector& u,const Cam_Vector& v)const;
    double CamGetVectorLength(const Cam_Vector& v);
    double CamGetMagnitudeSquared();
    double CamGetMagnitudeSquaredToVector(const Cam_Vector& v);
    double CamLerp(const Cam_Vector& v,double t);
    //Position check
    bool CamIsInWindow(const Cam_Vector& l_pnt,const Cam_Vector& r_pnt);
    bool CamIsInWindowOrdered(const Cam_Vector& u_pnt,const Cam_Vector& d_pnt);
    //type trans
    Cam_Vector CamToInteger();
    //basic operation









public:
    double x=0.0;
    double y=0.0;
    double z=0.0;
    bool is_valid=false;
};
























#endif // CAM_VECTOR_H























































