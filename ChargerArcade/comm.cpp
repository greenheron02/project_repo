#include "screen.h"
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
    QFile TEMPOut("C:/data.txt");
    TEMPOut.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream USB_out(&TEMPOut);
    USB_out << Line;
    OGfile.close();
    TEMPOut.close();
    //return Line;
}

void Screen::insert()
{
    QFile file("C:/data.txt");
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
    /*if (!QFile::remove(filePath)) {
        qDebug() << "Failed to remove original file:" << filePath;
        return;
    }*/

    /*if (!QFile::rename(tempFilePath, filePath)) {
        qDebug() << "Failed to rename temporary file:" << tempFilePath;
        return;
    }*/







    entries[id]->findChild<QLabel*>("title")->setText(info[1]);
    entries[id]->findChild<QLabel*>("credit")->setText("<i>"+info[3]+"</i>");

    images[id]->fill(QColor("#002D72"));
    images[id]->load(":/"+info[2]);
    *images[id] = images[id]->scaled(gridwidth, gridheight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    entries[id]->findChild<QLabel*>("logo")->setPixmap(*images[id]);
    entries[id]->findChild<QLabel*>("logo")->update();
    adjustFontSize(entries[id]->findChild<QLabel*>("title"));
    adjustFontSize(entries[id]->findChild<QLabel*>("credit"));

    if(spot == id)
        DrawFrame(UAHYellow, id);
    else
        DrawFrame(UAHDBlue, id);

    qDebug() << "Line replacement completed successfully.";

}

void Screen::CheckForUSB()
{
    QFile file("C:/key.txt");
    QTextStream stream(&file);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QString line=stream.readLine();
        //qDebug() << line;
        if(line == keyPhrase)
        {
            editMode=true;
            qDebug() << "ROOT USER";
        }
    }

}





void Screen::runGame()
{

    stack.setCurrentIndex(2);
    // QFile gameFile("/home/student/wr0018/ChargerArcade/ChargerArcade/programs/CharmStudies.sh");
    QFile gameFile(localpath + programs[spot]);
     qDebug() << "running" << programs[spot];
    if (!gameFile.exists()) {
        qDebug() << "File does not exist!";
    }

    //QString program = "/home/student/wr0018/ChargerArcade/ChargerArcade/programs/CharmStudies.sh";
    QString program = localpath + programs[spot];
    connect(game, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        qDebug() << "Process error:" << error;
    });
    connect(game, &QProcess::started, this, [=](){
        qDebug() << "started successully";
    });
    connect(game, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus == QProcess::NormalExit) {
            qDebug() << "Process ended successfully with exit code:" << exitCode;
        } else {
            qDebug() << "Process crashed with exit code:" << exitCode;
        }
    });

    QStringList arguments;
    arguments << "--window-width" << "1000" << "--window-height" << "1000"; // Example window size arguments
    game->start(program);
    makeAHKFile(spot+1);
}

void Screen::resizeWindowWithAHK(const QString &windowTitle, int width, int height) //incomplete
{
    if(AHKActive)
    {
    QString program = localpath + "AutoHotkey.exe";
    QString scriptPath = ":/ChargerArcade.ahk";

    QStringList arguments;
    arguments << scriptPath
              << windowTitle
              << QString::number(width)
              << QString::number(height);

    QProcess *process = new QProcess(this);
    connect(process, &QProcess::errorOccurred, [](QProcess::ProcessError error) {
        qDebug() << "Error starting AutoHotKey script:" << error;
    });

    connect(process, &QProcess::finished, [](int exitCode, QProcess::ExitStatus status) {
        if (status == QProcess::NormalExit)
            qDebug() << "AHK script finished with code:" << exitCode;
        else
            qDebug() << "AHK script crashed!";
    });

    process->start(program, arguments);
    }
}

void Screen::sendToAHK(const QString &key)
{
    if(AHKActive)
    {
    QString message = "A";
    QByteArray data = message.toUtf8();
    data.append('\0'); // Add null terminator
    ahksock.write(data); // Send input to AHK
    ahksock.flush();
    /*
    QStringList arguments;
    arguments << "C:/Users/wesri/Documents/AutoHotkey/ChargerArcadeControls.ahk" << key;
    QProcess::startDetached("AutoHotKey.exe", arguments);*/
    }
}

void Screen::makeAHKFile(int programid)
{
    if(AHKActive)
    {
        QStringList datas = {"Up=w,Down=s,Left=a,Right=d,w=Enter,a=e","Up=Up","Left=Up,w=Space,a=d"}; //reformat later
        QString data = datas[programid];
        qDebug() << "setting controls to " << data;
    //data = data.toLower();
    QStringList dataa=data.split(",");

    QFile ahk(localpath +"ahkfile.ahk");
    QTextStream stream(&ahk);
    ahk.open(QIODevice::WriteOnly|QIODevice::Text);
    stream << "#Requires AutoHotkey v2.0\n#SingleInstance Force\nesc::ExitApp";



    for(int i=0; i < dataa.length(); i++)
    {
        QStringList dataaa = dataa[i].split("=");
        stream << "\n" << dataaa[0] << "::" << dataaa[1]<<"\n";
        //stream << "\n" << dataaa[0] << "::\n{\nSend (\"" << dataaa[1]<<"\")\n}\n";
        //con(dataa[1])=dataa[2];
    }
    ahk.close();

    // Start the AHK script
    QString localpathbackslash = localpath;
    localpathbackslash.replace("/", "\\\\");
    bool success = QProcess::startDetached(localpath+"AutoHotkey.exe", QStringList() << localpathbackslash+"ahkfile.ahk");

    if (success)
        qDebug() << "AHK script executed successfully.";

    else
    {
        qDebug() << "Failed to execute AHK script.";
        AHKActive=false;
    }
    }
}

void Screen::endScript()
{
    if(AHKActive)
    {
    QFile ahk(localpath + "ahkfile.ahk");
    QTextStream stream(&ahk);
    ahk.open(QIODevice::WriteOnly|QIODevice::Text);
     stream << "#SingleInstance Force\nExitApp";
    ahk.close();
    QString localpathbackslash = localpath;
    localpathbackslash.replace("/", "\\\\");
    QProcess::startDetached(localpath+"AutoHotkey.exe", QStringList() << localpathbackslash+"ahkfile.ahk");
    }
}

