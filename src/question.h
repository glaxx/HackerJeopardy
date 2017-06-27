#ifndef QUESTION_H
#define QUESTION_H
#include <QObject>
#include <QJsonObject>

class question : public QObject
{
    Q_OBJECT

public:
    question();
    enum QuestionType {
        Text,
        Morse,
        Sound,
        Picture,
        Movie,
        Key_Sequence,
    };

    enum AnswerType {
        Correct,
        False,
        Oops,
    };

    void write(QJsonObject &json) const;
    void read(const QJsonObject &json);
    QuestionType getQuestionType() const;
    void setQuestionType(QuestionType type);
    QString getAnswer() const;
    void setAnswer(QString answer);
    QString getQuestion() const;
    void setQuestion(QString question);
    int getPoints() const;
    void setPoints(int points);
    Qt::CheckState getDoubleJeopardy() const;
    void setDoubleJeopardy(Qt::CheckState djeopardy);
    question(const question &obj);
    question(QuestionType type, QString answer, QString quest, int points, Qt::CheckState djeopardy);

private:
    QuestionType type;
    QString answer;
    QString quest;
    int points;
    Qt::CheckState djeopardy;

};

#endif // QUESTION_H
