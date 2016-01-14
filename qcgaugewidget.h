#ifndef QCGAUGEWIDGET_H
#define QCGAUGEWIDGET_H

#include <QWidget>              //includes
#include <QPainter>
#include <QObject>
#include <QRectF>
#include <QtMath>
                                //declare classes, so they can be referenced inside of one another
class QcGaugeWidget;            //derived from widget class, is the actual gauge
class QcItem;                   //derived from object class, every class is derived from this
class QcBackgroundItem;
class QcDegreesItem;
class QcValuesItem;
class QcArcItem;
class QcColorBand;
class QcNeedleItem;
class QcLabelItem;
class QcGlassItem;

///////////////////
/// QcItem Class///
///////////////////

class  QcItem : public QObject                                      //derived from QObject Class
{                                                                   //all classes derived from QcItem
public:
    QcItem(QObject *parent=0) : QObject(parent)                     //constructor holds position and parent
    {   parentWidget = qobject_cast<QWidget*>(parent);
        mPosition = 50; }

    int type()
    { return 50;}

    void update()                                                   //refreshes parent, not necessarily a reprint
    { parentWidget->update();}

    void setPosition(float position)                                //sets position in relation to 100
    {if(position>100)
        mPosition = 100;
     else if(position<0)
        mPosition = 0;
     else
        mPosition = position;
     update();}

    float position()
    { return mPosition;}

    QRectF rect()
    {return mRect; }

    QRectF adjustRect(float percentage)                             //moves rectangle
    {   float r = getRadius(mRect);
        float offset =   r-(percentage*r)/100.0;
        QRectF tmpRect = mRect.adjusted(offset,offset,-offset,-offset);
        return tmpRect;}

    float getRadius(const QRectF &tmpRect)                          //obtains 1/2 width or height
    {   float r = 0;
        if(tmpRect.width()<tmpRect.height())
            r = tmpRect.width()/2.0;
        else
            r = tmpRect.height()/2.0;
        return r;}

    QRectF resetRect()                                              //resize and center
    {   mRect = parentWidget->rect();
        float r = getRadius(mRect);
        mRect.setWidth(2.0*r);
        mRect.setHeight(2.0*r);
        mRect.moveCenter(parentWidget->rect().center());
        return mRect;}

    QPointF getPoint(float deg,const QRectF &tmpRect)               //gets point from center to arc
    {   float r = getRadius(tmpRect);
        float xx=cos(qDegreesToRadians(deg))*r;
        float yy=sin(qDegreesToRadians(deg))*r;
        QPointF pt;
        xx=tmpRect.center().x()-xx;
        yy=tmpRect.center().y()-yy;
        pt.setX(xx);
        pt.setY(yy);
        return pt;}

    float getAngle(const QPointF&pt, const QRectF &tmpRect)         //gets angle from rect to point
    {   float xx=tmpRect.center().x()-pt.x();
        float yy=tmpRect.center().y()-pt.y();
        return qRadiansToDegrees( atan2(yy,xx));}

    virtual void draw(QPainter *) = 0;
    enum Error{InvalidValueRange,InvalidDegreeRange,InvalidStep};
    QRectF mRect;
    QWidget *parentWidget;
    float mPosition;
};

////////////////////////
/// QcScaleItem Class///
////////////////////////

class  QcScaleItem : public QcItem                                  //derived from Item class
{                                                                   //ArcItem, ColorBand, Degrees
public:                                                             //Needle, and values are subclasses
    QcScaleItem(QObject *parent=0) :
        QcItem(parent)                                              //constructor has min/max degree
    {   mMinDegree = -45;                                           //and min/max value
        mMaxDegree = 225;
        mMinValue = 0;
        mMaxValue = 100;}

    void setValueRange(float minValue, float maxValue)              //sets top and bottom
    {   if(!(minValue<maxValue))
            throw( InvalidValueRange);
        mMinValue = minValue;
        mMaxValue = maxValue;}

    void setDgereeRange(float minDegree, float maxDegree)           //sets degree range
    {   if(!(minDegree<maxDegree))
            throw( InvalidValueRange);
        mMinDegree = minDegree;
        mMaxDegree = maxDegree;}

