#ifndef IHELPER_H
#define IHELPER_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QSettings>
#include <Crypter.h>
#include <QSystemTrayIcon>
#include <QMenu>

namespace Ui {
class IHelper;
}

class IHelper : public QMainWindow
{
    Q_OBJECT

public:
    explicit IHelper(QWidget *parent = 0);
    ~IHelper();

private:
    Ui::IHelper *ui;
    QString name;
    QString login;
    QString pwd;
    bool developer = false;
    QSettings *settings = new QSettings("HKEY_CURRENT_USER\\Software\\IHelper",QSettings::NativeFormat);
    QSettings *regSettings = new QSettings("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    QString savedValue;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
private slots:
    void changeEvent(QEvent*);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayActionExecute();
    void setTrayIconActions();
    void showTrayIcon();
    void OkPressed();
    void CancelPressed();
    void AutorunChecked();
protected:
    virtual void keyPressEvent(QKeyEvent *event);

};

#endif // IHELPER_H
