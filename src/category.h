#ifndef CATEGORY_H
#define CATEGORY_H
#include "question.h"
#include <QVector>
#include <QPointer>
class category
{

public:
    category(QString name = "");
    category(QString name, QVector<QPointer<question> > questions);
    void addQuestion(QPointer<question> quest);
    QVector<QPointer<question> > getQuestions();
    void write(QJsonObject &json) const;
    void read(const QJsonObject &json);
    void setName(QString name);
    QString getName();

private:
    QVector<QPointer<question> > questions;
    QString name;
};

#endif // CATEGORY_H
