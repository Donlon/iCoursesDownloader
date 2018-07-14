#include "CourseResourcesModel.h"
#include <QMetaObject>
#include "mainwnd.h"


CourseResourcesModel::CourseResourcesModel(CourseSourcesTree* root, QObject* parent) : QAbstractItemModel(parent){
	treeRoot = root;
}

Qt::ItemFlags CourseResourcesModel::flags(const QModelIndex& index) const{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant CourseResourcesModel::data(const QModelIndex& index, int role) const{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::DisplayRole){
		switch (index.column()){
		case 0:
			return static_cast<CourseSourcesTree*>(index.internalPointer())->visualName;
		case 1:
			return QString("CourseSourcesTree");
		case 2:
			return QString("Col: %1, Row:%1").arg(index.row(), index.column());
		default:
			break;
		}
	}else if (role == Qt::ToolTipRole || role == Qt::WhatsThisRole){
		switch (index.column()){
		case 0:
			return QString("The name of the object.");
		case 1:
			return QString("The name of the class.");
		default:
			break;
		}
	}

	return QVariant();
}

QVariant CourseResourcesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
		return QVariant();
	switch (section){
	case 0:
		return QString("Object");
	case 1:
		return QString("Class");
	case 2:
		return QString("Ds");
	default:
		return QVariant();
	}
}

int CourseResourcesModel::rowCount(const QModelIndex& parent) const
{
	CourseSourcesTree *parentNode;

	if (!parent.isValid())
		parentNode = treeRoot;
	else
		parentNode = static_cast<CourseSourcesTree*>(parent.internalPointer());

	return parentNode->childrenList.size();
}

int CourseResourcesModel::columnCount(const QModelIndex& parent) const
{
	return 3;
}

QModelIndex CourseResourcesModel::index(int row, int column, const QModelIndex& parent) const{
	//QObject* parentObject;
	CourseSourcesTree *parentNode;
	if (!parent.isValid())
		parentNode = treeRoot;
	else
		parentNode = static_cast<CourseSourcesTree*>(parent.internalPointer());
	/*if (row < parentObject->children().count())
		return createIndex(row, column, parentObject->children().at(row));
	else
		return QModelIndex();*/
	Q_ASSERT(row < parentNode->childrenList.size());
	return createIndex(row, column, parentNode->childrenList.at(row));
	//return QModelIndex(row, column, parentNode->childrenList.at(row), this);
}

QModelIndex CourseResourcesModel::parent(const QModelIndex& index) const{
	Q_ASSERT(index.isValid());
	if (!index.isValid())
		return QModelIndex();

	CourseSourcesTree* indexNode = static_cast<CourseSourcesTree*>(index.internalPointer());
	CourseSourcesTree* parentNode = indexNode->parent;

	if (parentNode == treeRoot)
		return QModelIndex();

	CourseSourcesTree* grandParentObject = parentNode->parent;

	return createIndex(grandParentObject->childrenList.indexOf(parentNode), 0, parentNode);
}