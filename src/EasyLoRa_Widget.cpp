#include "EasyLoRa_Widget.h"
#include "ui_EasyLoRa_Widget.h"

EasyLoRa_Widget::EasyLoRa_Widget(QWidget *parent)
: QDialog{ parent }
, ui{ new Ui_EasyLoRa_Widget{} } 
{
    ui->setupUi(this);

    listAvalaiblePorts();
    ui->widget_configurations->setEnabled(false);

    connect(ui->button_connect, &QPushButton::pressed, this, &EasyLoRa_Widget::onPushConnect);
    connect(ui->button_cancel, &QPushButton::pressed, this, &EasyLoRa_Widget::close);
    connect(ui->button_apply, &QPushButton::pressed, this, &EasyLoRa_Widget::onPushApply);
}

EasyLoRa_Widget::~EasyLoRa_Widget() {
    delete ui;
}

std::shared_ptr<EasyLoRa> EasyLoRa_Widget::getDevice() const noexcept {
    return easyLoRa_m;
}

void EasyLoRa_Widget::onPushApply() {
    try {
        const auto newConfiguration{ getBuiltConfiguration() };

        easyLoRa_m->setConfiguration(newConfiguration);
    }
    catch(const std::exception& e) {
        showErrorMessage(e.what());
    }
}

void EasyLoRa_Widget::showErrorMessage(std::string_view errorMessage) {
    QMessageBox::critical(this, "Error", QString::fromUtf8(errorMessage));
}

void EasyLoRa_Widget::refreshConfiguration() {
    try {   
        const auto actualConfiguration{ easyLoRa_m->getConfiguration() };

        setAddress(actualConfiguration);
        setChannel(actualConfiguration);
        setNETID(actualConfiguration);
        setBaudRate(actualConfiguration);
        setParityByte(actualConfiguration);
        setAirRate(actualConfiguration);
        setSubpacketLength(actualConfiguration);
        setWORCycle(actualConfiguration);
        setRelayMode(actualConfiguration);
        setWORMode(actualConfiguration);
        setFixedTransmission(actualConfiguration);
        setRSSIEnabled(actualConfiguration);
        setLBTEnabled(actualConfiguration);
        setEnableRSSIByte(actualConfiguration);
        setEnableAbnormalLogs(actualConfiguration);
    }
    catch(const std::exception& e) {
        showErrorMessage(e.what());
    }
}

void EasyLoRa_Widget::setAddress(const ModuleConfig &configuration) {
    ui->edit_address->setText(QString::number((configuration.getAddressHighByte() << Byte_Shift) | configuration.getAddressLowByte(), HEX_Base));
}

void EasyLoRa_Widget::setChannel(const ModuleConfig &configuration) {
    ui->edit_channel->setText(QString::number(configuration.getChannel()));
}

void EasyLoRa_Widget::setNETID(const ModuleConfig &configuration) {
    ui->edit_NETID->setText(QString::number(configuration.getNetId(), HEX_Base));
}

void EasyLoRa_Widget::setBaudRate(const ModuleConfig &configuration) {
    ui->combo_baudRate->setCurrentIndex(configuration.getUartBaudRate());
}

void EasyLoRa_Widget::setParityByte(const ModuleConfig & configuration) {
    ui->combo_parityByte->setCurrentIndex(configuration.getSerialPortParityByte());
}

void EasyLoRa_Widget::setAirRate(const ModuleConfig &configuration) {
    ui->combo_airDataRate->setCurrentIndex(configuration.getAirDataRate());
}

void EasyLoRa_Widget::setSubpacketLength(const ModuleConfig &configuration) {
    ui->combo_subpacketLength->setCurrentIndex(configuration.getSubpacketLength());
}

void EasyLoRa_Widget::setWORCycle(const ModuleConfig &configuration) {
    ui->combo_WORCycle->setCurrentIndex(configuration.getWorCycle());
}

void EasyLoRa_Widget::setRelayMode(const ModuleConfig &configuration) {
    ui->check_relayMode->setChecked(configuration.isRepeaterModeEnabled());
}

void EasyLoRa_Widget::setWORMode(const ModuleConfig &configuration) {
    ui->check_WORMode->setChecked(configuration.isWORModeEnabled());
}

void EasyLoRa_Widget::setFixedTransmission(const ModuleConfig &configuration) {
    ui->check_FixedMode->setChecked(configuration.isFixedTransmitionModeEnabled());
}

void EasyLoRa_Widget::setRSSIEnabled(const ModuleConfig &configuration) {
    ui->check_enableRSSI->setChecked(configuration.isRSSIEnabled());
}

