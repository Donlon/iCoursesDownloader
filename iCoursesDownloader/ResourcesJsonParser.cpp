#include "ResourcesJsonParser.h"
#include "DataManager.h"
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "CourseResourcesTree.h"

QFile *linksFile = nullptr;
#define _ERROR(a) ;Q_ASSERT_X(false, "ResourcesJsonParser", a);;

void ResourcesJsonParser::parse(QString filePath){
    QFile charptersJsonFile(
        StorageManager::getLocalFilePath(filePath));
    
    if (!charptersJsonFile.open(QIODevice::ReadOnly)){
        return;//TODO: Error alert.
    }

    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(charptersJsonFile.readAll(), &jsonErr);
    
    if(jsonDoc.isNull()){
        _ERROR("JSON is empty.");        
        return;
    }

    if(jsonErr.error != QJsonParseError::NoError){
        _ERROR("QJsonParseError parse error.");
        return;
    }
    if(!jsonDoc.isObject())    {
        _ERROR("JSON format error.");
        return;
    }

    QJsonObject rootObj = jsonDoc.object();
    if(!rootObj.contains("success")){
        _ERROR("JSON format error.");
        return;
    }

    if(!rootObj.value("success").toBool(false))    {
        _ERROR("Request isn't succeed.");
        return;
    }
    
    if(!rootObj.value("data").isArray()){
        _ERROR("JSON format error.");
        return;
    }
    QJsonArray data = rootObj.value("data").toArray();

    CourseResourcesTree *rootTree = new CourseResourcesTree("");//TODO: empty constructor

    recursionParseObject(data, rootTree);

    resultTreeNode = rootTree;
}

//TODO: report invalidated items
void ResourcesJsonParser::recursionParseObject(QJsonArray &jsonArrayNode, CourseResourcesTree *parentNode, int depth){

    foreach (QJsonValue& item, jsonArrayNode){
        if(!item.isObject()){
            continue;
        }
        QJsonObject itemObj = item.toObject();


        if(itemObj.value("childList").isArray() || itemObj.value("resList").isArray()){//Indicating that this node is actually a folder

            //demanding
            if(!itemObj.value("selfId").isString() || !itemObj.value("title").isString()){
                //invalidated
                continue;
            }

            CourseResourcesTree *folderNode = new CourseResourcesTree(parentNode);

            folderNode->index = itemObj.value("selfId").toString().toInt();//TODO: getString function implementation
            folderNode->type = CourseResourcesTree::Folder;
            folderNode->depth = depth + 1;

            if(depth == 0){
                folderNode->visualName = QString::fromLocal8Bit("ตฺ%1ีย ").arg(folderNode->index) + itemObj.value("title").toString();
                folderNode->expandedInTreeView = true;
            }else{
                QJsonValue t = itemObj.value("title");
                folderNode->visualName = QString::fromLocal8Bit("%1.%2 %3").arg(folderNode->parent->index)
                                                                           .arg(folderNode->index)
                                                                           .arg(itemObj.value("title").toString());
                    
            }

            if(!itemObj.value("childList").toArray().empty()){
                recursionParseObject(itemObj.value("childList").toArray(), folderNode, depth + 1);
            }


            //Children which is file entity exist, and create node object for them
            if(itemObj.value("resList").isArray()){
                int resNo = 1;
                foreach (QJsonValue& item, itemObj.value("resList").toArray()){
                    if(!item.isObject()){
                        continue;
                    }
                    QJsonObject resObj = item.toObject();
            
                    CourseResourcesTree *fileNode = new CourseResourcesTree(folderNode);
            
                    if(!resObj.value("title").isString()
                        || !resObj.value("resSize").isString()
                        || !resObj.value("mediaType").isString()
                        || !resObj.value("fullResUrl").isString()){
                        continue;
                    }

                    fileNode->visualName = QString("%1. ").arg(resNo) + resObj.value("title").toString();
                    fileNode->resUrl = resObj.value("fullResUrl").toString();
                    fileNode->setSize(resObj.value("resSize").toString().toInt());
                    fileNode->depth = depth + 1;

                    if(resObj.value("mediaType").toString() == "mp4"){
                        fileNode->type = CourseResourcesTree::Video;
                    }else if(resObj.value("mediaType").toString() == "ppt"){
                        fileNode->type = CourseResourcesTree::Document;
                    }else if(resObj.value("mediaType").toString() == "pdf"){
                        fileNode->type = CourseResourcesTree::Document;
                    }else{
                        fileNode->type = CourseResourcesTree::Other;
                    }

                    resNo++;
                }
            }
        }
        

    }
    
}