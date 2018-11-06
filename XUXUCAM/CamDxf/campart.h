#ifndef CAMPART_H
#define CAMPART_H

/*
 ************************************
 *** @Xu Xu
 *** @2017.05.10
 *** @NanJing,China
 *** @Hananiahhsu@live.cn
 ************************************
 */
#include "CamDxf/qpointfwithparent.h"
#include "camleads.h"
#include <QtGui>
#include <QGraphicsItem>
/** A part is a QGraphicsItem composed by a number of internal loops, its outline and
the lead-in out points
*/
///@todo show possible start poits (the user may want to change the lead point for route/space
/// A QGraphicItem representing a loop (closed or not)
class Loop : public QGraphicsItem{
    /// @todo use qobject to allow passing parent and using Tr inside getLoopNumber manullay
    //Q_OBJECT
         public:
    Loop(QGraphicsItem * parent = 0);


    QRectF boundingRect() const {
        return qtRect;
    }
    QPainterPath shape() const {
        return loopShape;
    }

    ///curretn loop rect
    QRectF qtRect ;

    ///curretn loop shape
    QPainterPath loopShape;

    ///loop shape without any modif (text for now...)
    QPainterPath originalShape;


    QSettings settings;
    /// Pen for basic drawing
    QPen myPen;
    /// Pen for seledted loops drawing
    QPen selectedPen;
    /// Pen for seledted loops drawing
    QPen unSelectedPen;
    /// Pen for loops outline drawing
    QPen  outlinePen;

    ///weither or not to display loop number @todo have to get it from settings
    bool showText;
    /**
          * set the current painting pen to selection
          */
    //void setSelected(){myPen=selectedPen;}

    /// set the original loop path entity by entity
    void addPath(QPainterPath pathToAdd);
    ///add a text path to the originalShape path
    void addTextPath(QPainterPath pathToAdd);
    ///Loop number in the current part
    int loopNumber;
    ///Weither or not the loop is a circle
    bool isCircle;
    ///Used to mark the outline loop (in plasma mode)
    bool isOutline;
    ///Used to mark the closed loops (in plasma mode)
    bool isClosed;
    ///used to stat painting the path when appropriate
    bool ready;
    /// Lead-In position for this loop
    QPointFWithParent leadIn;
    /// Lead out position for this loop
    QPointFWithParent leadOut;
    /// the loop start point. If it's a closed one, then startPoint=endPoint
    QPointFWithParent startPoint;
    /// the loop start point. If it's a closed one, then startPoint=endPoint
    QPointFWithParent endPoint;
    /// @note Remove touchPoin and use eiher Start or endPoint instead for both g-code and Optimi
    QPointFWithParent touchPoint;

    void setIsClosed (bool stat) { isClosed =stat;}

    void setReady(bool b){ready=b;if (b) update();}
    /**
          *        Set the loop start point
          * @param sP new loop start point
          */
    void setStart(QPointFWithParent sP){startPoint=sP;}
    /**
          *        Set the loop end point
          * @param sP new loop end point
          */
    void setEnd(QPointFWithParent eP){endPoint=eP;}
    /**
          * 	Mark the loop as part's outline if s
          * @param s weither or not the loop is the outline
          */
    void setIsOutline(bool s){ update(); isOutline=s; }
    /**
          *        Set the loop order when cutting it
          * @param nbr the new loop number
          */
    void setNumber(int nbr);//{loopNumber=nbr;}
    /**
          *        Identify the loop as a circle
          * @param type
          */
    void setTypeCircle(bool type){isCircle=type;}

    /**
          *        Set the loop lead-in position
          * @param p lead-in coord
          */
    void setLeadIn(QPointFWithParent p){leadIn=p;}
    /**
          *        Set the loop lead-out position
          * @param p lead-out coord
          */
    void setTouchPoint(QPointFWithParent p){touchPoint=p;}

    /// the set of Points making the loop
    QPFWPVector entities;
    /**
          *        set the loop entities
          * @param ts point set
          */
    void setEntities(QPFWPVector ts);
    /**
          *
          * @return  the entities forming the loop
          */
    QPFWPVector getEntities(){return entities;}

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *);

         protected:
    virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
};


