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




#include <QtWidgets>
#include <QDebug>

#include "TMainWnd.h"

#include "giflib522/gif_lib.h"






TMainWnd::TMainWnd(QWidget *parent)
    : QWidget{parent}
{


    initData();

    setupUI();



    resize(1000, 800);

}



void TMainWnd::initData()
{

}

void TMainWnd::setupUI()
{

    QVBoxLayout *mainLO = new QVBoxLayout();

    QPushButton *openGifBtn = new QPushButton(tr("Open Gif"));
    connect(openGifBtn, &QPushButton::clicked, this, &TMainWnd::openGifBtnClick2);

    mainLO->addWidget(openGifBtn);
    //
    QPushButton *showGifBtn = new QPushButton(tr("Show Gif"));
    connect(showGifBtn, &QPushButton::clicked, this, &TMainWnd::showGifBtnClick);

    mainLO->addWidget(showGifBtn);



   imgLb  = new QLabel();
   imgLb->setFixedSize(800,600);
  // imgLb->setSizePolicy(QSizePolicy::Expanding , QSizePolicy::Expanding);


   mainLO->addWidget(imgLb);

    setLayout(mainLO);

}



void TMainWnd::openGifBtnClick()
{


    int err = 0;
    GifFileType *gft =  DGifOpenFileName("C:\\Users\\thor\\Pictures\\gifs\\T_Emoj01.gif" , &err);

    qDebug() << "Err: " << err;

    if(!gft) return;


     qDebug() << gft->SHeight << gft->SWidth;;



     if(GIF_OK != DGifSlurp(gft))
     {
          qDebug() << "Err2 : " << gft->Error;
         return;
     }

      qDebug() << gft->Image.Height;
      qDebug()  << "img count: " << gft->ImageCount;



      DGifCloseFile(gft, &err);

}

void TMainWnd::openGifBtnClick2()
{


    QString fn = QFileDialog::getOpenFileName(this, tr("Choose a animated gif image") ,  QDir::home().absolutePath() ,"*.gif");


    if(fn.isEmpty()) return;



    int error;
       GifFileType* gifFile = DGifOpenFileName(fn.toUtf8().constData(), &error);
       if (!gifFile) {
           qDebug()  << "DGifOpenFileName() failed - " << error   ;
           return  ;
       }
       if (DGifSlurp(gifFile) == GIF_ERROR) {
           qDebug()  << "DGifSlurp() failed - " << gifFile->Error ;
           DGifCloseFile(gifFile, &error);
           return  ;
       }



       mImgLs.clear();
       mDelayLs.clear();



        ColorMapObject* commonMap = gifFile->SColorMap;
       qDebug() << fn << ": " << gifFile->SWidth << "x" << gifFile->SHeight ;

       mImgWight = gifFile->SWidth ;
       mImgHeight = gifFile->SHeight;


       for (int i = 0; i <  gifFile->ImageCount  ; ++i) {
           const SavedImage& saved = gifFile->SavedImages[i];
           const GifImageDesc& desc = saved.ImageDesc;
           const ColorMapObject* colorMap = desc.ColorMap ? desc.ColorMap : commonMap;
            qDebug() << "================================================================";
           qDebug() << "[" << i << "] "
               << desc.Width << "x" << desc.Height << " ---- left: " << desc.Left << ", top: " << desc.Top
               << ", has local colorMap: " << (desc.ColorMap ? "Yes" : "No")
               << " block cnt: " << saved.ExtensionBlockCount
               << " block cnt2: " << saved.ExtensionBlocks->ByteCount
                  ;



          // if(desc.Left != 0 || desc.Top != 0) continue;  //tootzoe, we don't support image fly away gif now....



           QImage tmpImg = QImage(desc.Width , desc.Height, QImage::Format_RGB888);



          if( i > 0 &&  saved.ExtensionBlocks->ByteCount > 3  ){ //  first frame no delay from -1 frame, skip....
              qint16 delayMs = saved.ExtensionBlocks->Bytes[1] | (saved.ExtensionBlocks->Bytes[2] << 8);
               delayMs = 1000 * 0.01 * delayMs; // delay in 0.01sec units, we translate value to ms
               qDebug() << "curr frame delay: " <<  delayMs;
               mDelayLs << delayMs;
          }else{
              mDelayLs << 50; // default delay 50ms;
          }

           QStringList ss;
           for (int v = 0; v < desc.Height; ++v) {
               for (int u = 0; u < desc.Width; ++u) {
                   int c = saved.RasterBits[v * desc.Width + u];

                   if( c == gifFile->SBackGroundColor && mImgLs.size() > 0){

                       QImage img0 = mImgLs.at(mImgLs.size() - 1);

                        tmpImg.setPixel(u, v , img0.pixel(u,v) ); // use last frame color....

                        continue;
                   }

                  // qDebug(" %02X", c);
                   if (colorMap) {
                       GifColorType rgb = colorMap->Colors[c];
                    //   ss << " [" << QString::number( (int)rgb.Red) << "," <<  QString::number( (int)rgb.Green) << "," <<  QString::number( (int)rgb.Blue) << "]";
                    qint32 cr = ((int)rgb.Red << 16 ) | ((int)rgb.Green << 8 ) | ((int)rgb.Blue << 0 ) ;
                    tmpImg.setPixel(u, v , cr );

                   }
               }
              // qDebug() << ":" << ss.join("") ;
           }


            mImgLs << tmpImg;
       }

         DGifCloseFile(gifFile, &error);


         qDebug() << "img size:  " << mImgLs.size() <<  "  delay size: " << mDelayLs.size();

         QTimer::singleShot(0 , this, &TMainWnd::showGifBtnClick);

}

void TMainWnd::showGifBtnClick()
{

    if(mImgLs.isEmpty()) return;

    imgLb->setFixedSize(mImgWight, mImgHeight);



    mShowImgIdx = 0;
    gifFrameLoopWithDelay();
}


void TMainWnd::gifFrameLoopWithDelay()
{
    imgLb->setPixmap(QPixmap::fromImage( mImgLs.at(mShowImgIdx) ) );
    mShowImgIdx ++ ;
    mShowImgIdx = mShowImgIdx % mImgLs.size();

    QTimer::singleShot(mDelayLs.at(mShowImgIdx) , this, &TMainWnd::gifFrameLoopWithDelay);
}
