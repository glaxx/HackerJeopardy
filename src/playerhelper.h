#ifndef PLAYERHELPER_H
#define PLAYERHELPER_H

#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

class PlayerHelper : public QObject
{
    Q_OBJECT
public:
    explicit PlayerHelper(QObject *parent = 0);
    PlayerHelper(QObject*, QLabel*, QPushButton*, QLineEdit*);

signals:
    void addFinished(QString, int);

public slots:
    void onTriggered();

private:
    QLabel *lbl;
    QPushButton *btn;
    QLineEdit *edit;
};

#endif // PLAYERHELPER_H
