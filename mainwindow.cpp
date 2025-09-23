#include "mainwindow.h"
#include <QDir>
#include <QDate>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 窗口基本设置
    setFixedSize(800, 600);
    setStyleSheet("background-color:" "#1a2228;");
    setWindowTitle("Qt 信息管理系统");

    // 数据持久化设置
    QString appPath = QApplication::applicationDirPath(); // 获取程序运行目录
    QDir photoDir(appPath + "/../photos"); // 定义照片文件夹路径
    if (!photoDir.exists()) { // 如果文件夹不存在
        photoDir.mkpath("."); // 则创建它
    }
    dataFilePath = appPath + "/student_data.csv"; // 定义数据文件路径

    studentList = new StuList();

    // 创建主窗口的中心 Widget 和布局
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QVBoxLayout *rightLayout = new QVBoxLayout();

    // 左侧布局
    dataListWidget = new QListWidget(this);
    dataListWidget->setStyleSheet(
        "QListWidget { background-color: #273038; border: 1px solid #3d4a57; color: #b0c4de; font-family: Arial; font-size: 14px; }"
        "QListWidget::item { padding: 4px; }"
        "QListWidget::item:selected { background-color: #3d4a57; color: #5bc0de; }"
        "QScrollBar:vertical { background: #1a2228; width: 10px; border: none; }"
        "QScrollBar::handle:vertical { background: #3d4a57; border-radius: 5px; min-height: 20px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { border: none; background: none; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }");

    // 搜索栏框架和内容 (使用布局管理器)
    searchFrame = new QFrame(this);
    searchFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    searchFrame->setStyleSheet("background-color: #273038; border: 1px solid #3d4a57;");
    QHBoxLayout *searchLayout = new QHBoxLayout(searchFrame);
    searchLayout->setContentsMargins(10, 2, 10, 2);
    searchLabel = new QLabel("搜索栏");
    searchLabel->setStyleSheet("color: #b0c4de;");
    searchLineEdit = new QLineEdit();
    searchButton = new QPushButton("搜索");
    QString searchButtonStyle =
        "QPushButton { background-color: #31b0d5; border: none; border-radius: 12px; color: white; font-weight: bold; font-size: 14px; padding: 6px 12px; }"
        "QPushButton:hover { background-color: #5bc0de; }"
        "QPushButton:pressed { background-color: #286090; border-style: inset; border-width: 2px; border-color: #1e4566; }";
    searchButton->setStyleSheet(searchButtonStyle);
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->setStretch(1, 1);

    photoDisplayLabel = new QLabel(this);
    photoDisplayLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    photoDisplayLabel->setStyleSheet("background-color: #273038; border: 1px solid #3d4a57; color: #b0c4de;");
    photoDisplayLabel->setAlignment(Qt::AlignCenter);
    photoDisplayLabel->setText("请选择信息查看");
    photoDisplayLabel->setMinimumSize(200, 200);

    leftLayout->addWidget(dataListWidget);
    leftLayout->addWidget(searchFrame);
    leftLayout->addWidget(photoDisplayLabel);

    // 右侧布局
    titleLabel = new QLabel("Qt 信息管理系统", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #5bc0de; font-size: 18px; font-weight: bold;");

    QString lightLabelStyle = "color: #b0c4de;";
    QString lineEditStyle = "background-color: #273038; border: 1px solid #3d4a57; color: #b0c4de;";
    QString clearButtonStyle = "QPushButton { background-color: transparent; border: none; color: #6a737d; font-weight: bold; font-size: 16px; border-radius: 12px; } QPushButton:hover { color: #b0c4de; background-color: #3d4a57; }";
    QString checkBoxStyle = "QCheckBox { color: #b0c4de; } QCheckBox::indicator { background-color: #273038; border: 1px solid #3d4a57; border-radius: 4px; width: 14px; height: 14px; } QCheckBox::indicator:hover { background-color: #3d4a57; } QCheckBox::indicator:checked { background-color: #5bc0de; border: 1px solid #5bc0de; } QCheckBox::indicator:checked:hover { background-color: #31b0d5; border: 1px solid #31b0d5; }";
    QString buttonStyle = "QPushButton { background-color: #5bc0de; border: none; border-radius: 14px; color: white; font-weight: bold; font-size: 12px; padding: 8px 16px; } QPushButton:hover { background-color: #31b0d5; } QPushButton:pressed { background-color: #286090; border-style: inset; border-width: 2px; border-color: #1e4566; }";

    // 使用 QFormLayout 来组织标签和输入框，更整洁
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setContentsMargins(0, 20, 0, 20);
    formLayout->setLabelAlignment(Qt::AlignLeft);

    auto createLineEditRow = [&](QLabel *label, QLineEdit *lineEdit, QPushButton *clearButton) {
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(lineEdit);
        hLayout->addWidget(clearButton);
        clearButton->setFixedSize(25, 25);
        lineEdit->setStyleSheet(lineEditStyle);
        clearButton->setStyleSheet(clearButtonStyle);
        formLayout->addRow(label, hLayout);
        label->setStyleSheet(lightLabelStyle);
    };

    photoPathLabel = new QLabel("照片 (仅文件名)");
    photoPathLineEdit = new QLineEdit();
    clearPhotoPathButton = new QPushButton("×");
    createLineEditRow(photoPathLabel, photoPathLineEdit, clearPhotoPathButton);

    idLabel = new QLabel("ID");
    idLineEdit = new QLineEdit();
    clearIdButton = new QPushButton("×");
    createLineEditRow(idLabel, idLineEdit, clearIdButton);

    nameLabel = new QLabel("姓名");
    nameLineEdit = new QLineEdit();
    clearNameButton = new QPushButton("×");
    createLineEditRow(nameLabel, nameLineEdit, clearNameButton);

    // 生日布局
    birthLabel = new QLabel("生日");
    QHBoxLayout *birthLayout = new QHBoxLayout();
    yearLineEdit = new QLineEdit();
    monthLineEdit = new QLineEdit();
    dayLineEdit = new QLineEdit();

    birthLayout->addWidget(yearLineEdit);
    QLabel *yearLabel = new QLabel("年");
    yearLabel->setStyleSheet(lightLabelStyle);
    birthLayout->addWidget(yearLabel);

    birthLayout->addWidget(monthLineEdit);
    QLabel *monthLabel = new QLabel("月");
    monthLabel->setStyleSheet(lightLabelStyle);
    birthLayout->addWidget(monthLabel);

    birthLayout->addWidget(dayLineEdit);
    QLabel *dayLabel = new QLabel("日");
    dayLabel->setStyleSheet(lightLabelStyle);
    birthLayout->addWidget(dayLabel);

    formLayout->addRow(birthLabel, birthLayout);
    birthLabel->setStyleSheet(lightLabelStyle);
    yearLineEdit->setStyleSheet(lineEditStyle);
    monthLineEdit->setStyleSheet(lineEditStyle);
    dayLineEdit->setStyleSheet(lineEditStyle);

    hobbyLabel = new QLabel("兴趣");
    hobbyLineEdit = new QLineEdit();
    clearHobbyButton = new QPushButton("×");
    createLineEditRow(hobbyLabel, hobbyLineEdit, clearHobbyButton);

    // 添加位置复选框和输入框
    QHBoxLayout *positionLayout = new QHBoxLayout();
    toHeadCheckBox = new QCheckBox("表头");
    toTailCheckBox = new QCheckBox("表尾");
    toPositionCheckBox = new QCheckBox("指定位置至");
    positionLineEdit = new QLineEdit();
    clearPositionButton = new QPushButton("×");
    positionLineEdit->setValidator(new QIntValidator(1, 10000, this));
    positionLayout->addWidget(toHeadCheckBox);
    positionLayout->addWidget(toTailCheckBox);
    positionLayout->addWidget(toPositionCheckBox);
    positionLayout->addWidget(positionLineEdit);
    positionLayout->addWidget(clearPositionButton);
    toHeadCheckBox->setStyleSheet(checkBoxStyle);
    toTailCheckBox->setStyleSheet(checkBoxStyle);
    toPositionCheckBox->setStyleSheet(checkBoxStyle);
    positionLineEdit->setStyleSheet(lineEditStyle);
    clearPositionButton->setStyleSheet(clearButtonStyle);

    positionGroup = new QButtonGroup(this);
    positionGroup->setExclusive(true);
    positionGroup->addButton(toHeadCheckBox);
    positionGroup->addButton(toTailCheckBox);
    positionGroup->addButton(toPositionCheckBox);
    toTailCheckBox->setChecked(true);
    positionLineEdit->setEnabled(false);

    // 按钮布局
    QHBoxLayout *buttonLayout1 = new QHBoxLayout();
    modifyButton = new QPushButton("修改");
    saveModifiedButton = new QPushButton("保存");
    addButton = new QPushButton("添加");
    clearButton = new QPushButton("清空");
    buttonLayout1->addWidget(modifyButton);
    buttonLayout1->addWidget(saveModifiedButton);
    buttonLayout1->addWidget(addButton);
    buttonLayout1->addWidget(clearButton);
    saveModifiedButton->setHidden(true); // 默认隐藏

    QHBoxLayout *buttonLayout2 = new QHBoxLayout();
    returnButton = new QPushButton("返回");
    deleteButton = new QPushButton("删除");
    viewButton = new QPushButton("查看");
    exportButton = new QPushButton("导出"); // 创建导出按钮
    buttonLayout2->addWidget(returnButton);
    buttonLayout2->addWidget(deleteButton);
    buttonLayout2->addWidget(viewButton);
    buttonLayout2->addWidget(exportButton); // 将按钮添加到布局

    modifyButton->setStyleSheet(buttonStyle);
    saveModifiedButton->setStyleSheet(buttonStyle);
    addButton->setStyleSheet(buttonStyle);
    clearButton->setStyleSheet(buttonStyle);
    returnButton->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle);
    viewButton->setStyleSheet(buttonStyle);
    exportButton->setStyleSheet(buttonStyle); // 为按钮设置样式

    // 查看信息框架 (使用布局管理器)
    viewFrame = new QFrame(this);
    viewFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    viewFrame->setStyleSheet("background-color: #273038; border: 1px solid #3d4a57;");
    QVBoxLayout *viewLayout = new QVBoxLayout(viewFrame);
    viewIdLabel = new QLabel("ID:");
    viewNameLabel = new QLabel("姓名:");
    viewBirthLabel = new QLabel("生日:");
    viewHobbyLabel = new QLabel("兴趣:");
    viewIdLabel->setStyleSheet(lightLabelStyle);
    viewNameLabel->setStyleSheet(lightLabelStyle);
    viewBirthLabel->setStyleSheet(lightLabelStyle);
    viewHobbyLabel->setStyleSheet(lightLabelStyle);
    viewLayout->addWidget(viewIdLabel);
    viewLayout->addWidget(viewNameLabel);
    viewLayout->addWidget(viewBirthLabel);
    viewLayout->addWidget(viewHobbyLabel);

    // 组合右侧布局
    rightLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    QLabel *photoNoteLabel = new QLabel(this);
    photoNoteLabel->setText(QString("照片文件夹的相对路径: \n ./build/Desktop_Qt_6_5_3_MinGW_64_bit-Debug/photos \n 添加信息前需要先把照片文件存放至照片文件夹里"));
    photoNoteLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    photoNoteLabel->setStyleSheet("background-color: #273038; border: 1px solid #3d4a57; color: #b0c4de; padding: 5px;");
    rightLayout->addWidget(photoNoteLabel);

    // **提示标签**
    viewNoteLabel = new QLabel(this);
    viewNoteLabel->setText("点击查看后即可查看照片与其他信息");
    viewNoteLabel->setWordWrap(true);
    viewNoteLabel->setStyleSheet("background-color: #273038; border: 1px solid #3d4a57; color: #b0c4de; padding: 5px;");
    rightLayout->addWidget(viewNoteLabel);

    modifyNoteLabel = new QLabel(this);
    modifyNoteLabel->setText("点击修改后，需要点击保存按钮，才能成功保存修改后的信息");
    modifyNoteLabel->setWordWrap(true);
    modifyNoteLabel->setStyleSheet("background-color: #273038; border: 1px solid #3d4a57; color: #b0c4de; padding: 5px;");
    rightLayout->addWidget(modifyNoteLabel);


    rightLayout->addLayout(formLayout);
    rightLayout->addLayout(positionLayout);
    rightLayout->addLayout(buttonLayout1);
    rightLayout->addLayout(buttonLayout2);
    rightLayout->addWidget(viewFrame);
    rightLayout->addStretch(1);

    // 组合主布局
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 1);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 信号与槽连接
    connect(clearPhotoPathButton, &QPushButton::clicked, photoPathLineEdit, &QLineEdit::clear);
    connect(clearIdButton, &QPushButton::clicked, idLineEdit, &QLineEdit::clear);
    connect(clearNameButton, &QPushButton::clicked, nameLineEdit, &QLineEdit::clear);
    connect(clearHobbyButton, &QPushButton::clicked, hobbyLineEdit, &QLineEdit::clear);
    connect(clearPositionButton, &QPushButton::clicked, positionLineEdit, &QLineEdit::clear);
    connect(toPositionCheckBox, &QCheckBox::stateChanged, positionLineEdit, &QLineEdit::setEnabled);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::handleAddButtonClicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::handleClearButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteButtonClicked);
    connect(viewButton, &QPushButton::clicked, this, &MainWindow::handleViewButtonClicked);
    connect(modifyButton, &QPushButton::clicked, this, &MainWindow::handleModifyButtonClicked);
    connect(saveModifiedButton, &QPushButton::clicked, this, &MainWindow::handleSaveModifiedButtonClicked);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::handleSearchButtonClicked);
    connect(returnButton, &QPushButton::clicked, this, &MainWindow::handleReturnButtonClicked);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::handleExportButtonClicked); // 连接信号与槽

    // 程序启动时加载初始数据
    loadData();
}

