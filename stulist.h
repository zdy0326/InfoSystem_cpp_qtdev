#ifndef STULIST_H
#define STULIST_H

#include <string>
#include <QString>

using namespace std;

// 学生信息类
class Student {
public:
    string id; // 学生id
    string name; // 姓名
    string birth; // 生日
    string path; // 照片路径
    string hobby; // 兴趣

    Student(const string& id = "",
            const string& name = "",
            const string& birth = "",
            const string& path = "",
            const string& hobby = "");
};

// 节点类
class Node {
public:
    Student student;
    Node* next;
    Node(const Student s); // 构造函数
};

// 学生链表类
class StuList {
private:
    Node* head;
    int size;

public:
    StuList(); // 构造函数
    ~StuList(); // 析构函数

    Node* getHead() const;
    Student* getStudent(int pos) const;

    int count() const;

    void addHead(const Student& s); // 头部插入函数
    void addTail(const Student& s); // 尾部插入函数
    bool addPos(int pos, const Student& s); // 指定位置插入
    bool deletePos(int pos); // 指定位置删除
    void clearList(); // 清除链表数据
    bool modify(int pos, const Student& s); // 修改节点信息

    void saveToFile(const QString& filename); // 保存文件
    void loadFromFile(const QString& filename); // 加载文件
};

#endif
