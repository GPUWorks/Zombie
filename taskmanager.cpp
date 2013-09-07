#include "taskmanager.h"

#include "task.h"
#include "graphictask.h"

using namespace zombie;

TaskManager::TaskManager() {
	time_ = 0.0;
}

TaskManager::~TaskManager() {	
	for (Task* task : tasks_) {
		delete task;
	}

	for (GraphicTask* task : graphicTasks_) {
		delete task;
	}
}

void TaskManager::add(Task* task) {
	tasks_.push_back(task);
}

void TaskManager::add(GraphicTask* task) {
	graphicTasks_.push_back(task);
}

void TaskManager::update(double deltaTime) {
	// for each Task, call execute
	time_ += deltaTime;

	// Draw all tasks and remove the non drawable.
	graphicTasks_.remove_if([&] (GraphicTask* task) {
		// Is active?
		if (task->draw(time_)) {
			return false;
		}

		// Not active, delete and remove!
		delete task;
		return true;
	});

	// Update all tasks.
	for (Task* task : tasks_) {
		if (task->isRunning()) {
			task->excecute(time_);
			Task* newTask = task->pull();
			if (newTask != 0) {
				// Add task to list.
				tasks_.push_back(newTask);
			}
		}
	}

	auto removeIfFunctionT = [] (Task* task) {
		// Is active?
		if (task->isRunning()) {
			return false;
		}

		// Not active, delete and remove!
		delete task;
		return true;
	};

	// Remove dead tasks.
	tasks_.remove_if(removeIfFunctionT);
}

