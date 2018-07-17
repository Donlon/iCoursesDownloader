#pragma once

#define CourseResourcesDFS_StackMax 6
#define CourseResourcesDFS_Start(tree, stack, treeStack, depth) \
{\
    CourseResourcesTree *_DFS_TREE = (tree);\
    int depth = 0;\
    int stack[CourseResourcesDFS_StackMax] = {0};\
    CourseResourcesTree* treeStack[CourseResourcesDFS_StackMax] = {0};\
\
    int *_DFS_depth = &depth;\
    int *_DFS_stack = stack;\
    CourseResourcesTree** _DFS_treeStack = treeStack;
  //{ //start here
#define CourseResourcesDFS_A \
        _DFS_treeStack[0] = _DFS_TREE;\
        (*_DFS_depth)++;\
        while(*_DFS_depth > 0){\
            if(_DFS_stack[*_DFS_depth] >= _DFS_treeStack[(*_DFS_depth) - 1]->childrenList.size()){\
                (*_DFS_depth)--;\
                _DFS_stack[*_DFS_depth]++;\
                continue;\
            }\
            _DFS_treeStack[*_DFS_depth] = _DFS_treeStack[(*_DFS_depth) - 1]->childrenList.at(_DFS_stack[*_DFS_depth]);\
            if(_DFS_treeStack[*_DFS_depth]->type != CourseResourcesTree::Folder){\
                {
                    //Do about files
#define CourseResourcesDFS_Folder\
                }\
                _DFS_stack[*_DFS_depth]++;\
                continue;\
            }\
            if(*_DFS_depth == CourseResourcesDFS_StackMax - 1){\
                (*_DFS_depth)--;\
                _DFS_stack[*_DFS_depth]++;\
                continue;\
            }\
            {
                //Do about folders
          //} //end here
#define CourseResourcesDFS_END \
            (*_DFS_depth)++;\
            _DFS_stack[*_DFS_depth] = 0;\
        }\
    }\
}