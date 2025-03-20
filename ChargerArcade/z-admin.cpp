#include "Charcade.h"
void Screen::insert()
{
    QFile file(filePath+"/USBData.txt");
    QTextStream in(&file);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString USBData = in.readLine();


    QStringList info=USBData.split("||||");
    int id = info[0].toInt()-1;

    file.close();
    QString pre = QString::number(id) + "||||";

    file.setFileName(":/ChargerArcadeData.txt");
    QString tempFilePath = ":/ChargerArcadeDataTEMP.txt";
    QFile tempFile(tempFilePath);

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    tempFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&tempFile);

    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (line.startsWith(pre))
            out << USBData << "\n";
        else
            out << line << "\n";
    }

    file.close();
    tempFile.close();

    // Replace the original file with the temporary file.
    if (!QFile::remove(filePath)) {
        qDebug() << "Failed to remove original file:" << filePath;
        return;
    }

    if (!QFile::rename(tempFilePath, filePath)) {
        qDebug() << "Failed to rename temporary file:" << tempFilePath;
        return;
    }







    entries[id]->findChild<QLabel*>("title")->setText(info[1]);
    entries[id]->findChild<QLabel*>("credit")->setText("<i>"+info[3]+"</i>");

    images[id]->fill(QColor("#002D72"));
    images[id]->load(":/"+info[2]);
    *images[id] = images[id]->scaled(gridwidth, gridheight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    entries[id]->findChild<QLabel*>("logo")->setPixmap(*images[id]);
    entries[id]->findChild<QLabel*>("logo")->update();
    adjustFontSize(entries[id]->findChild<QLabel*>("title"), gridwidth);
    adjustFontSize(entries[id]->findChild<QLabel*>("credit"), gridwidth);

    if(spot == id)
    {
        entries[spot]->findChild<QLabel *>("logo")->setPixmap(*DrawFrame(images[spot], UAHYellow, borderThickness));
        entries[spot]->findChild<QLabel *>("logo")->update();
    }
    else
    {
        entries[spot]->findChild<QLabel *>("logo")->setPixmap(*DrawFrame(images[spot], UAHDBlue, borderThickness));
        entries[spot]->findChild<QLabel *>("logo")->update();
    }

    qDebug() << "Line replacement completed successfully.";


}

void Screen::extract(int num)
{
    QFile OGfile(":/ChargerArcadeData.txt");
    QString Line;

    OGfile.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream Data_In(&OGfile);
    static QRegularExpression regex;

    regex.setPattern(QString::number(num)+"\\|\\|\\|\\|");


    while (!Data_In.atEnd())
    {
        QString line = Data_In.readLine();
        if (regex.match(line).hasMatch())
            Line = line;
    }

    //QFile outfile("C:/Users/wesri/School/ChargerArcade/ChargerArcade/TestUsbContents.txt");
    QFile TEMPOut(filePath+"USBData.txt");
    TEMPOut.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream USB_out(&TEMPOut);
    USB_out << Line;
    OGfile.close();
    TEMPOut.close();
    //return Line;
}

void Screen::CheckForUSB()
{
    QFile file(filePath+"key.txt");
    QTextStream stream(&file);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QString line=stream.readLine();
        //qDebug() << "key text: " << line;
        if(line == keyPhrase)
        {
            editMode=true;
            qDebug() << "ROOT USER";
        }
    }

}

