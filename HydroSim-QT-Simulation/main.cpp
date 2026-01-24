#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend.h"

int main(int argc, char *argv[])
{
    // Create the QML application
    QGuiApplication app(argc, argv);

    // Instantiate the backend object
    Backend backend;

    // Setup the QML engine
    QQmlApplicationEngine engine;

    // Expose the backend object to QML as "backend"
    engine.rootContext()->setContextProperty("backend", &backend);

    // Load the main QML file
    engine.load(QUrl(QStringLiteral("qrc:/qml/qml/Main.qml")));

    // Check if the QML loaded correctly
    if (engine.rootObjects().isEmpty())
        return -1;

    // Run the application event loop
    return app.exec();
}
