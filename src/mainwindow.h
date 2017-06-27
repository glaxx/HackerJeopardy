#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMap>
#include "addeditquestiondialog.h"
#include "question.h"
#include "category.h"
#include "playscreen.h"
#include <QtSerialPort/QSerialPort>
#include "serialsettingsdialog.h"
#include <QLineEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    ~MainWindow();
    enum Buzzer {
        None,
        First,
        Second,
        Third,
        Forth,
    };

private slots:
    void addItem(QString, question*);
    void openAEDialog();
    void openPlayScreen();
    void openDatabase();
    void writeDatabase();
    void addCategory(category *c);
    void addPlayer();
    void addPlayerFinished(QString, int);
    void startQuestionDialog(QString, int);
    void correctAnswer();
    void falseAnswer();
    void oopsAnswer();
    void clearQuestion(bool);
    void closeQuestion();
    void openSettings();
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void evaluateBuzzerState(const QByteArray &data);
    void resetBuzzerState();
    void playerOnAnswerQuestion(int);

signals:
    void categoryAdded(QString);
    void activeBuzzerChanged(int);
    void activeBuzzerChangedFromNull(int);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QMap<QString, QStandardItem *> *category_index;
    QList<category *> categorys;
    PlayScreen *playscreen;
    QSerialPort *serial;
    SettingsDialog *settings;
    Buzzer activeBuzzer;
    QMap<Buzzer, QString> player;
    int ply_count;
    QByteArray buffer;
    QMap<QString, QMap<int, QPushButton*> > buttons;
};

#endif // MAINWINDOW_H
