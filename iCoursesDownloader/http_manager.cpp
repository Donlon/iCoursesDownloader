#include <QDir>
#include "HttpManager.h"

HTTPManager HTTPManager::__Instance;
HTTPManager::HTTPManager()
{
    int i=0;
}

QNetworkReply* HTTPManager::get(QString url)
{
    QNetworkRequest request(url);
    return get(request);
}


QNetworkReply* HTTPManager::get(const QNetworkRequest& request)
{
    return __Instance.manager.get(request);
}

QNetworkReply* HTTPManager::post(const QNetworkRequest& request, const QByteArray & data)
{
    return __Instance.manager.post(request, data);
}

HTTPManager* HTTPManager::getInstance()
{
    return &__Instance;
}
/*
void HTTPDownload::saveFileData()
{ 
    qDebug()<<localPath;
    
    QFileInfo fileInfo(localPath);
    
    QDir dir(fileInfo.path());
    if(!dir.exists())
    {
        bool ok = dir.mkpath(fileInfo.path());
    }

    QFile file(localPath);

    if (!file.open(QIODevice::WriteOnly)) {
        emit finish(false);
        return;
    }

    file.write(reply->readAll());
    file.close();

    emit finish(true);
    return;
}
*/