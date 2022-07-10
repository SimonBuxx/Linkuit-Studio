#include "Gui/MainWindow.h"
#include "Configuration.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QFile>
#include <QFontDatabase>
#include <QCommandLineParser>

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
    QCoreApplication::setApplicationName("Linkuit Studio");
    QCoreApplication::setApplicationVersion(SW_VERSION_STRING);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    QCommandLineParser parser;
    parser.setApplicationDescription("Linkuit Studio");
    parser.addPositionalArgument("file", QCoreApplication::translate("file", "Circuit file to open."));

    parser.process(app);

    QApplication::setStyle("fusion");

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

    if (parser.positionalArguments().size() > 0)
    {
        if (window.GetCoreLogic().LoadJson(parser.positionalArguments().at(0).mid(1, parser.positionalArguments().at(0).size() - 2)))
        {
            window.setWindowTitle(QString("Linkuit Studio - %0").arg(QFileInfo(window.GetCoreLogic().GetFilePath().value()).fileName()));
        }
        else
        {
            qDebug() << "Could not open file";
        }
    }

    return app.exec();
}
