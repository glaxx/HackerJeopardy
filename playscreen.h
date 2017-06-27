#ifndef PLAYSCREEN_H
#define PLAYSCREEN_H

#include <QDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLabel>
#include "category.h"

namespace Ui {
class PlayScreen;
}

class PlayScreen : public QDialog
{
    Q_OBJECT

public:
    explicit PlayScreen(QWidget *parent = 0);
    ~PlayScreen();
    void addCategory(category *c);

public slots:
    void addPlayer(QString player = "");
    void startQuestionDialog(QString categoryName, int points);
    void setAnsweringPlayer(QString player);
    void questionAnswered(QString player, int points, question::AnswerType type);
    void changePlayerPoints(QString player, int points);
    void startMusic();
    void stopMusic();
    void closeQuestion();
    //void clearGame();
    void setVolume(int volume);
    void enterFullscreen();
    void showResult();

signals:
    //void gameFinished();

private:
    Ui::PlayScreen *ui;
    QMediaPlayer *media;
    QMediaPlaylist *plist;
    QMap<QString, QMap<int, QPushButton*> > buttons;
    QVector<category*> categories;
    QMap<QString, int> player;
    QMap<QString, QLabel*> player_label;
    int questions = 0;
};

#endif // PLAYSCREEN_H
