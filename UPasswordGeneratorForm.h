#ifndef UPASSWORDGENERATORFORM_H
#define UPASSWORDGENERATORFORM_H

#include <QtGui>
#include <QtCore>

namespace Ui {
class UPasswordGeneratorForm;
}

#include "UPasswordGenerator.h"
#include "UAboutProgram.h"

class UPasswordGeneratorForm: public QMainWindow
{
    Q_OBJECT
    
public:
    UPasswordGeneratorForm(QWidget *parent = 0);
    ~UPasswordGeneratorForm();
    
private:
    void createGUI();
    void createToolBars();
    void createMenuBar();
    void refreshEnabledSymbolSets();
    UPasswordGenerate::Sets generateFlags();    

private:
    QMenuBar *menuBar;

    QMenu *menuFile;
    QAction *actionSaveAs;
    QAction *actionQuit;

    QMenu *menuPassword;
    QAction *actionGenerate;
    QAction *actionCopyToClipboard;

    QMenu *menuHelp;
    QAction *actionAboutProgram;


    QToolBar *toolBar;
    QToolButton *tButtonSaveAs;
    QToolButton *tButtonCopyToClipboard;
    QToolButton *tButtonGenerate;
    QToolButton *tButtonQuit;


    Ui::UPasswordGeneratorForm *ui;
    QCheckBox *cBoxLatinUpper;
    QCheckBox *cBoxLatinLower;
    QCheckBox *cBoxRussianUpper;
    QCheckBox *cBoxRussianLower;
    QCheckBox *cBoxDigits;
    QCheckBox *cBoxSymbols;


    QStringListModel *model;
    QListView *view;


private slots:
    void clickedWidgetsSymbolSets(int id);
    void evaluatePassword();
    void testEnabledSaveAs();
    void testEnabledCopyToClipboard(QModelIndex index);

public slots:
    void saveAs();
    void copyToClipboard();
    void about();
    void generate();   
    void addPassword(QString password);
    void showTimeElapsed(int msec);
};

#endif // UPASSWORDGENERATORFORM_H
