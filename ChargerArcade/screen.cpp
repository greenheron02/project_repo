#include "screen.h"
#include "ui_screen.h"
#include <QApplication>
#include <QElapsedTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Screen w;
    w.show();

    return a.exec();
}

//QVector<QString> Screen::imageFiles


Screen::Screen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Screen)
{
    this->installEventFilter(this);
    this->setFocus();

    CheckForUSB();

    localpath = QDir::currentPath();
    localpath = localpath.split("build/")[0];
    qDebug() << localpath;
    makeAHKFile(0);

    /*ahksock.connectToHost("192.168.1.147", 5003);
     if (!ahksock.waitForConnected(1000))
        qDebug() << "AHK CONNECTION FAILURE";*/
    //runGame();
    //insert();

    /*   *\  }
    void Screen::temp()
    {/**/
    screenw = monitor.geometry().width();
    screenh = monitor.geometry().height();



    if(screenw > screenh*3)
        screenw /= 2;





    /*gridheight=monitor.geometry().height()/gridh-100;
    gridwidth=monitor.geometry().width()/gridw-200;*/

    //ui->setupUi(this);



    QWidget *screen = new QWidget(this);  // Set layout for Screen
    screen->setMinimumWidth(screenw);
    screen->setMinimumHeight(screenh);

    //grid->setSpacing(0);


    MakeTitleScreen();

    MakeGameScreen();
    MakeSelectScreen();
    MakeSelectScreen();



    header = new QLabel;
    header->setFixedHeight(40);
    QFont fontt = header->font();
    header->setText("Select a game");
    qDebug() << "edit " << editMode;
    if(editMode)
    {
        qDebug() << "ROOT USER ACCEPTED";
        header->setText("Edit mode active: press Blue to download selected data, Red to load to it");
        fontt.setPixelSize(20);
    }

    fontt.setPixelSize(30);
    header->setAlignment(Qt::AlignCenter);
    header->setFont(fontt);
    startLayout.addWidget(header);
    startLayout.addWidget(&Scroller);
    selectscreen.setLayout(&startLayout);

    grid->setContentsMargins(0, 0, 0, buffer);

    grid->setAlignment(Qt::AlignHCenter);
    Scroller.setWidgetResizable(false);
    //GridHolderLayout.setWidgetResizable(false);
    //selectscreen.setMaximumHeight(9999999);
    //GridHolder.setGeometry(100, 100, 1000, 2000); //!!!!!!!!!!!!!!

    //grid->SetMinimumSize(gridw * gridwidth, 500 * gridheight);
    //runGame();

    stack.setStackingMode(QStackedLayout::StackOne);
    stack.addWidget(&startscreen);
    stack.addWidget(&selectscreen);
    stack.addWidget(&gamescreen);
    stack.addWidget(&managescreen);


    screen->setLayout(&stack);
    //screen->show();

    showMaximized();


    gamescreen.setFixedSize(screenw, screenh-90);



    qDebug() << screenh;

    Scroller.setFixedHeight(screenh-header->height());

    selectscreen.setFixedSize(screenw, screenh-10);

    startscreen.setFixedSize(screenw, screenh);

    ScrollArea.setLayout(grid);
    Scroller.setWidget(&ScrollArea);

    QPalette back = ScrollArea.palette();
    back.setColor(QPalette::Window, UAHLGrey);

    ScrollArea.setAutoFillBackground(true);
    selectscreen.setPalette(back);           //maybe could be done to screen itself instead of each individual one
    gamescreen.setAutoFillBackground(true);
    gamescreen.setPalette(back);
    startscreen.setAutoFillBackground(true);
    startscreen.setPalette(back);
    stack.setCurrentIndex(0); //////GO TO SPECIFIC PAGE//////////
}


