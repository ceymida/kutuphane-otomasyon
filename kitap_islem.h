#ifndef KITAP_ISLEM_H
#define KITAP_ISLEM_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QModelIndex>

namespace Ui {
class kitap_islem;
}

class kitap_islem : public QDialog
{
    Q_OBJECT

public:
    explicit kitap_islem(QWidget *parent = nullptr);
    ~kitap_islem();
private slots:

    void listele();
    void on_pushButton_yeni_kayit_clicked();

    void on_pushButton_guncelle_clicked();

    void on_pushButton_sil_clicked();
    void updateTableView();

private:
    Ui::kitap_islem *ui;
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    QSqlQuery *sorgu;

};

#endif // KITAP_ISLEM_H
