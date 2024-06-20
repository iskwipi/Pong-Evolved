//macros and typedefs
#define global_variable static
#define internal static
typedef unsigned int u32;

//overflow catcher
inline int clamp(int min, int val, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}