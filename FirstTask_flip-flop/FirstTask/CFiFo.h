#pragma once

#include <list>

#include <memory>

#include <mutex>

#include <utility>

using std::list;
template<typename T> using s_ptr = std::shared_ptr<T>;



template<typename T>
class IWriteFiFo
{
public:
	IWriteFiFo() {}

	virtual T* GetFree() = 0;
	virtual void AddReady() = 0;

	virtual ~IWriteFiFo() {}
};



template<typename T>
class IReadFiFo
{
public:
	IReadFiFo() {}

	virtual T* GetReady() = 0;
	virtual void AddFree() = 0;

	virtual ~IReadFiFo() {};
};






template<typename T>
class CFiFo : public IWriteFiFo<T>,
	public IReadFiFo<T>
{
private:
	size_t count;

	list<T> *elements = nullptr;
	list<T*> *free = nullptr;
	list<T*> *ready = nullptr;

	std::mutex els_mtx, free_mtx, ready_mtx;

	size_t free_size = 0;
	size_t ready_size = 0;


public:
	template<class ...Args>
	CFiFo(size_t _count = 2, Args... args) :
		count(_count),
		free_size(_count),
		elements(new list<T>(count)),
		free(new list<T*>(count)),
		ready(new list<T*>(count))
	{
		for (size_t i = 0; i < count; i++) {

			elements->emplace(elements->begin(), std::forward<decltype(args)>(args)...);
			free->push_front((&elements->front()));
		}
	}

	virtual T* GetFree() override {
		std::lock_guard<std::mutex> lock_free(free_mtx);
		if (free_size < 1) {
			return nullptr;
		}

		//free_size--;

		return free->front();
	}
	virtual void AddReady() override {
		std::lock_guard<std::mutex> lock_ready(ready_mtx), lock_free(free_mtx);

		auto it = ready->begin();
		std::advance(it, ready_size);
		ready->insert(it, free->front());
		free->pop_front();

		free_size--;
		ready_size++;
	}


	virtual T* GetReady() override {
		std::lock_guard<std::mutex> lock_ready(ready_mtx);
		if (ready_size < 1) {
			return (nullptr);
		}

		//ready_size--;

		return ready->front();
	}
	virtual void AddFree() override {
		std::lock_guard<std::mutex> lock_ready(ready_mtx), lock_free(free_mtx);
		if (ready_size > 1 || count < 3) {

			auto it = free->begin();
			std::advance(it, free_size);
			free->insert(it, ready->front());

			ready->pop_front();

			free_size++;
			ready_size--;
		}
	}

	virtual ~CFiFo() {
		std::lock_guard<std::mutex> lock_ready(ready_mtx), lock_free(free_mtx);

		delete elements;
		delete ready;
		delete free;
	};
};


