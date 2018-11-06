#include "CamDxf/camleads.h"



/// @bug: some loops are drawn on top of leads, which make them unselectable thius can be solved by setting
/// their parent loop to a higher z value
Lead::Lead(Loop *loop):QGraphicsItem(loop){
    leadRadius= settings.value("Leads/radius").toDouble();

    if (leadRadius<=0 ) leadRadius=3;
    laserRect=QRectF (-leadRadius/2,-leadRadius/2,leadRadius,leadRadius);
    myBoundingRect=laserRect;
    myLoop=loop;
    myPos=&myLoop->leadIn;
    setPos(myLoop->leadIn);
    data(3)=myLoop->loopNumber;
    leadTouch=myLoop->touchPoint;

    if (myPos->parentType==QPointFWithParent::LeadUncertain){
        leadType=LeadUncertain;
        setZValue(3);
    }
    else {
        leadType=LeadCertain;
    }
    ///we ensure this way that leadTouch is kept below by giving it a lower z value

    //setCacheMode(DeviceCoordinateCache);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    //laserRect=QRectF (0,0,leadRadius,leadRadius);
    Lead *touchLead=new Lead(myLoop,0);
    myEdge=new Edge(this,touchLead);
}


Lead::Lead(Loop *loop,int j):QGraphicsItem(loop){
    leadRadius= settings.value("Leads/radius").toDouble();
    if (leadRadius==0 ) leadRadius=3;
    laserRect=QRectF (-leadRadius/2,-leadRadius/2,leadRadius,leadRadius);
    myBoundingRect=laserRect;
    myLoop=loop;
    setPos(myLoop->touchPoint);
    data(3)=myLoop->loopNumber;
    setOpacity(0.25);
    leadTouch=myLoop->touchPoint;
    leadType=LeadTouch;
    //setCacheMode(DeviceCoordinateCache);
    //setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}


void Lead::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //painter->setPen( settings.value("Colors/pen").value<QColor>());
    //painter->setBrush( settings.value("Colors/brush").value<QColor>());
    // painter->setRenderHint(QPainter::Antialiasing);
    if (leadType==LeadCertain) {
        painter->setBrush(Qt::green);
    }
    else if (leadType==LeadTouch) {
        painter->setBrush(Qt::blue);
    }
    else {
        painter->setBrush(Qt::red);
    }
    painter->setPen(QPen(Qt::black));
    painter->drawEllipse(laserRect);
    //qDebug()<<myPos<<myPos.leadTouch;
    //painter->drawLine(myPos,myPos.leadTouch);

}


QVariant Lead::itemChange(GraphicsItemChange change, const QVariant &value){

    switch (change) {
    case ItemPositionHasChanged:
        ///NOTE:We only need to update the matrix when we release the mouse buton
        update();
        break;
        //case ItemSelectedChange	:
        case ItemSelectedHasChanged :
        if (isSelected())
           leadType=LeadTouch;
        else
           leadType=LeadCertain;
        //qDebug()<<"Lead selected";
        break;
    default:

        break;
    };
    return QGraphicsItem::itemChange(change, value);
}

void Lead::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if (isObscured())
        qDebug()<<"Sorry I'm under another item";
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Lead::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){

    qDebug()<<"replacing "<<myPos->x()<<myPos->y();
    myPos->setX(pos().x());
    myPos->setY(pos().y());
    update();
    QGraphicsItem::mouseReleaseEvent(event);
    //myLoop->leadIn=myPos;
    /// note: shouldn't call myPos=scenePos(); to avoid loosing loopparent ,...
    ///qDebug()<<"replacing "<<myPiece->leadsPoints.at(parentLoop)<<"with"<<myPos;
    ///myPiece->leadsPoints.replace(parentLoop,myPos);

}


void Lead::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    // call adjust to redraw arrow
    myEdge->adjust();
    update();
    QGraphicsItem::mouseMoveEvent(event);
}



/* Edges are inspired from Qt exapmle elastic node*/

Edge::Edge(Lead *sourceLead,Lead *destLead): arrowSize(1) {
    //setAcceptedMouseButtons(0);
   // leadRadiusMotion= settings.value("Leads/radiusMotion").toDouble();
    source = sourceLead;
    dest = destLead;
    sourcePoint=QPointF(source->myPos->x(),source->myPos->y());
    destPoint=dest->leadTouch;

    // sourcePoint=sourceLead->myPos;

    //Note: Have to keep this although it emit a warnign Item alreayadde to the scene
    //set it's parent when adding functionnalty Add manual leads!
    setParentItem(sourceLead);

    source->setEdge(this);
    dest->setEdge(this);
    adjust();
}

void Edge::adjust()
{
    /// sourcePoint=source->myPos;
    /// NOTE: mapfrom Item is the key;)
    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    //QLineF line(sourcePoint,destPoint);
    qreal length = line.length();
    QPointF edgeOffset((line.dx() * 3) / length, (line.dy() * 3) / length);
    //prepareGeometryChange();
    update();
    sourcePoint = line.p1() ;//+ edgeOffset;
    destPoint = line.p2() ;//- edgeOffset;

}

QRectF Edge::boundingRect() const
{
    return QRectF(sourcePoint,destPoint);
}

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    // Draw the line itself
    QLineF line(sourcePoint, destPoint);
    painter->setPen(QPen(Qt::black, settings.value("Leads/radiusMotion").toDouble(), Qt::SolidLine));//, Qt::RoundCap, Qt::RoundJoin));

    painter->drawLine(line);

    // Draw the arrows if there's enough room
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    //QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,cos(angle + Pi / 3) * arrowSize);
    //QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,  cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
    //painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}

Edge::~Edge()
{
}

Lead::~Lead(){
}

















