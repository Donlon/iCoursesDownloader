#pragma once
#include <QTreeWidget>
#include <QStringBuilder>
class QString;

class ResourcesJsonParser
{
public:
	ResourcesJsonParser(void): failed(false){};
	bool parseFileToTreeWidget(QString filePath, QTreeWidget *treeWidget);
	void recursionParseObject(QJsonValue &json, QObject* parentNode, bool isTop);
	static QString fileSizeToString(int bytes);
	bool verifyJsonFileSucceed(QString filePath);

	QString errorString();
	bool failed;
protected:
	QString m_errorText;
	void onParseError(QString errorText);
};

