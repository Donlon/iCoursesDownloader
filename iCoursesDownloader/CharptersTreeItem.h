#pragma once
#include <QTreeWidgetItem>

class CharptersTreeItem :
	public QObject, 
	public QTreeWidgetItem
{
	Q_OBJECT;
public:
	CharptersTreeItem(QTreeWidget *parent): QTreeWidgetItem(parent), par(nullptr)
	{
		
	};
	CharptersTreeItem(CharptersTreeItem *parent): QTreeWidgetItem(parent), par(parent)
	{
		
	};
	
    inline CharptersTreeItem *parent() const { return par; }

	QString selfId;
private:
	CharptersTreeItem *par;
};