void Screen::DrawFrame(QColor col, int id)
{
    qDebug() << "framing";
    QPixmap *image = images[id];
    QPainter border(image);
    QPen pen;
    pen.setWidth(4);
    border.setPen(pen);
    border.drawRect(1,1,gridwidth-2,gridheight-2);
    border.end();
    *image = image->scaled(gridwidth, gridheight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    entries[id]->findChild<QLabel*>("logo")->setPixmap(*image);
    entries[id]->findChild<QLabel*>("logo")->update();
}

bool Screen::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        int key = (static_cast<QKeyEvent *>(event))->key();
        if(key == Qt::Key_W || key == Qt::Key_A || key == Qt::Key_S || key == Qt::Key_D)
        {
            DrawFrame(UAHDBlue, spot);

            if(key == Qt::Key_W)
                spot-=gridw;

            if(key == Qt::Key_A)
                spot--;

            if(key == Qt::Key_S)
                spot+=gridw;

            if(key == Qt::Key_D)
                spot++;

            UpdateSpot();
            DrawFrame(UAHYellow, spot);
            //qDebug() << Qt::endl << x << "   " << y << "  " << spot;
        }

       /* if(key == Qt::Key_N)
        {
            QTcpSocket *socket = new QTcpSocket(this);

            // Connect to server at localhost on port 12345
            socket->connectToHost("127.0.0.1", 12345);

            // Check if connected successfully
            if (socket->waitForConnected(5000))
            {
                qDebug() << "Connected to server!";
                socket->write("Hello from Qt client!");
            } else
            {
                qDebug() << "Failed to connect!";
            }
        }*/

        if(key == Qt::Key_W && editMode)
            extract(spot+1);

        if(key == Qt::Key_D && editMode)
            insert();





        /*if(key == Qt::Key_R)
        {
            qDebug() << "sending";
            sendToAHK("a");
        }*/



        /*if(key == Qt::Key_E)
        {
           endScript();
        }*/







        /*if(key == Qt::Key_J)
    {
        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
        QApplication::postEvent(process, event);
    }

    if(key == Qt::Key_I)
    {
        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
        QApplication::postEvent(process, event);
    }
    if(key == Qt::Key_K)
    {
        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
        QApplication::postEvent(process, event);
    }
    if(key == Qt::Key_L)
    {
        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
        QApplication::postEvent(process, event);
    }
    if(key == Qt::Key_U)
    {
    QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove, QPoint(100, 200), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    QApplication::postEvent(process, moveEvent);
    }

    if(key == Qt::Key_BracketRight)
    {
       diff+=0.1;
    }
    if(key == Qt::Key_BracketLeft)
    {
       diff-=0.1;
    }*/
        if(key == Qt::Key_Return)
        {
            QElapsedTimer timer;
            timer.start();
            qDebug() << "ENTERED";
            if(stack.currentIndex() == 1)
                runGame();
            if(stack.currentIndex() == 0)
            {
                stack.setCurrentIndex(1);
                CheckForUSB();
            }
             qDebug() << "The operation took" << timer.elapsed() << "milliseconds";
        }

        if(key == Qt::Key_E)
        {
            // QElapsedTimer timer;
            // timer.start();
            if(stack.currentIndex() == 1)
            {
                qDebug() << "moved to screen 0";
                stack.setCurrentIndex(0);
            }

            if(stack.currentIndex() == 2)
            {
                qDebug() << "moved to screen 1";
                stack.setCurrentIndex(1);
                makeAHKFile(0);
                game->kill();
            }

             //qDebug() << "The operation took" << timer.elapsed() << "milliseconds";
        }
        /*static double val=1.4;
        if(key == Qt::Key_K)
            val+=0.1;

        if(key == Qt::Key_L)
            val-=0.1;


        if(key == Qt::Key_O)
            val=(total/gridw)*0.45+0.4;

         ScrollArea.setFixedHeight((total/gridw)*gridheight*val+buffer);
        qDebug() << "val: " << val;*/







        if(key==Qt::Key_Q)
        {
            MakeSelectScreen();
        }
        if(key==Qt::Key_X)
        {
            clear(grid);
        }
        if(key == Qt::Key_F)
        {
            clear(grid);
            if(gridw < 4)
                gridw++;
            MakeSelectScreen();
            UpdateSpot();
        }
        if(key == Qt::Key_G)
        {
            clear(grid);
            if(gridw > 2)
                gridw--;
            MakeSelectScreen();
            UpdateSpot();
        }
    }

    return QWidget::eventFilter(obj, event);
}


void Screen::UpdateSpot()
{
    static int oldscroll;

    if(spot < 0)
        spot=0;
    if(spot > total-1)
        spot=total-1;
    if(oldscroll != (int)(spot/gridw))
    {
        //int oldscrollval = Scroller.verticalScrollBar()->value(), newscrollval = (((ceil(total/gridw))/(spot/gridw))*ScrollArea.height());
        int oldscrollval = Scroller.verticalScrollBar()->value(), newscrollval = ((spot/gridw)*gridheight*diff);







        //qDebug() << oldscroll << "-" << (int)(spot/gridw) << "-" << oldscrollval << "-" << newscrollval << "-" << oldscrollval+newscrollval-oldscrollval;

        int steps = 20;
        int ms = 6;
        int i = 0;

        QTimer* timer = new QTimer(this);

        // Capture variables in lambda to update scroll position incrementally
        QObject::connect(timer, &QTimer::timeout, [=]() mutable
        {
         i++;
         Scroller.verticalScrollBar()->setValue(oldscrollval + (newscrollval - oldscrollval)/steps*i);

         if (i >= steps)
         {
             timer->stop();
             timer->deleteLater();
         }
        });

        timer->start(ms);
    }
    oldscroll = oldscroll=(int)(spot/gridw);
}

