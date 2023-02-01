struct Sequence
{
    int step = 0;
    Timer timer;

    void next()
    {
        step += 1;
        timer.reset();
    }

    void previous()
    {
        step -= 1;
        timer.reset();
    }

    void goTo(int newStep)
    {
        step = newStep;
        timer.reset();
    }

    void nextAfterDelay(unsigned long duration)
    {
        if (timer.isFinished(duration))
        {
            next();
        }
    }

    void nextOnCondition(bool condition)
    {
        if (condition)
        {
            next();
        }
    }

    void reset()
    {
        step = 0;
        timer.reset();
    }
};
