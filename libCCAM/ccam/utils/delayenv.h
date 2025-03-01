#pragma once

class DelayEnv {
    bool trig = false;
    float elapsed = 1.0f;
    float delta = 1.0f;
    float length = 0.5f;
    float amp = 1.0f;
    float slew = 0.95;
public:
    void Init(float sample_rate);
    void SetLength(float length);
    float Process(bool gate);
};