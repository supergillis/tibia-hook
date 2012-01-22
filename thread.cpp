#include "thread.h"

void Thread::start() {
	if (!_started)
		_id = Thread::create(this);
}

void Thread::request_stop() {
	if(_started)
		_requested_stop = true;
}

void Thread::cancel() {
	if(_started)
		pthread_cancel(_id);
}

bool Thread::requested_stop() {
	return _requested_stop;
}

pthread_t Thread::create(Thread* thread) {
	pthread_attr_t attr;
	pthread_t id;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&id, &attr, Thread::run, reinterpret_cast<void*>(thread));
	return id;
}

void* Thread::run(void* ptr) {
	Thread* thread = reinterpret_cast<Thread*>(ptr);
	if (thread)
		thread->run();
	return ptr;
}
