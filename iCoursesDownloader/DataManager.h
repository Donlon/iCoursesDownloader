#pragma once

#include <QList>
#include <QDir>
#include "datatypes.h"
#include "HttpManager.h"


namespace StorageManager{
    const int STORAGE_DB_VERSION = 4;
    QList<Course*> loadCourseList();
    void saveCourseList(QList<Course*> *coursesList);
    void saveCourseList(QList<CourseModel*> *courseModlesList);

    bool saveFile(QNetworkReply *reply, QString localRelativePath);

    QString getLocalFilePath(QString relativePath);
    //QString STORAGE_PATH();

    //HTTPDownload* startDownload(QString url, QString localRelativePath);

    //getCourseC...
    namespace Utils{
        QString resourcesStorageAbsolutePath(int courseId, int typeSort);
    }
}