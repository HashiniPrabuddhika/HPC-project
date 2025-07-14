int clamp(int val) {
    return val < 0 ? 0 : (val > 255 ? 255 : val);
}