    float getDegFromValue(float v)                                   //gets degree of specific value
    {   float a = (mMaxDegree-mMinDegree)/(mMaxValue-mMinValue);
        float b = -a*mMinValue+mMinDegree;
        return a*v+b;}

    void setMinValue(float minValue)
    {   if(minValue>mMaxValue)
            throw (InvalidValueRange);
        mMinValue = minValue;
        update();}

    void setMaxValue(float maxValue)
    {   if(maxValue<mMinValue )
            throw (InvalidValueRange);
        mMaxValue = maxValue;
        update();}

    void setMinDegree(float minDegree)
    {   if(minDegree>mMaxDegree)
            throw (InvalidDegreeRange);
        mMinDegree = minDegree;
        update();}

    void setMaxDegree(float maxDegree)
    {   if(maxDegree<mMinDegree)
            throw (InvalidDegreeRange);
        mMaxDegree = maxDegree;
        update();}

    float mMinValue;
    float mMaxValue;
    float mMinDegree;
    float mMaxDegree;
};

/////////////////////////////
/// QcBackgroundItem Class///
/////////////////////////////

class  QcBackgroundItem : public QcItem                             //Background Item
{
public:
   QcBackgroundItem(QObject *parent = 0) : QcItem(parent)           //constructor sets position
    {   setPosition(88);                                            //pen, and adds color
        mPen = Qt::NoPen;
        addColor(0,Qt::yellow);
        addColor(0.125,Qt::darkYellow);
        addColor(0.25,Qt::red);
        addColor(0.375,Qt::darkMagenta);
        addColor(0.5,Qt::green);
        addColor(float(0.75),Qt::darkGreen);
        addColor(float(0.75),Qt::cyan);
        addColor(float(0.875),Qt::darkBlue);
        addColor(float(1),Qt::black);
    }
    void draw(QPainter* painter)                                    //takes rectangle
    {   QRectF tmpRect = resetRect();                               //makes lineargrad
        painter->setBrush(Qt::NoBrush);                             //from top left to bot right
        QLinearGradient linearGrad(tmpRect.topLeft(), tmpRect.bottomRight());
        for(int i = 0;i<mColors.size();i++){                        //adds colors to gradient
            linearGrad.setColorAt(mColors[i].first,mColors[i].second);
        }
        painter->setPen(mPen);
        painter->setBrush(linearGrad);
        painter->drawEllipse(adjustRect(position()));}              //draws ellipse

    void addColor(float position, const QColor &color)              //adds color & location
    {   if(position<0||position>1)                                  //in gradient
            return;
          QPair<float,QColor> pair;
          pair.first = position;
          pair.second = color;
          mColors.append(pair);
          update();}

    void clearrColors()                                             //clears array of colors
    {   mColors.clear();}

    void overHeat()                                                 //Testing function
    {   mColors.clear(); addColor(0.5,Qt::red); update();}

    QPen mPen;
    QList<QPair<float,QColor> > mColors;
    QLinearGradient mLinearGrad;
};

////////////////////////
/// QcGlassItem Class///
////////////////////////

class QcGlassItem : public QcItem                                   //Glass item
{
public:
    QcGlassItem(QObject *parent) : QcItem(parent)                   //constructor is just position
    {   setPosition(88);}

    void draw(QPainter *painter)
    {   resetRect();
        QRectF tmpRect1 = adjustRect(position());
        QRectF tmpRect2 = tmpRect1;
        float r = getRadius(tmpRect1);
        tmpRect2.setHeight(r/2.0);
        painter->setPen(Qt::NoPen);
        QColor clr1 = Qt::white ;                               //gets 2 colors
        QColor clr2 = Qt::black;
        clr1.setAlphaF(0.2);
        clr2.setAlphaF(0.4);                                    //creates gradient with 2
        QLinearGradient linearGrad1(tmpRect1.topLeft(), tmpRect1.bottomRight());
        linearGrad1.setColorAt(0.1, clr1);
        linearGrad1.setColorAt(0.5, clr2);
        painter->setBrush(linearGrad1);
        painter->drawPie(tmpRect1,0,16*180);                    //half circles drawn
        tmpRect2.moveCenter(rect().center());
        painter->drawPie(tmpRect2,0,-16*180);}
};

