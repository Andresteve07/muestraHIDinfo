#ifndef MOSTRADOR_H
#define MOSTRADOR_H

#include <QWidget>
#include "gestorhid.h"

namespace Ui {
class Mostrador;
}

class Mostrador : public QWidget
{
    Q_OBJECT

public:
    explicit Mostrador(QWidget *parent = 0);
    ~Mostrador();

public slots:
    void actualizar_datos(wchar_t* fabricante, wchar_t* producto, wchar_t* nro_serial, wchar_t* indice);

private slots:
    void on_botonEnvio_clicked();

    void on_botonLeerFlash_clicked();

    void on_barraProgreso_valueChanged(int value);

private:
    Ui::Mostrador *ui;
    GestorHID *gestor;
    //int progreso;
};

#endif // MOSTRADOR_H
