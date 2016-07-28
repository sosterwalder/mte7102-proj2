#pragma once

#include <nanogui/object.h>
#include <Eigen/Core>

NAMESPACE_BEGIN(QCE);


class Camera : public nanogui::Object {
public:
    template<class T>
    static Eigen::Matrix<T,4,4> perspective(const T fov, const T aspect, const T near, const T far)
    {
        assert(aspect > 0);
        assert(far > near);

        T fovRad  = (T)nvgDegToRad(fov);
        T halfFov = (T)tan(fovRad * 0.5);

        Eigen::Matrix<T,4,4> res = Eigen::Matrix<T,4,4>::Zero();
        res(0,0) = 1.0 / (aspect * halfFov);
        res(1,1) = 1.0 / (halfFov);
        res(2,2) = -(far + near) / (far - near);
        res(3,2) = -1.0;
        res(2,3) = -(2.0 * far * near) / (far - near);

        return res;
    }

private:
     Camera() {}
    ~Camera() {}
};


NAMESPACE_END(QCE);
