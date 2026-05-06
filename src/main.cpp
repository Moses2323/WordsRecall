#include <src/gui/mainwindow.h>

#include <QApplication>
#include <QDebug>
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>
#include <iostream>
#include <src/core/dict_parser.h>

// proper dict file
const char *_DICT_FILENAME = "../../../dictionaries/dict_1.txt";
// for simple test based on just english words
// const char *_DICT_FILENAME = "../../../dictionaries/dict_eng.txt";

int my_checking(int argc, char *argv[])
{
    std::string filename(_DICT_FILENAME);
    std::list<DictWord> words = parse_dict_file(filename);
    for (auto &e : words)
        std::cout << "size(" << e.word.size() << ", " << e.meaning.size() << "): " << e
                  << std::endl;

    return 0;
}

int main(int argc, char *argv[])
{
    // -------------------- local check -----------
    // try {
    //     return my_checking(argc, argv);
    // } catch (std::exception &e) {
    //     std::cerr << e.what() << std::endl;
    // } catch (...) {
    //     std::cerr << "Unknown exception occured" << std::endl;
    // }

    // -------------------- GUI -------------------

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "WordsRecall" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    std::string filename(_DICT_FILENAME);
    std::list<DictWord> words = parse_dict_file(filename);

    // for checking the words
    std::stringstream vss;
    for (auto &e : words)
        vss << e << "\n";
    w.change_label(vss.str());

    w.show();
    return QCoreApplication::exec();
}
