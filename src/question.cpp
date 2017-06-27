#include "question.h"
#include "QDebug"
#include <QJsonObject>

question::question()
{
    this->type = Text;
    this->answer = "";
    this->djeopardy = Qt::Unchecked;
    this->quest = "";
    this->points = 0;
}

question::question(const question &obj) {
    answer = obj.getAnswer();
    quest = obj.getQuestion();
    points = obj.getPoints();
    type = obj.getQuestionType();
    djeopardy = obj.getDoubleJeopardy();
}

question::question(QuestionType type, QString answer, QString quest, int points, Qt::CheckState djeopardy) {
    this->type = type;
    this->answer = answer;
    this->quest = quest;
    this->points = points;
    this->djeopardy = djeopardy;
}

void question::write(QJsonObject &json) const {
    json["type"] = this->type;
    json["answer"] = this->answer;
    json["question"] = this->quest;
    json["points"] = this->points;
    json["djeopardy"] = this->djeopardy;
}

void question::read(const QJsonObject &json) {
    this->type = QuestionType(json["type"].toInt());
    this->answer = json["answer"].toString();
    this->quest = json["question"].toString();
    this->points = json["points"].toInt();
    this->djeopardy = Qt::CheckState(json["djeopardy"].toInt());
}

question::QuestionType question::getQuestionType() const{
    return this->type;
}

void question::setQuestionType(QuestionType type) {
    this->type = type;
}

QString question::getAnswer() const{
    return this->answer;
}

void question::setAnswer(QString answer) {
    this->answer = answer;
}

QString question::getQuestion() const{
    return this->quest;
}

void question::setQuestion(QString question) {
    this->quest = question;
}

int question::getPoints() const{
    return this->points;
}

void question::setPoints(int points) {
    this->points = points;
}

Qt::CheckState question::getDoubleJeopardy() const {
    return this->djeopardy;
}

void question::setDoubleJeopardy(Qt::CheckState djeopardy) {
    this->djeopardy = djeopardy;
}

