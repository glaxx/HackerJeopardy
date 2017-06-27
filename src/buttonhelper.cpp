#include "buttonhelper.h"

ButtonHelper::ButtonHelper(QObject *parent, QString cat, int poi) : QObject(parent)
{
    this->category = cat;
    this->points = poi;
}

void ButtonHelper::onTriggered() {
    emit(this->startQuestion(this->category, this->points));
}
