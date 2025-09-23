#include "stulist.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

// Student 类构造函数
Student::Student(const string& id, const string& name, const string& birth, const string& path, const string& hobby)
    : id(id), name(name), birth(birth), path(path), hobby(hobby) {}

// Node 类构造函数
Node::Node(const Student s) : student(s), next(nullptr) {}

// StuList 类构造函数
StuList::StuList() : head(nullptr), size(0) {}

// StuList 类析构函数
StuList::~StuList() {
    clearList();
}

// 获取头节点
Node* StuList::getHead() const {
    return head;
}

// 获取学生信息
Student* StuList::getStudent(int pos) const {
    if (pos < 1 || pos > size) {
        return nullptr;
    }
    Node* current = head;
    for (int i = 1; i < pos; ++i) {
        current = current->next;
    }
    return &(current->student);
}

// 获取链表大小
int StuList::count() const {
    return size;
}

// 头插法
void StuList::addHead(const Student& s) {
    Node* newNode = new Node(s);
    newNode->next = head;
    head = newNode;
    size++;
}

// 尾插法
void StuList::addTail(const Student& s) {
    Node* newNode = new Node(s);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
    size++;
}

// 指定位置插入
bool StuList::addPos(int pos, const Student& s) {
    if (pos < 1 || pos > size + 1) {
        return false;
    }
    if (pos == 1) {
        addHead(s);
    } else {
        Node* newNode = new Node(s);
        Node* current = head;
        for (int i = 1; i < pos - 1; ++i) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        size++;
    }
    return true;
}

// 指定位置删除节点
bool StuList::deletePos(int pos) {
    if (pos < 1 || pos > size) {
        return false;
    }
    Node* temp;
    if (pos == 1) {
        temp = head;
        head = head->next;
    } else {
        Node* current = head;
        for (int i = 1; i < pos - 1; ++i) {
            current = current->next;
        }
        temp = current->next;
        current->next = temp->next;
    }
    delete temp;
    size--;
    return true;
}

// 清空链表
void StuList::clearList() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    size = 0;
}

// 修改指定位置的节点信息
bool StuList::modify(int pos, const Student& s) {
    if (pos < 1 || pos > size) {
        return false;
    }
    Node* current = head;
    for (int i = 1; i < pos; ++i) {
        current = current->next;
    }
    current->student = s;
    return true;
}

// 数据管理函数

// 将链表数据保存到CSV文件
void StuList::saveToFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return; // 打开文件失败
    }

    QTextStream out(&file);
    Node* current = head;
    while (current) {
        // 按逗号分隔格式写入一行
        out << QString::fromStdString(current->student.id) << ","
            << QString::fromStdString(current->student.name) << ","
            << QString::fromStdString(current->student.birth) << ","
            << QString::fromStdString(current->student.path) << ","
            << QString::fromStdString(current->student.hobby) << "\n";
        current = current->next;
    }
    file.close();
}

// 从CSV文件加载数据，从而恢复链表数据
void StuList::loadFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    clearList();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        if (fields.count() == 5) {
            Student s(fields[0].toStdString(),
                      fields[1].toStdString(),
                      fields[2].toStdString(),
                      fields[3].toStdString(),
                      fields[4].toStdString());
            addTail(s);
        }
    }
    file.close();
}
