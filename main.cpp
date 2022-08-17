#include "Gui/MainWindow.h"
#include "Configuration.h"
#include "RuntimeConfigParser.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QFile>
#include <QFileInfo>
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

    QString path = "";

    // Reassemble path from possibly multiple parts, split at the spaces
    for (const auto& arg : parser.positionalArguments())
    {
        path.push_back(arg);
        path.push_back(' ');
    }

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

    if (parser.positionalArguments().size() > 0)
    {
        window.GetCoreLogic().GetCircuitFileParser().LoadJson(QFileInfo(path.mid(1, path.size() - 3)));
    }
    else
    {
        if (window.GetCoreLogic().GetRuntimeConfigParser().IsWelcomeDialogEnabledOnStartup())
        {
            window.ShowWelcomeDialog(gui::SHOW_WELCOME_DIALOG_DELAY);
        }
    }

    window.showMaximized();

    app.processEvents();
    window.GetView().ResetViewport(); // Reset the viewport AFTER the scrollbars have their final size

    return app.exec();
}