////////////////////////
/// QcLabelItem Class///
////////////////////////

class  QcLabelItem : public QcItem                                  //Label item
{
public:
    QcLabelItem(QObject *parent) : QcItem(parent)                   //constructor has position,
    {   setPosition(50);                                            //angle, text, & color
        mAngle = 270;
        mText = "%";
        mColor = Qt::white;}

    void draw(QPainter *painter)
    {   resetRect();
        QRectF tmpRect = adjustRect(position());
        float r = getRadius(rect());
        QFont font("Meiryo UI", r/10.0, QFont::Bold);               //sets font of label
        painter->setFont(font);
        painter->setPen(QPen(mColor));                              //sets pen as color
        QPointF txtCenter = getPoint(mAngle,tmpRect);
        QFontMetrics fMetrics = painter->fontMetrics();
        QSize sz = fMetrics.size( Qt::TextSingleLine, mText );
        QRectF txtRect(QPointF(0,0), sz );
        txtRect.moveCenter(txtCenter);
        painter->drawText( txtRect, Qt::TextSingleLine,mText );}    //draws rectangle

    void setAngle(float a)                                          //sets angle of text
    {   mAngle = a;
        update();}

    float angle()                                                   //gets angle
    {   return mAngle; }

    void setText(const QString &text, bool repaint = true)          //sets text & repaints if necessary
    {   mText = text;
        if(repaint)
            update();}

    QString text()                                                  //returns string
    {   return mText;}

    void setColor(const QColor &color)                              //sets color of string
    {   mColor = color;
        update();}

    QColor color()                                                  //obtians color
    {   return mColor;}

    float mAngle;
    QString mText;
    QColor mColor;
};

//////////////////////
/// QcArcItem Class///
//////////////////////

class  QcArcItem : public QcScaleItem                               //Arc item
{
public:
    QcArcItem(QObject *parent) : QcScaleItem(parent)                //constructor, position and color
    {   setPosition(80);
        mColor = Qt::white;}

    void draw(QPainter *painter)                                    //draws the arc
    {   resetRect();
        QRectF tmpRect= adjustRect(position());
        float r = getRadius(tmpRect);
        QPen pen;
        pen.setColor(mColor);
        pen.setWidthF(r/40);
        painter->setPen(pen);                                       //drawArc function
        painter->drawArc(tmpRect,-16*(mMinDegree+180),-16*(mMaxDegree-mMinDegree));}

    void setColor(const QColor &color)                              //change color
    {   mColor = color;}

    QColor mColor;
};

////////////////////////
/// QcColorBand Class///
////////////////////////

class  QcColorBand : public QcScaleItem                             //ColorBand item
{
public:
    QcColorBand(QObject *parent) :  QcScaleItem(parent)             //constructor, position and array of color pair
    {   QColor tmpColor;
        tmpColor.setAlphaF(0.1);
        QPair<QColor,float> pair;
        pair.first = Qt::black;
        pair.second = 10;
        mBandColors.append(pair);
        pair.first = Qt::red;
        pair.second = 50;
        mBandColors.append(pair);
        pair.first = Qt::black;
        pair.second = 100;
        mBandColors.append(pair);
        setPosition(50);}

    QPainterPath createSubBand(float from, float sweep)         //NOT TOTALY SURE WHAT THIS IS
    {   QRectF tmpRect = adjustRect(position());
        QPainterPath path;
        path.arcMoveTo(tmpRect,180+from);
        path.arcTo(tmpRect,180+from,-sweep);
        return path;}

