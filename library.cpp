#include "library.h"
#include <new>
#include "CourseManager.h"


StatusType AddCourse(void* DS, int courseID, int numOfClasses) {
	if (courseID <= 0 || DS == nullptr || numOfClasses <= 0) {
		return INVALID_INPUT;
	}
	if ( (((CoursesManager*)DS)->accessCourseData(courseID))) {
		return FAILURE;
	}
	try {
		((CoursesManager*)DS)->addCourse(courseID, numOfClasses);
	}
	catch (std::bad_alloc & ba) {
		ba.what();
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

StatusType WatchClass(void* DS, int courseID, int classID, int time) {
	if (DS == nullptr || classID < 0 || courseID <= 0 || time<=0 ) {
		return INVALID_INPUT;
	}
	LectursArray* res = ((CoursesManager*)DS)->accessCourseData(courseID);
	if (!res) {
		return FAILURE;
	}
	if (classID >= res->Size()) {
		return INVALID_INPUT;
	}
	DoubleDirectionNode* currNode = res->operator[](classID);
	unsigned int prevSum = currNode->sum;
	try {
		AVL_TREE<int, Empty>* SSST = currNode->CoursesSameSumTree->FindInTree(courseID,0);
		SSST->RemoveInTree(classID,0);
		assert(!SSST->FindInTree(classID,0));
		/*int cnt = 0;
		while (cnt < time && currNode->previous != nullptr) {
			cnt++;
			currNode = currNode->previous;
		}
		if (currNode->previous == nullptr && cnt < time) {
				((CoursesManager*)DS)->accessClassCount().addNode(currNode, currNode->sum + time);
				currNode = currNode->previous;
		}*/
		while (currNode != nullptr) {
			if (currNode->sum == prevSum + time) {
				break;
			}
			if (currNode->sum < prevSum + time) {
				currNode = currNode->previous;
				continue;
			}
			if (currNode->sum > prevSum + time) {
				((CoursesManager*)DS)->accessClassCount().addNode(currNode->next, prevSum + time);
				currNode = currNode->next;
				break;
			}
		}
		DoubleDirectionNode* start = ((CoursesManager*)DS)->accessClassCount().getStart();
		if (currNode == nullptr && start->sum != prevSum + time &&start->sum<prevSum+time) {
			((CoursesManager*)DS)->accessClassCount().addNode(start, prevSum + time);
			currNode = ((CoursesManager*)DS)->accessClassCount().getStart();
		}

		res->operator[](classID) = currNode;

		SSST = currNode->CoursesSameSumTree->FindInTree(courseID,0);
		Empty* it = new Empty();
		if (SSST) {
assert(!SSST->FindInTree(classID,0));
			SSST->putNodeInMyTree(0,classID, it,0);
		}
		else {//then courseID wasn't found
			AVL_TREE<int, Empty>* newNode = new AVL_TREE<int, Empty>();
			currNode->CoursesSameSumTree->putNodeInMyTree(0,courseID, newNode,0);
			SSST = currNode->CoursesSameSumTree->FindInTree(courseID,0);
			SSST->putNodeInMyTree(0,classID, it,0);
		}
	}
	catch (std::bad_alloc & ba) {
		ba.what();
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

StatusType GetMostViewedClasses(void* DS, int numOfClasses, int* courses, int* classes) {
	if (!DS || numOfClasses <= 0 || !courses || !classes) {
		return INVALID_INPUT;
	}
	if (numOfClasses > ((CoursesManager*)DS)->getnum()) return FAILURE;
	DoubleDirectionNode* it = ((CoursesManager*)DS)->accessClassCount().getStart();
	int counter = numOfClasses, current = 0;
	while (it != nullptr && counter > 0)
	{
		Stack< AVL_TREE<int, AVL_TREE<int, Empty>>::AVLNode <int, AVL_TREE<int, Empty>>* > stack(numOfClasses);
		AVL_TREE<int, AVL_TREE<int, Empty>>* tree = it->CoursesSameSumTree;
		AVL_TREE<int, AVL_TREE<int, Empty>>::AVLNode <int, AVL_TREE<int, Empty>>* ptr = tree->Root;
		while (1) {//inOrder without recursion !
			while (ptr != nullptr) {
				stack.push(ptr);
				ptr = ptr->left_son;
			}
			if (stack.isEmpty()) {
				break;
			}
			//ptr=stack.pop();
			ptr = stack.data[--stack.nextIndex];
			AVL_TREE<int, Empty>* classesTree = ptr->m_Data;
			classesTree->internalInOrder(classesTree->Root, courses, classes, &counter, &current, ptr->m_Key);
			ptr = ptr->right_son;
		}
		it = it->next;
	}
	return counter == 0 ? SUCCESS : FAILURE;
}

void* Init() {
	auto* DS = new CoursesManager();
	return (void*)DS;
}

StatusType RemoveCourse(void* DS, int courseID) {
	if (DS == nullptr || courseID <= 0) {
		return INVALID_INPUT;
	}

	LectursArray* res = ((CoursesManager*)DS)->accessCourseData(50808);
	if (!res) {

	}

	/**LectursArray**/ res = ((CoursesManager*)DS)->accessCourseData(courseID);
	if (!res) {
		return FAILURE;
	}
	((CoursesManager*)DS)->RemoveCourse_M(courseID);
	return SUCCESS;
}

StatusType TimeViewed(void* DS, int courseID, int classID, int* timeViewed) {
	if (courseID <= 0 || DS == nullptr || classID < 0) {
		return INVALID_INPUT;
	}

	if (!(((CoursesManager*)DS)->accessCourseData(courseID))) {
		return FAILURE;
	}
	LectursArray* res = ((CoursesManager*)DS)->accessCourseData(courseID);
	if (classID >= res->Size()) {
		return INVALID_INPUT;
	}
	*timeViewed = res->operator[](classID)->sum;
	return SUCCESS;
}

void Quit(void** DS) {
	delete* (CoursesManager**)DS;
	//delete *DS;

	*DS = nullptr;
}