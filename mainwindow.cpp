#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addeditquestiondialog.h"
#include <QStandardItemModel>
#include <QDebug>
#include <QList>
#include <QMap>
#include "playscreen.h"
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include "question.h"
#include "category.h"
#include "buttonhelper.h"
#include "serialsettingsdialog.h"
#include <QMessageBox>
#include "playerhelper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(new SettingsDialog),
    serial(new QSerialPort)
{
    qDebug() << QString("lel");
    ui->setupUi(this);
    this->model = new QStandardItemModel(0);
    this->category_index = new QMap<QString, QStandardItem *>;
    QStringList header;
    header << tr("Type") << tr("Question") << tr("Answer") << tr("Points") << tr("Double Jeopardy");
    this->model->setHorizontalHeaderLabels(header);
    ui->treeView->setModel(this->model);
    connect(ui->action_Add,SIGNAL(triggered(bool)),this,SLOT(openAEDialog()));
    //connect(ui->actionRun, SIGNAL(triggered(bool)),this,SLOT(openPlayScreen()));
    connect(ui->actionRun, SIGNAL(triggered(bool)), this,SLOT(openPlayScreen()));
    connect(ui->action_Open, SIGNAL(triggered(bool)), this, SLOT(openDatabase()));
    connect(ui->action_Save, SIGNAL(triggered(bool)), this, SLOT(writeDatabase()));
    connect(ui->actionSettings, SIGNAL(triggered(bool)), this, SLOT(openSettings()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(ui->pushButton_Connect, SIGNAL(pressed()), this, SLOT(openSerialPort()));
    connect(ui->pushButton_Disconnect, SIGNAL(pressed()), this, SLOT(closeSerialPort()));
    this->ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->model;
    delete this->category_index;
}

void MainWindow::openAEDialog() {
    QStringList *lst = new QStringList(this->category_index->keys());
    AddEditQuestionDialog *aedialog = new AddEditQuestionDialog(this, lst);
    connect(aedialog,SIGNAL(values_sane(QString, question*)),this,SLOT(addItem(QString, question*)));
    connect(this, SIGNAL(categoryAdded(QString)), aedialog, SLOT(onCategoryAdded(QString)));

    aedialog->exec();

    delete aedialog;
}

void MainWindow::openSettings() {
    settings->exec();
}

void MainWindow::addCategory(category *c) {
    QVBoxLayout *catLayout = new QVBoxLayout();
    catLayout->setSpacing(1);
    catLayout->setMargin(0);
    catLayout->setContentsMargins(0,0,0,0);
    this->ui->categoryLayout->addLayout(catLayout);
    QPushButton *cbtn = new QPushButton(c->getName());
    cbtn->setEnabled(false);
    cbtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    catLayout->addWidget(cbtn);
    foreach (question *q, c->getQuestions()){
        QString spoint;
        spoint.setNum(q->getPoints());
        QPushButton *btn = new QPushButton(spoint);
        btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        this->buttons[c->getName()].insert(q->getPoints(), btn);
        catLayout->addWidget(btn);
        ButtonHelper *bhelper = new ButtonHelper(this, c->getName(), q->getPoints());
        connect(btn, SIGNAL(pressed()), bhelper, SLOT(onTriggered()));
        connect(bhelper, SIGNAL(startQuestion(QString,int)), this->playscreen, SLOT(startQuestionDialog(QString,int)));
        connect(bhelper, SIGNAL(startQuestion(QString,int)), this, SLOT(startQuestionDialog(QString,int)));
    }
}

void MainWindow::openPlayScreen() {
    PlayScreen *pscreen = new PlayScreen(this);
    this->playscreen = pscreen;
    pscreen->show();
    foreach (category *c, this->categorys) {
        pscreen->addCategory(c);
        this->addCategory(c);
    }
    this->ui->stackedWidget->setCurrentIndex(1);
    this->clearQuestion(false);
    connect(this->ui->pushButton_startMusic, SIGNAL(pressed()), pscreen, SLOT(startMusic()));
    connect(this->ui->pushButton_StopMusic, SIGNAL(pressed()), pscreen, SLOT(stopMusic()));
    connect(this->ui->volumeSlider, SIGNAL(sliderMoved(int)), pscreen, SLOT(setVolume(int)));
    connect(this->ui->pushButton_Correct, SIGNAL(pressed()), this, SLOT(correctAnswer()));
    connect(this->ui->pushButton_False, SIGNAL(pressed()), this, SLOT(falseAnswer()));
    connect(this->ui->pushButton_Oops, SIGNAL(pressed()), this, SLOT(oopsAnswer()));
    connect(this->ui->pushButton_CloseQuestion, SIGNAL(pressed()), this, SLOT(closeQuestion()));
    connect(this->ui->addPlayer_PushButton, SIGNAL(pressed()), this, SLOT(addPlayer()));
    connect(this->ui->pushButton_FullScreen, SIGNAL(pressed()), this->playscreen, SLOT(enterFullscreen()));
}

void MainWindow::openDatabase() {
    QFileDialog dialog;
    QString fileName = dialog.getOpenFileName(this, tr("Open Database"), QDir::homePath(), tr("JSON File (*.json)"));
    qDebug() << "Database Path " << fileName;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    QByteArray data = file.readAll();
    file.close();
    QJsonDocument json(QJsonDocument::fromJson(data));
    this->read(json.object());
}

void MainWindow::writeDatabase() {
    QFileDialog dialog;
    QString fileName = dialog.getSaveFileName(this,tr("Save Database"), QDir::homePath(), tr("JSON File (*.json)"));
    qDebug() << "Save Path" << fileName;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QJsonObject mainobj;
    this->write(mainobj);
    QJsonDocument doc(mainobj);
    file.write(doc.toJson());
    file.close();
}

void MainWindow::read(const QJsonObject &json) {
    QJsonArray ary = json["categorys"].toArray();

    for (int i = 0; i != ary.size(); i++) {
        QJsonObject obj = ary[i].toObject();
        category *cat = new category();
        cat->read(obj);
        foreach(question *q, cat->getQuestions()) {
            this->addItem(cat->getName(), q);
        }
    }
}

void MainWindow::write(QJsonObject &json) const {
    QJsonArray ary;
    foreach(category *c, this->categorys) {
        qDebug() << c->getName();
        foreach(question *q, c->getQuestions()) {
            qDebug() << q->getAnswer();
        }

        QJsonObject obj;
        c->write(obj);
        ary.append(obj);
    }
    json["categorys"] = ary;
}

void MainWindow::addItem(QString categ, question *ques) {
    QList<QStandardItem *> list;

    QString styp;
    styp.setNum(ques->getQuestionType());
    QStandardItem *typ = new QStandardItem(styp);
    QStandardItem *que = new QStandardItem(ques->getQuestion());
    QStandardItem *ans = new QStandardItem(ques->getAnswer());
    QString spoi;
    spoi.setNum(ques->getPoints());
    QStandardItem *poi = new QStandardItem(spoi);
    QStandardItem *dje = new QStandardItem(ques->getDoubleJeopardy());
    dje->setCheckable(true);
    dje->setCheckState(ques->getDoubleJeopardy());
    list << typ << que << ans << poi << dje;

    QStandardItem *cat;
    if (this->category_index->contains(categ)) {
        foreach (category* c, this->categorys) {
            if (c->getName() == categ) {
                c->addQuestion(ques);
            } else {
                qDebug() << "CATEGORY NOT FOUND";
            }
        }

        cat = this->category_index->value(categ);
        cat->appendRow(list);
        emit(this->model->itemChanged(cat));
    } else {
        category *cate = new category(categ);
        this->categorys.append(cate);
        cate->addQuestion(ques);
        cat = new QStandardItem(categ);
        this->category_index->insert(categ, cat);
        cat->appendRow(list);
        QList<QStandardItem *> res;
        res << cat;
        this->model->appendRow(res);
        emit(this->categoryAdded(categ));
    }
}

void MainWindow::correctAnswer() {
    this->playscreen->questionAnswered(this->ui->label_Player->text(), this->ui->label_Points->text().toInt(), question::Correct);
    this->clearQuestion(true);
}

void MainWindow::falseAnswer() {
    this->playscreen->questionAnswered(this->ui->label_Player->text(), this->ui->label_Points->text().toInt(), question::False);
    this->resetBuzzerState();
}

void MainWindow::oopsAnswer() {
    this->playscreen->questionAnswered(this->ui->label_Player->text(), this->ui->label_Points->text().toInt(), question::Oops);
    this->resetBuzzerState();
}

void MainWindow::closeQuestion() {
    this->playscreen->closeQuestion();
    this->clearQuestion(true);
}

void MainWindow::startQuestionDialog(QString cat, int poi) {
    bool b = false;
    foreach(category *c, this->categorys) {
        if (b) { break; }
        if (c->getName() == cat) {
            foreach(question *q, c->getQuestions()) {
                if (q->getPoints() == poi) {
                    ui->label_Answer->setText(q->getAnswer());
                    QString temp;
                    temp.setNum(q->getPoints());
                    ui->label_Points->setText(temp);
                    ui->label_Question->setText(q->getQuestion());
                    ui->label_Category->setText(c->getName());
                    temp.setNum(int(q->getQuestionType()));
                    ui->label_Type->setText(temp);
                    ui->pushButton_Bet->setEnabled(true);
                    ui->pushButton_CloseQuestion->setEnabled(true);
                    //ui->pushButton_Correct->setEnabled(true);
                    //ui->pushButton_False->setEnabled(true);
                    //ui->pushButton_Oops->setEnabled(true);
                    b = true;
                    connect(this, SIGNAL(activeBuzzerChangedFromNull(int)), this, SLOT(playerOnAnswerQuestion(int)));
                    break;
                }
            }
        }
    }
}

void MainWindow::playerOnAnswerQuestion(int bzr) {
    ui->pushButton_Correct->setEnabled(true);
    ui->pushButton_False->setEnabled(true);
    ui->pushButton_Oops->setEnabled(true);
    qDebug() << "playeronanswerquestion" << this->player[Buzzer(bzr)];
    this->ui->label_Player->setText(this->player[Buzzer(bzr)]);
    this->playscreen->setAnsweringPlayer(this->player[Buzzer(bzr)]);
}

void MainWindow::clearQuestion(bool b) {
    if (b) {
        this->buttons[ui->label_Category->text()][ui->label_Points->text().toInt()]->setEnabled(false);
    }
    this->ui->label_Answer->setText("None Selected");
    this->ui->label_Player->setText("None Selected");
    this->ui->label_Points->setText("None Selected");
    this->ui->label_Question->setText("None Selected");
    this->ui->label_Type->setText("None Selected");
    this->ui->label_Category->setText("None Selected");
    this->ui->pushButton_Bet->setEnabled(false);
    this->ui->pushButton_CloseQuestion->setEnabled(false);
    this->ui->pushButton_Correct->setEnabled(false);
    this->ui->pushButton_False->setEnabled(false);
    this->ui->pushButton_Oops->setEnabled(false);
    disconnect(this, SIGNAL(activeBuzzerChangedFromNull(int)), this, SLOT(playerOnAnswerQuestion(int)));
    this->resetBuzzerState();
}

void MainWindow::openSerialPort() {
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        ui->pushButton_Connect->setEnabled(false);
        ui->pushButton_Disconnect->setEnabled(true);
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
    }
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    this->resetBuzzerState();
}

void MainWindow::closeSerialPort() {
    disconnect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    if (serial->isOpen()) { serial->close(); }
    ui->pushButton_Connect->setEnabled(true);
    ui->pushButton_Disconnect->setEnabled(false);
}

void MainWindow::readData() {
    QByteArray data = serial->readAll();
    this->buffer.append(data);
    if(this->buffer.size() > 2) {
        if(this->buffer.contains("\r\n")) {
            this->evaluateBuzzerState(this->buffer);
            this->buffer.clear();
        }
    }
}

void MainWindow::evaluateBuzzerState(const QByteArray &data) {
    QString temp(data);
    QJsonDocument json(QJsonDocument::fromJson(data));
    int active = json.object()["first_buzzer"].toInt();
    qDebug() << "MainWindow: Evaluate" << active << this->activeBuzzer;
    if ((this->activeBuzzer == 0) && (this->activeBuzzer != active)) {
        qDebug() << "Mainwindow: evaluate: fromNull";
        this->activeBuzzer = Buzzer(active);
        emit(activeBuzzerChangedFromNull(active));
    }

    if (this->activeBuzzer != active) {
        qDebug() << "MainWindow: Evaluate: activebuzzer";
        this->activeBuzzer = Buzzer(active);
        emit(activeBuzzerChanged(active));
    }
}

void MainWindow::resetBuzzerState() {
    QByteArray ary("RESET_BUZZERS\n");
    this->writeData(ary);
    this->activeBuzzer = Buzzer(0);
}

void MainWindow::writeData(const QByteArray &data) {
    serial->write(data);
}

void MainWindow::addPlayer() {
    QHBoxLayout *ply = new QHBoxLayout();
    QLabel *bzr = new QLabel();
    bzr->setNum(int(this->activeBuzzer));
    QLineEdit *ledit = new QLineEdit();
    QPushButton *btn = new QPushButton("Add");
    PlayerHelper *helper = new PlayerHelper(this, bzr, btn, ledit);
    ply->addWidget(bzr);
    ply->addWidget(ledit);
    ply->addWidget(btn);
    this->ui->playerLayout->addLayout(ply);
    this->ply_count++;
    connect(btn, SIGNAL(pressed()), helper, SLOT(onTriggered()));
    connect(helper, SIGNAL(addFinished(QString,int)), this, SLOT(addPlayerFinished(QString,int)));
}

void MainWindow::addPlayerFinished(QString name, int bzr) {
    this->playscreen->addPlayer(name);
    this->player[Buzzer(bzr)] = name;
    this->resetBuzzerState();
}
