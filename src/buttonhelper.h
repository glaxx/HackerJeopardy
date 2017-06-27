#ifndef BUTTONHELPER_H
#define BUTTONHELPER_H

#include <QObject>
#include "playscreen.h"

class ButtonHelper : public QObject
{
    Q_OBJECT
public:
    explicit ButtonHelper(QObject *parent = 0, QString = "", int = 0);

signals:
    void startQuestion(QString, int);

public slots:
    void onTriggered();

private:
    QString category;
    int points;
};

#endif // BUTTONHELPER_H
