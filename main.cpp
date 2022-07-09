#include "Gui/MainWindow.h"
#include "Configuration.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QFile>
#include <QFontDatabase>

std::optional<QString> LoadStylesheet(const QString &pPath)
{
    QFile file(pPath);
    if (file.open(QFile::ReadOnly))
    {
        return QString(file.readAll());
    }
    else
    {
        return std::nullopt;
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    QFontDatabase::addApplicationFont(":/fonts/Quicksand-Medium.ttf");

    const auto&& stylesheet = LoadStylesheet(":/styles/style.css");

    if (stylesheet.has_value())
    {
        app.setStyleSheet(stylesheet.value());
    }
    else
    {
        qFatal("Could not open stylesheet");
        return -1;
    }

    MainWindow window;
    window.showMaximized();

    return app.exec();
}
