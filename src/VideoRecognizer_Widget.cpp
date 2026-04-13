#include "VideoRecognizer_Widget.h"
#include <opencv2/imgproc.hpp>

namespace {
const cv::Scalar kQRRectangleColor{ 255, 0, 255 };
const cv::Scalar kMovementRectangleColor{ 255, 255, 0 };
}

VideoRecognizer_Widget::VideoRecognizer_Widget(
    QWidget* parent,
    MatLabel* mainCameraLabel,
    MatLabel* secondaryCameraLabel,
    QCheckBox* detectQR,
    QCheckBox* detectMovement,
    QCheckBox* detectHazmat,
    QLabel* qrOutput,
    QLabel* movementOutput,
    QLabel* hazmatOutput)
: QDialog{ parent }
, mainCameraLabel_m{ mainCameraLabel }
, secondaryCameraLabel_m{ secondaryCameraLabel }
, detectQR_m{ detectQR }
, detectMovement_m{ detectMovement }
, detectHazmat_m{ detectHazmat }
, qrOutput_m{ qrOutput }
, movementOutput_m{ movementOutput }
, hazmatOutput_m{ hazmatOutput }
, qrDetector_m{ new QRDetector{this} }
, movementDetector_m{ new MovementDetector{this} }
, hazmatDetector_m{ new YOLOv8Model{this} }
, configurationDialog_m{ new VideoRecognizerConfiguration{this} }
, settings_m{ "ControladorRobot", "VideoRecognizer" }
{
    connect(detectMovement_m, &QCheckBox::toggled, this, [this](bool checked) {
        if (!checked) {
            movementOutput_m->clear();
            lastMovementRects_m.clear();
            renderFrameWithOverlays();
        }
    });

    connect(detectQR_m, &QCheckBox::toggled, this, [this](bool checked) {
        if (!checked) {
            qrOutput_m->clear();
            lastQRRect_m.reset();
            renderFrameWithOverlays();
        }
    });

    connect(detectHazmat_m, &QCheckBox::toggled, this, [this](bool checked) {
        if (!checked) {
            hazmatOutput_m->clear();
        }
    });

    connect(qrDetector_m, &QRDetector::QRDecoded, this, &VideoRecognizer_Widget::displayDetectedQRs);
    connect(movementDetector_m, &MovementDetector::movementDetected, this, &VideoRecognizer_Widget::displayDetectedMovement);
    connect(hazmatDetector_m, &YOLOv8Model::predictionsCompleted, this, &VideoRecognizer_Widget::displayDetectedHazmats);

    loadConfigurations();
}

VideoRecognizer_Widget::~VideoRecognizer_Widget() noexcept = default;

void VideoRecognizer_Widget::submitFrames(const cv::Mat& mainFrame, const cv::Mat& secondaryFrame) {
    if (!mainCameraLabel_m) {
        return;
    }

    cv::Mat primaryFrame = swapCameras_m ? secondaryFrame : mainFrame;
    cv::Mat secondaryFrameToShow = swapCameras_m ? mainFrame : secondaryFrame;

    if (primaryFrame.empty() && !secondaryFrameToShow.empty()) {
        primaryFrame = secondaryFrameToShow;
        secondaryFrameToShow = cv::Mat{};
    }

    if (secondaryCameraLabel_m && !secondaryFrameToShow.empty()) {
        secondaryCameraLabel_m->setMat(secondaryFrameToShow);
    }

    if (primaryFrame.empty()) {
        return;
    }

    lastFrame_m = primaryFrame;
    renderFrameWithOverlays();

    detectQRIfChecked(primaryFrame);
    detectMovementIfChecked(primaryFrame);
    detectHazmatIfChecked(primaryFrame);
}

void VideoRecognizer_Widget::showConfigurationDialog() {
    configurationDialog_m->exec();

    if (configurationDialog_m->result() == QDialog::Accepted) {
        applyConfigurationChanges();
        saveConfigurations();
    } else {
        configurationDialog_m->loadConfigurations();
    }
}

void VideoRecognizer_Widget::swapCameraViews() {
    swapCameras_m = !swapCameras_m;

    if (!mainCameraLabel_m || !secondaryCameraLabel_m) {
        return;
    }

    const auto primaryMat = mainCameraLabel_m->getMat();
    const auto secondaryMat = secondaryCameraLabel_m->getMat();

    if (primaryMat.empty() && secondaryMat.empty()) {
        return;
    }

    mainCameraLabel_m->setMat(secondaryMat);
    secondaryCameraLabel_m->setMat(primaryMat);

    lastFrame_m = mainCameraLabel_m->getMat();
    renderFrameWithOverlays();
}

