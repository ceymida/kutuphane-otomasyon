// ceyda unal - 22100011055
// gorsel programlama odev2 _ tum cpp dopsyalarimi bir tum dosyasinda kopya kontrolu icin
// birlestirdigim dosya


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uye_islem.h"
#include "ui_uye_islem.h"
#include "kitap_islem.h"
#include "ui_kitap_islem.h"
#include "odunc_alma.h"
#include "ui_odunc_alma.h"
#include "odunc_teslim.h"
#include "ui_odunc_teslim.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    // butonlara resim ekleme
    ui->pushButton->setIcon(QIcon(":/new/prefix1/user.jpg"));
    ui->pushButton_kitap_islem->setIcon(QIcon(":/new/prefix1/book.jpg"));
    ui->pushButton_odunc_alma->setIcon(QIcon(":/new/prefix1/odunc_1.jpg"));
    ui->pushButton_odunc_teslim->setIcon(QIcon(":/new/prefix1/odunc_2.jpg"));
    ui->pushButton->setIconSize(QSize(150, 150));
    ui->pushButton_kitap_islem->setIconSize(QSize(150, 150));
    ui->pushButton_odunc_alma->setIconSize(QSize(150, 150));
    ui->pushButton_odunc_teslim->setIconSize(QSize(150, 150));

    database.setDatabaseName(":/new/prefix1/kutuphaneOtomasyon.db");
    if(!database.open()){
        ui->statusbar->showMessage("baglanilamadi.");

    }else{
        ui->statusbar->showMessage("baglanildi.");
    }
    sorgu = new QSqlQuery(database);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    uye_islem * ekran = new uye_islem();
    ekran ->show();
}


void MainWindow::on_pushButton_kitap_islem_clicked()
{
    kitap_islem * ekran = new kitap_islem();
    ekran->show();
}


void MainWindow::on_pushButton_odunc_alma_clicked()
{
    odunc_alma * ekran = new odunc_alma();
    ekran->show();
}


void MainWindow::on_pushButton_odunc_teslim_clicked()
{
    odunc_teslim * ekran = new odunc_teslim();
    ekran->show();
}

#include "kitap_islem.h"
#include "ui_kitap_islem.h"

kitap_islem::kitap_islem(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::kitap_islem)
{
    ui->setupUi(this);
    // veri tabanı baglantisi
    //database.setDatabaseName("/Users/ceydaunal/Desktop/veritabanı/kutuphaneOtomasyon.db");
    // Ben ustteki sekilde calistirabildim. ama resource dosyamda kutuphaneOtomasyon.db mevcut bu sekilde calistirabilrisiniz.
    database.setDatabaseName(":/new/prefix1/kutuphaneOtomasyon.db");


    if (!database.open()) {
        QMessageBox::critical(this, "Hata", "Veritabanı bağlantısı başarısız: " + database.lastError().text(), "OK");
    }
    sorgu = new QSqlQuery(database);
    //QSqlQuery sorgu;
    listele();
}

kitap_islem::~kitap_islem()
{
    delete ui;
}

void kitap_islem::listele()
{
    sorgu->prepare("select * from kitap");
    if(!sorgu->exec()){

        QMessageBox::critical(this,"hata",sorgu->lastError().text(),"ok");
    }
    QSqlQueryModel *model1 = new QSqlQueryModel();
    model1 = new QSqlQueryModel();
    model1->setQuery(*sorgu);
    ui->tableView->setModel(model1);

}

