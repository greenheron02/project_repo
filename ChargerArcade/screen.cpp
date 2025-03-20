#include <QApplication>
#include <QElapsedTimer>
#include "Charcade.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //connect(a, QApplication::e)
    Screen m(1);
    Screen l(2);
    Screen w(3);
    Screen j(4);
    m.build();

    if (m.splitScreenVal) {
        l.build();
        w.build();
        j.build();
        if (m.doCommsVal) {
            m.Connect();
            l.Connect();
            w.Connect();
            j.Connect();
        }
    } else if (m.doCommsVal)
        m.Connect();

    // Create QThreads for each Screen
    /*QThread *threadM = new QThread();
    QThread *threadL = new QThread();
    QThread *threadW = new QThread();
    QThread *threadJ = new QThread();

    // Move the Screen instances to their respective threads
    m.moveToThread(threadM);
    l.moveToThread(threadL);
    w.moveToThread(threadW);
    j.moveToThread(threadJ);

    // Start the threads
    threadM->start();
    threadL->start();
    threadW->start();
    threadJ->start();*/

    // Now that threads are started, call Connect() on each instance

    return a.exec();
}

//QVector<QString> Screen::imageFiles

Screen::Screen(int id)
{
    ID = id;
    IdReference.remove(ID - 1);
    splitScreenVal = splitScreen;
    doCommsVal = DoComms;
}

void Screen::build()
{
    Sign = QString::number(ID) + ":";
    if (ID != 1)
        SetUpServer();

    this->installEventFilter(this);
    /*
        this->setFocus();

    CheckForUSB();

    localpath = QDir::currentPath();
    localpath = localpath.split("build/")[0];
    qDebug() << localpath;
    makeAHKFile(0);*/

    if (AHKControl) //INCOMPLETE
    {
        ahksock.connectToHost("192.168.1.147", 5003);
        if (!ahksock.waitForConnected(1000))
            qDebug() << "AHK CONNECTION FAILURE";
    }

    /* Invert the following slash to flip  *\  }
    void Screen::temp()
    {/**/
    screenw = monitor.geometry().width();
    screenh = monitor.geometry().height();
    if (splitScreen) {
        screenw /= 2;
        screenh /= 2;
    }

    if (screenw > screenh * 3)
        screenw /= 2;

    /*gridheight=monitor.geometry().height()/gridh-100;
    gridwidth=monitor.geometry().width()/gridw-200;*/

    //ui->setupUi(this);

    //QWidget *screen = new QWidget(this);  // Set layout for Screen
    //screen->hide();
    //screen->setMinimumWidth(screenw);
    //screen->setMinimumHeight(screenh);

    //grid->setSpacing(0);

    MakeTitleScreen();

    MakeGameScreen();
    MakeSelectScreen();
    ///////////TEMPORARY FIX FOR AN ISSUE I CANT TRY TO FIGURE OUT RIGHT NOW/////////////
    clear(grid);
    MakeSelectScreen();
    UpdateSpot();
    ///////////TEMPORARY FIX FOR AN ISSUE I CANT TRY TO FIGURE OUT RIGHT NOW/////////////

    header = new QLabel;
    QPalette headerback;
    headerback.setColor(QPalette::Window, UAHLBlue);
    header->setFixedHeight(40);
    header->setPalette(headerback);
    header->setAutoFillBackground(true);
    QFont fontt = header->font();
    header->setText("Select a game with A");
    //qDebug() << "edit " << editMode;
    if (editMode) {
        qDebug() << "ROOT USER ACCEPTED";
        header->setText(
            "Edit mode active: press Blue to download selected data, Red to load to it");
        fontt.setPixelSize(20);
    }

    fontt.setPixelSize(30);
    header->setAlignment(Qt::AlignCenter);
    header->setFont(fontt);
    adjustFontSize(header, screenw);

    selectLayout.addWidget(header);
    selectLayout.addWidget(&Scroller);
    selectscreen.setLayout(&selectLayout);

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
    setLayout(&stack);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    showNormal();
    resize(screenw, screenh);
    if (splitScreen) {
        xspot = (ID < 3) ? 0 : screenw;
        yspot = (ID % 2 == 1) ? 0 : screenh;
        move(xspot, yspot);
    } else
        move(0, 0);

    if (DoStatus)
        MakeStatusBar();
    MakeNoti();

    //qDebug() << screenh;
    gamescreen.setFixedSize(screenw, screenh - 90);
    Scroller.setFixedHeight(screenh - header->height());
    //selectscreen.setFixedSize(screenw, screenh-10);
    //startscreen.setFixedSize(screenw, screenh);

    ScrollArea.setLayout(grid);
    Scroller.setWidget(&ScrollArea);

    QPalette back = ScrollArea.palette();
    back.setColor(QPalette::Window, UAHLGrey);

    ScrollArea.setAutoFillBackground(true);
    setPalette(back); //maybe could be done to screen itself instead of each individual one
    gamescreen.setAutoFillBackground(true);
    //gamescreen.setPalette(back);
    startscreen.setAutoFillBackground(true);
    //startscreen.setPalette(back);
    stack.setCurrentIndex(0); //////GO TO SPECIFIC PAGE//////////
    makeAHKFile(0);
    setFocus();
}


