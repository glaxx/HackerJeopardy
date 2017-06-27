#include "playerhelper.h"

PlayerHelper::PlayerHelper(QObject *parent) : QObject(parent)
{

}

PlayerHelper::PlayerHelper(QObject *parent, QLabel *lbl, QPushButton *btn, QLineEdit *edit) : QObject(parent) {
    this->lbl = lbl;
    this->btn = btn;
    this->edit = edit;
}

void PlayerHelper::onTriggered() {
    QString temp;
    temp = this->lbl->text();
    int i = temp.toInt();
    this->btn->setEnabled(false);
    this->edit->setEnabled(false);
    emit(addFinished(this->edit->text(), i));
}
