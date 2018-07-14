#pragma once
#include <QList>

class CourseSourcesTree
{
	enum ItemType
	{
		Folder,
		Video,
		Document,
		Other
	};
public:
	CourseSourcesTree(QString visualName, CourseSourcesTree* parent) : parent(parent), depth(0), expandedInTreeView(true), index(0), visualName(visualName){
		if(parent){
			parent->addChild(this);
		}
	};

	CourseSourcesTree(QString visualName) : parent(nullptr), depth(0), expandedInTreeView(true), index(0), visualName(visualName){};


	~CourseSourcesTree(void);

	CourseSourcesTree *parent;
	int depth;
	ItemType type;
	bool expandedInTreeView;
	bool selectedInTreeView;
	int index;
	QList<CourseSourcesTree*> childrenList;

	QString visualName;

protected:
	void addChild(CourseSourcesTree *child){
		if(!childrenList.contains(child)){
			childrenList.append(child);
		}
	}
};