QPixmap* Screen::DrawFrame(QPixmap *image, QColor col, int thickness)
{
    QPainter border(image);
    QPen pen;
    pen.setWidth(thickness);
    pen.setColor(col);
    border.setPen(pen);
    border.drawRect(1, 1, gridwidth - 2, gridheight - 2);
    border.end();
    return image;
}

/*bkp void Screen::DrawFrame(QColor col, int id)
{
    QPixmap *Logo = images[id];
    QPainter border(Logo);
    QPen pen;
    pen.setWidth(borderThickness);
    pen.setColor(col);
    border.setPen(pen);
    border.drawRect(1, 1, gridwidth - 2, gridheight - 2);
    border.end();
    // *Logo = Logo->scaled(gridwidth, gridheight, Qt::IgnoreAspectRatio,Qt::SmoothTransformation); //is this necessary?
    entries[id]->findChild<QLabel *>("logo")->setPixmap(*Logo);
    entries[id]->findChild<QLabel *>("logo")->update();
}*/


void Screen::UpdateSpot()
{
    static int oldscroll;

    if (spot < 0)
        spot = 0;
    if (spot > total - 1)
        spot = total - 1;
    if (oldscroll != (int) (spot / gridw)) {
        //int oldscrollval = Scroller.verticalScrollBar()->value(), newscrollval = (((ceil(total/gridw))/(spot/gridw))*ScrollArea.height());
        int oldscrollval = Scroller.verticalScrollBar()->value(),
            newscrollval = ((spot / gridw) * gridheight * diff);

        //qDebug() << oldscroll << "-" << (int)(spot/gridw) << "-" << oldscrollval << "-" << newscrollval << "-" << oldscrollval+newscrollval-oldscrollval;

        int steps = 20;
        int ms = 6;
        int i = 0;

        QTimer *timer = new QTimer(this);

        // Capture variables in lambda to update scroll position incrementally
        QObject::connect(timer, &QTimer::timeout, [=]() mutable {
            i++;
            Scroller.verticalScrollBar()->setValue(oldscrollval
                                                   + (newscrollval - oldscrollval) / steps * i);

            if (i >= steps) {
                timer->stop();
                timer->deleteLater();
            }
        });

        timer->start(ms);
    }
    oldscroll = oldscroll = (int) (spot / gridw);
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

void Screen::adjustFontSize(QLabel *label, int max)
{
    int minFontSize = 2;
    int fontSize = label->font().pointSize();
    QFont font = label->font();

    //if(spot == 0)
    //qDebug() <<  ((double)(QFontMetrics(font).horizontalAdvance(label->text())))/gridwidth;
    while (fontSize >= minFontSize && QFontMetrics(font).horizontalAdvance(label->text()) > max) {
        fontSize--; // Decrease font size
        font.setPointSize(fontSize);
        label->setFont(font);
    }
}

Screen::~Screen() {}

void Screen::clear(QLayout *layout)
{
    prevSpot = spot;
    // Check if layout is valid
    if (!layout)
        return;

    // Iterate through all items in the layout
    QLayoutItem *item;
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

void Screen::Overlay()
{
    QWidget *overlay = new QWidget();
    overlay->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    overlay->setAttribute(Qt::WA_TranslucentBackground);
    overlay->resize(200, 200);
    overlay->show();
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
 * fix the scrolling so bottom options are able to be visible
 * edit the spacing of grid items
 * move the game info storage to sql and work on figuring sorting/filter options
 * maybe attach the grid spacing to a menu type thing
 *
*/
