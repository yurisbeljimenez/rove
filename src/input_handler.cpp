#include "input_handler.h"
#include "config.h"
#include <Bluepad32.h>
#include <Arduino.h>
#include "drive_state.h"
#include <uni.h>  // Bluepad32 platform headers

// Static instance for singleton pattern
InputHandler* InputHandler::instance = nullptr;

/**
 * @brief Constructs a new InputHandler object.
 */
InputHandler::InputHandler() {
    if (instance == nullptr) {
        instance = this;
    }
}

/**
 * @brief Initializes the input handler and prepares internal buffers.
 * 
 * Should be called once during system setup.
 */
void InputHandler::begin() {
    memset(&currentController, 0, sizeof(currentController));
    
    Logger::getInstance()->log(LogLevel::INFO, "INPUT", "Input handler initialized");
}

/**
 * @brief Retrieves the most recent gamepad data snapshot.
 * 
 * @return const Controller* A pointer to the current controller state. 
 *         Returns nullptr if no controller is currently connected/available.
 */
const Controller* InputHandler::getGamepad() const {
    return &currentController;
}

/**
 * @brief Links the input handler to a DriveState instance.
 * 
 * This allows the InputHandler to trigger mode switches and braking actions
 * directly on the state machine.
 * 
 * @param state Pointer to the active DriveState object.
 */
void InputHandler::setDriveState(DriveState* state) {
    driveState = state;
}

/**
 * @brief Processes gamepad input updates.
 * 
 * Currently a placeholder for future polling-based logic, as the system 
 * primarily uses event-driven callbacks from Bluepad32.
 */
void InputHandler::handleGamepadUpdate() {
    // This method is currently a placeholder for future polling-based logic.
    // The current implementation relies on Bluepad32 callbacks via onControllerData.
}

/**
 * @brief Bluepad32 event callback triggered when controller data changes.
 * 
 * This is a low-level callback that processes raw HID reports and translates
 * them into Rove-specific actions like mode switching or braking.
 * 
 * @param d Pointer to the HID device context.
 * @param ctl Pointer to the controller data structure provided by Bluepad32.
 */
void onControllerData(uni_hid_device_t* d, uni_controller_t* ctl) {
    (void)d;  // Unused parameter
    
    InputHandler* handler = InputHandler::getInstance();
    
    if (handler == nullptr || ctl->klass != UNI_CONTROLLER_CLASS_GAMEPAD) return;
    
    uni_gamepad_t* gp = &ctl->gamepad;
    
    // Process button presses from Xbox controller
    if (gp->buttons & BUTTON_Y) {
        Logger::getInstance()->log(LogLevel::DEBUG, "INPUT", "Y button pressed - mode switch requested");
        if (handler->driveState != nullptr) {
            handler->driveState->requestModeSwitch();
        }
    }
    
    if (gp->buttons & BUTTON_B) {
        Logger::getInstance()->log(LogLevel::DEBUG, "INPUT", "B button pressed - brake activated");
        if (handler->driveState != nullptr) {
            handler->driveState->applyBreak();
        }
    }
}

/**
 * @brief Bluepad32 platform implementation for Rove system.
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
    Logger::getInstance()->log(LogLevel::INFO, "INPUT", "Bluetooth device connected");
}

static void rove_platform_on_device_disconnected(uni_hid_device_t* d) {
    (void)d;
    Logger::getInstance()->log(LogLevel::WARN, "INPUT", "Bluetooth device disconnected");
}

static uni_error_t rove_platform_on_device_ready(uni_hid_device_t* d) {
    (void)d;
    return UNI_ERROR_SUCCESS;
}

static void rove_platform_on_controller_data(uni_hid_device_t* d, uni_controller_t* ctl) {
    onControllerData(d, ctl);
}

/**
 * @brief Get the Rove platform instance for Bluepad32 framework.
 * 
 * @return struct uni_platform* Pointer to the static platform structure.
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