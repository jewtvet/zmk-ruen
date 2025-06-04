#define DT_DRV_COMPAT zmk_behavior_ruen_switch

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/hid.h>
#include <zmk/lang.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/endpoints.h>
#include <zephyr/kernel.h>

struct behavior_ruen_switch_config {
    char *en_behavior_dev;
    char *ru_behavior_dev;
};

struct behavior_macro_config {
    uint32_t default_wait_ms;
    uint32_t default_tap_ms;
    uint32_t count;
    struct zmk_behavior_binding bindings[];
};

static int on_ruen_switch_pressed(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_ruen_switch_released(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_ruen_switch_config *cfg = dev->config;
    uint8_t wait = zmk_ruen_get_macos() ? 46 : 1;
    bool is_eng = binding->param1 != 0;
    // uint32_t code = binding->param2;
    zmk_hid_keyboard_clear();
    zmk_endpoints_send_report(HID_USAGE_KEY);
    zmk_ruen_set_eng(is_eng);
    // raise_zmk_keycode_state_changed_from_encoded(code, true, event.timestamp);
    // k_msleep(5);
    // raise_zmk_keycode_state_changed_from_encoded(code, false, event.timestamp + 5);
    const char *selected_macro = is_eng ? cfg->en_behavior_dev : cfg->ru_behavior_dev;
    const struct device *macro_dev = zmk_behavior_get_binding(selected_macro);
    const struct behavior_macro_config *macro_cfg = (const struct behavior_macro_config *)macro_dev->config;
    const struct zmk_behavior_binding *bindings = macro_cfg->bindings;
    int count = (int)macro_cfg->count;
    const struct device *kp_dev = DEVICE_DT_GET(DT_NODELABEL(kp));
    for (int i = 0; i < count; i++) {
        const struct zmk_behavior_binding *b = &bindings[i];
        const struct device *b_dev = zmk_behavior_get_binding(b->behavior_dev);
        if (b_dev == kp_dev) {
            uint32_t keycode = b->param1;
            raise_zmk_keycode_state_changed_from_encoded(keycode, true, event.timestamp);
            k_msleep(5);
            event.timestamp += 5;
            raise_zmk_keycode_state_changed_from_encoded(keycode, false, event.timestamp);
            k_msleep(5);
            event.timestamp += 5;
        }
    }
    k_msleep(wait);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_ruen_switch_driver_api = {
    .binding_pressed = on_ruen_switch_pressed,
    .binding_released = on_ruen_switch_released,
};

#define RUEN_SWITCH_INST(n)                                                             \
    static const struct behavior_ruen_switch_config behavior_ruen_switch_config_##n = { \
        .en_behavior_dev = DEVICE_DT_NAME(DT_INST_PHANDLE_BY_IDX(n, bindings, 0)),      \
        .ru_behavior_dev = DEVICE_DT_NAME(DT_INST_PHANDLE_BY_IDX(n, bindings, 1)),      \
    };                                                                                  \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, NULL, &behavior_ruen_switch_config_##n, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_ruen_switch_driver_api);
DT_INST_FOREACH_STATUS_OKAY(RUEN_SWITCH_INST)
