#pragma once
#include <vector>

/*
    Wrapper for a vector which prevents the vector itself from being mutated
    No adding or removing elements
*/

template<typename T>
class ConstVectorWrapper {
public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    ConstVectorWrapper(std::vector<T> *data) {
        this->data = data;
    }

    iterator begin() { return data->begin(); }
    iterator end() { return data->end(); }
    const_iterator cbegin() const { return data->cbegin(); }
    const_iterator cend() const { return data->cend(); }

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }

    size_t size() const { return data->size(); }

private:
    std::vector<T> *data;
};