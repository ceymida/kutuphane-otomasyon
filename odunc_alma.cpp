#include "odunc_alma.h"
#include "ui_odunc_alma.h"

odunc_alma::odunc_alma(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::odunc_alma)
{
    ui->setupUi(this);
    ui->setupUi(this);
    // veri tabanı baglantisi
    //database.setDatabaseName("/Users/ceydaunal/Desktop/veritabanı/kutuphaneOtomasyon.db");
    database.setDatabaseName(":/new/prefix1/kutuphaneOtomasyon.db");
    // Ben ustteki sekilde calistirabildim. ama resource dosyamda kutuphaneOtomasyon.db mevcut bu sekilde calistirabilrisiniz.
    connect(ui->pushButton_odunc_al, &QPushButton::clicked, this, &odunc_alma::odunc_al);


    if (!database.open()) {
        QMessageBox::critical(this, "Hata", "Veritabanı bağlantısı başarısız: " + database.lastError().text(), "OK");
    }
    sorgu = new QSqlQuery(database);
    //QSqlQuery sorgu;
    listele();

}

odunc_alma::~odunc_alma()
{
    delete ui;
}

void odunc_alma::listele()
{
    // uyelerin listelenmesi
    sorgu->prepare("select * from uye");
    if(!sorgu->exec()){

        QMessageBox::critical(this,"hata",sorgu->lastError().text(),"ok");
    }
    QSqlQueryModel *model1 = new QSqlQueryModel();
    model1 = new QSqlQueryModel();
    model1->setQuery(*sorgu);
    ui->tableView_tum_uyeler->setModel(model1);
    // kitapların listelenmesi
    sorgu->prepare("select * from kitap");
    if(!sorgu->exec()){

        QMessageBox::critical(this,"hata",sorgu->lastError().text(),"ok");
    }
    QSqlQueryModel *model2 = new QSqlQueryModel();
    model2 = new QSqlQueryModel();
    model2->setQuery(*sorgu);
    ui->tableView_tum_kitaplar->setModel(model2);


    // odunc alanlarin
    sorgu->prepare("select * from odunc_alinan");
    if(!sorgu->exec()){

        QMessageBox::critical(this,"hata",sorgu->lastError().text(),"ok");
    }
    QSqlQueryModel *model3 = new QSqlQueryModel();
    model3 = new QSqlQueryModel();
    model3->setQuery(*sorgu);
    ui->tableView_odunc_alinanlar->setModel(model3);
}


void odunc_alma::odunc_al()
{

    //QModelIndexList selectedUserIndexes = ui->tableView_tum_uyeler->selectionModel()->selectedIndexes();
   // QModelIndexList selectedBookIndexes = ui->tableView_tum_kitaplar->selectionModel()->selectedIndexes();

    if (ui->lineEdit_uye_no->text().isEmpty() || ui->lineEdit_kitap_no->text().isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen bir üye ve bir kitap seçin.", "OK");
        return;
    }

    int userId = ui->lineEdit_uye_no->text().toInt();
    int bookId = ui->lineEdit_kitap_no->text().toInt();
    QString currentDate = QDate::currentDate().toString("dd-MM-yyyy");

    // Seçilen kitabın stoğunu kontrol et
    sorgu->prepare("SELECT kitap_sayisi FROM kitap WHERE kitap_no = :bookId");
    sorgu->bindValue(":bookId", bookId);
    if (!sorgu->exec()) {
        QMessageBox::critical(this, "Hata", "Kitap stoğu kontrol edilemedi: " + sorgu->lastError().text(), "OK");
        return;
    }
    if (!sorgu->next()) {
        QMessageBox::critical(this, "Hata", "Kitap bulunamadı.", "OK");
        return;
    }
    int stock = sorgu->value(0).toInt();
    if (stock <= 0) {
        QMessageBox::warning(this, "Uyarı", "Seçilen kitap stokta bulunmamaktadır.", "OK");
        return;
    }

    // Seçilen üyenin daha önce aynı kitabı almış olup olmadığını kontrol et
    sorgu->prepare("SELECT COUNT(*) FROM odunc_alinan WHERE uye_no = :userId AND kitap_no = :bookId AND odunc_alma_tarihi IS NULL");
    sorgu->bindValue(":userId", userId);
    sorgu->bindValue(":bookId", bookId);
    if (!sorgu->exec()) {
        QMessageBox::critical(this, "Hata", "Ödünç alınan kitap kontrol edilemedi: " + sorgu->lastError().text(), "OK");
        return;
    }
    sorgu->next();
    int count = sorgu->value(0).toInt();
    if (count > 0) {
        QMessageBox::warning(this, "Uyarı", "Bu üye zaten seçilen kitabı almış durumda.", "OK");
        return;
    }

    // Ödünç alma işlemini gerçekleştir
    sorgu->prepare("INSERT INTO odunc_alinan (uye_no, kitap_no, odunc_alma_tarihi) VALUES (:userId, :bookId, :currentDate)");
    sorgu->bindValue(":userId", userId);
    sorgu->bindValue(":bookId", bookId);
    sorgu->bindValue(":currentDate", currentDate);
    if (!sorgu->exec()) {
        QMessageBox::critical(this, "Hata", "Ödünç alma işlemi gerçekleştirilemedi: " + sorgu->lastError().text(), "OK");
        return;
    }
    // Kitabın stoğunu güncelle
    sorgu->prepare("UPDATE kitap SET kitap_sayisi = kitap_sayisi - 1 WHERE kitap_no = :bookId");
    sorgu->bindValue(":bookId", bookId);
    if (!sorgu->exec()) {
        QMessageBox::critical(this, "Hata", "Kitap stoğu güncellenemedi: " + sorgu->lastError().text(), "OK");
        return;
    }

    QMessageBox::information(this, "Bilgi", "Kitap başarıyla ödünç alındı.", "OK");
    listele(); // Tabloyu güncelle




}

