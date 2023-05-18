#ifndef _PUBLIC_TOOL_H
#define _PUBLIC_TOOL_H
#include <thread>
#include <iostream>
#include <mutex>
#include <list>
#include "osa_std.h"
#include <string>
#include <sstream>
class XThread
{
public:
    virtual void Start();

    virtual void Exit();

    virtual void Wait();

    //停止线程（设置退出的标志，等待线程 的退出）
    virtual void Stop();

    //执行任务需要重载
    virtual void Do(void* arg)
    {

    }
    //设置责任链的下一个节点（线程安全）
    //传递到下一个责任链函数
    virtual void Next(void* arg)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        if (m_next)
        {
            m_next->Do(arg);
        }
    }

	void set_next(XThread* xt)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_next = xt;
	}
    void SetName(std::string strName);
	//保护类是让继承者能够进行访问
protected:

	//线程的入口函数（纯虚函数） 要求继承者必须存在（不能实例化对象）
	virtual void Main() = 0;
	//判断线程是否已经推出 杀死线程是十分危险的
	bool m_bIsExit = false;
	//线程的索引号码
	int m_iThreadIndex = 0;
    //int GetThreadId() {return m_iThreadIndex;};

private:
	//c++11的线程对象
	std::string m_Name;
	std::thread m_Thread;
	std::mutex m_Mutex;
	XThread * m_next = nullptr;

};


#define  XLOG(s) \
{ \
    std::stringstream ss; \
    ss <<"Verson:3" <<","<< s << ":" << __LINE__ <<":"<< __FUNCTION__<< ": "<<__FILE__; \
    OSA_Printf("%s \n",ss.str().c_str()); \
} \





#endif
