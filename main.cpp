#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <mutex>
#include <thread>
#include "ObjectPool.h"

using namespace std;

void test(void *arg)
{
    objpool<int> *p = reinterpret_cast<objpool<int> *>(arg);

    int *pTmp = p->getobj();

    cout << std::this_thread::get_id() << " " << "Get object with value: " << *pTmp << endl;

    this_thread::sleep_for(std::chrono::seconds(2));

    p->recallobj(pTmp);

    cout << "Return object" << endl;
}

//function object type
struct threadjoin
{
    void operator() (thread * t)
    {
        t->join();
    }
};

int main()
{
    {
        objpool<int> pool_(10);

        vector<thread *> ts;
        for (int i = 0; i < 20; i++)
        {
            thread *tmp = new thread(test, &pool_);
            ts.push_back(tmp);
        }

        //for_each(ts.begin(), ts.end(), threadjoin());

        //lambda expression
        for_each(ts.begin(), ts.end(), [](thread * p){p->join(); });

        for_each(ts.begin(), ts.end(), [](thread * p){delete p; });
    }

    system("pause");
    return 0;
}

