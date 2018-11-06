#include "CamDxf/camscene.h"


///Todo move to class window and get value from settings dialog
/// used when drawing toolpath and for homing on G-code
QPointF absoluteHome(0,0);
QPointF homePoint(0,0);
int currentLoop=0;
QGraphicsPixmapItem *toolPix;
QGraphicsLineItem toolLine;
/// margin used to properly show piece in preview sheet @todo Add in option dialog
const int rectMarg=20;


CamScene::CamScene (bool prev,QWidget *parent):QGraphicsScene (parent) {
    selectedSome=false;
    preview=prev;
    setMode(MoveItem);
    /// We don't show the sheet rect in preview dock
    if (!preview)
    setSheetRect();
    toolPen = QPen  (Qt::blue,1, Qt::DotLine);
}


void CamScene::setSheetRect(){
    // we ensure that the rect exist (may happen after clearinig the scene)

    QPen rectPen( settings.value("Colors/sheetPen").value<QColor>());
    QBrush rectBrush(Qt::NoBrush);
    if (settings.value("Options/colorizeInner").toInt()==2){
        rectBrush=QBrush(settings.value("Colors/sheetBrush").value<QColor>());
    }

    int h=2000;
    int w=3600;

    if (settings.value("Options/sheetH").toInt()!=0 && settings.value("Options/sheetW").toInt()!=0){
        h=settings.value("Options/sheetH").toInt();
        w=settings.value("Options/sheetW").toInt();
        /// @check if setting to (0,0,h,w) makes a diff
    }
    //scene->setSheetRect(QRect(0,0,h, w));

    sheetRect=addRect((QRect(0,0,h, w)),rectPen,rectBrush);
}


void CamScene::zoomFit(){
    //@todo: change to an approch not depending on the graphicxsView
    views().at(0)->fitInView(itemsBoundingRect(),Qt::KeepAspectRatio);
}

//The default mouse move event on diagScene handle a one by one item move. To move mutiple items
// we implment outr own one. @ note: multiple items MOVE IS DONE BY HOLDING CTRL



/// @todo: have to disable selection when zooming
void CamScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
   if (myMode == Zoom) {
                //emit zoomRect(mouseEvent->scenePos(),mouseEvent->lastScenePos());
                //return;
       views().at(0)->fitInView(QRectF(mouseEvent->scenePos(),mouseEvent->lastScenePos()),Qt::KeepAspectRatio);
            }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void CamScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mouseEvent->button()==Qt::MidButton){
        // views().at(0)->setDragMode(QGraphicsView::ScrollHandDrag);
        // QGraphicsScene::mousePressEvent(mouseEvent);

    }
    else{
        if ((selectedItems()).size()>=1){
            selectedSome=true;
            qDebug()<<"made selection";
            QGraphicsScene::mousePressEvent(mouseEvent);
        }
        else {
            selectedSome=false;
            mouseEvent->ignore();
        }
    }
}

void CamScene::wheelEvent(QGraphicsSceneWheelEvent *mouseEvent)    {
    /// @todo: add option to change modifier key to activate zoom
    //if(mouseEvent->modifiers()==Qt::ControlModifier){

    qreal scaleFactor=pow((double)2, -mouseEvent->delta() / 240.0);
    qreal factor =views().at(0)->matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    views().at(0)->scale(scaleFactor, scaleFactor);
    views().at(0)->centerOn(mouseEvent->scenePos());
    //necerrary to avoid  propagating event to child items although no mouse whell is implmenton parts for now...
    mouseEvent->setAccepted(true);
    //return;
    //}
}

void CamScene::zoom(bool in){
    qreal scaleFactor;
    if (in){
        scaleFactor=pow((double)2, -24 / 240.0);
    }
    else{
        scaleFactor=pow((double)2, 24 / 240.0);
    }
    qreal factor =views().at(0)->matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();

    if (factor < 0.07 || factor > 10)
        return;

    views().at(0)->scale(scaleFactor, scaleFactor);
}
void CamScene::cleanUpAnim(bool end){

    removeItem(&toolLine);
    removeItem(toolPix);
    currentLoop=0;

    /// @todo use Qt rc files
    if (!end){
        toolPix=addPixmap(QPixmap("/home/invite/Desktop/bazar/PFE/camnest/tool.png"));
        //to guarantee that the tool is on top
        /// @note: first we find part loops at z=0 then  comes Leads and loops numbers and finally the tool pix when in animation mode
        toolPix->setZValue(3);
        toolPix->setPos(absoluteHome);
    }
    toolLine.setZValue(3);

    homePoint=absoluteHome;
}

void CamScene::timerEvent(QTimerEvent *event) {

    if (event->timerId() == timer.timerId()) {

        //qDebug()<<"timer trigger";
        emit progressionDone(currentLoop);
        ++currentLoop;

    } else {
        /// deal with other event...this way it will make it's way and propagate
        qDebug()<<"other timer"<<event->timerId();

        event->ignore();
    }

}

void CamScene::moveTool(QPointF endP){
    //have to take into account the added margins
    endP+=QPointF(rectMarg/2,rectMarg/2);
    //qDebug()<<"Moving to point "<<endP;
    /// @bug there's a crash in setPos when animating clearing then reanomating
    toolLine.setLine( QLineF(homePoint,endP));
    toolLine.setPen(toolPen);
    /// @note as tooline is already inserted Qt gives us an info message
    /// @todo use set pos or something like that
    addItem(&toolLine);
    toolPix->setPos(endP);
    ///To ensure that we start at the core'ct position when going to the next part
    homePoint=endP;
}











