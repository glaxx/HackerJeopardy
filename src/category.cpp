#include "category.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

category::category(QString name) {
    this->name = name;
}

category::category(QString name, QVector<QPointer<question> > questions) {
    this->name = name;
    this->questions = questions;
}

void category::write(QJsonObject &json) const {
    QJsonArray ary;
    foreach (QPointer<question> q, this->questions) {
        qDebug() << q->getAnswer();
        QJsonObject obj;
        q->write(obj);
        ary.append(obj);
    }
    json["category"] = this->name;
    json["questions"] = ary;
}

void category::read(const QJsonObject &json) {
    this->name = json["category"].toString();

    QJsonArray temp = json["questions"].toArray();

    for (int i = 0; i != temp.size(); i++) {
        QJsonObject q = temp[i].toObject();
        QPointer<question> ques = new question();
        ques->read(q);
        this->addQuestion(ques);
    }
}

void category::addQuestion(QPointer<question> quest) {
    this->questions.append(quest);
}

QVector<QPointer<question> > category::getQuestions() {
    return this->questions;
}

void category::setName(QString name) {
    this->name = name;
}

QString category::getName() {
    return this->name;
}
