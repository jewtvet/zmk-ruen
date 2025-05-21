#define DT_DRV_COMPAT zmk_behavior_ruen_one_key

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/hid.h>
#include <zmk/lang.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/endpoints.h>
#include <zephyr/kernel.h>

struct behavior_ruen_one_key_config {
    char *en_behavior_dev;
    char *ru_behavior_dev;
};

static int on_ruen_one_key_pressed(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_ruen_one_key_released(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    uint8_t wait = zmk_ruen_get_macos() ? 50 : 5;
    bool is_eng = zmk_ruen_get_eng();
    bool need = binding->param1 != 0;
    uint32_t encoded = binding->param2;
    if (need == is_eng) {
        raise_zmk_keycode_state_changed_from_encoded(encoded, true, event.timestamp);
        k_msleep(5);
        raise_zmk_keycode_state_changed_from_encoded(encoded, false, event.timestamp + 5);
    } else {
        const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
        const struct behavior_ruen_one_key_config *cfg = dev->config;
        struct zmk_behavior_binding macro_binding1 = {.behavior_dev = need ? cfg->en_behavior_dev : cfg->ru_behavior_dev};
        struct zmk_behavior_binding macro_binding2 = {.behavior_dev = need ? cfg->ru_behavior_dev : cfg->en_behavior_dev};
        zmk_hid_keyboard_clear();
        zmk_endpoints_send_report(HID_USAGE_KEY);
        zmk_ruen_set_eng(need);
        zmk_behavior_invoke_binding(&macro_binding1, event, true);
        k_msleep(wait + 10);
        event.timestamp += wait + 10;
        raise_zmk_keycode_state_changed_from_encoded(encoded, true, event.timestamp);
        k_msleep(5);
        event.timestamp += 5;
        raise_zmk_keycode_state_changed_from_encoded(encoded, false, event.timestamp);
        k_msleep(5);
        event.timestamp += 5;
        zmk_hid_keyboard_clear();
        zmk_endpoints_send_report(HID_USAGE_KEY);
        zmk_ruen_set_eng(!need);    
        zmk_behavior_invoke_binding(&macro_binding2, event, true);
        k_msleep(wait + 10);
    }
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_ruen_one_key_driver_api = {
    .binding_pressed = on_ruen_one_key_pressed,
    .binding_released = on_ruen_one_key_released,
};

#define RUEN_ONE_KEY_INST(n)                                                              \
    static const struct behavior_ruen_one_key_config behavior_ruen_one_key_config_##n = { \
        .en_behavior_dev = DEVICE_DT_NAME(DT_INST_PHANDLE_BY_IDX(n, bindings, 0)),        \
        .ru_behavior_dev = DEVICE_DT_NAME(DT_INST_PHANDLE_BY_IDX(n, bindings, 1)),        \
    };                                                                                    \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, NULL, &behavior_ruen_one_key_config_##n, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_ruen_one_key_driver_api);

DT_INST_FOREACH_STATUS_OKAY(RUEN_ONE_KEY_INST)