    void draw(QPainter *painter)                                //draw color band
    {   resetRect();
        float r = getRadius(rect());
        QPen pen;
        pen.setCapStyle(Qt::FlatCap);
        pen.setWidthF(r/20.0);
        painter->setBrush(Qt::NoBrush);
        float offset = getDegFromValue(mBandStartValue);
        for(int i = 0;i<mBandColors.size();i++){
            QColor clr = mBandColors[i].first;
            float sweep;
            if(i==0)
                sweep = getDegFromValue(mBandColors[i].second)-getDegFromValue(mMinValue);
            else
                sweep = getDegFromValue(mBandColors[i].second)-getDegFromValue(mBandColors[i-1].second);
            QPainterPath path = createSubBand(-offset,sweep);
            offset += sweep;
            pen.setColor(clr);
            painter->setPen(pen);
            painter->drawPath(path);}
    }

    void setColors(const QList<QPair<QColor, float> > &colors)      //set colors
    {   mBandColors = colors;
        update();}

    QList<QPair<QColor,float> > mBandColors;
    float mBandStartValue;
};

//////////////////////////
/// QcDegressItem Class///
//////////////////////////

class  QcDegreesItem : public QcScaleItem                           //Degree item
{
public:
    QcDegreesItem(QObject *parent) :                                //constructor, step size, color, position
        QcScaleItem(parent)
    {   mStep = 10;
        mColor = Qt::white;
        mSubDegree = false;
        setPosition(90);}

    void draw(QPainter *painter)                                    //draws dashes
    {   resetRect();
        QRectF tmpRect = adjustRect(position());
        painter->setPen(mColor);
        float r = getRadius(tmpRect);
        for(float val = mMinValue;val<=mMaxValue;val+=mStep){
            float deg = getDegFromValue(val);
            QPointF pt = getPoint(deg,tmpRect);
            QPainterPath path;
            path.moveTo(pt);
            path.lineTo(tmpRect.center());
            pt = path.pointAtPercent(0.03);
            QPointF newPt = path.pointAtPercent(0.13);
            QPen pen;
            pen.setColor(mColor);
            if(!mSubDegree)
                pen.setWidthF(r/25.0);
            painter->setPen(pen);
            painter->drawLine(pt,newPt);}
    }

    void setStep(float step)                                        //sets step size
    {   mStep = step;
        update();}

    void setColor(const QColor& color)                              //sets color
    {   mColor = color;
        update();}

    void setSubDegree(bool b)                                       //sets sub degree
    {   mSubDegree = b;
        update();}

    float mStep;
    QColor mColor;
    bool mSubDegree;
};

/////////////////////////
/// QcNeedleItem Class///
/////////////////////////

class  QcNeedleItem : public QcScaleItem                            //Needle item
{
public:
    enum NeedleType{DiamonNeedle,TriangleNeedle,FeatherNeedle,AttitudeMeterNeedle,CompassNeedle}; //needle type
    QcNeedleItem(QObject *parent) : QcScaleItem(parent)             //constructor, value, color, label, type
    {   mCurrentValue = 0;
        mColor = Qt::black;
        mLabel = NULL;
        mNeedleType = FeatherNeedle;
        mWarnLabel = NULL;
        mData;}

    void draw(QPainter *painter)                                    //drawing needle
    {   resetRect();
        QRectF tmpRect = adjustRect(position());
        painter->save();
        painter->translate(tmpRect.center());
        float deg = getDegFromValue( mCurrentValue);
        painter->rotate(deg+90.0);
        painter->setBrush(QBrush(mColor));
        painter->setPen(Qt::NoPen);
        QLinearGradient grad;
        switch (mNeedleType) {                                      //case over needle type
        case QcNeedleItem::FeatherNeedle:
            createFeatherNeedle(getRadius(tmpRect));
            break;
        case QcNeedleItem::DiamonNeedle:
            createDiamonNeedle(getRadius(tmpRect));
            break;
        case QcNeedleItem::TriangleNeedle:
            createTriangleNeedle(getRadius(tmpRect));
            break;
        case QcNeedleItem::AttitudeMeterNeedle:
            createAttitudeNeedle(getRadius(tmpRect));
            break;
        case QcNeedleItem::CompassNeedle:
            createCompassNeedle(getRadius(tmpRect));
            grad.setStart(mNeedlePoly[0]);
            grad.setFinalStop(mNeedlePoly[1]);
            grad.setColorAt(0.9,Qt::black);
            grad.setColorAt(1,Qt::black);
            painter->setBrush(grad);
            break;
        default:
            break;}
        painter->drawConvexPolygon(mNeedlePoly);
        painter->restore();
    }

