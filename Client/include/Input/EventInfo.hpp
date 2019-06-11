#pragma once

struct EventInfo
{
    EventInfo() { m_code = 0; }
    EventInfo(int event) { m_code = event; }
    union { int m_code; };
};
