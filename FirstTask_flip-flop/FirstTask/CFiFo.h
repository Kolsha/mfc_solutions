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
class CFlipFlop :
	public IWriteFiFo<T>,
	public IReadFiFo<T>
{
private:
	size_t m_idx = 0;
	
	T els[2];

public:
	template<class ...Args>
	CFlipFlop(Args... args)
	{
	}

	virtual T* GetFree() override {
		return &els[(m_idx + 1) % 2];
	}
	virtual void AddReady() override {
		idx = (m_idx + 1) % 2;
	}


	virtual T* GetReady() override {
		return &els[m_idx];
	}
	virtual void AddFree() override {
		//idx = (idx + 1) % 2;
	}

	virtual ~CFlipFlop() {
	}
};


