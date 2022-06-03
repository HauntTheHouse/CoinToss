#pragma once

class DeltaTime
{
public:
    static void calculate() noexcept;
    static double get() noexcept;

private:
    static double mLastTime;
    static double mValue;
};
