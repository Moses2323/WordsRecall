#include <src/gui/debug_window.h>
#include <src/gui/wrmainwindow.h>

#include <QApplication>
#include <QDebug>
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>
#include <iostream>
#include <memory>
#include <src/core/dict_parser.h>

constexpr bool _DEBUG_DICT = false;

// proper dict file
const char *_DICT_FILENAME = "./dictionaries/dict_1.txt";
// for simple test based on just english words
// const char *_DICT_FILENAME = "./dictionaries/dict_eng.txt";

int my_checking(int /*argc*/, char ** /*argv*/)
{
    std::string filename(_DICT_FILENAME);
    std::vector<DictWord> words = wr::parse_dict_file(filename);
    for (auto &e : words)
        std::cout << "size(" << e.word.size() << ", " << e.meaning.size() << "): " << e
                  << std::endl;

    return 0;
}

int main(int argc, char **argv)
{
    try {
        //return my_checking(argc, argv);

        QApplication a(argc, argv);

        // translator
        QTranslator translator;
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale : uiLanguages) {
            const QString baseName = "WordsRecall" + QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName)) {
                a.installTranslator(&translator);
                break;
            }
        }

        std::unique_ptr<WRDebugWindow> wp = nullptr;
        std::unique_ptr<WRMainWindow> wp_mw = nullptr;
        if (_DEBUG_DICT) {
            // debug window
            wp.reset(new WRDebugWindow);
            std::string filename(_DICT_FILENAME);
            std::vector<DictWord> words = wr::parse_dict_file(filename);

            std::stringstream vss;
            for (auto &e : words)
                vss << e << "\n";
            wp->change_label(vss.str());
            wp->show();
        } else {
            // main GUI
            wp_mw.reset(new WRMainWindow(nullptr, ""));
            wp_mw->show();
        }

        return QCoreApplication::exec();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occured" << std::endl;
    }
}