void kitap_islem::on_pushButton_yeni_kayit_clicked()
{
    if(ui->lineEdit_kitap_no->text().isEmpty() || ui->lineEdit_kitap_ad->text().isEmpty() || ui->lineEdit_kitap_stok->text().isEmpty()) {
        QMessageBox::critical(this, "Hata", "Eksik veri girmeyiniz.", "OK");
        return;
    }


    sorgu->prepare("select * from kitap where kitap_no=?");
    sorgu->addBindValue(ui->lineEdit_kitap_no->text().toInt());
    sorgu->exec();
    if(sorgu->next()){
        QMessageBox::critical(this,"hata","bu kitap nosuna ait baska kitap vardir","ok");
        return;
    }
    else{
        sorgu->prepare("insert into kitap(kitap_no,kitap_ad,kitap_sayisi) values(?,?,?)");
        sorgu->addBindValue(ui->lineEdit_kitap_no->text().toInt());
        sorgu->addBindValue(ui->lineEdit_kitap_ad->text());
        sorgu->addBindValue(ui->lineEdit_kitap_stok->text());
        sorgu->exec();
        QMessageBox::information(this, "Başarılı", "Yeni kitap başarıyla eklendi", "OK");
    }
    int no = ui->lineEdit_kitap_no->text().toInt();
    QString ad = ui->lineEdit_kitap_ad->text();
    QString soyad = ui->lineEdit_kitap_stok->text();
    listele();

}


void kitap_islem::on_pushButton_guncelle_clicked()
{

    if(ui->lineEdit_kitap_no->text().isEmpty() || ui->lineEdit_kitap_ad->text().isEmpty() || ui->lineEdit_kitap_stok->text().isEmpty()) {
        QMessageBox::critical(this, "Hata", "Eksik veri girmeyiniz.", "OK");
        return;
    }

    QSqlQuery sorgu;
    sorgu.prepare("UPDATE kitap SET kitap_ad = ?, kitap_sayisi = ? WHERE kitap_no = ?");
    sorgu.addBindValue(ui->lineEdit_kitap_ad->text());
    sorgu.addBindValue(ui->lineEdit_kitap_stok->text());
    sorgu.addBindValue(ui->lineEdit_kitap_no->text().toInt());
    sorgu.exec();

    QMessageBox::information(this, "Başarılı", "Kitap bilgileri başarıyla güncellendi", "OK");

    updateTableView();
}


void kitap_islem::on_pushButton_sil_clicked()
{
    if(ui->lineEdit_kitap_no->text().isEmpty()) {
        QMessageBox::critical(this, "Hata", "Silinecek üye no'sunu giriniz.", "OK");
        return;
    }

    QSqlQuery sorgu;
    sorgu.prepare("SELECT * FROM odunc WHERE kitap_no = ? AND teslim_tarihi IS NULL");
    sorgu.addBindValue(ui->lineEdit_kitap_no->text().toInt());
    sorgu.exec();

    if (sorgu.next()) {
        QMessageBox::critical(this, "Hata", "  kitap silinemez.", "OK");
    } else {
        sorgu.prepare("DELETE FROM kitap WHERE kitap_no = ?");
        sorgu.addBindValue(ui->lineEdit_kitap_no->text().toInt());
        sorgu.exec();

        QMessageBox::information(this, "Başarılı", "Kitap başarıyla silindi", "OK");

        updateTableView();
    }

}

void kitap_islem::updateTableView()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query("SELECT * FROM kitap");
    model->setQuery(query);
    ui->tableView->setModel(model);
}

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

#include "uye_islem.h"
#include "ui_uye_islem.h"

uye_islem::uye_islem(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::uye_islem)
{
    ui->setupUi(this);
    // veri tabanı baglantisi
    //database.setDatabaseName("/Users/ceydaunal/Desktop/veritabanı/kutuphaneOtomasyon.db");
    // Ben ustteki sekilde calistirabildim. ama resource dosyamda kutuphaneOtomasyon.db mevcut bu sekilde calistirabilrisiniz.
    database.setDatabaseName(":/new/prefix1/kutuphaneOtomasyon.db");

    if (!database.open()) {
        QMessageBox::critical(this, "Hata", "Veritabanı bağlantısı başarısız: " + database.lastError().text(), "OK");
    }
    sorgu = new QSqlQuery(database);
    //QSqlQuery sorgu;
    listele();

}

uye_islem::~uye_islem()
{
    delete ui;
}

