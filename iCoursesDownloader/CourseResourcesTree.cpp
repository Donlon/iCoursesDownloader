#include "CourseResourcesTree.h"

CourseResourcesTree::~CourseResourcesTree(void)
{
}

void CourseResourcesTree::clearChildrenSelection(){
    foreach(CourseResourcesTree *child, childrenList){
        child->selectedInTreeView = false;
        child->clearChildrenSelection();
    }
}
void CourseResourcesTree::setSize(int bytes) {
    static QStringList unitStr;
    unitStr << "B" << "KB" << "MB" << "GB";
    int level = 0;
    float bytesFloat = bytes;
    while (level<unitStr.size() && bytesFloat >= 1024.0f) {
        bytesFloat /= 1024.0f;
        level++;
    }
    size = bytes;
    sizeStr = ("%1 " + unitStr.at(level)).arg(bytesFloat);
    //return ("%1 " + unitStr.at(level)).arg(bytesFloat);
}

void CourseResourcesTree::addChild(CourseResourcesTree *child) {
    if (!childrenList.contains(child)) {
        childrenList.append(child);
    }
}