// 析构函数
MainWindow::~MainWindow()
{
    delete studentList;
}

// 关闭窗口事件处理函数
void MainWindow::closeEvent(QCloseEvent *event)
{
    saveData(); // 在关闭前保存数据
    event->accept();
}

// 槽函数实现
void MainWindow::handleAddButtonClicked()
{
    // 获取输入框内容
    QString path = photoPathLineEdit->text().trimmed();
    QString id = idLineEdit->text().trimmed();
    QString name = nameLineEdit->text().trimmed();
    QString year = yearLineEdit->text().trimmed();
    QString month = monthLineEdit->text().trimmed();
    QString day = dayLineEdit->text().trimmed();
    QString hobby = hobbyLineEdit->text().trimmed();
    QString birth = year + "/" + month + "/" + day;

    // 使用自定义样式的消息框
    QMessageBox msgBox(this);
    applyMessageBoxStyles(msgBox);
    msgBox.setIcon(QMessageBox::Warning);

    if (path.isEmpty() || id.isEmpty() || name.isEmpty() || year.isEmpty() || month.isEmpty() || day.isEmpty() || hobby.isEmpty()) {
        msgBox.setText("存在空的字段！");
        msgBox.exec();
        return;
    }

    bool isIdValid;
    id.toInt(&isIdValid);
    if (!isIdValid) {
        msgBox.setText("ID必须是纯数字！");
        msgBox.exec();
        return;
    }

    bool isYearValid, isMonthValid, isDayValid;
    int intYear = year.toInt(&isYearValid);
    int intMonth = month.toInt(&isMonthValid);
    int intDay = day.toInt(&isDayValid);

    if (!isYearValid || !isMonthValid || !isDayValid || intYear < 1900 || intYear > QDate::currentDate().year()) {
        msgBox.setText("年份、月份或日期不合法！");
        msgBox.exec();
        return;
    }

    QDate date(intYear, intMonth, intDay);
    if (!date.isValid()) {
        msgBox.setText("日期不合法！请检查月份和天数！");
        msgBox.exec();
        return;
    }

    Student newStudent(id.toStdString(), name.toStdString(), birth.toStdString(), path.toStdString(), hobby.toStdString());

    // 根据选中的复选框执行不同的添加操作
    QAbstractButton *checkedButton = positionGroup->checkedButton();
    if (checkedButton == toHeadCheckBox) {
        studentList->addHead(newStudent);
    } else if (checkedButton == toTailCheckBox) {
        studentList->addTail(newStudent);
    } else if (checkedButton == toPositionCheckBox) {
        bool ok;
        int position = positionLineEdit->text().toInt(&ok);
        if (!ok || !studentList->addPos(position, newStudent)) {
            msgBox.setText("指定位置无效！");
            msgBox.exec();
            return;
        }
    }

    populateListWidget(); // 刷新列表
    handleClearButtonClicked(); // 清空输入框
    saveData(); // 保存更改到文件
}

