#include "Charcade.h"

bool Screen::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        int key = (static_cast<QKeyEvent *>(event))->key();
        int let = char(key);
        if(key == Qt::Key_W || key == Qt::Key_A || key == Qt::Key_S || key == Qt::Key_D)
        {
            entries[spot]->findChild<QLabel *>("logo")->setPixmap(*DrawFrame(images[spot], UAHDBlue, borderThickness));
            entries[spot]->findChild<QLabel *>("logo")->update();

            if(key == Qt::Key_W)
                spot-=gridw;

            if(key == Qt::Key_A)
                spot--;

            if(key == Qt::Key_S)
                spot+=gridw;

            if(key == Qt::Key_D)
                spot++;

            UpdateSpot();
            entries[spot]->findChild<QLabel *>("logo")->setPixmap(*DrawFrame(images[spot], UAHYellow, borderThickness));
            entries[spot]->findChild<QLabel *>("logo")->update();
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

        if(key == Qt::Key_O && editMode)
            extract(spot+1);

        if(key == Qt::Key_P && editMode)
            //insert();

        if(key==Qt::Key_O)
        {
            //ShowStatus();
        }




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
                if (editMode) {
                    qDebug() << "ROOT USER ACCEPTED";
                    header->setText("Edit mode active: press Blue to download selected data, Red to load to it");
                }
            }
            qDebug() << "Moving to the next screen took" << timer.elapsed() << "milliseconds";
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


        qDebug() << "pressed " << key << "   " << char(key);

        if(let == 'Q')
        {
            qDebug() << "opaq";
            //summonNoti();
            notitimebar2->setWindowOpacity(1);
        }

        /*if(key==Qt::Key_T)
        {
            sendMessage("Hello it's a me Mario!\n", 2);
        }
        if(key==Qt::Key_B)
        {
            broadcastMessage("YAHOO!\n");
        }*/

        if(let == 'R')
        {
            qDebug() << "invis";
           // dismissNoti();
            notitimebar2->setWindowOpacity(0);
        }
        if(key==Qt::Key_X)
        {
            close();
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