    void setCurrentValue(float value)                               //set value to input
    {   if(value<mMinValue)
            mCurrentValue = mMinValue;
        else if(value>mMaxValue)
            mCurrentValue = mMaxValue;
        else
            mCurrentValue = value;
        if(mLabel!=0)
            mLabel->setText(QString::number(mCurrentValue),false);
        update();}

    float currentValue()                                            //returns current value
    {   return mCurrentValue;}

    void setColor(const QColor &color)                              //sets needle color
    {   mColor = color;
        update();}

    QColor color()                                                  //returns current color
    {   return mColor;}

    void setLabel(QcLabelItem *label)                               //sets label to current value
    {   mLabel = label;
        update();}

    void setWarnLabel(QcLabelItem *label)                            //sets warning label
    {   mWarnLabel = label;
        update();}

    void setWarnFlag()                                               //shows warning
    {   if(mWarnLabel!=0)
            mWarnLabel->setColor(Qt::white);}

    void dropWarnFlag()                                              //hides warning
    {   mWarnLabel->setColor(Qt::transparent);}

    void addData()                                                  //gets data
    {   double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        double lastPointKey = 0;
        if (key-lastPointKey > 500){
            double temp = currentValue();
            QPair<double, double> pairtemp;
            pairtemp.first = key;
            pairtemp.second = temp;
            mData.append(pairtemp);

            if(mData.length()>4500){
                mData.removeFirst();}
            lastPointKey = key;
    }}

    QList<QPair<double, double> > returnData()
    {
        return mData;
    }

    QcLabelItem *label()                                            //returns label value
    {   return mLabel;}

    void setNeedle(QcNeedleItem::NeedleType needleType)             //chosing needle type
    {   mNeedleType = needleType;
        update();}

    void createDiamonNeedle(float r)
    {   QVector<QPointF> tmpPoints;
        tmpPoints.append(QPointF(0.0, 0.0));
        tmpPoints.append(QPointF(-r/20.0,r/20.0));
        tmpPoints.append(QPointF(0.0, r));
        tmpPoints.append(QPointF(r/20.0,r/20.0));
        mNeedlePoly = tmpPoints;}

    void createTriangleNeedle(float r)
    {   QVector<QPointF> tmpPoints;
        tmpPoints.append(QPointF(0.0, r));
        tmpPoints.append(QPointF(-r/40.0, 0.0));
        tmpPoints.append(QPointF(r/40.0,0.0));
        mNeedlePoly = tmpPoints;}

    void createFeatherNeedle(float r)
    {   QVector<QPointF> tmpPoints;
        tmpPoints.append(QPointF(0.0, r));
        tmpPoints.append(QPointF(-r/40.0, 0.0));
        tmpPoints.append(QPointF(-r/15.0, -r/5.0));
        tmpPoints.append(QPointF(r/15.0,-r/5));
        tmpPoints.append(QPointF(r/40.0,0.0));
        mNeedlePoly = tmpPoints;}

    void createAttitudeNeedle(float r)
    {   QVector<QPointF> tmpPoints;
        tmpPoints.append(QPointF(0.0, r));
        tmpPoints.append(QPointF(-r/20.0, 0.85*r));
        tmpPoints.append(QPointF(r/20.0,0.85*r));
        mNeedlePoly = tmpPoints;}

    void createCompassNeedle(float r)
    {   QVector<QPointF> tmpPoints;
        tmpPoints.append(QPointF(0.0, r));
        tmpPoints.append(QPointF(-r/15.0, 0.0));
        tmpPoints.append(QPointF(0.0, -r));
        tmpPoints.append(QPointF(r/15.0,0.0));
        mNeedlePoly = tmpPoints;}

