#pragma once
#include <QList>
#include <QModelIndex>

class CourseResourcesTree
{
public:
	enum ItemType
	{
		Folder,
		Video,
		Document,
		Other
	};
	CourseResourcesTree(QString visualName, CourseResourcesTree* parent) : parent(parent), depth(0), expandedInTreeView(true), index(0), visualName(visualName){
		if(parent){
			parent->addChild(this);
		}
	}

	void clearChildrenSelection();

	CourseResourcesTree(QString visualName) : parent(nullptr), depth(0), expandedInTreeView(true), index(0), visualName(visualName){};
	CourseResourcesTree(CourseResourcesTree* parent) : parent(parent), depth(0), expandedInTreeView(true), index(0){
		if(parent){
			parent->addChild(this);
		}
	};

	~CourseResourcesTree(void);

	CourseResourcesTree *parent;
	int depth;
	ItemType type;
	bool expandedInTreeView;
	bool selectedInTreeView;
	int index;
	QList<CourseResourcesTree*> childrenList;
	QModelIndex parallelModelIndex;
	QString visualName;
	int size;//TODO: function implementation of setSize(int)
	QString resUrl;

protected:
	void addChild(CourseResourcesTree *child){
		if(!childrenList.contains(child)){
			childrenList.append(child);
		}
	}
};

