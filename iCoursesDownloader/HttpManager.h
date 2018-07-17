#ifndef HTTP_MANAGER_H
#define HTTP_MANAGER_H


#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QUrl>

class HTTPManager
{
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloads;
public:
    HTTPManager();
    static QNetworkReply* get(QString url);
    static QNetworkReply* get(const QNetworkRequest &request);
    static QNetworkReply* post(const QNetworkRequest &request, const QByteArray & data);
    HTTPManager* getInstance();
//private:
    static HTTPManager __Instance;
};

/*
class HTTPDownload:public QObject
{

Q_OBJECT
public:
    HTTPDownload(QNetworkReply* reply, QString localPath): reply(reply), localPath(localPath)
    {
        
    }
    HTTPDownload()
    {
        reply = nullptr;
    }
    QNetworkReply* reply;
    QString localPath;
public slots:
    void saveFileData();
signals:
    void finish(bool succceed);
};
*/
#endif // HTTP_MANAGER_H
