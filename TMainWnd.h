/**************************************************************************
**   Copyright @ 2025 TOOTzoe.com
**   Special keywords: thor 1/5/2025 2025
**   Environment variables:
**
**
**
**   E-mail : toot@tootzeo.com
**   Tel    : 13802205042
**   Website: http://www.tootzoe.com
**
**************************************************************************/



#ifdef Q_CC_MSVC
#pragma execution_character_set("UTF-8")
#endif


//------------------------------------------------------------------------



#ifndef TMAINWND_H
#define TMAINWND_H

#include <QWidget>


QT_FORWARD_DECLARE_CLASS(QLabel)

class TMainWnd : public QWidget
{
    Q_OBJECT
public:
    explicit TMainWnd(QWidget *parent = nullptr);

signals:




private slots:

    void openGifBtnClick();
    void openGifBtnClick2();
    void showGifBtnClick ();
    //
    void gifFrameLoopWithDelay ();


private:

    void initData();
    void setupUI();




    QList<QImage> mImgLs;
    QList<qint32> mDelayLs;
    qint32 mImgWight;
    qint32 mImgHeight;
    quint8 mShowImgIdx;
    //
    QLabel *imgLb;


};

#endif // TMAINWND_H
