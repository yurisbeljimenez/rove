#include "input_handler.h"
#include <Bluepad32.h>
#include <Arduino.h>
#include "drive_state.h"
#include <uni.h>  // Bluepad32 platform headers

// Static instance for singleton pattern
InputHandler* InputHandler::instance = nullptr;

// Constructor
InputHandler::InputHandler() {
    if (instance == nullptr) {
        instance = this;
    }
}

/**
 * @brief Initialize the input handler and register Bluepad32 callbacks
 */
void InputHandler::begin() {
    memset(&currentController, 0, sizeof(currentController));
    
    Logger::getInstance()->log(LOG_INFO, "INPUT", "Input handler initialized");
}

/**
 * @brief Get current controller data
 */
const Controller* InputHandler::getGamepad() const {
    return &currentController;
}

/**
 * @brief Set drive state reference (for mode switching)
 */
void InputHandler::setDriveState(DriveState* state) {
    driveState = state;
}

/**
 * @brief Handle gamepad update in instance context
 * 
 * Uses Bluepad32's controller data structure to check button states.
 * Button checking uses bitwise operations on the buttons field:
 * - BUTTON_Y for mode switching (Xbox Y button)
 * - BUTTON_B for brake/stop (Xbox B button)
 */
void InputHandler::handleGamepadUpdate() {
    static uint32_t lastPollTime = 0;
    const uint32_t POLL_INTERVAL_MS = 50;
    
    if (millis() - lastPollTime < POLL_INTERVAL_MS) {
        return;
    }
    lastPollTime = millis();
}

/**
 * @brief Callback for when controller data arrives from Bluepad32
 * 
 * This function is registered as a friend of InputHandler to access private members.
 */
void onControllerData(uni_hid_device_t* d, uni_controller_t* ctl) {
    (void)d;  // Unused parameter
    
    InputHandler* handler = InputHandler::getInstance();
    
    if (handler == nullptr || ctl->klass != UNI_CONTROLLER_CLASS_GAMEPAD) return;
    
    uni_gamepad_t* gp = &ctl->gamepad;
    
    // Process button presses from Xbox controller
    if (gp->buttons & BUTTON_Y) {
        Logger::getInstance()->log(LOG_DEBUG, "INPUT", "Y button pressed - mode switch requested");
        if (handler->driveState != nullptr) {
            handler->driveState->requestModeSwitch();
        }
    }
    
    if (gp->buttons & BUTTON_B) {
        Logger::getInstance()->log(LOG_DEBUG, "INPUT", "B button pressed - brake activated");
        if (handler->driveState != nullptr) {
            handler->driveState->applyBreak();
        }
    }
}

/**
 * @brief Bluepad32 platform implementation for Rove system
 */
static void rove_platform_init(int argc, const char** argv) {
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
}

static void rove_platform_on_init_complete(void) {
    uni_bt_enable_new_connections_unsafe(true);
}

static void rove_platform_on_device_connected(uni_hid_device_t* d) {
    (void)d;
    Logger::getInstance()->log(LOG_INFO, "INPUT", "Bluetooth device connected");
}

static void rove_platform_on_device_disconnected(uni_hid_device_t* d) {
    (void)d;
    Logger::getInstance()->log(LOG_WARN, "INPUT", "Bluetooth device disconnected");
}

static uni_error_t rove_platform_on_device_ready(uni_hid_device_t* d) {
    (void)d;
    return UNI_ERROR_SUCCESS;
}

static void rove_platform_on_controller_data(uni_hid_device_t* d, uni_controller_t* ctl) {
    onControllerData(d, ctl);
}

/**
 * @brief Get the Rove platform instance for Bluepad32 framework
 */
struct uni_platform* get_rove_platform(void) {
    static struct uni_platform plat = {
        .name = "rove",
        .init = rove_platform_init,
        .on_init_complete = rove_platform_on_init_complete,
        .on_device_connected = rove_platform_on_device_connected,
        .on_device_disconnected = rove_platform_on_device_disconnected,
        .on_device_ready = rove_platform_on_device_ready,
        .on_controller_data = rove_platform_on_controller_data,
    };
    return &plat;
}