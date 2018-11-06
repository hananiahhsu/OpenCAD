/*****************************************************************************
**  The name of class have been modified to CamDxf by Hubery.x.Hsu(NanJing,2017/05/31)
**  $Id: CamDxf.cpp 8865 2008-02-04 18:54:02Z andrew $
**
**  This is part of the dxflib library
**  Copyright (C) 2001 Andrew Mustun
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU Library General Public License as
**  published by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Library General Public License for more details.
**
**  You should have received a copy of the GNU Library General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

#include "CamDxf/camdxf.h"

#include <iostream>
#include <stdio.h>


/**
 * Default constructor.
 */
CamDxf::CamDxf() {
      polylineNew= true;
      polylineFirst= true;
      corrupted=false;
      inBlock=false;
      partPath.moveTo( 0,0);
      partPath.setFillRule(Qt::WindingFill);
}

/**
 * Sample implementation of the method which handles line entities.
 */

  QPainterPath CamDxf::drawLine( const QPointF & startPoint, const QPointF & endPoint) {
     QPainterPath lineEntity=QPainterPath ();
     lineEntity.moveTo(startPoint);
     lineEntity.lineTo(endPoint);
     return lineEntity;
    }

void CamDxf::addLine(const DL_LineData& data) {

     if (inBlock) return;

     QPointFWithParent p1( roundMe(data.x1), roundMe(data.y1));
     QPointFWithParent p2( roundMe(data.x2),roundMe(data.y2));
     /// if a line ending= it's begining have to skip it (may occur in some poorly coded cad)
     if ( p1==p2 ) return;

     /// TODO:check if we previously had added this line to avoid crashes

     QPainterPath lineTemp=drawLine(p1,p2);
     /// store the line in the paths list if not already present, some CAD frawinfg contains repeated entities
     if (!partPathsList.contains(lineTemp)){
         partPathsList.append(lineTemp);
         //pointsPathList.append( p1);
         //pointsPathList.append( p2);

         pointsPathVector.append( p1);
         pointsPathVector.append( p2);
         }
     else{
        qDebug () << "WARNING: line already present skipping...the part may be corrupted!!";
        corrupted=true;
        return;
        }
     partPath.moveTo( p1);
     partPath.lineTo( p2);
    }


QPainterPath CamDxf::drawArc( const QPointFWithParent & startPoint, const QRectF & rectangle, qreal startAngle, qreal sweepAngle) {
     QPainterPath arcEntity=QPainterPath ();
     arcEntity.moveTo(startPoint);
      /// Clockwise arcs can be specified using negative angles(selon doc Qt) fabs or qAbs ?
     arcEntity.arcTo(rectangle,-(startAngle),-qAbs(sweepAngle));
     return arcEntity;
    }

