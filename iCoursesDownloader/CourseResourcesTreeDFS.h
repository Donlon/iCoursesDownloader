#pragma once
#define CourseResourcesDFS_StackMax 6
#define CourseResourcesDFS
#define CourseResourcesDFS_Start(tree) { CourseResourcesTree *_DFS_TREE = (tree);
#define CourseResourcesDFS_A int _DFS_depth = 0;int _DFS_stack[CourseResourcesDFS_StackMax]={0};CourseResourcesTree *_DFS_treeStack[CourseResourcesDFS_StackMax]={0};\
	_DFS_treeStack[0] = _DFS_TREE;_DFS_depth++;\
	while(_DFS_depth > 0){\
		if(_DFS_stack[_DFS_depth] >= _DFS_treeStack[_DFS_depth - 1]->childrenList.size()){\
			_DFS_depth--;_DFS_stack[_DFS_depth]++;continue;\
		}\
		_DFS_treeStack[_DFS_depth] = _DFS_treeStack[_DFS_depth - 1]->childrenList.at(_DFS_stack[_DFS_depth]);\
		if(_DFS_treeStack[_DFS_depth]->type != CourseResourcesTree::Folder){{
#define CourseResourcesDFS_Folder }_DFS_stack[_DFS_depth]++;continue;}if(_DFS_depth == CourseResourcesDFS_StackMax - 1){_DFS_depth--;_DFS_stack[_DFS_depth]++;continue;}{
#define CourseResourcesDFS_END _DFS_depth++;_DFS_stack[_DFS_depth] = 0;} }}

void rename(){
}

void rename2(){

	CourseResourcesDFS_Start(nullptr){
		QString baseDir = "";
	CourseResourcesDFS_A
		/**/
	CourseResourcesDFS_Folder
		//DO_NOTHING
	}CourseResourcesDFS_END

}