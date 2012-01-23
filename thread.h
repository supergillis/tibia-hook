#ifndef THREADS_H_
#define THREADS_H_

#include <pthread.h>

class Thread {
public:
	virtual ~Thread();

	virtual void run() = 0;

	void start();
	void request_stop();
	void cancel();

	bool requested_stop();

private:
	static pthread_t create(Thread* thread);
	static void* run(void* ptr);

	pthread_t _id;
	bool _started;
	bool _requested_stop;
};

#endif /* THREADS_H_ */
