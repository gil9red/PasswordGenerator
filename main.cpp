#include "UPasswordGeneratorForm.h"
#include <QApplication>

class UTextCodec
{
public:
    static void initTextCodecs(const char *codecName)
    {
        QTextCodec *codec = QTextCodec::codecForName(codecName);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForTr(codec);
    }
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    UTextCodec::initTextCodecs("cp1251");

    app.setWindowIcon(QIcon(":/IconProgram"));

    UPasswordGeneratorForm w;
    w.setWindowTitle("WikiPassGen - Генератор паролей");
    w.show();        

    return app.exec();
}
