#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QButtonGroup>
#include <QIntValidator>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include "stulist.h"
#include <QFormLayout>
#include <QCloseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void handleAddButtonClicked();
    void handleDeleteButtonClicked();
    void handleViewButtonClicked();
    void handleModifyButtonClicked();
    void handleSaveModifiedButtonClicked();
    void handleClearButtonClicked();
    void handleSearchButtonClicked();
    void handleReturnButtonClicked();
    void handleExportButtonClicked();
private:
    // UI 控件
    QLabel *titleLabel;
    QLabel *photoPathLabel;
    QLabel *idLabel;
    QLabel *nameLabel;
    QLabel *birthLabel;
    QLabel *hobbyLabel;

    QLineEdit *photoPathLineEdit;
    QLineEdit *idLineEdit;
    QLineEdit *nameLineEdit;
    QLineEdit *yearLineEdit;
    QLineEdit *monthLineEdit;
    QLineEdit *dayLineEdit;
    QLineEdit *hobbyLineEdit;
    QLineEdit *positionLineEdit;

    QPushButton *clearPhotoPathButton;
    QPushButton *clearIdButton;
    QPushButton *clearNameButton;
    QPushButton *clearHobbyButton;
    QPushButton *clearPositionButton;

    QCheckBox *toHeadCheckBox;
    QCheckBox *toTailCheckBox;
    QCheckBox *toPositionCheckBox;
    QButtonGroup *positionGroup;

    QPushButton *addButton;
    QPushButton *clearButton;
    QPushButton *deleteButton;
    QPushButton *viewButton;
    QPushButton *modifyButton;
    QPushButton *saveModifiedButton;
    QPushButton *searchButton;
    QPushButton *returnButton;
    QPushButton *exportButton;

    QListWidget *dataListWidget;

    QFrame *searchFrame;
    QLabel *searchLabel;
    QLineEdit *searchLineEdit;

    QFrame *viewFrame;
    QLabel *viewIdLabel;
    QLabel *viewNameLabel;
    QLabel *viewBirthLabel;
    QLabel *viewHobbyLabel;

    QLabel *photoDisplayLabel;

    QLabel *viewNoteLabel;
    QLabel *modifyNoteLabel;

    StuList *studentList;
    QString dataFilePath;

    void loadData();
    void saveData();
    void populateListWidget();
    QString formatData(const Student &student);
    void applyMessageBoxStyles(QMessageBox &msgBox);
};

#endif
