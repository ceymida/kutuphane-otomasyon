#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include<QSql>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QMessageBox>
#include <QSqlError>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_kitap_islem_clicked();

    void on_pushButton_odunc_alma_clicked();

    void on_pushButton_odunc_teslim_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    QSqlQuery *sorgu;
};
#endif // MAINWINDOW_H
