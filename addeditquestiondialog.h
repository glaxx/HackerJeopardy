#ifndef ADDEDITQUESTIONDIALOG_H
#define ADDEDITQUESTIONDIALOG_H

#include <QGraphicsScene>
#include <QDialog>
#include "question.h"

namespace Ui {
class AddEditQuestionDialog;
}

class AddEditQuestionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEditQuestionDialog(QWidget *parent = 0, QStringList *categories = 0);
    ~AddEditQuestionDialog();

private slots:
    void check_values();
    void onCategoryAdded(QString);
    void onTypeSwitched(int);
    void openPicture();

signals:
    void values_sane(QString category, question *quest);

private:
    Ui::AddEditQuestionDialog *ui;
    QString file;
};

#endif // ADDEDITQUESTIONDIALOG_H
