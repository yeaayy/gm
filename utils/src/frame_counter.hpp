#ifndef UTILS_FRAME_COUNTER_HPP
#define UTILS_FRAME_COUNTER_HPP

class FrameCounter
{
public:
    FrameCounter(int frameCount, float fps);
    void reset(int frame = 0);
    void reset(int frameCount, float fps, int frame = 0);
    void setFrameIndex(int frameIndex);
    void setFrameCount(int frameCount);
    void setFPS(float fps);
    int getFrameIndex() const;
    int getFrameCount() const;
    float getFPS() const;
    void update(float dt);

private:
    int index;
    int count;
    float fps;
    float time;
};

#endif /* UTILS_FRAME_COUNTER_HPP */
