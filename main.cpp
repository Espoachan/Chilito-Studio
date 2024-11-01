#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MiVentana ventana;
    ventana.setWindowTitle("Chilito Studio");
    ventana.resize(800, 600); // Tamaño de la ventana
    ventana.show();

    return app.exec();
}

