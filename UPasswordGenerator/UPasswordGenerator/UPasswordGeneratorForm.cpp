#include "UPasswordGeneratorForm.h"
#include "ui_UPasswordGeneratorForm.h"

bool isChecked(QCheckBox *cBox)
{ return cBox->checkState() == Qt::Checked; }

/// PUBLIC
UPasswordGeneratorForm::UPasswordGeneratorForm(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::UPasswordGeneratorForm)
{
    ui->setupUi(this);

    view = ui->listViewPasswords;

    createGUI();
    createMenuBar();
    createToolBars();

    refreshEnabledSymbolSets();
    evaluatePassword();

    testEnabledSaveAs();
    testEnabledCopyToClipboard(view->currentIndex());
}

UPasswordGeneratorForm::~UPasswordGeneratorForm()
{
    delete ui;
}

/// PRIVATE
void UPasswordGeneratorForm::createGUI()
{
    setContextMenuPolicy(Qt::NoContextMenu);

    cBoxLatinUpper = ui->cBoxLatinUpper;
    cBoxLatinLower = ui->cBoxLatinLower;
    cBoxRussianUpper = ui->cBoxRussianUpper;
    cBoxRussianLower = ui->cBoxRussianLower;
    cBoxDigits = ui->cBoxDigits;
    cBoxSymbols = ui->cBoxSymbols;


    QButtonGroup *bGroup = new QButtonGroup(this);
    bGroup->setExclusive(false);
    bGroup->addButton(cBoxLatinUpper);
    bGroup->addButton(cBoxLatinLower);
    bGroup->addButton(cBoxRussianUpper);
    bGroup->addButton(cBoxRussianLower);
    bGroup->addButton(cBoxDigits);
    bGroup->addButton(cBoxSymbols);

    connect(bGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(clickedWidgetsSymbolSets(int)));
    connect(ui->sBoxPasswordLength, SIGNAL(valueChanged(int)),
            this, SLOT(clickedWidgetsSymbolSets(int)));    


    model = new QStringListModel(this);

    view->setModel(model);
    view->setAlternatingRowColors(true);
    connect(view, SIGNAL(clicked(QModelIndex)),
            this, SLOT(testEnabledCopyToClipboard(QModelIndex)));
}

void UPasswordGeneratorForm::createToolBars()
{    
    tButtonSaveAs = new QToolButton();
    tButtonSaveAs->setText("Сохранить");
    tButtonSaveAs->setToolTip(tButtonSaveAs->text());
    tButtonSaveAs->setIcon(QIcon(":/IconSave"));
    connect(tButtonSaveAs, SIGNAL(clicked()),
            this, SLOT(saveAs()));


    tButtonCopyToClipboard = new QToolButton();
    tButtonCopyToClipboard->setText("Копировать в буфер обмена");
    tButtonCopyToClipboard->setToolTip(tButtonCopyToClipboard->text());
    tButtonCopyToClipboard->setIcon(QIcon(":/IconClipboard"));
    connect(tButtonCopyToClipboard, SIGNAL(clicked()),
            this, SLOT(copyToClipboard()));


    tButtonGenerate = new QToolButton();
    tButtonGenerate->setText("Сгенерировать");
    tButtonGenerate->setToolTip(tButtonGenerate->text());
    tButtonGenerate->setIcon(QIcon(":/IconGenerate"));
    connect(tButtonGenerate, SIGNAL(clicked()),
            this, SLOT(generate()));


    tButtonQuit = new QToolButton();
    tButtonQuit->setText("Выйти");
    tButtonQuit->setToolTip(tButtonQuit->text());
    tButtonQuit->setIcon(QIcon(":/IconQuit"));
    connect(tButtonQuit, SIGNAL(clicked()),
            qApp, SLOT(quit()));


    toolBar = new QToolBar();
    toolBar->setFloatable(false);
    toolBar->setMovable(false);  
    toolBar->addWidget(tButtonGenerate);
    toolBar->addWidget(tButtonSaveAs);
    toolBar->addWidget(tButtonCopyToClipboard);
    toolBar->addWidget(tButtonQuit);

    addToolBar(toolBar);
}