void uye_islem::listele()
{
    sorgu->prepare("select * from uye");
    if(!sorgu->exec()){

        QMessageBox::critical(this,"hata",sorgu->lastError().text(),"ok");
    }
    QSqlQueryModel *model1 = new QSqlQueryModel();
    model1 = new QSqlQueryModel();
    model1->setQuery(*sorgu);
    ui->tableView->setModel(model1);
}

void uye_islem::on_pushButton_yeni_kayit_clicked()
{

    if(ui->lineEdit_uye_no->text().isEmpty() || ui->lineEdit_uye_ad->text().isEmpty() || ui->lineEdit_uye_soyad->text().isEmpty()) {
        QMessageBox::critical(this, "Hata", "Eksik veri girmeyiniz.", "OK");
        return;
    }


    sorgu->prepare("select * from uye where uye_no=?");
    sorgu->addBindValue(ui->lineEdit_uye_no->text().toInt());
    sorgu->exec();
    if(sorgu->next()){
        QMessageBox::critical(this,"hata","bu uye nosuna ait baska uye vardir","ok");
        return;
    }
    else{
        sorgu->prepare("insert into uye(uye_no,uye_ad,uye_soyad) values(?,?,?)");
        sorgu->addBindValue(ui->lineEdit_uye_no->text().toInt());
        sorgu->addBindValue(ui->lineEdit_uye_ad->text());
        sorgu->addBindValue(ui->lineEdit_uye_soyad->text());
        sorgu->exec();
        QMessageBox::information(this, "Başarılı", "Yeni üye başarıyla eklendi", "OK");
    }



    /*if(ui->lineEdit_uye_no->text().isEmpty() || ui->lineEdit_uye_ad->text().isEmpty() || ui->lineEdit_uye_soyad->text().isEmpty()) {
        QMessageBox::critical(this, "Hata", "Eksik veri girmeyiniz.", "OK");
        return;
    }*/
    int no = ui->lineEdit_uye_no->text().toInt();
    QString ad = ui->lineEdit_uye_ad->text();
    QString soyad = ui->lineEdit_uye_soyad->text();
    listele();



}
void uye_islem::updateTableView()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query("SELECT * FROM uye");
    model->setQuery(query);
    ui->tableView->setModel(model);
}

void uye_islem::on_pushButton_guncelle_clicked()
{
    if(ui->lineEdit_uye_no->text().isEmpty() || ui->lineEdit_uye_ad->text().isEmpty() || ui->lineEdit_uye_soyad->text().isEmpty()) {
        QMessageBox::critical(this, "Hata", "Eksik veri girmeyiniz.", "OK");
        return;
    }

    QSqlQuery sorgu;
    sorgu.prepare("UPDATE uye SET uye_ad = ?, uye_soyad = ? WHERE uye_no = ?");
    sorgu.addBindValue(ui->lineEdit_uye_ad->text());
    sorgu.addBindValue(ui->lineEdit_uye_soyad->text());
    sorgu.addBindValue(ui->lineEdit_uye_no->text().toInt());
    sorgu.exec();

    QMessageBox::information(this, "Başarılı", "Üye bilgileri başarıyla güncellendi", "OK");

    updateTableView();
}

void uye_islem::on_pushButton_sil_clicked()
{
    if(ui->lineEdit_uye_no->text().isEmpty()) {
        QMessageBox::critical(this, "Hata", "Silinecek üye no'sunu giriniz.", "OK");
        return;
    }

    QSqlQuery sorgu;
    sorgu.prepare("SELECT * FROM odunc WHERE uye_no = ? AND teslim_tarihi IS NULL");
    sorgu.addBindValue(ui->lineEdit_uye_no->text().toInt());
    sorgu.exec();

    if (sorgu.next()) {
        QMessageBox::critical(this, "Hata", "Üyenin teslim etmediği kitap(lar) var, üye silinemez.", "OK");
    } else {
        sorgu.prepare("DELETE FROM uye WHERE uye_no = ?");
        sorgu.addBindValue(ui->lineEdit_uye_no->text().toInt());
        sorgu.exec();

        QMessageBox::information(this, "Başarılı", "Üye başarıyla silindi", "OK");

        updateTableView();
    }
}

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

