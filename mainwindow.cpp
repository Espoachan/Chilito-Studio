#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    canvas = new Canvas(this);
    setCentralWidget(canvas);

    // Crear acciones para guardar y cargar proyectos
    QAction *saveAction = new QAction("Guardar Proyecto", this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveProject);

    QAction *loadAction = new QAction("Cargar Proyecto", this);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadProject);

    // Crear menú de proyectos
    QMenu *projectMenu = menuBar()->addMenu("Proyecto");
    projectMenu->addAction(saveAction);
    projectMenu->addAction(loadAction);
}

MainWindow::~MainWindow() {
    // Liberar recursos si es necesario
}

void MainWindow::saveProject(const QString &filePath) {
    QJsonObject projectObj;

    // Guardar información básica del proyecto
    projectObj["width"] = canvas->width();
    projectObj["height"] = canvas->height();

    // Guardar información de las capas
    QJsonArray layersArray;
    for (const auto &layer : layers) {
        QJsonObject layerObj;
        layerObj["name"] = layer->getName();
        layerObj["opacity"] = layer->opacity();
        // Agrega más propiedades según sea necesario
        layersArray.append(layerObj);
    }
    projectObj["layers"] = layersArray;

    // Guardar el JSON en un archivo con la extensión .csp
    QFile saveFile(filePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("No se pudo abrir el archivo para guardar");
        return;
    }

    QJsonDocument saveDoc(projectObj);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}

void MainWindow::loadProject(const QString &filePath) {
    QFile loadFile(filePath);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("No se pudo abrir el archivo para cargar");
        return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject projectObj = loadDoc.object();

    // Recuperar información básica del proyecto
    int width = projectObj["width"].toInt();
    int height = projectObj["height"].toInt();
    canvas->setFixedSize(width, height);

    // Recuperar información de las capas
    QJsonArray layersArray = projectObj["layers"].toArray();
    layers.clear(); // Limpiar capas existentes
    for (const auto &layerVal : layersArray) {
        QJsonObject layerObj = layerVal.toObject();
        Layer *layer = new Layer(QSize(width, height));
        layer->setOpacity(layerObj["opacity"].toDouble());
        // Agrega más propiedades según sea necesario
        layers.append(layer);
    }
}
