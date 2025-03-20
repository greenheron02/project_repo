#ifndef CHARCADE_H
#define CHARCADE_H

#include <QMainWindow>

class Screen : public QMainWindow
{
    Q_OBJECT

public:
    Screen(QWidget *parent = nullptr);
    ~Screen();
};
#endif // CHARCADE_H
