#ifndef UTIL_PROGRESS_H
#define UTIL_PROGRESS_H

#include "./common.h"

namespace tcpr
{

class Progress
{
public:
    enum class Style
    {
        InlineDots,
        PerLine,
    };

    explicit Progress(
        size_t      total,
        std::string label = "Render Progress",
        size_t      segments = 10,
        Style       style = Style::InlineDots
    );

    ~Progress();

    Progress(const Progress&) = delete;
    Progress& operator=(const Progress&) = delete;
    Progress(Progress&&) = delete;
    Progress& operator=(Progress&&) = delete;

    void advance(size_t delta = 1, bool print_segment = true);

    void finish();

private:
    void printSegmentLine(size_t segment, bool final_newline = false);

    std::string m_label;
    size_t      m_total;
    size_t      m_segments;
    size_t      m_step;
    Style       m_style;

    std::atomic<size_t> m_current;
    long long           m_last_segment;
    std::atomic<bool>   m_finished;
    std::mutex          m_cout_mtx;
};

} // namespace tcpr

#endif // UTIL_PROGRESS_H
