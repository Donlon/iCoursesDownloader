#include "CourseResourcesSelectionModel.h"
#include "CourseResourcesTree.h"

CourseResourcesSelectionModel::~CourseResourcesSelectionModel(void)
{
}

bool CourseResourcesSelectionModel::isSelected(const QModelIndex& index) const{
	return static_cast<CourseResourcesTree*>(index.internalPointer())->selectedInTreeView;
}

void CourseResourcesSelectionModel::setCurrentIndex(const QModelIndex& index,
	QItemSelectionModel::SelectionFlags command)
{
}

void CourseResourcesSelectionModel::select(const QModelIndex& index, QItemSelectionModel::SelectionFlags command)
{
    if (command & Clear) {
        static_cast<CourseResourcesTree*>(index.internalPointer())->selectedInTreeView = false;
    }else
    {
	    static_cast<CourseResourcesTree*>(index.internalPointer())->selectedInTreeView = true;
    };
}

void CourseResourcesSelectionModel::select(const QItemSelection& selection, QItemSelectionModel::SelectionFlags command)
{	//static_cast<CourseResourcesTree*>(index.internalPointer())->selectedInTreeView = true;

	QModelIndexList indexList = selection.indexes();

	if(indexList.size()!=1){
		return;
	}

	if (command & Clear) {
        static_cast<CourseResourcesTree*>(indexList.at(0).internalPointer())->selectedInTreeView = false;
    }else
    {
	    static_cast<CourseResourcesTree*>(indexList.at(0).internalPointer())->selectedInTreeView = true;
    };
}

void CourseResourcesSelectionModel::clear(){
	treeRoot->clearChildrenSelection();
	//TODO: emit.
}

void CourseResourcesSelectionModel::reset()
{
	clear();
}

void CourseResourcesSelectionModel::clearCurrentIndex()
{
}