    QPolygonF mNeedlePoly;
    float mCurrentValue;
    QColor mColor;
    NeedleType mNeedleType;
    QcLabelItem *mLabel;
    QcLabelItem *mWarnLabel;
    QList<QPair<double,double> > mData;

};

/////////////////////////
/// QcValuesItem Class///
/////////////////////////

class  QcValuesItem : public QcScaleItem                            //Values item
{
public:
     QcValuesItem(QObject *parent) :  QcScaleItem(parent)           //constructor has position, color, & step size
    {   setPosition(70);
        mColor = Qt::white;
        mStep = 10;}

    void draw(QPainter*painter)                                     //draws numbers
    {   QRectF  tmpRect = resetRect();
        float r = getRadius(adjustRect(99));
        QFont font("Meiryo UI",0, QFont::Bold);
        font.setPointSizeF(0.08*r);
        painter->setFont(font);
        painter->setPen(mColor);
        for(float val = mMinValue;val<=mMaxValue;val+=mStep){
            float deg = getDegFromValue(val);
            QPointF pt = getPoint(deg,tmpRect);
            QPainterPath path;
            path.moveTo(pt);
            path.lineTo(    tmpRect.center());
            QString strVal = QString::number(val);
            QFontMetrics fMetrics = painter->fontMetrics();
            QSize sz = fMetrics.size( Qt::TextSingleLine, strVal );
            QRectF txtRect(QPointF(0,0), sz );
            QPointF textCenter = path.pointAtPercent(1.0-position()/100.0);
            txtRect.moveCenter(textCenter);
            painter->drawText( txtRect, Qt::TextSingleLine, strVal );}
        }

    void setStep(float step)                                        //set step size
    {   mStep = step;}

    void setColor(const QColor& color)                              //set color
    {   mColor = color;}

    float mStep;
    QColor mColor;
};

//////////////////////////
/// QcGaugeWidget Class///
//////////////////////////

class  QcGaugeWidget : public QWidget                               //derived from QWidget class
{
public:
    QList <QcItem*> mItems;
    QcGaugeWidget(QWidget *parent=0) : QWidget(parent)              //constructor, minimum size
    {   setMinimumSize(225,225);
        setMaximumSize(225,225);}

    void addItem(QcItem *item,float position)                       //add item to gauge, contains position
    {   item->setParent(this);
        item->setPosition(position);
        mItems.append(item);}

    int removeItem(QcItem *item)                                    //removes specific item
    {return mItems.removeAll(item);}

    QList<QcItem *> items()                                         //returns list of all items
    {   return mItems;}

    QcBackgroundItem * addBackground(float position)
    {   QcBackgroundItem * item = new QcBackgroundItem(this);
        item->setPosition(position);
        mItems.append(item);
        return item;}

    QcDegreesItem * addDegrees(float position)
    {   QcDegreesItem * item = new QcDegreesItem(this);
        item->setPosition(position);
        mItems.append(item);
        return item;}

    QcValuesItem * addValues(float position)
    {   QcValuesItem * item = new QcValuesItem(this);
        item->setPosition(position);
        mItems.append(item);
        return item;}

    QcArcItem * addArc(float position)
    {   QcArcItem * item = new QcArcItem(this);
        item->setPosition(position);
        mItems.append(item);
        return item;}

    QcColorBand * addColorBand(float position)
    {   QcColorBand * item = new QcColorBand(this);
        item->setPosition(position);
        mItems.append(item);
        return item;}

    QcNeedleItem * addNeedle(float position)
    {   QcNeedleItem * item = new QcNeedleItem(this);
        item->setPosition(position);
        mItems.append(item);
        return item;}

    QcLabelItem * addLabel(float position)
    {   QcLabelItem * item = new QcLabelItem(this);
        item->setPosition(position);
        mItems.append(item);
        return item;}

    QcGlassItem * addGlass(float position)
    {   QcGlassItem * item = new QcGlassItem(this);
        item->setPosition(position);
        mItems.append(item);
        return item;}

    void paintEvent(QPaintEvent * /*paintEvt*/)                     //draws every item in gauge
    {   QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        foreach (QcItem * item, mItems) {
            item->draw(&painter);}}


};
#endif