/// A part is a set of organised loops.
class Part: public QGraphicsItem
        // Qobject is needed for sending signals to alert about errenous parts
{
    // Q_OBJECT
public:
    ///@todo: cleanup this constructor
    ///@todo use set/get in naming
    /// value used when moving the part by keyboar
    static double movePartVal;

    /**
          *        set movePartVal to val
          * @param val distance the part will be translated with @see movePartVal
          */
    static void setMovePartVal(double val);
    /**
          *        Setup the part for G-code generation
          * @param shape The part shape
          * @param pointsList The points list
          * @param partPathsList The paths forming the part
          * @param circlePointsList The circles points list
          * @param circlesPathsList the circles path list
          */
    Part(const QPainterPath shape,QPFWPVector pointsList,QPPVector partPathsList,QPFWPVector circlePointsList,QPPVector circlesPathsList);

    /// used to create a copy on the sheet metal of the current part
    Part(const Part &parentPart);
    virtual ~Part();

    QRectF boundingRect() const {
        return qtRect;
    }
    /// @note : to implement collison detection and part selection Qt refers to Shape or as when drawing the part outline

    QPainterPath shape() const {
        return partOutShape;
    }


    /// define the part shape for geometry collision and painting
    QPainterPath partShape;
    /// used for selection along with partshape
    QRectF qtRect ;
    /// The painterPath refered to for selection
    QPainterPath partOutShape;
    ///declare as static in main app and use everywhere needed
    QSettings settings;
    // a croo pixmap that will allow easy item mouvement
    QGraphicsPixmapItem *selecPix;
    /// The angle at which we place a lead on a circle
    int leadAngle;
    /// the minimal distance at which we can put the leads
    double leadDist;
    /// used to approximate lead surface.
    double leadRadius;
    /// used to track erros like small radius that can't be cut with plasma
    /// @todo use error code to track them
    bool errorsFound;
    /// used to track erros like open loop "useful only in plasma mode"
    /// @todo use error code to track them
    bool openLoopError;
    ///@see COPY constuctor
    Part* copy();
    ///If the part is for preview or to be inserted in sheetmetal
    bool preview;
    ///number of closedloops
    int nbrClosedPath;
    ///number of circles
    int nbrCircles;
    ///the outline position in partLoops list
    int outlinePos;
    /// toolpath lentgh
    double tpLength;

    // @note:As we are using pointers to loops any change in a previewed part will affect all the others.
    //This is a desirbvale behavior!
    /// The loops making the part
    QVector <Loop* > partLoops;
    /// The loops making the part after GA optimization
    QVector <Loop* > partLoopsOpt;
    ///A part is a combinaison of loops (themselves composed of a number of paths ) each path has a start / end point the points definig the part
    QPFWPVector ptsVector;
    ///the painterPath used to draw the part
    QPPVector pathsList;
    ///the circles points coordinates
    QPFWPVector cirPtsList;
    ///the painterPath used to draw part circles
    QPPVector cirPathsList;
    /// the part filename, used to track it when on the sheet metal
    QString partName;

    //@todo:Maybe to be removed
    /// the coord of the outline point
    QPointFWithParent outlinePoint;
    /// The path of the outline
    QPainterPath outLinePath;


    /// stiores the lead/in out points
    QPFWPVector gLeadPoints;
    /// stiores the loops for gcode generation
    QList<QPFWPVector> gLoops;

    /**
          *        Start the part geometry parsing
          */
    void start();


    /**
          *        Generate the part lead in/out
          */
    void generateLeads();
    /**
          *        Search for the part outiline loop
          */
    void findOutline();

    /**
          *        Search for circles in the file
          */
    void  filterCircles();
    /**
          *        Starts the process of closed loop search
          * @param ptsVector the points list to parse
          */
    void generateLoops(QPFWPVector ptsVector);




    /**
          *       removes a point from the list
          * @param pointsList The old points list
          * @param pointsListNew The new points list
          * @param pos the position to take from
          * @param oldPos
          */
    void shrink(QPFWPVector &pointsList,QPFWPVector &pointsListNew,int &pos,int &oldPos);
    ///find a lonley point if any or repreted one
    /**
          *        Find an open loop ( a point in one entity
          * @param pointsList the part points list
          * @return A lonley point posistion within pointsList
          */
    int newPos(QPFWPVector &pointsList);
    /// we use the QGraphicsItem default implementation for those events
    void drawArc(QPointFWithParent point,QPainterPath &pathToModify);

    /**
          *        Return a translated coordinate
          * @param oldPoint original coor
          * @param offset delta
          * @return the new translated posiiton
          */
    QPointFWithParent translateEntity(QPointFWithParent oldPoint, QPointF offset);


    /**
          *        starts the optimlization process by calling GA TSP
          */
    void optimizeRoute();
    /**
          *        Move a part (translation)
          * @param dx Delta X
          * @param dy Delta Y
          */
    void moveMeBy(qreal dx, qreal dy);

    /**
          * Set the part name to its filename
          * @param f part filename
          */
    void setPartName(QString f){partName=f;}

    /** this matrix holds the transformations applied to the part, transformations can be one of the following:
         / translation , rotation ,shearing. wWe'll discard shearing and keep both translation and rotation
         / therefor  x' = m11*x + m21*y + dx
          /  	 	 y' = m22*y + m12*x + dy
          / where The dx and dy elements specify horizontal and vertical translation.
         / The m11 and m22 elements specify horizontal and vertical scaling.
         /And finally, the m21 and m12 elements specify horizontal and vertical shearing.
         */
    QTransform transform;
    enum { Type = UserType + 1 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return  Type;
    }

    //public slots:
    //void setMoveVal(int);
    //{qDebug()<<"test";movePartVal=val;}
    //signals:
    //void progressionStep();
    //void descStep( QString desc);
private:
  QBasicTimer timer;


    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *);
protected:
    /// called by QGraphicsItem to notify custom items that some part of the item's state changes.
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    void keyPressEvent ( QKeyEvent * keyEvent );
};



class LoopThread : public QThread
{
    Q_OBJECT
         public:
    LoopThread(Part *piece);
    Part *currentPiece;
    //QList <QPFWPList> filterLoop;
protected:
    void run();
};
#endif // PART_H



