void UPasswordGeneratorForm::createMenuBar()
{
    /// Menu File
    menuFile = new QMenu("&Файл");
    actionSaveAs = new QAction(this);
    actionSaveAs->setText("Сохранить...");
    actionSaveAs->setIcon(QIcon(":/IconSave"));
    actionSaveAs->setShortcut(QKeySequence("Ctrl+S"));
    connect(actionSaveAs, SIGNAL(triggered()),
            this, SLOT(saveAs()));


    actionQuit = new QAction(this);
    actionQuit->setText("Выход");
    actionQuit->setIcon(QIcon(":/IconQuit"));
    actionQuit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(actionQuit, SIGNAL(triggered()),
            qApp, SLOT(quit()));


    menuFile->addAction(actionSaveAs);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);
    /// Menu File


    /// Menu Password
    actionGenerate = new QAction(this);
    actionGenerate->setText("Сгенерировать");
    actionGenerate->setIcon(QIcon(":/IconGenerate"));
    actionGenerate->setShortcut(QKeySequence("Ctrl+G"));
    connect(actionGenerate, SIGNAL(triggered()),
            this, SLOT(generate()));


    actionCopyToClipboard = new QAction(this);
    actionCopyToClipboard->setText("Скопировать в буфер обмена");
    actionCopyToClipboard->setIcon(QIcon(":/IconClipboard"));
    connect(actionCopyToClipboard, SIGNAL(triggered()),
            this, SLOT(copyToClipboard()));

    menuPassword = new QMenu("&Пароль");
    menuPassword->addAction(actionGenerate);
    menuPassword->addSeparator();
    menuPassword->addAction(actionCopyToClipboard);
    /// Menu Password


    /// Menu Help
    menuHelp = new QMenu("&Справка");
    actionAboutProgram = new QAction(this);
    actionAboutProgram->setText("О программе...");
    connect(actionAboutProgram, SIGNAL(triggered()),
            this, SLOT(about()));

    menuHelp->addAction(actionAboutProgram);
    /// Menu Help


    menuBar = new QMenuBar();
    setMenuBar(menuBar);

    menuBar->addMenu(menuFile);
    menuBar->addMenu(menuPassword);
    menuBar->addMenu(menuHelp);
}

void UPasswordGeneratorForm::refreshEnabledSymbolSets()
{
    bool checkedLatinUpper = isChecked(cBoxLatinUpper);
    bool checkedLatinLower = isChecked(cBoxLatinLower);
    bool checkedRussianUpper = isChecked(cBoxRussianUpper);
    bool checkedRussianLower = isChecked(cBoxRussianLower);
    bool checkedDigits = isChecked(cBoxDigits);
    bool checkedSymbols = isChecked(cBoxSymbols);

    int countChecked = 0;

    if(checkedLatinUpper)
        countChecked++;

    if(checkedLatinLower)
        countChecked++;

    if(checkedRussianUpper)
        countChecked++;

    if(checkedRussianLower)
        countChecked++;

    if(checkedDigits)
        countChecked++;

    if(checkedSymbols)
        countChecked++;

    ui->labelStrengthPassword->setVisible(countChecked);
    ui->labelLevelStrengthPassword->setVisible(countChecked);
    tButtonGenerate->setEnabled(countChecked);
    actionGenerate->setEnabled(countChecked);
}

UPasswordGenerate::Sets UPasswordGeneratorForm::generateFlags()
{
    UPasswordGenerate::Sets flags;

    if(isChecked(cBoxLatinLower))
        flags |= UPasswordGenerate::LatinLower;

    if(isChecked(cBoxLatinUpper))
        flags |= UPasswordGenerate::LatinUpper;

    if(isChecked(cBoxRussianLower))
        flags |= UPasswordGenerate::RussianLower;

    if(isChecked(cBoxRussianUpper))
        flags |= UPasswordGenerate::RussianUpper;

    if(isChecked(cBoxDigits))
        flags |= UPasswordGenerate::Digits;

    if(isChecked(cBoxSymbols))
        flags |= UPasswordGenerate::Symbols;

    return flags;
}


/// PRIVATE SLOTS
void UPasswordGeneratorForm::clickedWidgetsSymbolSets(int id)
{
    Q_UNUSED(id);
    refreshEnabledSymbolSets();
    evaluatePassword();
}

void UPasswordGeneratorForm::evaluatePassword()
{
    UPasswordGenerate password;
    password.setFlags(generateFlags());
    password.setLength(ui->sBoxPasswordLength->value());

    int bits = password.strength();
    int level = password.levelStrength();
    QString stringLevel = password.stringLevelStrength();


    QString textStrength = QString("Стойкость пароля: <b>%1 бита</b>")
            .arg(bits);

    QPair <int, QString> levelStrength;

    switch(level)
    {
    case 4:
        levelStrength.first = 5;
        levelStrength.second = "#00C43A";
        break;

    case 3:
        levelStrength.first = 4;
        levelStrength.second = "#7FFF00";
        break;

    case 2:
        levelStrength.first = 3;
        levelStrength.second = "#FF5300";
        break;

    case 1:
        levelStrength.first = 2;
        levelStrength.second = "#FF0000";
        break;
    }

    QString textLevelStrength = QString("Уровень стойкости: <font size=%2 color=%3><b>%1</b></font>")
            .arg(stringLevel)
            .arg(levelStrength.first)
            .arg(levelStrength.second);

    ui->labelStrengthPassword->setText(textStrength);
    ui->labelLevelStrengthPassword->setText(textLevelStrength);
}

