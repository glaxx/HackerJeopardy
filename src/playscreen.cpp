#include "playscreen.h"
#include "ui_playscreen.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPixmap>
#include <QMapIterator>
#include "category.h"

PlayScreen::PlayScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayScreen),
    media(new QMediaPlayer()),
    plist(new QMediaPlaylist())
{
    ui->setupUi(this);
    this->plist->addMedia(QUrl("qrc:///sounds/Jeopardy-theme-song.mp3"));
    this->plist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    this->media->setPlaylist(plist);
    this->media->setVolume(50);
    QFont f = ui->label_Question->font();
    f.setPixelSize(32);
    f.setBold(true);
    this->ui->label_Question->setFont(f);
    this->ui->label_Question->setStyleSheet("color: yellow;");

    this->ui->stackedWidget->setStyleSheet("background-color: blue");
    f.setPixelSize(22);
    this->ui->label_Player->setFont(f);
    this->ui->label_Player->setStyleSheet("color: yellow; background-color: blue");
    this->ui->label_Points->setFont(f);
    this->ui->label_Points->setStyleSheet("color: yellow; background-color: blue");
    questions = 0;

}

PlayScreen::~PlayScreen()
{
    this->media->stop();
    delete ui;
}

void PlayScreen::startMusic() {
   this->media->play();
}

void PlayScreen::stopMusic() {
    this->media->stop();
}

void PlayScreen::setVolume(int volume) {
    this->media->setVolume(volume);
}

void PlayScreen::addCategory(category *c) {
    this->categories.append(c);
    QVBoxLayout *catLayout = new QVBoxLayout();
    catLayout->setSpacing(1);
    catLayout->setMargin(0);
    catLayout->setContentsMargins(0,0,0,0);
    this->ui->categories->addLayout(catLayout);
    QPushButton *cbtn = new QPushButton(c->getName());
    cbtn->show();
    cbtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    cbtn->setStyleSheet("background-color: blue; color: white");
    QFont f = cbtn->font();
    f.setBold(true);
    f.setPixelSize(36);
    cbtn->setFont(f);
    catLayout->addWidget(cbtn);
    foreach (question *q, c->getQuestions()){
        this->questions++;
        QString spoint;
        spoint.setNum(q->getPoints());
        QPushButton *btn = new QPushButton(spoint);
        f = btn->font();
        f.setBold(false);
        f.setPixelSize(22);
        btn->setFont(f);
        btn->setStyleSheet("background-color: blue; color: yellow");
        btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        this->buttons[c->getName()].insert(q->getPoints(),btn);
        catLayout->addWidget(btn);
        qDebug() << "PLAYSCREEN: QUESTION ADDED" << q->getQuestion();
        btn->show();
    }
}

void PlayScreen::startQuestionDialog(QString categoryName, int points) {
    bool b = false;
    foreach(category *c, this->categories) {
        if (b) { break; }
        if (c->getName() == categoryName) {
            foreach(question *q, c->getQuestions()) {
                if (q->getPoints() == points) {
                    this->buttons[c->getName()][q->getPoints()]->setStyleSheet("background-color: blue; color: blue");

                    this->buttons[c->getName()][q->getPoints()]->setEnabled(false);
                    QString spoints;
                    spoints.setNum(points);
                    this->ui->label_Points->setText(spoints);
                    switch (q->getQuestionType()) {
                        case question::Text: 	{this->ui->label_Question->setText(q->getQuestion());
                                                break;}
                    case question::Picture: {qDebug() << "Play" << q->getQuestion() << this->size();
                                                QPixmap p = QPixmap(q->getQuestion());
                                                qDebug() << "Play II" << p.size() << p.isNull();
                                                this->ui->label_Question->clear();
                                                this->ui->label_Question->setPixmap(p.scaled(this->size()-QSize(128,128),Qt::KeepAspectRatio, Qt::SmoothTransformation));
                                                qDebug() << "Play III" << p.size();
                                                break;}
                    default: 				{this->ui->label_Question->setText("DEBUG");
                                                break;}

                    }
                    b = true;
                    this->ui->stackedWidget->setCurrentIndex(1);
                    break;
                }
            }
        }
    }
}

void PlayScreen::questionAnswered(QString player, int points, question::AnswerType type) {
    qDebug() << "PLAYSCREEN: QUESTION ANSWERED:" << player << points << type;
    switch (type) {
        case question::Correct: this->changePlayerPoints(player, points);
                                this->closeQuestion();
                                break;
        case question::False: 	this->changePlayerPoints(player, 0 - points);
                                break;
        case question::Oops: 	break;
    }
    this->ui->label_Player->setText("Press a Button ...");
}

void PlayScreen::showResult() {
    QMapIterator<QString, int> i(this->player);
    while(i.hasNext()) {
        i.next();
        QHBoxLayout *l = new QHBoxLayout();
        QLabel *lbl = new QLabel(i.key());
        QFont f = lbl->font();
        f.setBold(true);
        f.setPixelSize(22);
        lbl->setStyleSheet("background-color: blue; color: yellow");
        lbl->setFont(f);
        QLabel *lbl2 = new QLabel();
        lbl2->setNum(i.value());
        lbl2->setFont(f);
        lbl2->setStyleSheet("background-color: blue; color: yellow");
        l->addWidget(lbl);
        l->addWidget(lbl2);
        this->ui->result_layout->addLayout(l);
    }

    this->ui->stackedWidget->setCurrentIndex(3);
}

void PlayScreen::changePlayerPoints(QString player, int points) {
    this->player[player] = this->player[player] + points;
    this->player_label[player]->setNum(this->player[player]);
}

void PlayScreen::closeQuestion() {
    this->questions--;
    if (this->questions != 0) {
        this->ui->stackedWidget->setCurrentIndex(0);
    } else {
        this->showResult();
    }
    this->ui->label_Player->setText("Press a Button ...");
}

void PlayScreen::addPlayer(QString player) {
    QVBoxLayout *ply = new QVBoxLayout();
    ply->setSpacing(1);
    QLabel *name = new QLabel(player, this);
    QFont f = name->font();
    f.setPixelSize(22);
    f.setBold(true);
    name->setFont(f);
    name->setStyleSheet("background-color: blue; color: yellow");
    QLabel *points = new QLabel("0", this);
    points->setStyleSheet("background-color: blue; color: yellow");
    points->setFont(f);
    this->player[player] = 0;
    this->player_label[player] = points;
    ply->addWidget(name);
    ply->addWidget(points);
    this->ui->player->addLayout(ply);
}

void PlayScreen::setAnsweringPlayer(QString player) {
    this->ui->label_Player->setText(player);
}

void PlayScreen::enterFullscreen() {
    this->showFullScreen();
}
