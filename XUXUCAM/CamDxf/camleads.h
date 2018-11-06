#ifndef CAMLEADS_H
#define CAMLEADS_H

/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */



#include "CamDxf/qpointfwithparent.h"
#include "CamDxf/campart.h"
#include <QtGui>
#include <QGraphicsItem>
 //Forward declaration
class Lead;
class Loop;

 /// A line linking between the lead-in and lead-out
 class Edge : public QGraphicsItem
 {
 public:
     /**
      * construct an edge between the source and destination lead in/out
      * @param sourceLead the lead-in to link to
      * @param destLead the lead-out to link to
      */
     Edge(Lead *sourceLead,Lead *destLead);
     ~Edge();


     /**
      * called to update the edge  parameters before repainting
      */
     void adjust();


 protected:
     QRectF boundingRect() const;
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

 private:
    QSettings settings;
     Lead *source, *dest;
     /// the associated source lead posisition
     QPointF sourcePoint;
      /// for graphical representation of the laser path between lead-in/out
        //double leadRadiusMotion;
     /// the associated destiation lead posisition
     QPointF destPoint;
     /// the size of the arrow at the end of the edge
     qreal arrowSize;
 };

/// A  QGraphicsEllipseItem represanting graphically the lead in/out
class Lead: public QGraphicsItem {

     public :
         ///constructor for leads certain/uncertain
     /**
      *        Construct a lead and attach it to loop
      * @param loop the loop the lead-in belongs to
      */
     Lead (Loop *loop);
         /// @todo create a more meaningfull constructor
         /// Construct the the Lead out= lead touch and attach it to loop
     Lead(Loop *loop,int j );
     virtual ~Lead();



     void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *);
     QRectF boundingRect() const {return myBoundingRect;  }
     QPainterPath shape() const{QPainterPath path; path.addEllipse(myBoundingRect);return path;}

     int type() const { return  Type; }

     QRectF myBoundingRect;
     enum { Type = UserType + 2 };
        /// the lead can be an uncertain/certain lead-in (in which case it's drawn in a diferent color) or a lead-out
     enum Leadtype {LeadCertain=0x1,LeadUncertain=0x2,LeadTouch=0x3};
     Q_DECLARE_FLAGS(leadTypes, Leadtype)
     Lead::leadTypes leadType;


     Loop *myLoop;
     /// the currtn lead position
         QPointFWithParent *myPos;
     /// the lead-out position
     QPointFWithParent leadTouch;
     /// the associated edge @see edge
     Edge *myEdge;
     /**
      *        Set myedge to the given edge
      * @param edge
      */
     void setEdge(Edge *edge){myEdge=edge;}

     ///the ellipse container (rectangle)
       QRectF laserRect;
       /// for graphical representation of the laser lead-in/out
    double leadRadius;

    QSettings settings;
     protected:
     /// called by QGraphicsItem to notify custom items that some part of the item's state changes.
     QVariant itemChange(GraphicsItemChange change, const QVariant &value);
     void mousePressEvent(QGraphicsSceneMouseEvent *event);
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
     void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
     //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};
 Q_DECLARE_OPERATORS_FOR_FLAGS(Lead::leadTypes)





#endif // CAMLEADS_H





























































