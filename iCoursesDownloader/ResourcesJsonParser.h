#pragma once
#include <QTreeWidget>
#include <QStringBuilder>
#include "CourseResourcesTree.h"
class QString;

class ResourcesJsonParser
{
public:
	ResourcesJsonParser(void): failed(false), resultTreeNode(nullptr){};
	void parse(QString filePath);
	void recursionParseObject(QJsonArray &jsonNode, CourseResourcesTree* parentNode, int depth = 0);

	bool verifyJsonFileSucceed(QString filePath);

	QString errorString();
	bool failed;

	CourseResourcesTree *resultTreeNode;
protected:
	QString m_errorText;
	void onParseError(QString errorText);
	static QString fileSizeToString(int bytes);

};

