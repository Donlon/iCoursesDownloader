#include "DataManager.h"


#define CONST_STORAGE_PATH "E:\\iCourses\\"
QString STORAGE_PATH = QString("E:\\iCourses\\");

//TODO: use curves to generate.
char COURSE_DB_HEADER[17] = {"DonlonDBFile\0\0\r\n"};
char COURSE_DB_HEADER2[17] = {"\x22\x11\x54\xdf\x22\x11\x54\xdf\0\0\0\0\0\0\0\0"};

QList<Course*> StorageManager::loadCourseList(){
    QList<Course*> list;

    QFile file(CONST_STORAGE_PATH"CourseList.dat");

    if (!file.open(QIODevice::ReadOnly)) {
        return list;
    }

    QDataStream in(&file);

    char header[32] = {0};

    if(in.readRawData(header, 32) == -1){
        return list;
    }
    if(memcmp(header, COURSE_DB_HEADER, 16)!=0){
        return list;
    }
    if(memcmp(header + 16, COURSE_DB_HEADER2, 12)!=0){
        return list;
    }

    int dbVer = *reinterpret_cast<int*>(header + 16 + 12);
    if(dbVer != STORAGE_DB_VERSION){
        //TODO: alert
        return list;
    }

    int size = 0;
    in>>size;
    if(size <= 0){
        return list;
    }
    
    Course *t = nullptr;
    for(;size>0;size--){
        t = new Course();
        in>>*t;
        list.append(t);
    }
    return list;
}

void StorageManager::saveCourseList(QList<Course*>* coursesList){
    QFile file(CONST_STORAGE_PATH"CourseList.dat");

    if (!file.open(QIODevice::WriteOnly)){
        return;
    }
    QDataStream out(&file);
    out.writeRawData(COURSE_DB_HEADER, 16);
    *reinterpret_cast<int*>(COURSE_DB_HEADER2 + 12) = STORAGE_DB_VERSION;
    out.writeRawData(COURSE_DB_HEADER2, 16);

    out<<coursesList->size();
    foreach(Course* course, *coursesList){
        out<<*course;
    }
}

void StorageManager::saveCourseList(QList<CourseModel*>* courseModlesList){
    QFile file(CONST_STORAGE_PATH"CourseList.dat");

    if (!file.open(QIODevice::WriteOnly)){
        return;
    }
    QDataStream out(&file);
    out.writeRawData(COURSE_DB_HEADER, 16);
    *reinterpret_cast<int*>(COURSE_DB_HEADER2 + 12) = STORAGE_DB_VERSION;
    out.writeRawData(COURSE_DB_HEADER2, 16);

    out<<courseModlesList->size();
    foreach(CourseModel* courseModel, *courseModlesList){
        out<<*courseModel->courseInfo;
    }
}

bool StorageManager::saveFile(QNetworkReply *reply, QString localRelativePath){
    
    QString localPath = STORAGE_PATH + localRelativePath;

    QFileInfo fileInfo(localPath);
    
    QDir dir(fileInfo.path());

    if(!dir.exists()){
        if(!dir.mkpath(fileInfo.path())){
            return false;
        }
    }

    QFile file(localPath);

    if (!file.open(QIODevice::WriteOnly)){
        return false;
    }

    file.write(reply->readAll());
    file.close();

    return true;
}

QString StorageManager::getLocalFilePath(QString relativePath){
    return STORAGE_PATH + relativePath;
}
