#include "osa_std.h"
#include "public_tool.h"
#include <sstream>
#include <mutex>
#include <pthread.h>


using namespace std;

void XThread::Start()
{
    unique_lock<mutex> lock(m_Mutex);
    static int i = 0;
    i++;
    m_iThreadIndex = i;

    m_bIsExit = false;
    m_Thread = thread(&XThread::Main, this);
    if (!m_Name.empty())
    {
        pthread_setname_np(m_Thread.native_handle(), m_Name.c_str());
    }
    //子线程和主线程进行分离，主 线程就不能在进行控制子线程了 主线程退出的时候不会造成资源的出错
    //但是主线程就没法控制了子线程了
    //这里不进行线程分离
    //m_Thread.detach();
    stringstream ss;
    ss << "XThread::Start ID" <<  m_iThreadIndex <<",Thread name :" << m_Name ;
    OSA_Printf("%s", ss.str().c_str());
}

#if 0
//停止线程（设置退出的标志，等待线程 的退出）
void XThread::StopThread()
{
    stringstream ss;
    ss << "XThread::Stop() Begin" << m_iThreadIndex;
    OSA_Printf("%s", ss.str().c_str());
    m_bIsExit = true;
    if (m_Thread.joinable())
    {
        //等待子线程的退出
        m_Thread.join();
    }

    ss.str("");
    ss << "XThread::Stop() End" << m_iThreadIndex;
    OSA_Printf("%s", ss.str().c_str());
}
#endif

void XThread::Wait()
{
    stringstream ss;
    if (m_Thread.joinable()) //判断线程是否可以等待
        m_Thread.join();     //等待子线程的退出
    ss.str("");
    ss << "XThread::Stop() end" << m_iThreadIndex;
    OSA_Printf("%s \n", ss.str().c_str());
}
//设置线程退出标志
void XThread::Exit()
{
    stringstream ss;
    ss << "XThread::Stop() begin" << m_iThreadIndex;
    OSA_Printf("%s \n", ss.str().c_str());
    m_bIsExit = true;
}

//停止线程（设置退出标志，等待线程退出）
void XThread::Stop()
{
    Exit();
    Wait();
}

void XThread::SetName(std::string strName)
{
    //注意在start 之前进行设置线程的名字
    unique_lock<mutex> lock(m_Mutex);
    if (m_Name.empty())
        m_Name = strName;
}





