#ifndef LAYER_H
#define LAYER_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QPainter>

class Layer : public QGraphicsItem {
public:
    Layer(const QSize &size) {
        pixmap = QPixmap(size);
        pixmap.fill(Qt::transparent);
    }

    QRectF boundingRect() const override {
        return QRectF(0, 0, pixmap.width(), pixmap.height());
    }

    // Dibuja una línea con el color y grosor del lápiz
    void drawLine(const QPointF &start, const QPointF &end, const QPen &pen) {
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(pen);
        painter.drawLine(start, end);
        update();
    }

    // Borra una línea con un pincel transparente en el grosor especificado
    void eraseLine(const QPointF &start, const QPointF &end, int brushSize) {
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);  // Modo de borrador
        QPen eraserPen(Qt::transparent, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(eraserPen);
        painter.drawLine(start, end);
        update();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->drawPixmap(0, 0, pixmap);
    }

private:
    QPixmap pixmap;
};

#endif // LAYER_H
