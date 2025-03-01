#pragma once

#include "Utility/dsp.h"

class Shaper
{
    float threshold = 0.5;
    float shape = 1.0f;

    public:
        float Process(float in);
        
        void SetTresh(float threshold) {
            this->threshold = daisysp::fclamp(threshold, 0.0f, 1.0f);
        }

        void SetShape(float amount) {
            amount = daisysp::fclamp(amount, 0.0f, 1.0f);
            shape = daisysp::fmap(amount, 1.0f, 25.0f);
        }
};