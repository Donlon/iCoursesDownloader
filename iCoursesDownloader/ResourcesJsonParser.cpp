#include "ResourcesJsonParser.h"
#include "StorageManager.h"

#include <QFile>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include "CharptersTreeItem.h"

QFile *linksFile = nullptr;
#define _ERROR(a) ;;

bool ResourcesJsonParser::parseFileToTreeWidget(QString filePath, QTreeWidget *treeWidget)
{
	linksFile = new QFile(StorageManager::getLocalFilePath(filePath)+"_links");
	
    if (!linksFile->open(QIODevice::WriteOnly)) {
		return false;
    }

	QFile charptersJsonFile(
		StorageManager::getLocalFilePath(filePath));
	
    if (!charptersJsonFile.open(QIODevice::ReadOnly)) {
		return false;//TODO: Error alert.
    }

	QJsonParseError jsonErr;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(charptersJsonFile.readAll(), &jsonErr);
	
	if(jsonDoc.isNull()){
		_ERROR("JSON is empty.");		
		return false;
	}

	if(jsonErr.error!=QJsonParseError::NoError)	{
		_ERROR("QJsonParseError parse error.");
		return false;
	}
	if(!jsonDoc.isObject())	{
		_ERROR("JSON format error.");
		return false;
	}

	QJsonObject rootObj = jsonDoc.object();
	if(!rootObj.contains("success")){
		_ERROR("JSON format error.");
		return false;
	}

	if(!rootObj.value("success").toBool(false))	{
		_ERROR("Request didn't succeed.");
		return false;
	}

	
	if(!rootObj.value("data").isArray()){
		_ERROR("JSON format error.");
		return false;
	}
	QJsonArray data = rootObj.value("data").toArray();

    foreach (QJsonValue &v, data){
    	recursionParseObject(v, treeWidget, true);
    }

	return true;
}

void ResourcesJsonParser::recursionParseObject(QJsonValue &json, QObject *parentNode, bool isTop = false){
	if(!json.isObject()){
		Q_ASSERT(false);
		return;
	}

	QJsonObject itemObj = json.toObject();

	CharptersTreeItem* newNodeItem = nullptr;
	if(isTop){
		newNodeItem = new CharptersTreeItem(qobject_cast<QTreeWidget*>(parentNode));
	}else{
		newNodeItem = new CharptersTreeItem(qobject_cast<CharptersTreeItem*>(parentNode));
	}

	if(itemObj.value("selfId").isString()){
		newNodeItem->selfId = itemObj.value("selfId").toString();
	}else	{
		//;
	}

	if(itemObj.value("title").isString()){
		if(isTop){
			newNodeItem->setText(0,
				QString::fromLocal8Bit("ตฺ%1ีย ").arg(newNodeItem->selfId) + itemObj.value("title").toString());
			newNodeItem->setExpanded(true);
		}else{
			newNodeItem->setText(0, 
				(reinterpret_cast<CharptersTreeItem*>(newNodeItem->parent()))->selfId+
				"."+
				newNodeItem->selfId+
				" "+itemObj.value("title").toString());
		}

	}

	if(itemObj.value("childList").isArray()){
		foreach (QJsonValue& item, itemObj.value("childList").toArray()){
			recursionParseObject(item, newNodeItem);
		}
	}

	if(itemObj.value("resList").isArray()){
		int resNo = 1;
		foreach (QJsonValue& item, itemObj.value("resList").toArray()){
			if(!item.isObject()){
				continue;
			}
			
			QTreeWidgetItem* newResItem = new QTreeWidgetItem(newNodeItem);
			QJsonObject resObj = item.toObject();
			
			if(resObj.value("title").isString()){
				newResItem->setText(0, QString("%1. ").arg(resNo) + resObj.value("title").toString());
			}
			if(resObj.value("resSize").isString()){
				newResItem->setText(1, fileSizeToString(resObj.value("resSize").toString().toInt()));
			}
			if(resObj.value("mediaType").isString()){
				newResItem->setText(2, resObj.value("mediaType").toString());
			}
			resNo++;
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
