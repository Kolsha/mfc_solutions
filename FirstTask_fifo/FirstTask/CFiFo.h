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
class CFiFo :
	public IWriteFiFo<T>,
	public IReadFiFo<T>
{
private:
	size_t m_size = 2;
	size_t m_free_size = 2;
	size_t m_ready_size = 0;

	list<T>  *m_elements = nullptr;
	list<T*> *m_free = nullptr;
	list<T*> *m_ready = nullptr;

	std::mutex m_free_mtx, m_ready_mtx;
public:
	template<class ...Args>
	CFiFo(size_t size = 2, Args... args) :
		m_size(size),
		m_free_size(size),
		m_elements(new list<T>(size)),
		m_free(new list<T*>(size)),
		m_ready(new list<T*>(size))
	{
		for (size_t i = 0; i < size; i++) {

			m_elements->emplace(m_elements->begin(), std::forward<decltype(args)>(args)...);
			m_free->push_front((&m_elements->front()));
		}
	}


	virtual T* GetFree() override {
		std::lock_guard<std::mutex> lock_free(m_free_mtx);
		if (m_free_size < 1) {
			return nullptr;
		}

		return m_free->front();
	}
	virtual void AddReady() override {
		std::lock_guard<std::mutex> lock_ready(m_ready_mtx), lock_free(m_free_mtx);

		auto it = m_ready->begin();
		std::advance(it, m_ready_size);
		m_ready->insert(it, m_free->front());
		m_free->pop_front();

		m_free_size--;
		m_ready_size++;
	}


	virtual T* GetReady() override {
		std::lock_guard<std::mutex> lock_ready(m_ready_mtx);
		if (m_ready_size < 1) {
			return nullptr;
		}

		return m_ready->front();
	}
	virtual void AddFree() override {
		std::lock_guard<std::mutex> lock_ready(m_ready_mtx), lock_free(m_free_mtx);


		auto it = m_free->begin();
		std::advance(it, m_free_size);
		m_free->insert(it, m_ready->front());

		m_ready->pop_front();

		m_free_size++;
		m_ready_size--;
	}

	virtual ~CFiFo() {
		std::lock_guard<std::mutex> lock_ready(m_ready_mtx), lock_free(m_free_mtx);
		m_elements->resize(0);
		delete m_elements;
		delete m_ready;
		delete m_free;
	}

	inline size_t getFreeSize() const {
		return m_free_size;
	}

	inline size_t getReadySize() const {
		return m_ready_size;
	}
	inline size_t getSize() const {
		return m_size;
	}
};


