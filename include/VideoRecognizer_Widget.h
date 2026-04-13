#ifndef VIDEO_RECOGNIZER_WIDGET_H
#define VIDEO_RECOGNIZER_WIDGET_H

#include <QDialog>
#include <QSettings>
#include <QCheckBox>
#include <QLabel>
#include <optional>
#include <vector>
#include <opencv2/core.hpp>
#include "MatLabel.h"
#include "MovementDetector.h"
#include "QRDetector.h"
#include "VideoRecognizerConfiguration.h"
#include "YOLOv8Model.h"

/// @brief Encapsula la deteccion de video y la configuracion asociada.
class VideoRecognizer_Widget : public QDialog {
    Q_OBJECT

public:
    explicit VideoRecognizer_Widget(
        QWidget* parent,
        MatLabel* mainCameraLabel,
        MatLabel* secondaryCameraLabel,
        QCheckBox* detectQR,
        QCheckBox* detectMovement,
        QCheckBox* detectHazmat,
        QLabel* qrOutput,
        QLabel* movementOutput,
        QLabel* hazmatOutput);

    ~VideoRecognizer_Widget() noexcept;

    void submitFrames(const cv::Mat& mainFrame, const cv::Mat& secondaryFrame);

public slots:
    void showConfigurationDialog();
    void swapCameraViews();

private slots:
    void displayDetectedQRs(QString QRDecoded, cv::Rect boundingRectangle);
    void displayDetectedMovement(const std::vector<cv::Rect>& boundingRectangles);
    void displayDetectedHazmats(std::vector<PredictionsData> predictions);

private:
    MatLabel* mainCameraLabel_m;
    MatLabel* secondaryCameraLabel_m;
    QCheckBox* detectQR_m;
    QCheckBox* detectMovement_m;
    QCheckBox* detectHazmat_m;
    QLabel* qrOutput_m;
    QLabel* movementOutput_m;
    QLabel* hazmatOutput_m;

    QRDetector* qrDetector_m;
    MovementDetector* movementDetector_m;
    YOLOv8Model* hazmatDetector_m;
    VideoRecognizerConfiguration* configurationDialog_m;

    QSettings settings_m;

    cv::Mat lastFrame_m;
    std::optional<cv::Rect> lastQRRect_m;
    std::vector<cv::Rect> lastMovementRects_m;
    bool swapCameras_m{ false };

    void detectQRIfChecked(const cv::Mat& frame);
    void detectMovementIfChecked(const cv::Mat& frame);
    void detectHazmatIfChecked(const cv::Mat& frame);

    void applyConfigurationChanges();
    void loadConfigurations();
    void saveConfigurations();
    void renderFrameWithOverlays();
};

#endif // VIDEO_RECOGNIZER_WIDGET_H
