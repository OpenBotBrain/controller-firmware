#include <assert.h>
#include <config/board-rev.hpp>
#include <config/board-rev-a.hpp>
#include <stm-hal/hal-board-init.hpp>

#define lengthof(a) (sizeof(a) / sizeof(a[0]))

// These pins can not change from board version
static constexpr GPIOInitConfig s_gpio_board_rev_id[2] =
{
    { 0, GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_NOPULL, 0, GPIO_SPEED_FREQ_LOW, DONT_CARE },
    { 1, GPIOC, GPIO_PIN_14, GPIO_MODE_INPUT, GPIO_NOPULL, 0, GPIO_SPEED_FREQ_LOW, DONT_CARE },
};

static uint8_t s_board_rev = BOARD_REV_SIZE;
static const BoardSpecificConfig* s_board_specific_config = nullptr;

static void board_rev_id_initialize(void)
{
    // Configure GPIO Ids
    hal_gpio_init_initialize(s_gpio_board_rev_id, lengthof(s_gpio_board_rev_id));
}

static uint8_t board_rev_id_get_raw()
{
    uint8_t board_id = 0;

    for (size_t i = 0; i < lengthof(s_gpio_board_rev_id); i++)
    {
        board_id |= hal_gpio_read_pin_default(s_gpio_board_rev_id[i].port ,s_gpio_board_rev_id[i].pin) << i;
    }

    return board_id;
}

void board_rev_init(void)
{
    const GPIOInitConfig* board_gpio_config;

    // Initialize first board IDs
    board_rev_id_initialize();

    // Get board ID
    s_board_rev = board_rev_id_get_raw();

    switch (s_board_rev)
    {
        case BOARD_REV_A:
            board_gpio_config = board_rev_a_get_gpio_config();
            s_board_specific_config = board_rev_a_get_specific_config();
            break;

        default:
            assert(0);
            break;
    }

    // Initialize ID table
    hal_gpio_init_config(board_gpio_config, TOTAL_GPIO);

    // Initialize GPIOS
    hal_gpio_init_initialize(board_gpio_config, TOTAL_GPIO);

    // Initialize the rest of the board, clock and hal library
    hal_board_init(s_board_rev);
}

const BoardSpecificConfig* board_rev_get_specific_configuration()
{
    assert(s_board_specific_config);

    return s_board_specific_config;
}

uint8_t board_rev_get_revision(void)
{
    assert(s_board_rev != BOARD_REV_SIZE);

    return s_board_rev;
}