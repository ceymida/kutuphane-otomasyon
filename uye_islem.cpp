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