void CamDxf::addArc(const DL_ArcData& data) {
      double ret=180;
     //static const double Pi = 3.14159265358979323846264338327950288419717;
     if (inBlock) return;
     double teta1=data.angle1,teta2=data.angle2,sweep;
     bool cw=true;
    // qDebug()<<teta1<<teta2;
     //if teta1==0 teta1=360;
     if (teta1>teta2) { /// have to go clockwise or swap values
         teta2=ret*2+teta2;
        }
         //sweep=data.angle2-data.angle1;
         // if (sweep>0) {qDebug()<<"counter clockwise arc";cw=false;}
         //else {qDebug()<<"clockWise arc";cw=true;}

        /**IMPORTANT NOTE:As we always start from p1 (our start point) we have to assign clockwaise
        to P1 and counterClockwise to P2 this way when we organsie the entities and start from p2 instead
        from p1 the G2/G3 codes are coretly assigned!
        */

     QPointFWithParent p1(getPoint(data.cx,teta1,data.radius,true),getPoint(data.cy,teta1,data.radius,false),data.cx,data.cy ,data.radius,QPointFWithParent::Arc,cw,teta1,teta2);
     QPointFWithParent p2(getPoint(data.cx,teta2,data.radius,true),getPoint(data.cy,teta2,data.radius,false),data.cx,data.cy ,data.radius,QPointFWithParent::Arc,!cw,teta1,teta2);
     ///QPointFWithParent p1(data.cx + cos((teta1)*M_PI/ret) * data.radius,data.cy + sin((teta1)*M_PI/ret) * data.radius,data.cx ,data.cy ,data.radius,QPointFWithParent::Arc,cw,teta1,teta2);
     ///QPointFWithParent p2(data.cx + cos((teta2)*M_PI/ret) * data.radius,data.cy + sin((teta2)*M_PI/ret) * data.radius,data.cx ,data.cy ,data.radius,QPointFWithParent::Arc,!cw,teta1,teta2);

     QPointF p3(data.cx + cos((teta2)*M_PI/ret) * data.radius,data.cy + sin((teta2)*M_PI/ret) * data.radius);
     QRectF boundingRect(data.cx-data.radius, data.cy-data.radius,2*data.radius,2*data.radius);


     QPainterPath arcTemp=drawArc(p1,boundingRect,teta1,teta1-teta2);
     /// store the line in the paths list if not already present, some CAD frawinfg contains repeated entities
     if (!partPathsList.contains(arcTemp)){
         partPathsList.append(arcTemp);
         }
     else{
        qDebug () << "Arc already present skipping...the part may be corrupted!!";
        corrupted=true;
        return;
        }

     //pointsPathList.append(p1);
     //pointsPathList.append(p2);
     pointsPathVector.append( p1);
     pointsPathVector.append( p2);

     partPath.moveTo(p1);
     partPath.arcTo(boundingRect,-(teta1),-qAbs(teta1-teta2));
     //qDebug() << "started arc  at:"<<p1;
     //qDebug() << "Drawn arc ends at:" <<partPath.currentPosition();
     //qDebug() << "Expected arc ends at:" <<p2;
     //qDebug() << "Expected arc ends at:" <<p3;
}

QPainterPath CamDxf::drawCircle( const QPointFWithParent &centerPoint) {
     QPainterPath circleEntity=QPainterPath ();
     qreal radius=centerPoint.parentRadius;
     circleEntity.addEllipse(centerPoint,radius,radius);
     return circleEntity;
    }


    double CamDxf::roundMe(double val){
     return (qRound(val* 1000000.0) / 1000000.0);
    }

    double CamDxf::getPoint(double pos,double teta1,double radius,bool cosin){
     double ret=180;
     if (cosin){
     return (qRound((pos + cos((teta1)*M_PI/ret) * radius)* 1000000.0) / 1000000.0);
     }
     else {
     return (qRound((pos + sin((teta1)*M_PI/ret) * radius)* 1000000.0) / 1000000.0);
     }
    }
    ///To deal with the round problem we trim to 7 digits (maybe has to be an option?

    /**
 * Circles are special entities as they 're defined only be a unique point, their center (and the radius)
 */
void CamDxf::addCircle(const DL_CircleData& data) {
     if (inBlock) return;
     QPointFWithParent center(data.cx,data.cy,data.cx,data.cy,data.radius,QPointFWithParent::Circle);

     if (circlePathsList.contains(drawCircle(center))){
        qDebug () << "Circle already present skipping...the part may be corrupted!!";
        corrupted=true;
        return;
        }
     //pointsCircleList.append(center);
     pointsCircleVector.append(center);
     circlePathsList.append(drawCircle(center));
     //QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem (data.cx, data.cy, data.radius,data.radius);
     //ellipsesList.append(ellipse);
     partPath.addEllipse(QPointF(data.cx, data.cy), data.radius,data.radius);

    }

void CamDxf::addBlock(const DL_BlockData& data) {

     //printAttributes();
    // qDebug() <<  data.name.c_str();
       //printf(" Type: %s\n", data.name.c_str());
       ///blocks have to be stored as paths and called afterwhile with insert
     inBlock=true;
    }

    void CamDxf::endBlock(){

     inBlock=false;
    // qDebug() << "end block";
    }

    void CamDxf:: addMText(const DL_MTextData& data){

    //qDebug() << " Adding Mtext";
    }


    void CamDxf:: addText(const DL_TextData& data){

    qDebug() << " Adding text";

    }
     void CamDxf:: addInsert(const DL_InsertData& data)
{

    //qDebug() << " Adding block "<<data.name.c_str();

    }
