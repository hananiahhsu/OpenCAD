#ifndef QPOINTFWITHPARENT_H
#define QPOINTFWITHPARENT_H

/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */


#include <QtGui>
class QPointFWithParent;
typedef QList <QPointFWithParent > QPFWPList;
typedef QVector <QPointFWithParent > QPFWPVector;

typedef  QList <QPainterPath > QPPList;
typedef  QVector <QPainterPath > QPPVector;

/// holds the point parent loop and its infos
class QPointFWithParent : public QPointF {
	 public:
		/// Refers to the entity's type the point is attached to 
	 enum type {
         Line = 0x0,
         Arc = 0x1,
         Circle = 0x2,
         Polyline = 0x3,
		 LeadCertain=0x4,
		 LeadUncertain=0x5
     };
     Q_DECLARE_FLAGS(Types, type)
	 QPointFWithParent::Types parentType;
	 /// the loop the point is linked to
	 int parentLoop;
	 /// the circle center X coord
	 double centerX;
	 ///the circle center Y coord
	 double centerY;
	 ///the circle center 
	 QPointF center;
	 /// @todo remove leadTouch at it's now loop dependant
	 QPointF leadTouch;
	 ///the circle radius 
	 double parentRadius;
	 ///the arc angle
	 double angle1;
	 /// the arc angle
	 double angle2;
	 /// The arc is clockwise
	 bool cWise;
	 
	 /**
	  *  Set the lead-out point 
	  * @param myLeadTouch Lead-out coord
	  */
	 void setLeadTouch(QPointF myLeadTouch){ leadTouch=myLeadTouch;}
	 
	 /// TODO: rename to setParentLoop for homongenity
	 /**
	  *        Set the loop number on the toolpath
	  * @param nbr loop number
	  */
	 void setLoopNbr(int nbr){ parentLoop=nbr; }
	 /**
	  *        Set the point parent type to type
	  * @param type the point parent type
	  */
	 void setParentType(QPointFWithParent::Types type ){  parentType=type; }
	 
	 /**
	  *        Set the point circle center coordinates
	  * @param newCenter 
	  */
	 void setCenter(QPointF newCenter){ center=newCenter; centerX=newCenter.x();centerY=newCenter.y();}
	 /// create by default a point stores its parents as a Line
	 QPointFWithParent (double x=0, double y=0, double cx=0,double cy=0,double radius=0,QPointFWithParent::Types type=0x0,bool cw=true, double teta1=0, double teta2=0);
	 virtual ~QPointFWithParent();
	 QPointFWithParent operator+= (QPointF point );
	 QPointFWithParent operator= (const QPointF point ); // create an overloaded ont to WithParent
	 //QPointFWithParent operator+ (const QPointFWithParent &point1 );
};
 Q_DECLARE_OPERATORS_FOR_FLAGS(QPointFWithParent::Types)


#endif // QPOINTFWITHPARENT_H
