#include "ResourcesJsonParser.h"
#include "DataManager.h"
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "CourseResourcesTree.h"

QFile *linksFile = nullptr;
#define _ERROR(a) ;Q_ASSERT_X(false, "ResourcesJsonParser", a);;

void ResourcesJsonParser::parse(QString filePath){
	/*linksFile = new QFile(StorageManager::getLocalFilePath(filePath)+"_links");
	
    if (!linksFile->open(QIODevice::WriteOnly)) {
		return false;
    }*/

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
	if(!jsonDoc.isObject())	{
		_ERROR("JSON format error.");
		return;
	}

	QJsonObject rootObj = jsonDoc.object();
	if(!rootObj.contains("success")){
		_ERROR("JSON format error.");
		return;
	}

	if(!rootObj.value("success").toBool(false))	{
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

//TODO£ºreport invalidated items
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
				folderNode->visualName = QString::fromLocal8Bit("µÚ%1ÕÂ ").arg(folderNode->index) + itemObj.value("title").toString();
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
						|| !resObj.value("mediaType").isString()){
						continue;
					}

					fileNode->visualName = QString("%1. ").arg(resNo) + resObj.value("title").toString();
					fileNode->size = resObj.value("resSize").toString().toInt();
					//fileNode->  resObj.value("mediaType").toString());
					fileNode->type = CourseResourcesTree::Other;
					fileNode->depth = depth + 1;

					resNo++;
				}
			}
		}
		

	}
	
}

/*QTreeWidgetItem* convertJsonItemToWidgetItem(QJsonObject &json){
    QTreeWidgetItem *item = new QTreeWidgetItem();
	item->setText(0, json.value("title").toString());
	item->setText(1, json.value("title").toString());
}
*/


QString ResourcesJsonParser::fileSizeToString(int bytes){
	static QStringList unitStr;
	unitStr<<"B"<<"KB"<<"MB"<<"GB";
	int level = 0;
	float bytesFloat = bytes;
	while(level<unitStr.size()&&bytesFloat>=1024.0f){
		bytesFloat/=1024.0f;
		level++;
	}
	return ("%1 " + unitStr.at(level)).arg(bytesFloat);
}
