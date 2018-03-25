#pragma once

#include <vector>

#include <memory>

#include <mutex>

#include <utility>

#include <Windows.h>

using std::vector;
using std::string;
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




namespace {
	struct shmemq_info {
		unsigned long max_count = 0;

		unsigned int read_index = 0;
		unsigned int write_index = 0;
		
		unsigned int element_size = 0;

		unsigned int reader_count = 0;

		char data[1];
	};

}


class MFCMutexLocker{
private:
	HANDLE m_mutex = INVALID_HANDLE_VALUE;
public:
	MFCMutexLocker(HANDLE mutex) : m_mutex(mutex) {
		::WaitForSingleObject(mutex, INFINITE);
	}

	virtual ~MFCMutexLocker() {
		::ReleaseMutex(m_mutex);
	}

};



template<typename T>
class CFiFo :
	public IWriteFiFo<T>,
	public IReadFiFo<T>
{
private:


	volatile bool m_initialized = false;

	unsigned long m_max_count = 2;

	unsigned int m_ready = 0;
	unsigned int m_free = 0;


	unsigned int m_element_size = sizeof(T);





	string m_prefix;

	bool m_need_clear = false;


	struct shmemq_info * m_mem;

	HANDLE	shmem = INVALID_HANDLE_VALUE;
	HANDLE m_mutex = INVALID_HANDLE_VALUE;

	void updateCounters() {
		if (m_mem == nullptr)
			return;

		m_ready = m_mem->write_index - m_mem->read_index;
		m_free =  m_max_count - m_ready;
	}


public:
	CFiFo(unsigned int element_len,
		unsigned long max_count = 2,
		string &prefix = "my_unique_fifo",
		const bool need_clear = false) :
		m_max_count(max_count),
		m_element_size(sizeof(T) * element_len),
		m_prefix(prefix),
		m_need_clear(need_clear)
	{

	}

	bool Init() {

		if (m_initialized)
			return true;

		unsigned long max_size = m_max_count * m_element_size;
		unsigned long mmap_size = max_size + sizeof(shmemq_info) - 1;

		m_mutex = ::CreateMutexA(NULL, FALSE, (string("mutex_" + m_prefix).c_str()));
		if (m_mutex == NULL)
			return false;

		//WaitForSingleObject(m_mutex, INFINITE);

		MFCMutexLocker locker(m_mutex);

		shmem = ::CreateFileMappingA(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			mmap_size,
			string("shared_memory_" + m_prefix).c_str()
		);

		if (shmem == NULL) {

			//::ReleaseMutex(m_mutex);
			return false;
		}
		m_mem = (struct shmemq_info*)::MapViewOfFile(shmem, FILE_MAP_ALL_ACCESS, 0, 0, mmap_size);

		if (m_mem == NULL) {

			//::ReleaseMutex(m_mutex);
			return false;
		}

		DWORD last_error = GetLastError();

		if (last_error == ERROR_ALREADY_EXISTS) {

			if (m_element_size != m_mem->element_size) {
				m_need_clear = false;
				//::ReleaseMutex(m_mutex);
				return false;
			}
			m_max_count = m_mem->max_count;

		}
		else {
			m_mem->max_count = m_max_count;
			m_mem->element_size = m_element_size;
			m_mem->read_index = m_mem->write_index = 0;

			m_mem->reader_count = 0;
		}
		m_initialized = true;
		updateCounters();

		//::ReleaseMutex(m_mutex);
		return true;

	}


	virtual T* GetFree() override {
		if (!m_initialized)
			return nullptr;

		try {
			MFCMutexLocker locker(m_mutex);
			//WaitForSingleObject(m_mutex, INFINITE);


			if (m_mem->write_index - m_mem->read_index >= m_mem->max_count) {
				//::ReleaseMutex(m_mutex);
				return nullptr;
			}

			updateCounters();

			unsigned int pos = (m_mem->write_index % m_mem->max_count) * m_mem->element_size;
			T* res = (T*)&m_mem->data[pos];
			//::ReleaseMutex(m_mutex);

			//new
			
			//new

			return res;

		}
		catch (...) {}

		//::ReleaseMutex(m_mutex);
		return nullptr;
	}


	virtual void AddReady() override {
		if (!m_initialized)
			return;

		try {
			MFCMutexLocker locker(m_mutex);

			m_mem->write_index++;
			//WaitForSingleObject(m_mutex, INFINITE);

			//m_mem->write_index++;
			
		}
		catch (...) {}

		//::ReleaseMutex(m_mutex);
	}


	virtual T* GetReady() override {
		if (!m_initialized)
			return nullptr;

		try {
			MFCMutexLocker locker(m_mutex);
			//WaitForSingleObject(m_mutex, INFINITE);

			if (m_mem->read_index >= m_mem->write_index) {
				//::ReleaseMutex(m_mutex);
				return nullptr;
			}

			updateCounters();

			unsigned int pos = (m_mem->read_index % m_mem->max_count) * m_mem->element_size;
			T* res = (T*)&m_mem->data[pos];

			m_mem->reader_count++;
			//new 
			//m_mem->read_index++;
			//new

			//::ReleaseMutex(m_mutex);

			return res;

		}
		catch (...) {}

		//::ReleaseMutex(m_mutex);
		return nullptr;
	}

	virtual void AddFree() override {
		if (!m_initialized)
			return;

		try {
			MFCMutexLocker locker(m_mutex);
			//WaitForSingleObject(m_mutex, INFINITE);
			m_mem->reader_count--;

			if (m_mem->reader_count < 1) {
				m_mem->read_index++;
			}
		}
		catch (...) {}

		//::ReleaseMutex(m_mutex);
	}

	virtual ~CFiFo() {

		if (m_mem != NULL && m_need_clear) {
			::UnmapViewOfFile(m_mem);

			::CloseHandle(shmem);
			::ReleaseMutex(m_mutex);
			::CloseHandle(m_mutex);
		}
	}

	inline unsigned long getFreeSize() const {

		return m_free;
	}

	inline unsigned long getReadySize() const {
		return m_ready;
	}
	inline unsigned long getSize() const {
		return m_max_count;
	}

};