void VideoRecognizer_Widget::displayDetectedQRs(QString QRDecoded, cv::Rect boundingRectangle) {
    if (QRDecoded.isEmpty()) {
        return;
    }

    qrOutput_m->setText(QRDecoded);
    lastQRRect_m = boundingRectangle;
    renderFrameWithOverlays();
}

void VideoRecognizer_Widget::displayDetectedMovement(const std::vector<cv::Rect>& boundingRectangles) {
    if (boundingRectangles.empty()) {
        return;
    }

    lastMovementRects_m = boundingRectangles;
    movementOutput_m->setText("Detectado!");
    renderFrameWithOverlays();
}

void VideoRecognizer_Widget::displayDetectedHazmats(std::vector<PredictionsData> predictions) {
    if (predictions.empty()) {
        return;
    }

    hazmatOutput_m->setText(QString::fromStdString(predictions.front().className));
}

void VideoRecognizer_Widget::detectQRIfChecked(const cv::Mat& frame) {
    if (detectQR_m->isChecked()) {
        qrDetector_m->detectQRsOnMat(frame);
    }
}

void VideoRecognizer_Widget::detectMovementIfChecked(const cv::Mat& frame) {
    if (detectMovement_m->isChecked()) {
        movementOutput_m->clear();
        movementDetector_m->addImage(frame);
    }
}

void VideoRecognizer_Widget::detectHazmatIfChecked(const cv::Mat& frame) {
    if (detectHazmat_m->isChecked()) {
        hazmatDetector_m->predictOnMat(frame);
    }
}

void VideoRecognizer_Widget::applyConfigurationChanges() {
    const auto onnxPath = configurationDialog_m->getONNXPath();
    const auto classesPath = configurationDialog_m->getClassesPath();

    if (!onnxPath.empty() && onnxPath != "~") {
        hazmatDetector_m->loadOnnxNetwork(onnxPath, { 640, 640 }, false);
    }

    if (!classesPath.empty() && classesPath != "~") {
        hazmatDetector_m->loadClasses(classesPath);
    }

    movementDetector_m->setHistory(configurationDialog_m->getImageHistory());
    movementDetector_m->setRectangleMinSize(static_cast<size_t>(configurationDialog_m->getRectangleMinSize()));
    movementDetector_m->setDetectionThreshold(configurationDialog_m->getDetectionThreshold());
    movementDetector_m->setThreshold(configurationDialog_m->getMovementThreshold());
    movementDetector_m->setDetectShadows(configurationDialog_m->getDetectShadows());
}

void VideoRecognizer_Widget::loadConfigurations() {
    configurationDialog_m->loadConfigurations();
    applyConfigurationChanges();
}

void VideoRecognizer_Widget::saveConfigurations() {
    settings_m.beginGroup("VideoRecognizer");

    settings_m.setValue("onnxPath", QString::fromStdString(configurationDialog_m->getONNXPath()));
    settings_m.setValue("classesPath", QString::fromStdString(configurationDialog_m->getClassesPath()));
    settings_m.setValue("imageHistory", configurationDialog_m->getImageHistory());
    settings_m.setValue("rectangleMinSize", configurationDialog_m->getRectangleMinSize());
    settings_m.setValue("detectionThreshold", configurationDialog_m->getDetectionThreshold());
    settings_m.setValue("movementThreshold", configurationDialog_m->getMovementThreshold());
    settings_m.setValue("detectShadows", configurationDialog_m->getDetectShadows());

    settings_m.endGroup();
}

void VideoRecognizer_Widget::renderFrameWithOverlays() {
    if (lastFrame_m.empty() || !mainCameraLabel_m) {
        return;
    }

    cv::Mat displayFrame = lastFrame_m.clone();

    if (lastQRRect_m.has_value()) {
        cv::rectangle(displayFrame, lastQRRect_m->tl(), lastQRRect_m->br(), kQRRectangleColor);
    }

    for (const auto& rect : lastMovementRects_m) {
        cv::rectangle(displayFrame, rect.tl(), rect.br(), kMovementRectangleColor);
    }

    mainCameraLabel_m->setMat(displayFrame);
}
