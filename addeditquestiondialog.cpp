#include <QtMultimediaWidgets>
#include "addeditquestiondialog.h"
#include "ui_addeditquestiondialog.h"
#include <QDebug>
#include <QFileDialog>
#include "question.h"

AddEditQuestionDialog::AddEditQuestionDialog(QWidget *parent, QStringList *categories) :
    QDialog(parent),
    ui(new Ui::AddEditQuestionDialog)
{
    ui->setupUi(this);
    connect(this->ui->buttonBox, SIGNAL(accepted()), this, SLOT(check_values()));

    connect(this, SIGNAL(values_sane(QString, question*)), this, SLOT(accept()));

    QStringList lst;
    lst << tr("Text") << tr("Morse") << tr("Sound") << tr("Picture") << tr("Movie") << tr("Key Sequence"); // Corresponding to the QuestionType enum
    ui->comboBox_Type->addItems(lst);
    ui->comboBox_category->addItems(*categories);

    connect(ui->comboBox_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeSwitched(int)));
    connect(ui->pushButton_Picture, SIGNAL(clicked()), this, SLOT(openPicture()));
    onTypeSwitched(question::Text);
}

AddEditQuestionDialog::~AddEditQuestionDialog()
{
    delete ui;
}

void AddEditQuestionDialog::check_values() {
    QString quest;
    switch(this->ui->comboBox_Type->currentIndex()) {
        case question::Text:	quest = this->ui->lineEdit_text->text(); break;
        case question::Morse:	quest = this->ui->lineEdit_morse->text(); break;
        case question::Picture:	quest = this->file; break;
        default:	quest = "DEBUG";
    }

    question *ques = new question(question::QuestionType(this->ui->comboBox_Type->currentIndex()),
                                 this->ui->lineEdit_answer->text(),
                                 quest,
                                 this->ui->spinBox_points->value(),
                                 this->ui->checkBox_double_jeopardy->checkState());

    emit(this->values_sane(this->ui->comboBox_category->currentText(), ques));
}

void AddEditQuestionDialog::openPicture() {
    QFileDialog dialog;
    QString fileName = dialog.getOpenFileName(this->parentWidget(), tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
    this->ui->label_Picture->clear();
    qDebug() << fileName;
    this->file = fileName;
    this->ui->label_Picture->setPixmap(QPixmap(fileName).scaledToWidth(this->width()-contentsMargins().left()*2,Qt::SmoothTransformation));
    adjustSize();
}

void AddEditQuestionDialog::onCategoryAdded(QString category) {
    this->ui->comboBox_category->addItem(category);
}

void AddEditQuestionDialog::onTypeSwitched(int type) {
    switch (type) {
    case question::Text:			this->ui->stackedWidget->setCurrentIndex(question::Text);
                                    break;

    case question::Morse:			this->ui->stackedWidget->setCurrentIndex(question::Morse);
                                    break;

    case question::Sound:			this->ui->stackedWidget->setCurrentIndex(question::Sound);
                                    break;

    case question::Picture:			this->ui->stackedWidget->setCurrentIndex(question::Picture);
                                    break;

    case question::Movie:			this->ui->stackedWidget->setCurrentIndex(question::Movie);
                                    break;

    case question::Key_Sequence:	this->ui->stackedWidget->setCurrentIndex(question::Key_Sequence);
                                    break;
    }
    adjustSize();
}
