#include "CourseResourcesTree.h"


void CourseResourcesTree::clearChildrenSelection()
{
	foreach(CourseResourcesTree *child, childrenList){
		child->selectedInTreeView = false;
		child->clearChildrenSelection();
	}
}

CourseResourcesTree::~CourseResourcesTree(void)
{
}
