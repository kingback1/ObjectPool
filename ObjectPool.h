#ifndef _OBJECTPOOL_H_
#define _OBJECTPOOL_H_

#include <iostream>
#include <list>
#include <mutex>

using namespace std;

template <class T>
class objpool
{
public:

	objpool(int n)
	{
		for (int i = 0; i < n; i++)
		{
			T *tmp = new T();
			l.push_back(tmp);
		}
	}	

	T *getobj()
	{
		//RAII，加锁
		lock_guard<mutex> guard(m_mutex);

		if (l.size() != 0)
		{
			T *tmp = l.front();
			l.pop_front();
			return tmp;
		}
		else
		{
			T *tmp = new T;
			return tmp;
		}
	}

	
	void recallobj(T *tmp)
	{
		lock_guard<mutex> guard(m_mutex);
		l.push_back(tmp);
	}

	inline static void destroy(T* obj)
	{
		delete obj;
	}

	~objpool()
	{
		//直接释放了T的内存，但是list size没变
		for_each(l.begin(), l.end(), destroy);
		//cout << "l.size(): "<< l.size() << endl;
	}

private:
	list<T *> l;
	mutex m_mutex;
};

#endif