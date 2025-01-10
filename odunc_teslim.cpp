#include "odunc_teslim.h"
#include "ui_odunc_teslim.h"
#include <QMessageBox>
#include <QDate>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

odunc_teslim::odunc_teslim(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::odunc_teslim)
{
    ui->setupUi(this);
    database = QSqlDatabase::addDatabase("QSQLITE");
    //database.setDatabaseName("/Users/ceydaunal/Desktop/veritabanı/kutuphaneOtomasyon.db");
    // Ben ustteki sekilde calistirabildim. ama resource dosyamda kutuphaneOtomasyon.db mevcut bu sekilde calistirabilrisiniz.
    database.setDatabaseName(":/new/prefix1/kutuphaneOtomasyon.db");

    if (!database.open()) {
        QMessageBox::critical(this, "Hata", "Veritabanı bağlantısı başarısız: " + database.lastError().text(), "OK");
    }
    sorgu = new QSqlQuery(database);

    model_odunc_alinan = new QSqlQueryModel();
    model_odunc_teslim_edilen = new QSqlQueryModel();

    listele();

    connect(ui->pushButton_odunc_ver, &QPushButton::clicked, this, &odunc_teslim::on_pushButton_odunc_ver_clicked);
}

odunc_teslim::~odunc_teslim()
{
    delete ui;
}

void odunc_teslim::listele()
{
    sorgu->prepare("SELECT * FROM odunc_alinan");
    if(!sorgu->exec()){
        QMessageBox::critical(this, "Hata", "Ödünç alınan kitaplar listelenemedi: " + sorgu->lastError().text(), "OK");
        return;
    }
    model_odunc_alinan->setQuery(*sorgu);
    ui->tableView_odunc_alinanlar->setModel(model_odunc_alinan);

    sorgu->prepare("SELECT * FROM odunc_teslim_edilen");
    if(!sorgu->exec()){
        QMessageBox::critical(this, "Hata", "Teslim edilen kitaplar listelenemedi: " + sorgu->lastError().text(), "OK");
        return;
    }
    model_odunc_teslim_edilen->setQuery(*sorgu);
    ui->tableView_teslim_edilenler->setModel(model_odunc_teslim_edilen);
}

void odunc_teslim::on_pushButton_odunc_ver_clicked()
{
    QModelIndexList selectedIndexes = ui->tableView_odunc_alinanlar->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen teslim edilecek bir ödünç kaydı seçin.", "OK");
        return;
    }
    int row = selectedIndexes.at(0).row();
    QString kitapNo = ui->tableView_odunc_alinanlar->model()->data(ui->tableView_odunc_alinanlar->model()->index(row, 0)).toString();
    QString uyeNo = ui->tableView_odunc_alinanlar->model()->data(ui->tableView_odunc_alinanlar->model()->index(row, 1)).toString();
    QString almaTarihi = ui->tableView_odunc_alinanlar->model()->data(ui->tableView_odunc_alinanlar->model()->index(row, 2)).toString();
    QString vermeTarihi = QDate::currentDate().toString("dd-MM-yyyy");

    QDate almaDate = QDate::fromString(almaTarihi, "dd-MM-yyyy");
    QDate vermeDate = QDate::fromString(vermeTarihi, "dd-MM-yyyy");

    int daysLate = almaDate.daysTo(vermeDate) - 15;
    int borc = daysLate * 2 ;

    sorgu->prepare("INSERT INTO odunc_teslim_edilen (kitap_no, uye_no, alma_tarihi, verme_tarihi, borc) VALUES (:kitapNo, :uyeNo, :almaTarihi, :vermeTarihi, :borc)");
    sorgu->bindValue(":kitapNo", kitapNo);
    sorgu->bindValue(":uyeNo", uyeNo);
    sorgu->bindValue(":almaTarihi", almaTarihi);
    sorgu->bindValue(":vermeTarihi", vermeTarihi);
    sorgu->bindValue(":borc", borc);
    if(!sorgu->exec()){
        QMessageBox::critical(this, "Hata", "Kitap teslim edilemedi: " + sorgu->lastError().text(), "OK");
        return;
    }

    sorgu->prepare("DELETE FROM odunc_alinan WHERE kitap_no = :kitapNo AND uye_no = :uyeNo");
    sorgu->bindValue(":kitapNo", kitapNo);
    sorgu->bindValue(":uyeNo", uyeNo);
    if(!sorgu->exec()){
        QMessageBox::critical(this, "Hata", "Ödünç alınan kayıt silinemedi: " + sorgu->lastError().text(), "OK");
        return;
    }

    listele();
    QMessageBox::information(this, "Başarılı", "Kitap teslim edildi.", "OK");
}