void EasyLoRa_Widget::setLBTEnabled(const ModuleConfig &configuration) {
    ui->check_enableLBT->setChecked(configuration.isLBTEnabled());
}

void EasyLoRa_Widget::setEnableRSSIByte(const ModuleConfig &configuration) {
    ui->check_RSSIByte->setChecked(configuration.isRSSIByteEnabled());
}

void EasyLoRa_Widget::setEnableAbnormalLogs(const ModuleConfig &configuration) {
    ui->check_enableAbnormalLogs->setChecked(configuration.isAbnormalLogEnabled());
}

uint8_t EasyLoRa_Widget::getHighAddress() {
    return static_cast<uint8_t>(static_cast<uint16_t>(ui->edit_address->text().toUInt(nullptr, HEX_Base)) >> Byte_Shift);
}

uint8_t EasyLoRa_Widget::getLowAddress() {
    return static_cast<uint8_t>(static_cast<uint16_t>(ui->edit_address->text().toUInt(nullptr, HEX_Base)) & Low_Address_Mask);
}

uint8_t EasyLoRa_Widget::getChannel() {
    return static_cast<uint8_t>(ui->edit_channel->text().toUShort());
}

uint8_t EasyLoRa_Widget::getNETID() {
    return static_cast<uint8_t>(ui->edit_NETID->text().toUShort());
}

UARTBaudRate EasyLoRa_Widget::getBaudRate() {
    return static_cast<UARTBaudRate>(ui->combo_baudRate->currentIndex());
}

SerialPortParityByte EasyLoRa_Widget::getParityByte() {
    return static_cast<SerialPortParityByte>(ui->combo_parityByte->currentIndex());
}

AirDataRate EasyLoRa_Widget::getAirRate() {
    return static_cast<AirDataRate>(ui->combo_airDataRate->currentIndex());
}

SubpacketLenght EasyLoRa_Widget::getSubpacketLength() {
    return static_cast<SubpacketLenght>(ui->combo_subpacketLength->currentIndex());
}

WORCycle EasyLoRa_Widget::getWORCycle() {
    return static_cast<WORCycle>(ui->combo_WORCycle->currentIndex());
}

bool EasyLoRa_Widget::getWORMode() {
    return ui->check_WORMode->isChecked();
}

bool EasyLoRa_Widget::getRelayMode() {
    return ui->check_relayMode->isChecked();
}

bool EasyLoRa_Widget::getFixedTransmission() {
    return ui->check_FixedMode->isChecked();
}

bool EasyLoRa_Widget::getRSSIEnabled() {
    return ui->check_enableRSSI->isChecked();
}

bool EasyLoRa_Widget::getLBTEnabled() {
    return ui->check_enableLBT->isChecked();
}

bool EasyLoRa_Widget::getEnableRSSIByte() {
    return ui->check_RSSIByte->isChecked();
}

bool EasyLoRa_Widget::getEnableAbnormalLogs() {
    return ui->check_enableAbnormalLogs->isChecked();
}

void EasyLoRa_Widget::listAvalaiblePorts() {
    for (const auto& i : QSerialPortInfo::availablePorts()) {
        if (!i.description().isEmpty()) {
            ui->combo_port->addItem(i.systemLocation());
        }
    }
}

ModuleConfig EasyLoRa_Widget::getBuiltConfiguration() {
    ModuleConfig configuration;

    configuration
    .setAddressHighByte(getHighAddress())
    .setAddressLowByte(getLowAddress())
    .setChannel(getChannel())
    .setNetId(getNETID())
    .setUartBaudRate(getBaudRate())
    .setSerialPortParityByte(getParityByte())
    .setAirDataRate(getAirRate())
    .setSubpacketLength(getSubpacketLength())
    .setEnableWORMode(getWORMode())
    .setWorCycle(getWORCycle())
    .setEnableRepeaterMode(getRelayMode())
    .setEnableFixedTransmitionMode(getFixedTransmission())
    .setEnableRSSI(getRSSIEnabled())
    .setEnableLBT(getLBTEnabled())
    .setRSSIByteEnabled(getEnableRSSIByte())
    .setEnableAbnormalLog(getEnableAbnormalLogs());

    return configuration;
}

void EasyLoRa_Widget::onPushConnect() {
    try {
        easyLoRa_m = std::make_shared<EasyLoRa>(ui->combo_port->currentText().toStdString());

        refreshConfiguration();
        ui->widget_configurations->setEnabled(true);

        emit succesfulConnection();
    }
    catch(const std::exception& e) {
        showErrorMessage(e.what());
    }
}
