#ifndef UPASSWORDGENERATOR_H
#define UPASSWORDGENERATOR_H

#include <QtGui>
#include <QtCore>

const static char *latinLower = "abcdefghijklmnopqrstuvwxyz";
const static char *latinUpper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const static char *russianLower = "àáâãäå¸æçèéêëìíîïğñòóôõö÷øùúûüışÿ";
const static char *russianUpper = "ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß";
const static char *digits = "1234567890";
const static char *symbols = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";


class UPasswordGenerate
{
public:
    enum Set
    {
        LatinLower   = 1,
        LatinUpper   = 2,
        RussianLower = 4,
        RussianUpper = 8,
        Digits       = 16,
        Symbols      = 32
    };
    Q_DECLARE_FLAGS(Sets, Set)

public:
    UPasswordGenerate(UPasswordGenerate::Sets sets, int length)
        : d_flags(sets),
          d_length(length){}

    UPasswordGenerate(){}

    void setFlags(UPasswordGenerate::Sets flags)
    {
        letters.clear();

        if(flags.testFlag(UPasswordGenerate::LatinLower))
            letters.append(latinLower);

        if(flags.testFlag(UPasswordGenerate::LatinUpper))
            letters.append(latinUpper);

        if(flags.testFlag(UPasswordGenerate::RussianLower))
            letters.append(russianLower);

        if(flags.testFlag(UPasswordGenerate::RussianUpper))
            letters.append(russianUpper);

        if(flags.testFlag(UPasswordGenerate::Digits))
            letters.append(digits);

        if(flags.testFlag(UPasswordGenerate::Symbols))
            letters.append(symbols);
    }

    void setLength(int length)
    { d_length = length; }

    int length()
    { return d_length; }

    QString generate()
    {
        QString password;

        for(int i = 0; i < d_length; i++)
            password += letters[qrand() % letters.size()];

        return password;
    }

    int strength()
    {
        if(letters.isEmpty()
                && letters.isNull())
            return -1;

        return log(letters.size()) * (d_length / log(2));
    }

    int levelStrength()
    {
        int bits = strength();

        if(bits >= 128)
            return 4;
        else if(bits >= 64 && bits < 128)
            return 3;
        else if(bits >= 56 && bits < 64)
            return 2;
        else if(bits < 56)
            return 1;

        return 0;
    }

    QString stringLevelStrength()
    {
        int level = levelStrength();

        switch(level)
        {
        case 4:
            return "Îòëè÷íûé";

        case 3:
            return "Ñòîéêèé";

        case 2:
            return "Ñğåäíèé";

        case 1:
            return "Ñëàáûé";

        default:
            return "Íèêàêîé";
        }
    }


private:
    UPasswordGenerate::Sets d_flags;
    int d_length;
    QString letters;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(UPasswordGenerate::Sets)


class UPasswordGenerator: public QObject
{
    Q_OBJECT

public:
    UPasswordGenerator()
        : d_numberPasswords(0)
    {
        qsrand(QDateTime::currentMSecsSinceEpoch());
    }

    void setNumberPasswords(int number)
    { d_numberPasswords = number; }

    int numberPasswords()
    { return d_numberPasswords; }

    void setFlags(UPasswordGenerate::Sets sets)
    { flags = sets; }

    void setLength(int length)
    { d_length = length; }

private:
    UPasswordGenerate::Sets flags;
    int d_numberPasswords;
    int d_length;

public slots:
    void startGenerating()
    {
        qsrand(QDateTime::currentMSecsSinceEpoch());

        UPasswordGenerate passGenerate;
        passGenerate.setFlags(flags);
        passGenerate.setLength(d_length);

        QTime time;
        time.start();

        for(int i = 0; i < d_numberPasswords; i++)
        {
            emit progress((i + 1) * 100 / d_numberPasswords);
            emit password(passGenerate.generate());
        }

        emit timeElapsed(time.elapsed());

        emit finished();
    }

signals:
    void password(QString);
    void timeElapsed(int);
    void progress(int);
    void finished();
};

#endif // UPASSWORDGENERATOR_H
