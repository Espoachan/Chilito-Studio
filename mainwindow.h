#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QDockWidget>
#include <QListWidget>
#include <QColorDialog>
#include <QSpinBox>
#include <QMouseEvent>
#include <QPainter>
#include "layer.h"

class Canvas : public QGraphicsView {
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr) : QGraphicsView(parent), currentLayer(nullptr), eraserMode(false) {
        setScene(new QGraphicsScene(this));
        setRenderHint(QPainter::Antialiasing);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setBackgroundBrush(QBrush(Qt::darkGray));
    }

    void addLayer() {
        Layer *layer = new Layer(size());
        layer->setZValue(layers.size());  // Asegura que cada capa esté en una posición única
        scene()->addItem(layer);
        layers.append(layer);
        setCurrentLayer(layer);
    }

    void setCurrentLayer(Layer *layer) {
        if (currentLayer) currentLayer->setVisible(false);  // Oculta la capa anterior
        currentLayer = layer;
        currentLayer->setVisible(true);  // Muestra la capa seleccionada
    }

    void selectLayer(int index) {
        if (index >= 0 && index < layers.size()) {
            setCurrentLayer(layers[index]);
        }
    }

    void setPenColor(const QColor &color) {
        pen.setColor(color);
        eraserMode = false;
    }

    void setBrushSize(int size) {
        pen.setWidth(size);
    }

    void enableEraser() {
        eraserMode = true;
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (currentLayer) {
            lastPoint = mapToScene(event->pos());
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (currentLayer && event->buttons() & Qt::LeftButton) {
            QPointF newPoint = mapToScene(event->pos());
            if (eraserMode) {
                currentLayer->eraseLine(lastPoint, newPoint, pen.width());  // Modo borrador
            } else {
                currentLayer->drawLine(lastPoint, newPoint, pen);  // Modo dibujo
            }
            lastPoint = newPoint;
        }
    }

private:
    QList<Layer*> layers;
    Layer *currentLayer;
    QPointF lastPoint;
    QPen pen;
    bool eraserMode;  // Indica si estamos en modo borrador o no
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setupUI();
        applyDarkTheme();
    }

private:
    Canvas *canvas;
    QDockWidget *timelineDock;
    QDockWidget *toolsDock;
    QListWidget *layersList;
    QToolBar *mainToolBar;
    QColor currentColor = Qt::black;

    void setupUI() {
        canvas = new Canvas(this);
        setCentralWidget(canvas);

        createToolbar();
        createTimelinePanel();
        createToolsPanel();
        canvas->addLayer();

        resize(1200, 800);
    }

    void createToolbar() {
        mainToolBar = addToolBar(tr("Main Tools"));
        mainToolBar->setMovable(false);

        QAction *newFrameAction = mainToolBar->addAction("Nuevo Frame");
        QAction *penAction = mainToolBar->addAction("Lápiz");
        QAction *eraserAction = mainToolBar->addAction("Borrador");
        QAction *colorAction = mainToolBar->addAction("Color");

        connect(newFrameAction, &QAction::triggered, this, &MainWindow::addNewLayer);
        connect(penAction, &QAction::triggered, this, [=]() { canvas->setPenColor(currentColor); });
        connect(eraserAction, &QAction::triggered, this, &MainWindow::activateEraser);
        connect(colorAction, &QAction::triggered, this, &MainWindow::showColorDialog);
    }

    void createTimelinePanel() {
        timelineDock = new QDockWidget(tr("Timeline"), this);
        timelineDock->setAllowedAreas(Qt::BottomDockWidgetArea);

        QWidget *timelineWidget = new QWidget(timelineDock);
        QVBoxLayout *timelineLayout = new QVBoxLayout(timelineWidget);

        layersList = new QListWidget(timelineWidget);
        connect(layersList, &QListWidget::currentRowChanged, this, &MainWindow::onLayerSelected);

        timelineLayout->addWidget(layersList);
        timelineDock->setWidget(timelineWidget);
        addDockWidget(Qt::BottomDockWidgetArea, timelineDock);
    }

    void createToolsPanel() {
        toolsDock = new QDockWidget(tr("Herramientas"), this);
        toolsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

        QWidget *toolsWidget = new QWidget(toolsDock);
        QVBoxLayout *toolsLayout = new QVBoxLayout(toolsWidget);

        QSpinBox *brushSize = new QSpinBox(toolsWidget);
        brushSize->setRange(1, 100);
        brushSize->setValue(5);
        connect(brushSize, QOverload<int>::of(&QSpinBox::valueChanged), canvas, &Canvas::setBrushSize);
        toolsLayout->addWidget(brushSize);

        toolsDock->setWidget(toolsWidget);
        addDockWidget(Qt::LeftDockWidgetArea, toolsDock);
    }

    void applyDarkTheme() {
        setStyleSheet(R"(
            QMainWindow {
                background-color: #2b2b2b;
                color: #ffffff;
            }
            QDockWidget {
                background-color: #2b2b2b;
                color: #ffffff;
                border: 1px solid #3f3f3f;
            }
            QDockWidget::title {
                background-color: #3f3f3f;
                padding: 5px;
            }
            QToolBar {
                background-color: #3f3f3f;
                border: none;
            }
            QToolBar QToolButton {
                background-color: #3f3f3f;
                color: #ffffff;
                border: none;
                padding: 5px;
            }
            QToolBar QToolButton:hover {
                background-color: #4f4f4f;
            }
            QListWidget {
                background-color: #2b2b2b;
                color: #ffffff;
                border: 1px solid #3f3f3f;
            }
            QSpinBox {
                background-color: #3f3f3f;
                color: #ffffff;
                border: 1px solid #4f4f4f;
            }
        )");
    }

    void addNewLayer() {
        canvas->addLayer();
        layersList->addItem("Layer " + QString::number(layersList->count() + 1));
    }

    void onLayerSelected(int index) {
        canvas->selectLayer(index);
    }

    void activateEraser() {
        canvas->enableEraser();
    }

    void showColorDialog() {
        QColor color = QColorDialog::getColor(currentColor, this, "Selecciona un color");
        if (color.isValid()) {
            currentColor = color;
            canvas->setPenColor(currentColor);
        }
    }
};

#endif // MAINWINDOW_H
