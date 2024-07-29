/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 * Xu Xueyang <xuxueyang@senseauto.com>
 */
#pragma once

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "json11.hpp"

namespace senseAD {
namespace perception {

/**
 * @brief get camera size from vehicle_config
 * @param camera_name camera name
 * @return true / false
 */
bool GetCameraSizeFromCalib(const std::string& camera_name,
                            uint32_t& width,
                            uint32_t& height);

}  // namespace perception
}  // namespace senseAD

class CameraModelBase {
 public:
    CameraModelBase(const std::string& CalibDataPath,
                    const std::string& CameraName);
    virtual ~CameraModelBase(){};

    virtual void ploy2invploy(const Eigen::VectorXd& ploy,
                              Eigen::VectorXd& inv_ploy) {
        funcNotImpl();
    };
    virtual void undistort(const Eigen::MatrixXd& srcIMG,
                           Eigen::MatrixXd& trgIMG) {
        funcNotImpl();
    };

    virtual void calibExtrinsic(const Eigen::MatrixXd& p2d,
                                const Eigen::MatrixXd& p3d_b,
                                Eigen::MatrixXd& T_b2c) {
        funcNotImpl();
    };

    virtual bool unit2image(const Eigen::MatrixXd& p3d, Eigen::MatrixXd& p2d) {
        funcNotImpl();
        return false;
    };
    virtual bool unit2image(const Eigen::Vector3d& p3d, Eigen::Vector2d& p2d) {
        funcNotImpl();
        return false;
    };

    virtual bool image2unit(const Eigen::MatrixXd& p2d, Eigen::MatrixXd& p3d) {
        funcNotImpl();
        return false;
    };
    virtual bool image2unit(const Eigen::Vector2d& p2d, Eigen::Vector3d& p3d) {
        funcNotImpl();
        return false;
    };

    virtual double polyval(const Eigen::VectorXd& param, const double x) = 0;
    virtual Eigen::VectorXd polyval(const Eigen::VectorXd& param,
                                    const Eigen::VectorXd& x) = 0;

    const Eigen::VectorXd getPolyParam() const;
    const Eigen::VectorXd getInvPolyParam() const;
    const Eigen::Matrix2d getAffine() const;
    const Eigen::Matrix2d getInvAffine() const;
    const Eigen::MatrixXd getTransformation() const;
    const Eigen::Vector2d getImageCenter() const;
    const std::string getVersion() const;
    const std::string getSensorName() const;
    const std::string getDeviceType() const;
    const std::string getDistortionType() const;
    const int getSensorID() const;
    const bool isExtExist() const;

    /**
     * @brief judge if a pixel is inside the camera's range
     *
     * @param pixel input array [x, y]
     * @param msg pixel info for error msg
     */
    const bool isPixelValid(const Eigen::Vector2f pixel,
                            const std::string& msg) const;
    const bool isPixelValid(const Eigen::Vector2d pixel,
                            const std::string& msg) const;

    /**
     * @brief judge if a pixel is at photo edge
     *
     * @param pixel input array [x, y]
     * @param score score for given pixel
     * @param edge edge range from each side on the picture
     */
    const void isEdgePixel(const Eigen::Vector2f& pixel,
                           float& score,
                           float& edge) const;

    /**
     * @brief load sigma values from text or binary file
     *  The file path is generated the same as extrinsics.
     */
    const void loadSigmaFile();

    /**
     * @brief Get the Pixel Sigma values
     *
     * @param pixel [u, v] u in range [0, width), v in range [0, height)
     * @param sigma [x, y] sigma_x and sigma_y in ego_car coordinate
     */
    const bool getPixelSigma(const Eigen::Vector2d& pixel,
                             Eigen::Matrix2f& sigma) const;

    /**
     * @brief Get the Pixel Sigma values
     *
     * @param pixel [u, v] u in range [0, width), v in range [0, height)
     * @param sigma [x, y] sigma_x and sigma_y in ego_car coordinate
     */
    static const bool getPositionSigma(const Eigen::Vector3f& position,
                                       Eigen::Matrix2f& sigma);

    enum class CameraType : int { Fisheye = 0, Pinhole = 1, FisheyeKB = 2 };
    const CameraType getCameraType() const;

    int getWidth() { return width_; };
    int getHeight() { return height_; };

 protected:
    Eigen::VectorXd poly_param_;
    Eigen::VectorXd inv_poly_param_;
    Eigen::Matrix2d aff_;
    Eigen::Matrix2d inv_aff_;
    Eigen::MatrixXd T_cam2world_;
    double xc_, yc_;
    int width_, height_;
    int grid_x_num_, grid_y_num_;
    std::string IntrinsicVersion_;
    std::string ExtrinsicVersion_;
    std::string SensorName_;
    std::string DeviceType_;
    std::string DistortionType_;
    std::string ConfigPath_;
    int SensorID_;
    bool isExtExist_;
    bool isPixelSigma_ = false;
    CameraType camera_type_;

    std::shared_ptr<std::vector<std::vector<Eigen::Matrix2f>>> pixel_sigma_;
    static const int grid_pixel_ = 10;

    std::shared_ptr<json11::Json> paraInsJsonPtr = nullptr;
    std::shared_ptr<json11::Json> paraExtJsonPtr = nullptr;
    std::string insFileName;
    std::string extFileName;
    const void funcNotImpl() const;
};
typedef std::shared_ptr<CameraModelBase> CameraModelBasePtr;

class FisheyeCameraModel : public CameraModelBase {
 public:
    FisheyeCameraModel(const std::string& CalibDataPath,
                       const std::string& CameraName);
    virtual ~FisheyeCameraModel(){};

    bool unit2image(const Eigen::MatrixXd& p3d, Eigen::MatrixXd& p2d) override;
    bool unit2image(const Eigen::Vector3d& p3d, Eigen::Vector2d& p2d) override;

    bool image2unit(const Eigen::MatrixXd& p2d, Eigen::MatrixXd& p3d) override;
    bool image2unit(const Eigen::Vector2d& p2d, Eigen::Vector3d& p3d) override;

    double polyval(const Eigen::VectorXd& param, const double x) override;
    Eigen::VectorXd polyval(const Eigen::VectorXd& param,
                            const Eigen::VectorXd& x) override;

 private:
    void polyfit(const std::vector<double>& theta,
                 const std::vector<double>& rho,
                 const int& order,
                 std::vector<double>& coeff);
};

class PinholeCameraModel : public CameraModelBase {
 public:
    PinholeCameraModel(const std::string& CalibDataPath,
                       const std::string& CameraName);
    ~PinholeCameraModel(){};

    bool unit2image(const Eigen::MatrixXd& p3d, Eigen::MatrixXd& p2d) override;
    bool unit2image(const Eigen::Vector3d& p3d, Eigen::Vector2d& p2d) override;

    bool image2unit(const Eigen::MatrixXd& p2d, Eigen::MatrixXd& p3d) override;
    bool image2unit(const Eigen::Vector2d& p2d, Eigen::Vector3d& p3d) override;

    double polyval(const Eigen::VectorXd& param, const double x) override;
    Eigen::VectorXd polyval(const Eigen::VectorXd& param,
                            const Eigen::VectorXd& x) override;

 private:
    std::vector<std::vector<Eigen::Vector2d>> remap_;
};