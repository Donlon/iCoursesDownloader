#include "CourseResourcesSelectionModel.h"

CourseResourcesSelectionModel::~CourseResourcesSelectionModel(void)
{
}

bool CourseResourcesSelectionModel::isSelected(const QModelIndex& index) const{
	return index.internalPointer();
}

void CourseResourcesSelectionModel::setCurrentIndex(const QModelIndex& index,
	QItemSelectionModel::SelectionFlags command)
{
}

void CourseResourcesSelectionModel::select(const QModelIndex& index, QItemSelectionModel::SelectionFlags command)
{
}

void CourseResourcesSelectionModel::select(const QItemSelection& selection, QItemSelectionModel::SelectionFlags command)
{
}

void CourseResourcesSelectionModel::clear()
{
}

void CourseResourcesSelectionModel::reset()
{
}

void CourseResourcesSelectionModel::clearCurrentIndex()
{
}
