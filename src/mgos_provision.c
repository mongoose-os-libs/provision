#include "mgos.h"

static mgos_timer_id s_hold_timer = MGOS_INVALID_TIMER_ID;

static void reset_to_factory_defaults(void) {
  LOG(LL_INFO, ("Reseting to factory defaults"));
  mgos_config_reset(MGOS_CONFIG_LEVEL_USER);
  mgos_fs_gc();
  mgos_system_restart_after(100);
}

static void button_timer_cb(void *arg) {
  int pin = mgos_sys_config_get_provision_button_pin();
  enum mgos_gpio_pull_type pull = (enum mgos_gpio_pull_type)(intptr_t) arg;
  int n = 0; /* Number of times the button is reported down */
  mgos_gpio_disable_int(pin);
  for (int i = 0; i < 10; i++) {
    int level = mgos_gpio_read(pin);
    if (pull == MGOS_GPIO_PULL_UP && level == 0) n++;
    if (pull == MGOS_GPIO_PULL_DOWN && level > 0) n++;
    mgos_msleep(1);
  }
  if (s_hold_timer != MGOS_INVALID_TIMER_ID) mgos_clear_timer(s_hold_timer);
  if (n > 7) {
    reset_to_factory_defaults();
  } else {
    LOG(LL_INFO, ("Button released, NOT resetting"));
  }
  mgos_gpio_enable_int(pin);
}

static void button_down_cb(int pin, void *arg) {
  int duration = mgos_sys_config_get_provision_button_hold_ms();
  if (s_hold_timer != MGOS_INVALID_TIMER_ID) mgos_clear_timer(s_hold_timer);
  LOG(LL_INFO, ("Button pressed, setting %d ms timer", duration));
  s_hold_timer = mgos_set_timer(duration, 0, button_timer_cb, arg);
  (void) pin;
}

bool mgos_provision_init(void) {
  int pin = mgos_sys_config_get_provision_button_pin();
  int hold = mgos_sys_config_get_provision_button_hold_ms();
  enum mgos_gpio_pull_type pull =
      (enum mgos_gpio_pull_type) mgos_sys_config_get_provision_button_pull();

  LOG(LL_INFO, ("Factory reset via button: pin %d (%s), hold_ms %d (%s)", pin,
                pin < 0 ? "disabled" : "enabled", hold,
                hold == 0 ? "reset on reboot" : "reset on long press"));

  if (pin < 0 || hold < 0) return true; /* disabled */

  if (hold == 0) {
    /* Check if button is pressed on reboot */
    button_timer_cb((void *) pull);
  } else {
    /* Set a long press handler. Note: user code can override it! */
    mgos_gpio_set_button_handler(pin, pull, pull == MGOS_GPIO_PULL_UP
                                                ? MGOS_GPIO_INT_EDGE_NEG
                                                : MGOS_GPIO_INT_EDGE_POS,
                                 50, button_down_cb, (void *) pull);
  }
  return true;
}
