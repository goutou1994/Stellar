//
// Created by baidu on 2019/3/14.
//

// #include <map>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>

#ifndef STELLAR_ANIMATOR_H
#define STELLAR_ANIMATOR_H

template<typename T, int k>
glm::vec<k, T, glm::highp> mat_multiply(glm::mat<k, k, float, glm::highp> mat, glm::vec<k, T, glm::highp> vec) {
    glm::vec<k, T, glm::highp> result(0);
//    for (int i = 0; i < k; i++) result[i] = T(0);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            result[i] = result[i] + vec[j] * mat[j][i];
        }
    }
    return result;
};

// type T should support +, - , *, /

template <typename T>
class BaseAnimator {
public:
    virtual void addKeyFrame(float time, T value) = 0;
    virtual void generate() { generated = true; }
    virtual T at(float time) = 0;
protected:
    int cts = 0;
    virtual float getTimeFromKeyFrame(int i) = 0;
    bool checkTimeSection(float t, int s, int size) {
        return (s < 0 || getTimeFromKeyFrame(s) <= t)
               && (s >= size || getTimeFromKeyFrame(s + 1) >= t);
    }
    int findTimeSection(float t, int size) {
        int left = 0, right = size - 1;
        while (left < right) {
            int middle = (left + right + 1) / 2;
            if (getTimeFromKeyFrame(middle) > t) {
                right = middle - 1;
            } else {
                left = middle;
            }
        }
        if (getTimeFromKeyFrame(right) > t) return -1;
        return right;
    }
    int getTimeSection(float time, int size) {
        if (checkTimeSection(time, cts, size)) return cts;
        if (checkTimeSection(time, cts + 1, size)) return ++cts;
        return findTimeSection(time, size);
    }
    bool generated = false;
};

template <typename T>
class CubicBezierAnimator : public BaseAnimator<T> {
    struct ext_v;
    struct kf;
    typedef glm::vec<4, ext_v, glm::highp> vec;
public:
    void addKeyFrame(float time, T value) {
        keyFrames.push_back({
            .pos = {time, value},
            .cp = {{time - 1.f, value}, {time + 1.f, value}}
        });
    }
    void addKeyFrame(const kf &k) {
        keyFrames.push_back(k);
    }
    void generate() {
        BaseAnimator<T>::generate();
        sort(keyFrames.begin(), keyFrames.end(), sort_by_time);
        for (int i = 0; i < keyFrames.size() - 1; i++) {
            kf &k = keyFrames[i], &l = keyFrames[i + 1];
            glm::mat4 C = glm::mat4(
                    0, 0, 0, 1,
                    1, 1, 1, 1,
                    0, 0, 1, 0,
                    3, 2, 1, 0
            );
            glm::mat4 B = glm::inverse(glm::transpose(C));
            vec x = {
                    k.pos, l.pos, (k.cp[1] - k.pos) * 3, (l.pos - l.cp[0]) * 3
            };
            curves.push_back(mat_multiply<ext_v, 4>(B, x));
        }
    }

    ext_v getCurveV(int s, float w) {
        vec &c = curves[s];
        return c[0] * w * w * w + c[1] * w * w + c[2] * w + c[3];
    }

    float getCurveW(int s, float t) {
        float left = 0, right = 1;
        while (left < right && abs(left - right) > 1e-3) {
            float middle = (left + right) / 2;
            if (getCurveV(s, middle).t > t) {
                right = middle;
            } else {
                left = middle;
            }
        }
        return (left + right) / 2;
    }

    T at(float t) {
        if (!this->generated) { this->generate(); }
        float s = BaseAnimator<T>::getTimeSection(t, keyFrames.size());
        if (s < 0) return keyFrames[0].pos.v;
        if (s >= keyFrames.size() - 1) return keyFrames.rbegin()->pos.v;
        float w = getCurveW(s, t);
        return getCurveV(s, w).v;
    }
private:
    struct ext_v {
        float t;
        T v;
        ext_v operator+(ext_v b) { return {this->t + b.t, this->v + b.v}; }
        ext_v operator-(ext_v b) { return {this->t - b.t, this->v - b.v}; }
        ext_v operator*(float b) { return {this->t * b, this->v * b}; }
        ext_v operator/(float b) { return {this->t / b, this->v / b}; }
        ext_v() { t = 0; v = T(0); }
        ext_v(float i) { t = i; v = T(i); }
        ext_v(float t, const T &v) : t(t), v(v) {}
    };
    struct kf {
         ext_v pos, cp[2];
    };
    std::vector<kf> keyFrames;
    std::vector<vec> curves;
    static bool sort_by_time(const kf &a, const kf &b) {
        return a.pos.t < b.pos.t;
    }
    float getTimeFromKeyFrame(int i) {
        return keyFrames[i].pos.t;
    }
};



template <typename T, int k>
class PolynomialAnimator : public BaseAnimator<T> {
public:
    void addKeyFrame(float time, T value) {
        keyFrames.emplace_back(time, value);
    }
    void generate() {
        sort(keyFrames.begin(), keyFrames.end(), sort_by_time);
    }
    virtual T at(float time) = 0;
protected:
    struct kf {
        kf(float t, T v) : time(t) { this->v[0] = v; }
        float time;
        T v[k];
    };
    std::vector<kf> keyFrames;
    static bool sort_by_time(const kf &a, const kf &b) {
        return a.time < b.time;
    }
    float getTimeFromKeyFrame(int i) {
        return keyFrames[i].time;
    }
};

template <typename T>
class NaturalCubicAnimator : public PolynomialAnimator<T, 3> {
public:
    using PolynomialAnimator<T, 3>::keyFrames;
    void generate(T v1, T v2) {
        PolynomialAnimator<T, 3>::generate();
        if (keyFrames.size() < 1) return;
        keyFrames[0].v[1] = v1;
        keyFrames[0].v[2] = v2;
        for (int i = 0; i < keyFrames.size() - 1; i++) {
            float t1 = keyFrames[i].time, t2 = keyFrames[i + 1].time;
            glm::mat4 C = glm::mat4(
                    t1 * t1 * t1,   t1 * t1,    t1, 1,
                    3 * t1 * t1,    2 * t1,     1,  0,
                    6 * t1,         2,          0,  0,
                    t2 * t2 * t2,   t2 * t2,    t2, 1
            );
            glm::mat4 B = glm::inverse(glm::transpose(C));
            vec x = {
                    keyFrames[i].v[0], keyFrames[i].v[1], keyFrames[i].v[2], keyFrames[i + 1].v[0]
            };
            curves.push_back(mat_multiply<T, 4>(B, x));

            keyFrames[i + 1].v[1] = 3 * t2 * t2 * curves[i][0] + 2 * t2 * curves[i][1] + curves[i][2];
            keyFrames[i + 1].v[2] = 6 * t2 * curves[i][0] + 2.f * curves[i][1];
        }
    }
    T at(float t) {
        int s = BaseAnimator<T>::getTimeSection(t);
        if (s < 0) return keyFrames[0].v[0];
        if (s >= keyFrames.size() - 1) return keyFrames.rbegin()->v[0];
        return t * t * t * curves[s][0] + t * t * curves[s][1] + t * curves[s][2] + curves[s][3];
    }
private:
    typedef glm::vec<4, T, glm::highp> vec;
    std::vector<vec> curves;
};


#endif //STELLAR_ANIMATOR_H