/**
 * Sample implementation of the method which handles polyline entities.
 */


 /// see 14- 249.dxf for a polyline in a part
void CamDxf::addPolyline(const DL_PolylineData& data) {
       if (inBlock) return;

     ///FIXME : use the built in DXF SEQEND to draw polylines

     printf("Adding a POLYLINE \n");
     //printf("flags: %d\n", (int)data.flags);
     //printAttributes();
     /// inform the vertex joiner that's a new path
     polylineNew=true;

     /// we have to wait for the polyline to end before ending it to polylinesList
     if (!polylineFirst) {
     /// store the polyline as a PathItem for later use
     QGraphicsPathItem *polyline = new QGraphicsPathItem ();
     polyline->setPath (polylineCurrent);
     ///add the polyline to the polylinesList
     polylinesList.append(polyline);
     partPath.addPath(polylineCurrent);
     }
     /// Prepare the new polyline path
     QPainterPath polylineCurrent;//= new ();

}


/**
 * Sample implementation of the method which handles vertices.
 */

 //NOTE IF a vertex comes between two other entities it's lost !!!
void CamDxf::addVertex(const DL_VertexData& data) {
    // printf("VERTEX   (%6.3f, %6.3f, %6.3f) %6.3f\n",data.x, data.y, data.z,data.bulge);
     // printAttributes();
     if (inBlock) return;
     QPointF vertex ( roundMe(data.x), roundMe(data.y) );
     vertexesList.append(&vertex);
     /// FIXME handle the case: a vertex is encountered which isn't a part of polyline !!

    /// The vertexes are members of the same polyline as long as polylineNew=false
    if (polylineNew) {
     polylineCurrent.moveTo(vertex);
     partPath.moveTo(vertex);

      qDebug() << "startuing a new polyLine ";
    }
    else 	{
     /// add the vertex to the current path
     polylineCurrent.lineTo(vertex);
     partPath.lineTo(vertex);
     qDebug() << "joining vertexes";
    }
     polylineNew=false;
     polylineFirst=false;
}


/**
 * Sample implementation of the method which handles layers.
 */
   void CamDxf::addLayer(const DL_LayerData& data) {
     printf("LAYER: %s flags: %d\n", data.name.c_str(), data.flags);
     // printAttributes();
  }

/**
 * Sample implementation of the method which handles point entities.
 */
void CamDxf::addPoint(const DL_PointData& data) {
     //  printf("POINT    (%6.3f, %6.3f, %6.3f)\n",
      //     data.x, data.y, data.z);
      // printAttributes();
     // if (inBlock) return;
}

void CamDxf::add3dFace(const DL_3dFaceData& data) {
    printf("3DFACE\n");
    for (int i=0; i<4; i++) {
        printf("   corner %d: %6.3f %6.3f %6.3f\n",
            i, data.x[i], data.y[i], data.z[i]);
    }
    printAttributes();
}


void CamDxf::printAttributes() {
    printf("  Attributes: Layer: %s, ", attributes.getLayer().c_str());
    printf(" Color: ");
    if (attributes.getColor()==256)	{
        printf("BYLAYER");
    } else if (attributes.getColor()==0) {
        printf("BYBLOCK");
    } else {
        printf("%d", attributes.getColor());
    }
    printf(" Width: ");
    if (attributes.getWidth()==-1) {
        printf("BYLAYER");
    } else if (attributes.getWidth()==-2) {
        printf("BYBLOCK");
    } else if (attributes.getWidth()==-3) {
        printf("DEFAULT");
    } else {
        printf("%d", attributes.getWidth());
    }
    //printf(" Type: %s\n", attributes.getLineType().c_str());//2017-06-01
}



// EOF
