#include "shaper.h"

float Shaper::Process(float in)
{
    float out = fabs(in);
    float amp = in > 0.0f ? 1.0f : -1.0f;
    
    out = pow(out, 1.0f / shape);

    if (out >= threshold) {
        out = threshold - out;
    }

    return out * amp;
}
