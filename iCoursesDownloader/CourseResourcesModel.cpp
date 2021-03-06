#include "mainwnd.h"
#include "CourseResourcesModel.h"


CourseResourcesModel::CourseResourcesModel(CourseResourcesTree* root, QObject* parent)
    : QAbstractItemModel(parent), filterType(FilterType::All){
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
            return static_cast<CourseResourcesTree*>(index.internalPointer())->visualName;
        case 1:
            switch(static_cast<CourseResourcesTree*>(index.internalPointer())->type)
            {
            case CourseResourcesTree::Video:
                return QString("Video");//TODO: static string
            case CourseResourcesTree::Document:
                return QString("Document");//TODO: static string
            case CourseResourcesTree::Folder:
                return QString("");//TODO: static string
            case CourseResourcesTree::Other:
            default:
                return QString("Other");//TODO: static string
            }
        case 2:
            return static_cast<CourseResourcesTree*>(index.internalPointer())->sizeStr;
        default:
            break;
        }
    }

    return QVariant();
}

QVariant CourseResourcesModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();
    switch (section){
    case 0:
        return QString("Name");
    case 1:
        return QString("Type");
    case 2:
        return QString("Size");
    default:
        return QVariant();
    }
}

int CourseResourcesModel::rowCount(const QModelIndex& parent) const{
    if(!treeRoot){
        return 0;
    }

    CourseResourcesTree *parentNode;
    if (!parent.isValid())
        parentNode = treeRoot;
    else
        parentNode = static_cast<CourseResourcesTree*>(parent.internalPointer());

    parentNode->showableChildrenList.clear();
    foreach(CourseResourcesTree* subNode, parentNode->childrenList){
        if(filterType == FilterType::All){
            parentNode->showableChildrenList.append(subNode);
            continue;
        }

        if(subNode->type == CourseResourcesTree::ItemType::Folder){
            parentNode->showableChildrenList.append(subNode);
        }
        if(subNode->type == CourseResourcesTree::ItemType::Video && filterType == FilterType::Videos){
            parentNode->showableChildrenList.append(subNode);
        }
        if(subNode->type == CourseResourcesTree::ItemType::Document && filterType == FilterType::Documents){
            parentNode->showableChildrenList.append(subNode);
        }
        if(subNode->type == CourseResourcesTree::ItemType::Other && filterType == FilterType::Others){
            parentNode->showableChildrenList.append(subNode);
        }
    }
    return parentNode->showableChildrenList.size();
}

int CourseResourcesModel::columnCount(const QModelIndex& parent) const{
    return 3;
}

QModelIndex CourseResourcesModel::index(int row, int column, const QModelIndex& parent) const{
    //QObject* parentObject;
    CourseResourcesTree *parentNode;
    if (!parent.isValid())
        parentNode = treeRoot;
    else
        parentNode = static_cast<CourseResourcesTree*>(parent.internalPointer());

    if(parentNode->showableChildrenList.empty()){
        return QModelIndex();
    }

    Q_ASSERT(row < parentNode->showableChildrenList.size());
    return createIndex(row, column, parentNode->showableChildrenList.at(row));
}

QModelIndex CourseResourcesModel::parent(const QModelIndex& index) const{
    Q_ASSERT(index.isValid());
    if (!index.isValid())
        return QModelIndex();

    CourseResourcesTree* indexNode = static_cast<CourseResourcesTree*>(index.internalPointer());
    CourseResourcesTree* parentNode = indexNode->parent;

    if (parentNode == treeRoot)
        return QModelIndex();

    CourseResourcesTree* grandParentObject = parentNode->parent;

    return createIndex(grandParentObject->childrenList.indexOf(parentNode), 0, parentNode);
}

void CourseResourcesModel::applyFilterType(FilterType type){
    filterType = type;
    //TODO: emit reflush signal
}
