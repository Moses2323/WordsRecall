#include <src/gui/mainwindow.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>
#include <src/core/dict_parser.h>

int my_checking(){
    std::string filename("../../../dictionaries/dict_1.txt");
    std::list<DictWord> words = parse_dict_file(filename);
    for (auto &e : words)
        std::cout << "size(" << e.word.size() << ", " << e.meaning.size() << "): " << e
                  << std::endl;
    return 0;
}

int main(int argc, char *argv[])
{
    try {
        return my_checking();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occured" << std::endl;
    }

    // QApplication a(argc, argv);

    // QTranslator translator;
    // const QStringList uiLanguages = QLocale::system().uiLanguages();
    // for (const QString &locale : uiLanguages) {
    //     const QString baseName = "WordsRecall" + QLocale(locale).name();
    //     if (translator.load(":/i18n/" + baseName)) {
    //         a.installTranslator(&translator);
    //         break;
    //     }
    // }
    // MainWindow w;
    // w.change_label();
    // w.show();

    // return QCoreApplication::exec();
}
