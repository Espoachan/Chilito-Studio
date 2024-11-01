#ifndef LIENZO_H
#define LIENZO_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPen>
#include <QBrush>

class Lienzo : public QWidget {
    Q_OBJECT

public:
    Lienzo(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(800, 600); // Tamaño del lienzo
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        // Aquí dibujaremos lo que se necesita
        painter.setRenderHint(QPainter::Antialiasing); // Para suavizar bordes
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            // Comenzar a dibujar al hacer clic
            dibujar(event->pos());
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (event->buttons() & Qt::LeftButton) {
            // Seguir dibujando mientras se mueve el ratón
            dibujar(event->pos());
        }
    }

private:
    void dibujar(const QPoint &pos) {
        // Método que se llamará para dibujar en el lienzo
        // Aquí se puede implementar la lógica de dibujo
    }
};


#endif // LIENZO_H
