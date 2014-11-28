#include "mostrador.h"
#include "ui_mostrador.h"
#include <QString>
#include <QByteArray>
#include <QChar>

Mostrador::Mostrador(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mostrador)
{
    ui->setupUi(this);
    //this->progreso=0;
    this->gestor=new GestorHID();
    connect(gestor,
            SIGNAL(datos_dispositivo_HID(wchar_t*,wchar_t*,wchar_t*,wchar_t*)),
            this,
            SLOT(actualizar_datos(wchar_t*,wchar_t*,wchar_t*,wchar_t*)));
    connect(this->gestor,
            SIGNAL(carga_realizada_HID(int)),
            this,
            SLOT(on_barraProgreso_valueChanged(int)));
    this->gestor->conectar();
}

Mostrador::~Mostrador()
{
    disconnect(gestor,
            SIGNAL(datos_dispositivo_HID(wchar_t*,wchar_t*,wchar_t*,wchar_t*)),
            this,
            SLOT(actualizar_datos(wchar_t*,wchar_t*,wchar_t*,wchar_t*)));
    delete this->gestor;
    delete this->ui;
}
void Mostrador::actualizar_datos(wchar_t* fabricante, wchar_t* producto, wchar_t* nro_serial, wchar_t* indice){
//    QString auxiliar;
//    auxiliar.toWCharArray(fabricante);
//    ui->etiqFabricante->setText(auxiliar);
//    auxiliar.toWCharArray(producto);
//    ui->etiqProducto->setText(auxiliar);
//    auxiliar.toWCharArray(nro_serial);
//    ui->etiqNroSerie->setText(auxiliar);
//    auxiliar.toWCharArray(indice);
//    ui->etiqIndice->setText(auxiliar);

    ui->etiqFabricante->setText(QString::fromWCharArray(fabricante));
    ui->etiqProducto->setText(QString::fromWCharArray(producto));
    ui->etiqNroSerie->setText(QString("(")+
                              QString::number(QString::fromWCharArray(nro_serial).at(0).toLatin1())+
                              QString(")")+
                              QString::fromWCharArray(nro_serial));
    ui->etiqIndice->setText(QString::fromWCharArray(indice));
}



void Mostrador::on_botonEnvio_clicked()
{
    this->gestor->enviarFrase(ui->editorLineaEnvio->text());
    ui->editorLineaRecibida->setText(this->gestor->recibirFrase());
}

void Mostrador::on_botonLeerFlash_clicked()
{
    this->gestor->enviarArchivo("/home/steve07-ultra/workspace_Qt/muestraHIDinfo/certificado.txt");
}

void Mostrador::on_barraProgreso_valueChanged(int progreso)
{
    this->ui->barraProgreso->setValue(progreso);
}
