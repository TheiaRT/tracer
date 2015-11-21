#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <numeric>
#include <cmath>
#include <sstream>
#include <vector>
#include <initializer_list>

#include "dist/json/json.h"

/* VECTOR TEMPLATE CLASS
 * template from which vector3_t and vector2_t types inherit
 * 
 * N is size of vector
 * T is typename of values (default is double)
 *
 */
template<int N, typename T = double>
struct vector_t {
    std::vector<T> arr;

    // empty constructor;
    // sets all N vals to 0
    vector_t (T val = 0) {
        arr.resize(N);
        for (int i = 0; i < N; i++) {
            arr[i] = val;
        }
    }

    // constructor takes list of form {x1, x2, x3} for any N xs of type T
    // inserts to arr vector in order args were given
    vector_t (std::initializer_list<T> arg_list) : arr(arg_list) {
    }

    // constructor takes vector of type T with N args
    // inserts vals to arr vector in order
    vector_t (std::vector<T> arg_list) : arr(arg_list) {
    }

    double magnitude() {
        double result;
        // folds to get sum of all x*x for all x in vector
        result = std::accumulate(arr.begin(), arr.end(), 0,
                                 // lambda to fold with
                                 [] (double result, T x) { 
                                     return result + (x * x);
                                 });
        return sqrt(result);
    }

    vector_t<N, T> normalize() {
        return *this * (1/magnitude());
    }

    // accepts args for start and end chars (default is '<' and '>')
    std::string to_string(char start = '<', char end = '>') {
        std::ostringstream strs;
        strs << start;

        for (int i = 0; i < N - 1; i++) {
            strs << arr[i] << ", ";
        }

        strs << arr[N-1] << end;
        return strs.str();
    }

    std::string to_json() {
        std::ostringstream strs;
        Json::Value root;
        root["object_type"] = "vector";
        root["length"] = N;
        for (int i = 0; i < N; i++) {
            root["value"].append(arr[i]);
        }
        strs << root;
        return strs.str();
    }

    T dot(const vector_t<N, T> &otherVec) {
        T accum = 0;

        for (int i = 0; i < N; i++) {
            accum += (arr[i] * otherVec.arr[i]);
        }

        return accum;
    }

    double distance_from(const vector_t<N, T> &otherVec) {
        return (*this - otherVec).magnitude();
    }

    vector_t<N, T> operator+(const vector_t<N, T> &otherVec) {
        // new vector to work on; resize to reduce (potential) unnecessary space
        std::vector<T> new_arr;
        new_arr.resize(arr.size());

        // loop to 'zip'
        for (int i = 0; i < N; i++) {
            new_arr[i] = arr[i] + otherVec.arr[i];
        }

        return vector_t<N, T>(new_arr);
    }

    vector_t<N, T> operator-(const vector_t<N, T> &otherVec) {
        // new vector to work on; resize to reduce (potential) unnecessary space
        std::vector<T> new_arr;
        new_arr.resize(arr.size());

        for (int i = 0; i < N; i++) {
            new_arr[i] = arr[i] - otherVec.arr[i];
        }

        return vector_t<N, T>(new_arr);
    }

    vector_t<N, T> operator*(T b) {
        // new vector to work on; resize to reduce (potential) unnecessary space
        std::vector<T> new_arr;
        new_arr.resize(arr.size());

        // maps over vector to get sum of all x*x for all x in vector
        std::transform(arr.begin(), arr.end(), new_arr.begin(),
                       // lambda to fold with; needs '=' in [] to ensure
                       // all bound vars are captured by VALUE
                       [=] (T x) {
                           return x * b;
                       });

        return vector_t<N, T>(new_arr);
    }
};

typedef vector_t<3> vector3_t;
typedef vector_t<2> vector2_t;

struct ray_t {
    vector3_t start, direction;

    ray_t(vector3_t start, vector3_t direction) {
        this->start = start;
        this->direction = direction;
    }
};

#endif