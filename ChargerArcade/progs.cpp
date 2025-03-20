#include "Charcade.h"



void Screen::runGame()
{
    qDebug() << "Size" << programs.size() << "   " << spot;
    stack.setCurrentIndex(2);
    // QFile gameFile("/home/student/wr0018/ChargerArcade/ChargerArcade/programs/CharmStudies.sh");
    QFile gameFile(localpath + programs[spot]);
     qDebug() << "running" << programs[spot];
    if (!gameFile.exists()) {
        qDebug() << "File does not exist!";
    }

    //QString program = "/home/student/wr0018/ChargerArcade/ChargerArcade/programs/CharmStudies.sh";
    QString program = localpath + programs[spot];
    qDebug() << "path" << localpath + programs[spot];
    connect(game, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        qDebug() << "Process error:" << error;
    });
    connect(game, &QProcess::started, this, [](){
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
    QStringList datas = {"Up=w,Down=s,Left=a,Right=d,a=Enter,b=e","a=x,b=z"}; //reformat later
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

