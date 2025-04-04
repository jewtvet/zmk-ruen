#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/keymap.h>
#include <zmk/behavior.h>
#include <drivers/behavior.h>
#include <zmk/hid.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// Global variable to track language state
static bool isEng = true;

struct behavior_ruen_config {
    uint8_t behavior_type;
};

// Behavior types
enum behavior_ruen_types {
    BEHAVIOR_TOGGLE_LANG, // Switches language and sends Left_GUI+9
    BEHAVIOR_CONDITIONAL_KEY, // Sends A or B based on language
};

// Toggle language behavior
static int on_lang_toggle_binding_pressed(struct zmk_behavior_binding *binding,
                                        struct zmk_behavior_binding_event event) {
    // Change language state to Russian
    isEng = false;
    
    // Send LEFT_GUI+9
    struct zmk_behavior_binding tmp_binding = {
        .behavior_dev = "key_press",
        .param1 = ZMK_HID_USAGE(HID_USAGE_KEY, HID_USAGE_KEY_KEYBOARD_LEFT_GUI),
        .param2 = ZMK_HID_USAGE(HID_USAGE_KEY, HID_USAGE_KEY_KEYBOARD_9),
    };
    
    int ret = behavior_keymap_binding_pressed(&tmp_binding, event);
    if (ret < 0) {
        LOG_ERR("Failed to send LEFT_GUI+9: %d", ret);
    }
    
    return ret;
}

static int on_lang_toggle_binding_released(struct zmk_behavior_binding *binding,
                                         struct zmk_behavior_binding_event event) {
    // Release LEFT_GUI+9
    struct zmk_behavior_binding tmp_binding = {
        .behavior_dev = "key_press",
        .param1 = ZMK_HID_USAGE(HID_USAGE_KEY, HID_USAGE_KEY_KEYBOARD_LEFT_GUI),
        .param2 = ZMK_HID_USAGE(HID_USAGE_KEY, HID_USAGE_KEY_KEYBOARD_9),
    };
    
    return behavior_keymap_binding_released(&tmp_binding, event);
}

// Conditional key behavior based on language
static int on_conditional_key_binding_pressed(struct zmk_behavior_binding *binding,
                                            struct zmk_behavior_binding_event event) {
    // Send A if isEng is true, B otherwise
    uint8_t keycode = isEng ? HID_USAGE_KEY_KEYBOARD_A : HID_USAGE_KEY_KEYBOARD_B;
    
    struct zmk_behavior_binding tmp_binding = {
        .behavior_dev = "key_press",
        .param1 = ZMK_HID_USAGE(HID_USAGE_KEY, keycode),
    };
    
    return behavior_keymap_binding_pressed(&tmp_binding, event);
}

static int on_conditional_key_binding_released(struct zmk_behavior_binding *binding,
                                             struct zmk_behavior_binding_event event) {
    // Release A if isEng is true, B otherwise
    uint8_t keycode = isEng ? HID_USAGE_KEY_KEYBOARD_A : HID_USAGE_KEY_KEYBOARD_B;
    
    struct zmk_behavior_binding tmp_binding = {
        .behavior_dev = "key_press",
        .param1 = ZMK_HID_USAGE(HID_USAGE_KEY, keycode),
    };
    
    return behavior_keymap_binding_released(&tmp_binding, event);
}

static const struct behavior_driver_api behavior_ruen_driver_api = {
    .binding_pressed = NULL,
    .binding_released = NULL,
};

static int behavior_ruen_init(const struct device *dev) {
    const struct behavior_ruen_config *config = dev->config;
    
    switch (config->behavior_type) {
        case BEHAVIOR_TOGGLE_LANG:
            ((struct behavior_driver_api *)dev->api)->binding_pressed = on_lang_toggle_binding_pressed;
            ((struct behavior_driver_api *)dev->api)->binding_released = on_lang_toggle_binding_released;
            break;
        case BEHAVIOR_CONDITIONAL_KEY:
            ((struct behavior_driver_api *)dev->api)->binding_pressed = on_conditional_key_binding_pressed;
            ((struct behavior_driver_api *)dev->api)->binding_released = on_conditional_key_binding_released;
            break;
        default:
            return -ENOTSUP;
    }
    
    return 0;
}

// Language Toggle Behavior definition
#define TOGGLE_LANG_INST(n) \
    static const struct behavior_ruen_config behavior_toggle_lang_config_##n = { \
        .behavior_type = BEHAVIOR_TOGGLE_LANG, \
    }; \
    DEVICE_DT_INST_DEFINE(n, behavior_ruen_init, NULL, NULL, \
                         &behavior_toggle_lang_config_##n, \
                         POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, \
                         &behavior_ruen_driver_api);

// Conditional Key Behavior definition
#define CONDITIONAL_KEY_INST(n) \
    static const struct behavior_ruen_config behavior_conditional_key_config_##n = { \
        .behavior_type = BEHAVIOR_CONDITIONAL_KEY, \
    }; \
    DEVICE_DT_INST_DEFINE(n, behavior_ruen_init, NULL, NULL, \
                         &behavior_conditional_key_config_##n, \
                         POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, \
                         &behavior_ruen_driver_api);

DT_INST_FOREACH_STATUS_OKAY_VARGS(TOGGLE_LANG_INST);
DT_INST_FOREACH_STATUS_OKAY_VARGS(CONDITIONAL_KEY_INST);