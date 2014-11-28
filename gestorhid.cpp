#include "gestorhid.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

GestorHID::GestorHID(QObject *parent): QObject(parent) {
    estaConectado = false;

    this->fabricante[0] = 0x0000;
    this->producto[0] = 0x0000;
    this->nro_serial[0] = 0x0000;
    this->indice[0] = 0x0000;

    dispositivo = NULL;

    memset(buf,0x00,sizeof(buf));
    buf[0] = 0x01;
    buf[1] = 0x81;

    //connect(timer, SIGNAL(timeout()), this, SLOT(PollUSB()));
    //hid_set_nonblocking(this->dispositivo, 1);
}
GestorHID::~GestorHID(){
    this->desconectar();
}
void GestorHID::conectar(){
    int res=0;
    dispositivo = hid_open(0x1234, 0x6, NULL);
    if (!dispositivo) {
        qDebug("unable to open device\n");
        this->estaConectado = false;
        //return 1;
    }
    else
        this->estaConectado = true;

    this->conec_establecida_HID(this->estaConectado);

    // Read the Manufacturer String

    res = hid_get_manufacturer_string(dispositivo, this->fabricante, MAX_STR);
    if (res < 0)
        printf("Unable to read manufacturer string\n");
    printf("Manufacturer String: %ls\n", this->fabricante);

    // Read the Product String

    res = hid_get_product_string(dispositivo, this->producto, MAX_STR);
    if (res < 0)
        printf("Unable to read product string\n");
    printf("Product String: %ls\n", this->producto);

    // Read the Serial Number String

    res = hid_get_serial_number_string(dispositivo, this->nro_serial, MAX_STR);
    if (res < 0)
        printf("Unable to read serial number string\n");
    printf("Serial Number String: (%d) %ls", this->nro_serial[0], this->nro_serial);
    printf("\n");

    // Read Indexed String 1

    res = hid_get_indexed_string(dispositivo, 1, this->indice, MAX_STR);
    if (res < 0)
        printf("Unable to read indexed string 1\n");
    printf("Indexed String 1: %ls\n", this->indice);

    this->datos_dispositivo_HID(this->fabricante,this->producto,this->nro_serial,this->indice);

    return;
}

void GestorHID::desconectar(){
    //close HID device
    hid_close(this->dispositivo);

    /* Free static HIDAPI objects. */
    hid_exit();
}

void GestorHID::enviarFrase(QString frase){
    //qDebug()<<frase;
    //frase.insert(0,QChar('\0'));
    if(frase.size()>=MAX_STR-1){
        qDebug("Se desea enviar una frase demasiado larga (>250chars) por favor reconsidere esta atrocidad...\n");
        return;
    }

    memset(buf, 0, sizeof(buf));

    QByteArray arreglo = frase.toLocal8Bit();
    //const unsigned char* contenedor = reinterpret_cast<const unsigned char*> (arreglo.data());
    //char* contenedor = (arreglo.data());
    buf[0]=0x0;
    //qDebug() << toDebug(QByteArray(arreglo));
    //printf("tamanio %i.\n",arreglo.size());
    for(int i = 0;i<arreglo.size();i++){
        buf[i+1]=arreglo[i];
        //printf("_%c_",buf[i+1]);
    }

    //printf("tamanio del buffer%i.\n",sizeof(buf));
    res = hid_write(this->dispositivo, buf, sizeof(buf));
    if (res < 0) {
        printf("No pude escribirle al token\n");
        printf("Error: %ls\n", hid_error(this->dispositivo));
    }

    usleep(500*1000);

//    printf("Se envio:%i\n",sizeof(buf));
//    for (unsigned int i = 0; i < sizeof(buf); i++)
//        printf("_%c_", buf[i]);
//    printf("\n");
}
QString GestorHID::recibirFrase(){
    res = 0;
    memset(buf, 0, sizeof(buf));
    res = hid_read(this->dispositivo, buf, sizeof(buf));
    if (res < 0){
      printf("No pude leer ningun mensaje del token\n");
      return QString("");
    }
//    printf("Se recibio:%i\n",res);
//    for (int i = 0; i < res; i++)
//        printf("_%c_", buf[i]);
//    printf("\n");
    return QString::fromLocal8Bit(reinterpret_cast<const char*> (buf));
}

int GestorHID::enviarArchivo(QString nombreArchivo){
    QFile archivo(nombreArchivo);//"/home/alex/1.txt"
    if(!archivo.open(QIODevice::ReadOnly | QIODevice::Text))
       qDebug("No pude abrir el archivo");
    QTextStream flujoDatos(&archivo);
    flujoDatos.setCodec("UTF-8"); // change the archivo codec to UTF-8.
    //QFile archivoSalida("/home/steve07-ultra/workspace_Qt/muestraHIDinfo/salida.txt");
    //if(!archivoSalida.open(QIODevice::WriteOnly | QIODevice::Text))
    //    qDebug("No pude abrir el archivo de salida");
    //QTextStream flujoSalida(&archivoSalida);
    int i = 0;
    int tamanioArchivo = archivo.size();
    double progreso = 0;
    qDebug()<<QString::number(tamanioArchivo);
       while(!flujoDatos.atEnd())
       {
           QString cadena = flujoDatos.read(60);
           //QString line = flujoDatos.readLine();
           //flujoSalida<<cadena;
           this->enviarFrase(cadena);
           i++;
           progreso = (double)((i*60+cadena.size())*100)/tamanioArchivo;
           qDebug()<<QString::number(progreso);
           this->carga_realizada_HID(progreso);
       }
       this->enviarFrase(QString("FIN"));
       //printf("___%i___\n",i);
       //archivoSalida.close();
       return 0;
}
