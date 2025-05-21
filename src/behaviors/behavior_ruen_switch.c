#define DT_DRV_COMPAT zmk_behavior_ruen_switch

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/hid.h>
#include <zmk/lang.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/endpoints.h>
#include <zephyr/kernel.h>

struct behavior_ruen_switch_config {
    const struct device *macro_to_en;
    const struct device *macro_to_ru;
};

static int on_ruen_switch_pressed(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_ruen_switch_released(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_ruen_switch_config *cfg = dev->config;
    uint8_t wait = zmk_ruen_get_macos() ? 50 : 5;
    bool is_eng = binding->param1 != 0;
    const struct device *macro = is_eng ? cfg->macro_to_en : cfg->macro_to_ru;
    struct zmk_behavior_binding macro_binding = {.behavior_dev = macro};
    zmk_hid_keyboard_clear();
    zmk_endpoints_send_report(HID_USAGE_KEY);
    zmk_ruen_set_eng(is_eng);
    zmk_behavior_invoke_binding(&macro_binding, event, true);
    zmk_behavior_invoke_binding(&macro_binding, event, false);
    k_msleep(wait);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_ruen_switch_driver_api = {
    .binding_pressed = on_ruen_switch_pressed,
    .binding_released = on_ruen_switch_released,
};

#define RUEN_SWITCH_INST(n)
    static const struct behavior_ruen_switch_config behavior_ruen_switch_config##n = { \
        .macro_to_en = DEVICE_DT_GET(DT_PHANDLE(DT_DRV_INST(n), macro_to_en)),               \
        .macro_ru = DEVICE_DT_GET(DT_PHANDLE(DT_DRV_INST(n), macro_to_ru)),               \
    };                                                                                 \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_ruen_switch_driver_api);
DT_INST_FOREACH_STATUS_OKAY(RUEN_SWITCH_INST)
