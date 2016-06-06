#include "ihelper.h"
#include "ui_ihelper.h"
#include <QProcess>
#include <QMessageBox>
QString rasdial;
IHelper::IHelper(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IHelper)
{
    this->setTrayIconActions();
    //this->showTrayIcon();
    ui->setupUi(this);
    connect(ui->pbOk,SIGNAL(clicked(bool)),this,SLOT(OkPressed()));
    connect(ui->pbExit,SIGNAL(clicked(bool)),this,SLOT(CancelPressed()));
    connect(ui->boxAuto,SIGNAL(clicked(bool)),this,SLOT(AutorunChecked()));
    QRegExp rx("[a-zA-Z0-9]+");
    QValidator *valid = new QRegExpValidator(rx,this);
    ui->leLogin->setValidator(valid);
    ui->lePwd->setValidator(valid);
    //прикручиваем валидатор
    ui->leName->setText(settings->value("rasdial/name").toString());
    ui->leLogin->setText(settings->value("rasdial/login").toString());
    ui->lePwd->setText(Crypter::decryptString(settings->value("rasdial/pwd").toString()));
    if (settings->value("settings/autorun") == 1){
            ui->boxAuto->setChecked(true);
        }
    //если конфиг существует, загружаем данные
}

IHelper::~IHelper()
{
    delete ui;
}

void IHelper::OkPressed(){
    if ((ui->leLogin->text().isEmpty() == false) && (ui->leName->text().isEmpty() == false)
            && (ui->lePwd->text().isEmpty() == false) && ui->leLogin->validator())
    {
        //защита от дурака
    name = "\""+ui->leName->text()+"\"";
    login = " "+ui->leLogin->text();
    pwd = " "+ui->lePwd->text();
    rasdial = "rasdial.exe "+name+login+pwd;
    //ЩИТО????!??!?!??!
    if (developer == false){
        system(qPrintable("rasdial /disconnect"));
        system(qPrintable(rasdial));
        ui->leDebug->setText(rasdial);
    } else {
        ui->leDebug->setText(rasdial);
    }
    //выполнение команды в зависимости от режима работы
    settings->setValue("rasdial/name",ui->leName->text());
    settings->setValue("rasdial/login",ui->leLogin->text());
    settings->setValue("rasdial/pwd",Crypter::cryptString(ui->lePwd->text()));
    settings->sync();
    //запись полей
  //  ui->boxAuto->setDisabled(true);
    } else {
        QMessageBox *box = new QMessageBox();
        box->setText("Одно из полей не заполнено.");
        box->setIcon(QMessageBox::Warning);
        box->show();
        //MessageBox в случае срабатывания защиты
    }
}
void IHelper::CancelPressed(){
    exit(1);
    //выход!!
}

void IHelper::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_F5){
        if (developer == true){
            ui->leDebug->hide();
            developer = false;
        } else {
            ui->leDebug->show();
            developer = true;
        }
        //режим разработчика
    } else if (event->key() == Qt::Key_F9){
        settings->clear();
    } else if (event->key() == Qt::Key_F1){
                QMessageBox *help = new QMessageBox();
                help->setIconPixmap(QPixmap(":/Qt.png"));
                help->setWindowTitle("IHelper");
                help->setText("<p align='center'>IHelper v1.5</p>");
                help->setInformativeText("<p align='center'>Создано&nbsp;на&nbsp;"
                                         "Qt&nbsp;5.6(MSVC 2015x32)<br><br>Горячие клавиши:<br>"
                                         "<br>F5&nbsp;-&nbsp;включение&nbsp;режима&nbsp;разработчика<br>"
                                         "<br>F9&nbsp;-&nbsp;удаление&nbsp;конфигурационного&nbsp;файла</p>");
                help->setWindowIcon(QIcon(":/ico.ico"));
                help->show();
    }
  }
    //хелп бокс

void IHelper::AutorunChecked(){
    if (settings->value("settings/autorun",0) == 0){
        settings->setValue("settings/autorun",1);
        name = "\""+ui->leName->text()+"\"";
        login = " "+ui->leLogin->text();
        pwd = " "+ui->lePwd->text();
        rasdial = "rasdial.exe "+name+login+pwd;
        regSettings->setValue("Internet",rasdial);
    } else {
        regSettings->remove("Internet");
        settings->setValue("settings/autorun",0);
    }
    //автозагрузка, в зависимости от конфига либо выключается либо включается
}
void IHelper::showTrayIcon()
{
    // Создаём экземпляр класса и задаём его свойства...
    trayIcon = new QSystemTrayIcon(this);
    QIcon trayImage(":/icomain.png");
    trayIcon->setIcon(trayImage);
    trayIcon->setContextMenu(trayIconMenu);
    // Подключаем обработчик клика по иконке...
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    // Выводим значок...
    trayIcon->show();
}
void IHelper::trayActionExecute()
{
    this->show();
    this->setWindowState(Qt::WindowActive);
}
void IHelper::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            this->trayActionExecute();
            break;
        default:
            break;
    }
}
void IHelper::setTrayIconActions()
{
    // Создаем "экшены"
    restoreAction = new QAction("Развернуть", this);
    quitAction = new QAction("Выход", this);

    // Соединение сигналов и слотов
    connect (restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    connect (quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Меню
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction (restoreAction);
    trayIconMenu->addAction (quitAction);
}
void IHelper::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange)
    {
        if (isMinimized())
        {
            this->showTrayIcon();
            this->hide();
        }
    }
}
void IHelper::showMesDisconnected(){
    trayIcon->showMessage("IHelper","Соединение с интернетом отсутствует. Попытка подключения...",QSystemTrayIcon::Critical,1000);
}
void IHelper::showMesConnected(){
    trayIcon->showMessage("IHelper","Соединение с интернетом восстановлено.",QSystemTrayIcon::Information,1000);
}
/* TODO: допилить сворачивание в трей
 * TODO: автоподруб соединения при дисконнекте/проверка соединения
 * 1.0 - релиз
 * 1.1 - защита от дурака
 * 1.2 - улучшение интерфейса
 * 1.3 - валидатор ввода
 * 1.4 - сворачивание в трей
 * 1.5 - теперь идёт запись в реестр => убран костыль на проверку существования файла
*/
