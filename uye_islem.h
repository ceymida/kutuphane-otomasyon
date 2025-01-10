#ifndef UYE_ISLEM_H
#define UYE_ISLEM_H

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
class uye_islem;
}

class uye_islem : public QDialog
{
    Q_OBJECT

public:
    explicit uye_islem(QWidget *parent = nullptr);
    ~uye_islem();

private slots:

    void listele();

    void on_pushButton_yeni_kayit_clicked();
    void on_pushButton_guncelle_clicked(); // Güncelle butonunun slot fonksiyonu
    void on_pushButton_sil_clicked(); // Sil butonunun slot fonksiyonu
    void updateTableView(); // Tabloyu güncellemek için fonksiyon

private:
    Ui::uye_islem *ui;
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    QSqlQuery *sorgu;
    //QSqlQueryModel *model1;
};

#endif // UYE_ISLEM_H
