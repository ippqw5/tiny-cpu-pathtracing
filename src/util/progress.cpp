#include "util/progress.h"

namespace tcpr
{

Progress::Progress(size_t total, std::string label, size_t segments)
    : m_label(std::move(label)), m_total(total == 0 ? 1 : total),
      m_segments(segments == 0 ? 1 : segments),
      m_step(m_total / m_segments),
      m_current(0),
      m_last_segment(-1),
      m_finished(false)
{
    if (m_step == 0)
    {
        m_step = 1;
    }
}

Progress::~Progress()
{
    finish();
}

void Progress::advance(size_t delta, bool print_on)
{
    if (delta == 0)
    {
        return;
    }

    const size_t old_value = m_current.fetch_add(delta);
    const size_t new_value = old_value + delta;

    if (new_value % m_step == 0)
    {
        const size_t segment = new_value / m_step;
        if (segment >= 1 && segment < m_segments && print_on)
        {
            printSegmentLine(segment);
        }
    }
}

void Progress::finish()
{
    bool expected = false;
    if (m_finished.compare_exchange_strong(expected, true))
    {
        printSegmentLine(m_segments);
    }
}

void Progress::printSegmentLine(size_t segment)
{
    std::scoped_lock lk(m_cout_mtx);

    if (std::cmp_less_equal(segment, m_last_segment))
    {
        return;
    }
    m_last_segment = static_cast<long long>(segment);

    const size_t percent = segment * 100 / m_segments;

    // Default logger -> colored stdout, thread-safe, one line per segment.
    spdlog::info("{}: {}%", m_label, percent);
}

} // namespace tcpr