/*void Screen::UpdateSpot()
{
    qDebug() << "start";
    static int oldscroll;


    if(spot < 0)
        spot=0;
    if(spot > total-1)
        spot=total-1;


    if(oldscroll != (int)(spot/gridw))
    {
       Screen::stopscroll = true;
        int oldscrollval = selectscreen.verticalScrollBar()->value(), newscrollval = ((spot/gridw)*gridheight*diff);
        //qDebug() << oldscroll << "-" << (int)(spot/gridw) << "-" << oldscrollval << "-" << newscrollval << "-" << oldscrollval+newscrollval-oldscrollval;
        Screen::stopscroll=false;
        int ms = 6;
        int i = 0;
        double k = 0.03;
        QTimer* timer = new QTimer(this);

        // Capture variables in lambda to update scroll position incrementally

        QObject::connect(timer, &QTimer::timeout, [=]() mutable
        {
            if(Screen::stopscroll == false)
            {
                i++;
                selectscreen.verticalScrollBar()->setValue(oldscrollval + ((1 - std::exp(-k * i)) * (newscrollval - oldscrollval)));
                //qDebug() << oldscrollval + ((1 - std::exp(-k * i)) * (newscrollval - oldscrollval));

                if (abs(newscrollval-(selectscreen.verticalScrollBar()->value())) < 30 || i > (1000/ms))
                {
                    timer->stop();
                    timer->deleteLater();
                    qDebug() << "endd";
                }
            }
        });
        timer->start(ms);
         qDebug() << "end";
    }
        oldscroll = oldscroll=(int)(spot/gridw);

}*/


void Screen::MakeSelectScreen()
{


    spot=-1;

    gridwidth = screenw/(gridw*1.3);
    gridheight = gridwidth*0.5;
    ScrollArea.setFixedSize(screenw, (ceil(double(total)/gridw))*gridheight*1.407+buffer);

    QGuiApplication::setFont(QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/RedHatText.ttf")).at(0), (int)(gridwidth/30)));

    images.clear();
    programs.clear();
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
                Logo->fill(QColor("#002D72"));

                QPainter border(Logo);
                *image = image->scaled(gridwidth - (2+borderThickness), gridheight - (2+borderThickness), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                border.drawPixmap(6, 6, *image);
                border.end();
                images.append(Logo);







                QLabel *logo = new QLabel;
                logo->setPixmap(*Logo);
                logo->setObjectName("logo");
                //logo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                title->setText(info[1]);
                title->setAlignment(Qt::AlignHCenter);
                title->setMinimumWidth(gridwidth);
                title->setMaximumWidth(gridwidth);
                adjustFontSize(title);
                title->setObjectName("title");


                credit->setText("<i>"+info[3]+"</i>");
                credit->setAlignment(Qt::AlignHCenter);
                credit->setMinimumWidth(gridwidth);
                credit->setMaximumWidth(gridwidth);
                adjustFontSize(credit);
                credit->setObjectName("credit");


                box->addWidget(title);
                box->addWidget(logo);
                box->addWidget(credit);


                entry->setLayout(box);
                entries.append(entry);
                grid->addWidget(entry, i, j);

                programs.append(info[4]);
                //programs.append("CharmStudies.sh");
            }
        }
    }
    //qDebug() << "total:" << spot;
    total=spot+1;
    spot=prevSpot;
    DrawFrame(UAHYellow, spot);



}

void Screen::adjustFontSize(QLabel* label)
{
    int minFontSize = 2;
    int fontSize = label->font().pointSize();
    QFont font = label->font();

    //if(spot == 0)
    //qDebug() <<  ((double)(QFontMetrics(font).horizontalAdvance(label->text())))/gridwidth;
    while (fontSize >= minFontSize && QFontMetrics(font).horizontalAdvance(label->text()) > gridwidth)
    {
        fontSize--; // Decrease font size
        font.setPointSize(fontSize);
        label->setFont(font);
    }
}


Screen::~Screen()
{
    ahksock.disconnectFromHost();
    game->kill();
    endScript();
    delete ui;
}


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
    qDebug() << UAHw << " sdfgfde" << UAHh;
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

    QPixmap *box = new QPixmap;

    box->fill(QColor("red"));

    QLabel *boxx = new QLabel;
    boxx->setPixmap(*box);
    *box = box->scaled(300, 300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    gamelayout->addWidget(boxx);
    gamelayout->addStretch();
    gamelayout->addWidget(bar);


    gamescreen.setLayout(gamelayout);
}

void Screen::MakeEditScreen()
{

}




void Screen::clear(QLayout* layout)
{
    prevSpot = spot;
    // Check if layout is valid
    if (!layout) return;

    // Iterate through all items in the layout
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        // If the item is a widget, delete it
        if (item->widget()) {
            delete item->widget();
        }
        // If the item is a layout, recursively delete it
        else if (item->layout()) {
            clear(item->layout());
            delete item->layout();
        }
        // Delete the item itself
        delete item;
    }
}



/*void Screen::flash(QString text)
{
    qDebug() << text;
}*/

/*TO DO LIST:
 * Add support for imageless games
 * Make the insert function actually replace the file
 * - have the grid update with the new data
 * add the ability to add game, not just replace one
 * add the ability to remove games
 * Sanitize all user-input data (from the usb or going into the usb for the other program)
 * ensure the user can't plug in a keyboard and mess things up
*/