// 处理删除按钮点击事件
void MainWindow::handleDeleteButtonClicked()
{
    QListWidgetItem *currentItem = dataListWidget->currentItem();
    QMessageBox msgBox(this);
    applyMessageBoxStyles(msgBox);

    if (currentItem) {
        int row = dataListWidget->currentRow();
        if (studentList->deletePos(row + 1)) {
            populateListWidget();
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText("信息删除成功！");
            saveData(); // 保存更改到文件
        } else {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("删除失败，位置无效！");
        }
    } else {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("请选择要删除的信息！");
    }
    msgBox.exec();
}

// 处理查看按钮点击事件
void MainWindow::handleViewButtonClicked()
{
    QListWidgetItem *currentItem = dataListWidget->currentItem();
    if (currentItem) {
        int row = dataListWidget->currentRow();
        Student* s = studentList->getStudent(row + 1);
        if (s) {
            viewIdLabel->setText("ID: " + QString::fromStdString(s->id));
            viewNameLabel->setText("姓名: " + QString::fromStdString(s->name));
            viewBirthLabel->setText("生日: " + QString::fromStdString(s->birth));
            viewHobbyLabel->setText("兴趣: " + QString::fromStdString(s->hobby));

            // 修正照片路径，使其指向上一级目录的 photos 文件夹
            QString photoFullPath = QApplication::applicationDirPath() + "/../photos/" + QString::fromStdString(s->path);

            // 显示照片
            QPixmap pixmap(photoFullPath);
            if (!pixmap.isNull()) {
                photoDisplayLabel->setPixmap(pixmap.scaled(photoDisplayLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                photoDisplayLabel->setText("");
            } else {
                photoDisplayLabel->clear();
                photoDisplayLabel->setText("照片未找到");
            }
        }
    } else {
        QMessageBox msgBox(this);
        applyMessageBoxStyles(msgBox);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("请选择要查看的信息！");
        msgBox.exec();
    }
}

// 处理修改按钮点击事件
void MainWindow::handleModifyButtonClicked()
{
    QListWidgetItem *currentItem = dataListWidget->currentItem();
    if (currentItem) {
        int row = dataListWidget->currentRow();
        Student* s = studentList->getStudent(row + 1);
        if (s) {
            idLineEdit->setText(QString::fromStdString(s->id));
            nameLineEdit->setText(QString::fromStdString(s->name));
            QStringList birthParts = QString::fromStdString(s->birth).split('/');
            if (birthParts.count() == 3) {
                yearLineEdit->setText(birthParts[0]);
                monthLineEdit->setText(birthParts[1]);
                dayLineEdit->setText(birthParts[2]);
            }
            hobbyLineEdit->setText(QString::fromStdString(s->hobby));
            photoPathLineEdit->setText(QString::fromStdString(s->path));

            // 切换按钮显示
            modifyButton->setHidden(true);
            saveModifiedButton->show();
            // 保存当前修改的行索引
            dataListWidget->setProperty("modifyingRow", row);
        }
    } else {
        QMessageBox msgBox(this);
        applyMessageBoxStyles(msgBox);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("请选择要修改的信息！");
        msgBox.exec();
    }
}

// 保存修改的槽函数
void MainWindow::handleSaveModifiedButtonClicked()
{
    int row = dataListWidget->property("modifyingRow").toInt();
    QMessageBox msgBox(this);
    applyMessageBoxStyles(msgBox);
    msgBox.setIcon(QMessageBox::Warning);

    if (row < 0) {
        msgBox.setText("未找到要修改的信息！");
        msgBox.exec();
        return;
    }

    // 获取输入框新内容并验证 (此处省略与添加按钮重复的验证代码)
    QString path = photoPathLineEdit->text().trimmed();
    QString id = idLineEdit->text().trimmed();
    QString name = nameLineEdit->text().trimmed();
    QString year = yearLineEdit->text().trimmed();
    QString month = monthLineEdit->text().trimmed();
    QString day = dayLineEdit->text().trimmed();
    QString hobby = hobbyLineEdit->text().trimmed();
    QString birth = year + "/" + month + "/" + day;

    Student newStudent(id.toStdString(), name.toStdString(), birth.toStdString(), path.toStdString(), hobby.toStdString());

    if (studentList->modify(row + 1, newStudent)) {
        populateListWidget();
        dataListWidget->setCurrentRow(row); // 重新选中修改的行

        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("信息修改成功！");
        msgBox.exec();

        saveModifiedButton->setHidden(true);
        modifyButton->show();
        dataListWidget->setProperty("modifyingRow", -1);
        handleClearButtonClicked();
        saveData(); // 保存更改到文件
    } else {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("信息修改失败！");
        msgBox.exec();
    }
}

// 处理清空按钮点击事件
void MainWindow::handleClearButtonClicked()
{
    idLineEdit->clear();
    nameLineEdit->clear();
    yearLineEdit->clear();
    monthLineEdit->clear();
    dayLineEdit->clear();
    hobbyLineEdit->clear();
    positionLineEdit->clear();
    photoPathLineEdit->clear();
}

// 处理搜索按钮点击事件
void MainWindow::handleSearchButtonClicked()
{
    populateListWidget(); // 刷新列表，会根据搜索框内容自动过滤
}

// 处理返回按钮点击事件（清空搜索并显示所有数据）
void MainWindow::handleReturnButtonClicked()
{
    searchLineEdit->clear();
    populateListWidget();
    photoDisplayLabel->setText("请选择信息查看");
    viewIdLabel->setText("ID:");
    viewNameLabel->setText("姓名:");
    viewBirthLabel->setText("生日:");
    viewHobbyLabel->setText("兴趣:");
}

// 处理导出按钮点击事件
void MainWindow::handleExportButtonClicked()
{
    // 弹出文件保存对话框，让用户选择保存位置和文件名
    // 参数：父窗口，对话框标题，默认目录，文件类型过滤器
    QString filePath = QFileDialog::getSaveFileName(this, "导出到 CSV", "", "CSV Files (*.csv);;All Files (*)");

    // 如果用户点击了“取消”或关闭了对话框，filePath会为空
    if (filePath.isEmpty()) {
        return; // 用户取消操作，直接返回
    }

    // 调用StuList中的保存函数，将数据写入用户选择的文件
    studentList->saveToFile(filePath);

    // 弹出成功提示
    QMessageBox msgBox(this);
    applyMessageBoxStyles(msgBox);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("数据已成功导出到：\n" + filePath);
    msgBox.exec();
}

// 数据处理与UI辅助函数

// 从文件加载数据
void MainWindow::loadData()
{
    studentList->loadFromFile(dataFilePath);
    populateListWidget();
}

// 保存数据到文件
void MainWindow::saveData()
{
    studentList->saveToFile(dataFilePath);
}

// 刷新/填充UI列表控件
void MainWindow::populateListWidget()
{
    dataListWidget->clear();
    QString searchText = searchLineEdit->text().trimmed().toLower();

    Node* current = studentList->getHead();
    while (current) {
        Student &s = current->student;
        // 如果搜索框为空，或任一字段包含搜索文本，则显示
        if (searchText.isEmpty() ||
            QString::fromStdString(s.id).toLower().contains(searchText) ||
            QString::fromStdString(s.name).toLower().contains(searchText) ||
            QString::fromStdString(s.birth).toLower().contains(searchText) ||
            QString::fromStdString(s.hobby).toLower().contains(searchText))
        {
            dataListWidget->addItem(formatData(s));
        }
        current = current->next;
    }
}

// 辅助函数：格式化显示数据
QString MainWindow::formatData(const Student &student)
{
    return QString("%1 %2 %3 %4")
    .arg(QString::fromStdString(student.id))
        .arg(QString::fromStdString(student.name))
        .arg(QString::fromStdString(student.birth))
        .arg(QString::fromStdString(student.hobby));
}

// 为QMessageBox应用自定义暗色主题样式
void MainWindow::applyMessageBoxStyles(QMessageBox &msgBox)
{
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #273038; font-family: Arial; font-size: 14px; }"
        "QMessageBox QLabel { color: #b0c4de; }"
        "QMessageBox QPushButton {"
        "   background-color: #5bc0de;"
        "   border: none; border-radius: 12px; color: white;"
        "   font-weight: bold; padding: 8px 24px; min-width: 80px;"
        "}"
        "QMessageBox QPushButton:hover { background-color: #31b0d5; }"
        "QMessageBox QPushButton:pressed { background-color: #286090; }"
        );
}
