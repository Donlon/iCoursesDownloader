#include "CourseResourcesManager.h"
#include "ResourcesJsonParser.h"


QMap<int, QList<CourseResourcesTree*>> g_data;
QMap<int, QList<CourseResourcesModel*>> g_models;

void CourseResourcesManager::loadResources(QList<CourseModel*>* courseModelList){
    foreach(CourseModel* modelItem, *courseModelList){
        appendResources(modelItem->courseInfo);
    }
}

void CourseResourcesManager::appendResources(Course* course){
    QList<CourseResourcesTree*> treeList;
    QList<CourseResourcesModel*> modelsList;

    for(int i=0; i < 4/*TODO*/; i++){
        ResourcesJsonParser charptersJsonParser;
        charptersJsonParser.parse(course->resLocalPaths[i]);

        treeList.append(charptersJsonParser.resultTreeNode);
        CourseResourcesModel *m = new CourseResourcesModel(charptersJsonParser.resultTreeNode);
        m->applyFilterType(CourseResourcesModel::FilterType::Videos);
        modelsList.append(m);
    }
        
    g_data.insert(course->id, treeList);
    g_models.insert(course->id, modelsList);
}

CourseResourcesModel* CourseResourcesManager::getListModel(int courseId, int sort){
    if(!g_models.contains(courseId)){
        return nullptr;
    }
    return g_models.value(courseId).at(sort);
}

CourseResourcesTree* CourseResourcesManager::getResourcesTree(int courseId, int sort){
    if(!g_data.contains(courseId)){
        return nullptr;
    }
    return g_data.value(courseId).at(sort);
}
