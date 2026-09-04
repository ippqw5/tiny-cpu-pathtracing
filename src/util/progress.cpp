#include "util/progress.h"

#include <iostream>

namespace tcpr
{

// ============================================================================
// 构造函数：记录参数、计算段步长、立即打印 "Render Progress: 0%"
// ============================================================================
Progress::Progress(size_t total, std::string label, size_t segments)
    : m_label(std::move(label))
    , m_total(total == 0 ? 1 : total)
    , m_segments(segments == 0 ? 1 : segments)
    , m_step(m_total / m_segments)
    , m_current(0)
    , m_last_segment(-1)
    , m_finished(false)
{
    // 总任务量 < 分段数：步长退化为 1，每个任务都触发边界判断
    if (m_step == 0)
        m_step = 1;

    // 初始 0%（构造阶段没有多线程竞争，不需要原子保护，但统一走 printSegmentLine 也安全）
    printSegmentLine(0);
    std::cout << std::flush;
}

// ============================================================================
// 析构函数：兜底调用 finish()，确保至少补打 100% + 换行
// ============================================================================
Progress::~Progress()
{
    finish();
}

// ============================================================================
// advance：推进 delta 个任务单位
//   使用 fetch_add 的返回值确定当前段边界归属：
//   只有 old_value + delta 恰好等于 N * step 的那一次调用才会打印该段节点
// ============================================================================
void Progress::advance(size_t delta)
{
    if (delta == 0) return;

    const size_t old_value = m_current.fetch_add(delta);
    const size_t new_value = old_value + delta;

    // new_value 若恰好是段倍数，则当前线程为该节点的唯一归属者
    // 注意：100% 节点（segment == m_segments）永远不在这里打印，
    //       留给 finish() / 析构函数统一输出，确保一定附带换行符 \n
    if (new_value % m_step == 0)
    {
        const size_t segment = new_value / m_step;
        if (segment >= 1 && segment < m_segments)
            printSegmentLine(segment, false);
    }
}

// ============================================================================
// finish：强制打印到 100% 段并换行；CAS 保证只会真正执行一次
// ============================================================================
void Progress::finish()
{
    bool expected = false;
    if (m_finished.compare_exchange_strong(expected, true))
    {
        printSegmentLine(m_segments, true);
    }
}

// ============================================================================
// printSegmentLine：在互斥锁内构造并写出一整行进度字符串
//   segment      : 本次要打印到的段序号（0 ~ m_segments）
//   final_newline: 是否在尾部加 '\n'（仅 finish/ 析构调用时为 true）
// ============================================================================
void Progress::printSegmentLine(size_t segment, bool final_newline)
{
    std::lock_guard<std::mutex> lk(m_cout_mtx);

    // 已经打印过更后面的段就不要回退（极端调度乱序下的防御）
    if ((long long)segment <= m_last_segment)
        return;
    m_last_segment = (long long)segment;

    std::ostream& os = std::cout;

    // \r 回到行首，整行覆盖写
    os << '\r' << m_label << ": 0%";

    for (size_t s = 1; s <= segment; ++s)
    {
        os << " ... " << (s * 100 / m_segments) << "%";
    }

    if (final_newline)
        os << '\n';

    os << std::flush;
}

} // namespace tcpr
