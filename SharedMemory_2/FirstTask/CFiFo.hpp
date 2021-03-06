#pragma once

#include <Windows.h>

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

		unsigned int write_index = 0;
		
		unsigned int element_size = 0;

		unsigned int client_count = 0;

		char data[1];
	};

}


class MFCMutexLocker{
private:
	HANDLE m_mutex = INVALID_HANDLE_VALUE;
public:
	MFCMutexLocker(HANDLE mutex) : m_mutex(mutex) {
		if (m_mutex != INVALID_HANDLE_VALUE) {
			::WaitForSingleObject(mutex, INFINITE);
		}
	}

	virtual ~MFCMutexLocker() {
		if (m_mutex != INVALID_HANDLE_VALUE) {
			::ReleaseMutex(m_mutex);
		}
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

	/*unsigned int m_ready = 0;
	unsigned int m_free = 0;
	*/

	unsigned int m_element_size = sizeof(T);

	unsigned int m_read_index = 0;

	unsigned int m_write_index = 0;





	string m_prefix;

	bool m_need_clear = false;


	struct shmemq_info * m_mem;

	HANDLE	shmem = INVALID_HANDLE_VALUE;
	HANDLE m_mutex = INVALID_HANDLE_VALUE;

	void updateCounters() {
		if (m_mem == nullptr)
			return;
		m_write_index = m_mem->write_index;
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

		// Global\\

		unsigned long max_size = m_max_count * m_element_size;
		unsigned long mmap_size = max_size + sizeof(shmemq_info) - 1;

		m_mutex = ::CreateMutexA(NULL, FALSE, (string("mutex_" + m_prefix).c_str()));
		if (m_mutex == NULL)
			return false;

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
			return false;
		}
		m_mem = (struct shmemq_info*)::MapViewOfFile(shmem, FILE_MAP_ALL_ACCESS, 0, 0, mmap_size);

		if (m_mem == NULL) {
			return false;
		}

		DWORD last_error = GetLastError();

		if (last_error == ERROR_ALREADY_EXISTS) {

			if (m_element_size != m_mem->element_size) {
				m_need_clear = false;
				return false;
			}
			m_max_count = m_mem->max_count;
			m_read_index = m_mem->write_index;

		}
		else {
			m_mem->max_count = m_max_count;
			m_mem->element_size = m_element_size;
			m_mem->client_count = m_read_index = m_mem->write_index = 0;
		}

		m_mem->client_count++;

		m_initialized = true;
		updateCounters();
		return true;

	}


	virtual T* GetFree() override {
		if (!m_initialized)
			return nullptr;

		try {
			MFCMutexLocker locker(m_mutex);

			if (m_mem->write_index - m_read_index >= m_mem->max_count) {
				m_read_index = m_mem->write_index;
				return nullptr;
			}

			updateCounters();

			unsigned int pos = (m_mem->write_index % m_mem->max_count) * m_mem->element_size;
			T* res = (T*)&m_mem->data[pos];

			return res;

		}
		catch (...) {}

		return nullptr;
	}


	virtual void AddReady() override {
		if (!m_initialized)
			return;

		try {
			MFCMutexLocker locker(m_mutex);
			m_mem->write_index++;
			
		}
		catch (...) {}
	}


	virtual T* GetReady() override {
		if (!m_initialized)
			return nullptr;

		try {
			MFCMutexLocker locker(m_mutex);

			

			if (m_read_index >= m_mem->write_index) {
				return nullptr;
			}

			if (m_mem->write_index - m_read_index >= m_mem->max_count) {
				m_read_index = m_mem->write_index;
			}

			updateCounters();

			unsigned int pos = (m_read_index % m_mem->max_count) * m_mem->element_size;
			T* res = (T*)&m_mem->data[pos];


			return res;

		}
		catch (...) {}

		return nullptr;
	}

	virtual void AddFree() override {
		if (!m_initialized)
			return;

		try {
			//MFCMutexLocker locker(m_mutex);
			m_read_index++;
		}
		catch (...) {}

	}

	virtual ~CFiFo() {

		if (m_mem != NULL) {
			MFCMutexLocker locker(m_mutex);


			m_mem->client_count--;

			if (m_need_clear && m_mem->client_count < 1) {

				::UnmapViewOfFile(m_mem);

				::CloseHandle(shmem);
				::ReleaseMutex(m_mutex);
				::CloseHandle(m_mutex);
			}
		}
	}

	inline unsigned int getReadIndex() const {
		return m_read_index;// % m_max_count;
	}

	inline unsigned int getWriteIndex() const {
		return m_write_index;// % m_max_count;
	}
	inline unsigned long getSize() const {
		return m_max_count;
	}

};


