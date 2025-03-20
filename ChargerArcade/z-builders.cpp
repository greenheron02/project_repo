#include "Charcade.h"
void Screen::MakeTitleScreen()
{
    QVBoxLayout *startlayout = new QVBoxLayout(); //MIGHT NOT NEED TO BE A POINTER

    QLabel *start = new QLabel(), *title = new QLabel();
    title->setText("Charger Arcade");
    QFont font = title->font();
    font.setPointSize(screenw/30);
    title->setFont(font);

    start->setText("<font color='#888888'>Press Start</font>");
    font = start->font();
    font.setPointSize(screenw/40);
    start->setFont(font);

    QPixmap *UAH = new QPixmap(":/UAH.png");
    QLabel *UAHl = new QLabel();

    //int newHeight = UAHwidth * UAH->height() / UAH->width();


    int UAHw = static_cast<int>(screenw * 0.6);
    int UAHh = UAHw*0.5306;
    //qDebug() << UAHw << " sdfgfde" << UAHh;
    *UAH = UAH->scaled(UAHw, UAHh, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    //UAHl->setMaximumWidth(static_cast<int>(screenw * 0.2));
    UAHl->setPixmap(*UAH);

    UAHl->setAlignment(Qt::AlignHCenter);
    title->setAlignment(Qt::AlignHCenter);
    start->setAlignment(Qt::AlignHCenter);

    startscreen.setLayout(startlayout);

    startlayout->addWidget(UAHl);
    startlayout->addWidget(title);
    startlayout->addWidget(start);
    startlayout->setAlignment(Qt::AlignTop);
}

void Screen::MakeSelectScreen()
{
    spot=-1;
    gridwidth = screenw/(gridw*1.3);
    gridheight = gridwidth*0.5;
    ScrollArea.setFixedSize(screenw, (ceil(double(total)/gridw))*gridheight*1.407+buffer);

    QGuiApplication::setFont(QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/RedHatText.ttf")).at(0), (int)(gridwidth/30)));

    entries.clear();
    images.clear(); //MIGHT NEED TO DESTRUCT THESE TO AVOID MEMORY LEAKS
    programs.clear();
    imageholders.clear();
    QFile file(":/ChargerArcadeData.txt");
    QTextStream stream(&file);
    file.open(QIODevice::ReadOnly|QIODevice::Text);


    bool coninueAdding=true;
    for(int i=1; coninueAdding; i++)
    {
        for(int j=1; j < gridw+1 && coninueAdding; j++)
        {
            QString line=stream.readLine();
            QStringList info=line.split("||||");
            //qDebug() << "Drawing game";
            if(line.isEmpty() || line=="\t")
            {
                total++;
                coninueAdding=false;
                /* gridh=i;
                if(j==1)
                    gridh=i-1;*/
                //qDebug() << gridw << "x" << gridh  << "    " << total;
            }
            else
            {
                //qDebug() << info[1] << "~~" << info[2] << "~~" << info[3] << "~~" << info[4];

                spot++;
                QDialog *entry = new QDialog;
                QVBoxLayout *box = new QVBoxLayout;
                QLabel *title = new QLabel, *credit = new QLabel;

                QPixmap *image = new QPixmap(":/" + info[2]);
                QPixmap *Logo = new QPixmap(gridwidth, gridheight);
                Logo->fill(QColor("#002D72")); ///////////MAYBE MAKE BACKGROUND COLOR INSTEAD/////////////

                QPainter imagedrawer(Logo);
                *image = image->scaled(gridwidth - (2+borderThickness), gridheight - (2+borderThickness), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                imagedrawer.drawPixmap(6, 6, *image);
                imagedrawer.end();
                images.append(Logo);




                QLabel *logo = new QLabel;
                logo->setObjectName("logo");
                imageholders.append(logo);


                title->setText(info[1]);
                title->setAlignment(Qt::AlignHCenter);
                title->setMinimumWidth(gridwidth);
                title->setMaximumWidth(gridwidth);
                adjustFontSize(title, gridwidth);
                title->setObjectName("title");


                credit->setText("<i>"+info[3]+"</i>");
                credit->setAlignment(Qt::AlignHCenter);
                credit->setMinimumWidth(gridwidth);
                credit->setMaximumWidth(gridwidth);
                adjustFontSize(credit, gridwidth);
                credit->setObjectName("credit");


                box->addWidget(title);
                box->addWidget(logo);
                box->addWidget(credit);


                entry->setLayout(box);
                entries.append(entry);
                grid->addWidget(entry, i, j);


                programs.append(info[4]);
                //programs.append("CharmStudies.sh");

                entries[spot]->findChild<QLabel *>("logo")->setPixmap(*DrawFrame(images[spot], UAHDBlue, borderThickness));
                entries[spot]->findChild<QLabel *>("logo")->update();

            }
        }
    }
    //qDebug() << "total:" << spot;
    total=spot+1;
    spot=prevSpot;
    entries[spot]->findChild<QLabel *>("logo")->setPixmap(*DrawFrame(images[spot], UAHYellow, borderThickness));
    entries[spot]->findChild<QLabel *>("logo")->update();
}

void Screen::MakeGameScreen()
{
    QVBoxLayout *gamelayout = new QVBoxLayout();

    QPalette pal = QPalette();

    // set black background
    // Qt::black / "#000000" / "black"

    pal.setColor(QPalette::Window, Qt::white);
    gamescreen.setAutoFillBackground(true);
    gamescreen.setPalette(pal);

    QHBoxLayout *barlayout = new QHBoxLayout;

    QWidget *bar = new QWidget;

    pal.setColor(QPalette::Window, Qt::gray);
    bar->setAutoFillBackground(true);
    bar->setPalette(pal);
    //bar->show();

    bar->setMinimumWidth(screenw/2);
    bar->setMaximumHeight(30);
    barlayout->setAlignment(Qt::AlignBottom);



    QLabel *ginfo = new QLabel("START: pause game SELECT: View creator information (note: these are just a placeholder)");
    QFont font=ginfo->font();
    font.setPointSize(10);                        //change to adapt
    ginfo->setFont(font);
    barlayout->addWidget(ginfo);
    bar->setLayout(barlayout);

    //QPixmap *box = new QPixmap(300, 300);

    //box->fill(QColor("red"));

    //QLabel *boxx = new QLabel;
    //boxx->setPixmap(*box);
    //*box = box->scaled(300, 300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    //gamelayout->addWidget(boxx);
    gamelayout->addStretch();
    gamelayout->addWidget(bar);


    gamescreen.setLayout(gamelayout);
}

void Screen::MakeEditScreen()
{

}

QPixmap Screen::colorizePixmap(const QString &filePath, const QColor &color)
{
    // Load the PNG into a QPixmap
    QPixmap pixmap(filePath);
    pixmap = pixmap.scaled(screenh/18, screenh/18, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // Convert QPixmap to QImage for easier manipulation
    QImage image = pixmap.toImage();


    // Create a QPainter to modify the image
    QPainter painter(&image);

    // Set the painter to blend the color with transparency preserved
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(image.rect(), color);

    painter.end();

    // Convert the modified QImage back to QPixmap
    return QPixmap::fromImage(image);
}

void Screen::MakeStatusBar()
{
    status->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);
    //overlay->setAttribute(Qt::WA_TranslucentBackground);
    qDebug() << "aaaa";

    status->setStyleSheet("background-color: #EEEEEE;");

    QLabel *id = new QLabel;
    QFont font = id->font();
    font.setPointSize(screenw/70);
    id->setFont(font);
    id->setText(QString::number(ID));
    bar.addWidget(id);

    QPalette pal;
    pal.setColor(QPalette::Window, QColor("#AA0000"));
    QVector<QString> colors = {"red","green","yellow","purple"};
    for(int i=(ID==1)?1:0; i<TerminalCount; i=(i+1==(ID-1))?i+2:i+1)
    {
        StatusIcons[i]->setPalette(pal);
        StatusIcons[i]->autoFillBackground();

        StatusIcons[i]->setPixmap(colorizePixmap(":/ComWarning.png", QColor(colors[i])));
        StatusIcons[i]->resize(40,40);
        bar.addWidget(StatusIcons[i]);
    }




    bar.setSpacing(10);
    status->setLayout(&bar);
    //connect(this, &QWidget::destroyed, status, &QWidget::close);

    status->move(xspot,yspot);
    status->resize(-1,screenh/18);
    status->show();

}

void Screen::MakeNoti()
{
    noti->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //noti->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);
    //overlay->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout lay;
    noti->setStyleSheet("background-color: #EEEEEE;");


    lay.addSpacing(100);

    QStackedLayout timerholder;
    QWidget* timerback = new QWidget;

    timerback->setStyleSheet("background-color: #333344;"); //black(ish)
    timerback->setMaximumHeight(20);
    timerback->setMinimumHeight(20);
    timerback->setMaximumWidth(notiwid-50);

    notitimebar->setStyleSheet("background-color: #DD9922;"); //orange
    notitimebar->setMaximumHeight(20);
    //notitimebar->setMinimumHeight(20);
    notitimebar->setMaximumWidth(notiwid-50);

    QStackedLayout *timestack = new QStackedLayout;
    notitimebar2->setStyleSheet("background-color: #FF1111;");//red
    //notitimebar2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    notitimebar2->setMaximumHeight(20);
    //notitimebar2->setMinimumHeight(20);
    notitimebar2->setMaximumWidth(notiwid-50);
   // timer->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    //timerholder.addWidget(notitimebar2);


    timestack->addWidget(notitimebar2);
    notitimebar->setLayout(timestack);

    timerholder.addWidget(notitimebar);

    timerback->setLayout(&timerholder);

    lay.addWidget(timerback);



    QLabel *title = new QLabel;
    QFont font = title->font();
    font.setPointSize(screenw/70);
    title->setFont(font);
    title->setText("AAAAA");
    lay.addWidget(title);

    QLabel *text = new QLabel;
    font = text->font();
    font.setPointSize(screenw/70);
    text->setFont(font);
    text->setText("BBBBB");
    lay.addWidget(text);





    noti->setLayout(&lay);
    //connect(this, &QWidget::destroyed, status, &QWidget::close);


    noti->resize(notiwid,screenh);
    //noti->show();
}

void Screen::summonNoti(QString title, QString text)
{
    notititle.setText(title);
    notitext.setText(text);
    noti->show();
    QTimer* timer = new QTimer(this);
    int i = screenw;
    QObject::connect(timer, &QTimer::timeout, [=]() mutable
     {
        i-=5;
        noti->move(i,0);

        if (i <= screenw-notiwid)
        {
            timer->stop();
            timer->deleteLater();
        }
    });
    timer->start(5);

    QTimer *timerr = new QTimer(this);
    i = notiwid-50;
    notitimebar->setMaximumWidth(i);
    notitimebar->resize(i,20);

    QString startcolor="#FFFFFF", endcolor="#000000";
    QObject::connect(timerr, &QTimer::timeout, [=]() mutable
    {
        i-=1;
        notitimebar->setMaximumWidth(i);
        notitimebar2->setMaximumWidth(i);
        //noti->setStyleSheet("background-color: " + startcolor.interpolated(endcolor,(notiwid-50)).name());
        //qDebug() << startcolor.interpolated(endcolor,(1-(float)i/(notiwid-50))).name();
        //qDebug() << (1-(float)i/(notiwid-50));
        //notitimebar2->setWindowOpacity((1-(float)i/(notiwid-50)));

        if (i == 0)
        {
            timerr->stop();
            delete(timerr);
            dismissNoti();
        }
    });
    timerr->start(8);
}

void Screen::dismissNoti()
{
    QTimer* timer = new QTimer(this);
    int i = screenw-notiwid;
    QObject::connect(timer, &QTimer::timeout, [=]() mutable
    {
        i+=5;
        noti->move(i,0);

        if (i >= screenw)
        {
            timer->stop();
            timer->deleteLater();
        }
    });

    timer->start(5);
    //noti->hide();
}