void UPasswordGeneratorForm::testEnabledSaveAs()
{
    bool enabledSaveAs = !model->stringList().isEmpty();

    tButtonSaveAs->setEnabled(enabledSaveAs);
    actionSaveAs->setEnabled(enabledSaveAs);
}
void UPasswordGeneratorForm::testEnabledCopyToClipboard(QModelIndex index /*= 0*/)
{
    bool enabledCopyToClipboard = !model->stringList().isEmpty()
            && view->currentIndex().isValid();

    tButtonCopyToClipboard->setEnabled(enabledCopyToClipboard);
    actionCopyToClipboard->setEnabled(enabledCopyToClipboard);
}


/// PUBLIC SLOTS
void UPasswordGeneratorForm::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    QString(),
                                                    "Пароли",
                                                    "Текстовые файлы (*.txt)");

    if(fileName.isEmpty()
            && fileName.isNull())
        return;

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly
                  | QIODevice::Truncate))
    {
        QMessageBox::critical(this,
                              "Ошибка",
                              "По каким то неизвестным причинам,\n"
                              " произошла ошибка");
        return;
    }

    const QStringList &list = model->stringList();

    QTextStream in(&file);

    foreach(QString string, list)
        in << string << endl;
}
void UPasswordGeneratorForm::copyToClipboard()
{
   QModelIndex item = view->currentIndex();

   if(!item.isValid())
       return;

   QString text = item.data(Qt::EditRole).toString();

   qApp->clipboard()->setText(text);

   QMessageBox::information(this,
                            "Сообщение",
                            "Пароль скопирован в буфер обмена");
}
void UPasswordGeneratorForm::about()
{
    UAboutProgram *about = new UAboutProgram();
    about->exec();
    about->deleteLater();
}

void UPasswordGeneratorForm::generate()
{
    UPasswordGenerate::Sets flags = generateFlags();

    // очищаем модель
    model->removeRows(0, model->rowCount());


    int lenghtPassword = ui->sBoxPasswordLength->value();
    int numberPasswords = ui->sBoxNumberPasswords->value();


    UPasswordGenerator *generator = new UPasswordGenerator();
    generator->setFlags(flags);
    generator->setLength(lenghtPassword);
    generator->setNumberPasswords(numberPasswords);

    connect(generator, SIGNAL(password(QString)), this, SLOT(addPassword(QString)));
    connect(generator, SIGNAL(progress(int)), ui->progressBar, SLOT(setValue(int)));

    QThread *workerThread = new QThread(this);

    // при вызове сигнала, выполнится метод класса генератора паролей
    connect(workerThread, SIGNAL(started()), generator, SLOT(startGenerating()));

    // при завершении поток, удаление генератора паролей
    connect(workerThread, SIGNAL(finished()), generator, SLOT(deleteLater()));

    // при вызыве сигнала завершения, удаляем класс генератора паролей
    // и поток, в котором выполнялся код генерации паролей
    connect(generator, SIGNAL(finished()), generator, SLOT(deleteLater()));
    connect(generator, SIGNAL(finished()), workerThread, SLOT(quit()));
    connect(generator, SIGNAL(finished()), this, SLOT(testEnabledSaveAs()));
    connect(generator, SIGNAL(timeElapsed(int)), this, SLOT(showTimeElapsed(int)));

    // помещаем указатель класса генератора паролей в поток
    generator->moveToThread(workerThread);

    // запускаем поток
    workerThread->start();    
}

void UPasswordGeneratorForm::addPassword(QString password)
{
    ui->labelOnlyPasswords->setText(QString("Всего: %1")
                                    .arg(model->rowCount() + 1));

    model->insertRow(model->rowCount());
    QModelIndex index = model->index(model->rowCount() - 1);
    model->setData(index, password);
}

void UPasswordGeneratorForm::showTimeElapsed(int msec)
{
    QString text = QString("Время генерации: %1 секунд %2 миллисекунд")
            .arg(msec / 1000)
            .arg(msec);
    ui->statusBar->showMessage(text, 5000);
}
