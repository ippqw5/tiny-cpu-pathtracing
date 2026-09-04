#ifndef UTIL_PROGRESS_H
#define UTIL_PROGRESS_H

#include <atomic>
#include <cstddef>
#include <mutex>
#include <string>

namespace tcpr
{

// ============================================================================
// Progress —— 同一行覆盖式进度条（线程安全）
//
// 输出效果（始终在同一行用 \r 覆盖刷新，不主动换行）：
//   构造：   \rRender Progress: 0%
//   10% :   \rRender Progress: 0% ... 10%
//   20% :   \rRender Progress: 0% ... 10% ... 20%
//   结束：  \rRender Progress: 0% ... 10% ... ... 100%\n
//
// 用法：
//   Progress p(total, "Render Progress", 10);  // 立即输出 0%
//   p.advance(1);                               // 每完成 1 份任务调用一次
//   // 离开作用域时，析构函数自动补齐 100% 并换行
// ============================================================================
class Progress
{
public:
    explicit Progress(size_t total, std::string label = "Render Progress", size_t segments = 10);
    ~Progress();

    Progress(const Progress&)            = delete;
    Progress& operator=(const Progress&) = delete;
    Progress(Progress&&)                 = delete;
    Progress& operator=(Progress&&)      = delete;

    // 向前推进 delta 个任务单位；跨过段边界时自动在同一行刷新进度
    void advance(size_t delta = 1);

    // 显式结束：补齐 100% 并换行；可重复调用（只会真正执行一次）
    void finish();

private:
    // 在互斥锁内打印到第 segment 段（segment ∈ [0, m_segments]）
    // final_newline = true 时末尾附 '\n'（仅在 finish 时为 true）
    void printSegmentLine(size_t segment, bool final_newline = false);

    std::string        m_label;
    size_t             m_total;
    size_t             m_segments;
    size_t             m_step;       // 每个段对应的任务数量 = total / segments

    std::atomic<size_t> m_current;   // 已完成任务数（原子 + fetch_add 归属）

    // 以下成员仅在 m_cout_mtx 锁定期间读写
    long long         m_last_segment;  // 已打印到的段序号（-1 表示未开始打印）
    std::atomic<bool> m_finished;      // finish() 是否已执行
    std::mutex        m_cout_mtx;      // 串行化 std::cout 写入
};

} // namespace tcpr

#endif // UTIL_PROGRESS_H
