#ifndef GESTORHID_H
#define GESTORHID_H

#include "HIDAPI/hidapi.h"

#include <QObject>

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 255

class GestorHID : public QObject
{
    Q_OBJECT

public:
    explicit GestorHID(QObject *parent = 0);
    ~GestorHID();
    void conectar();
    void desconectar();
    void enviarFrase(QString frase);
    QString recibirFrase();
    int enviarArchivo(QString nombreArchivo);


signals:
    void conec_establecida_HID(bool estaConectado);
    void datos_dispositivo_HID(wchar_t* fabricante, wchar_t* producto, wchar_t* nro_serial, wchar_t* indice);
    void carga_realizada_HID(int progreso);

//public slots:
    //void toggle_leds();
    //void PollUSB();

private:
    bool estaConectado;

    hid_device *dispositivo;

    int res;
    unsigned char buf[61];// 1 extra byte for the report ID
    wchar_t fabricante[MAX_STR];
    wchar_t producto[MAX_STR];
    wchar_t nro_serial[MAX_STR];
    wchar_t indice[MAX_STR];

    void cerrarDispositivo();
};

#endif // GESTORHID_H
