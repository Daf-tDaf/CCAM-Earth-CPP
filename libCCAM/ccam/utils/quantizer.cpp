#include "quantizer.h"

constexpr std::array<uint8_t, 12> all_scale = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
constexpr std::array<uint8_t, 5> major_penta = { 0, 2, 4, 7, 9 };
constexpr std::array<uint8_t, 5> minor_penta = { 0, 3, 5, 7, 10 };
constexpr std::array<uint8_t, 5> octatonic_even = { 0, 2, 4, 8, 10 };
constexpr std::array<uint8_t, 6> octatonic_odd = { 1, 3, 5, 7, 9, 11 };
constexpr std::array<uint8_t, 8> phrygian_dom = { 0, 1, 4, 5, 7, 8, 10 };
constexpr std::array<uint8_t, 8> lydian_dom = { 0, 2, 4, 6, 7, 9, 10 };

float Quantizer::apply(Scale scale, float note) {
    switch(scale) {
        case Scale::ALL:
            return apply(all_scale, note);
        case Scale::MAJOR:
            return apply(major_penta, note);
        case Scale::MINOR:
            return apply(minor_penta, note);
        case Scale::OCTATONIC_EVEN:
            return apply(octatonic_even, note);
        case Scale::OCTATONIC_ODD:
            return apply(octatonic_odd, note);
        case Scale::LYDIAN_DOM:
            return apply(lydian_dom, note);
        case Scale::PHRYGIAN_DOM:
            return apply(phrygian_dom, note);
        default:
            return note;
    }
    return note;
}
