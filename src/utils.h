#pragma once
#include "core.h"


Module z
{
    template <typename T = f>
    constexpr inline T clamp(const T& expr, const T& low, const T& high)
    {
        return (expr < low) ? low :
            (expr > high) ? high :
            expr;
    }


    template <typename T>
    inline void limit(T& expr, const T& low, const T& high)
    {
        // fast path: already valid, return immediately
        if (expr >= low && expr <= high) return;

        // clamp only when needed
        expr = (expr < low) ? low : high;
    }


    constexpr inline bool printable(uchar key) {
        return (key >= 32 && key <= 126);
    }


    template<typename T, uint64 N>
    void insert_for_stdArray(std::array<T, N>& arr, size_t index, const T& value) {
        // Make space by shifting right from the end
        for (size_t i = N - 1; i > index; --i) {
            arr[i] = arr[i - 1];
        }
        arr[index] = value;
    }

};



template <typename StorageType, typename T>
class Grid
{
private:
    StorageType data;

public:
    Grid() {
        if constexpr (std::is_same_v<StorageType, std::vector<std::vector<T>>>) {
            constexpr size_t height = 5;  // example fixed height
            constexpr size_t width = 8;   // example fixed width
            data.resize(height);
            for (auto& row : data)
                row.resize(width);
        }
    }

    uchar& operator()(size_t x, size_t y) {
        return data[y][x];
    }

    const uchar& operator()(size_t x, size_t y) const {
        return data[y][x];
    }

    // Return total number of elements
    constexpr uint64 size() const {
        if constexpr (std::is_same_v<StorageType, std::array<std::array<T, 8>, 5>>) {
            return 8 * 5;
        } else if constexpr (std::is_same_v<StorageType, std::vector<std::vector<T>>>) {
            return data.size() * (data.empty() ? 0 : data[0].size());
        } else {
            return 0;
        }
    }

    // Get width (number of columns)
    uint64 getWidth() const {
        if constexpr (std::is_same_v<StorageType, std::array<std::array<T, 8>, 5>>) {
            return 8;
        } else if constexpr (std::is_same_v<StorageType, std::vector<std::vector<T>>>) {
            return data.empty() ? 0 : data[0].size();
        } else {
            return 0;
        }
    }

    // Get height (number of rows)
    uint64 getHeight() const {
        if constexpr (std::is_same_v<StorageType, std::array<std::array<T, 8>, 5>>) {
            return 5;
        } else if constexpr (std::is_same_v<StorageType, std::vector<std::vector<T>>>) {
            return data.size();
        } else {
            return 0;
        }
    }
};




// FIXED Strvec class (replace in utils.h):
class Strvec
{
public:
    std::vector<string> data;

    char& operator()(uint x, uint y) {
        // Expand vertically if needed
        if (y >= data.size())
            data.resize(y + 1);

        // Expand horizontally if needed
        if (x >= data[y].size())
            data[y].resize(x + 1, '\0'); // Use '\0' instead of ' ' for empty cells

        return data[y][x];
    }

    const char& operator()(uint x, uint y) const {
        static const char dummy = '\0';
        if (y >= data.size() || x >= data[y].size())
            return dummy;
        return data[y][x];
    }

    string& at(uint i) {
        return data.at(i);
    }

    const string& at(uint i) const {
        return data.at(i);
    }
    
    // FIXED: Return maximum width across all rows
    uint64 getWidth() const {
        if (data.empty()) return 100; // Return reasonable default
        
        uint64 max_width = 0;
        for (const auto& row : data) {
            max_width = std::max(max_width, (uint64)row.size());
        }
        return std::max(max_width, (uint64)100); // Ensure minimum width
    }

    uint64 getHeight() const {
        return std::max((uint64)data.size(), (uint64)50); // Ensure minimum height
    }
};




template <typename u_type = uint, uint _Size = 2>
class Uvec
{
private:
    std::array<u_type, _Size> data;

public:
    using type = u_type;

    static_assert(_Size >= 2, "Uvec requires at least 2 elements.");

    type& x;
    type& first;
    type& y;
    type& second;


//  Constructors :
    Uvec()
    : x(data[0]), first(data[0]),
      y(data[1]), second(data[1])
    {}

    Uvec(const type _x, const type _y)
    : x(data[0]), first(data[0]),
    y(data[1]), second(data[1])
    {
        data[0] = _x;
        data[1] = _y;
    }


    // get-size, get-value, set-value, push-value  - methods:

    uint64 size() const { return _Size; }

    auto& get(const uint64& index) { return data[index]; }
    const auto& get(const uint64& index) const { return data[index]; }

    void set(const uint64& index, const type& value) { data[index] = value; }

    void push(const uint64& index, const type& value) {
        z::insert_for_stdArray(data, index, value);
    }
};


