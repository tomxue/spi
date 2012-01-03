#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include "mux.h"

static void __init omap3_beagle_config_mcspi3_mux(void)
{
        // NOTE: Clock pins need to be in input mode
	omap_mux_init_signal("sdmmc2_clk.mcspi3_clk", OMAP_PIN_INPUT);
	omap_mux_init_signal("sdmmc2_dat3.mcspi3_cs0", OMAP_PIN_OUTPUT);
	omap_mux_init_signal("sdmmc2_dat2.mcspi3_cs1", OMAP_PIN_OUTPUT);
	omap_mux_init_signal("sdmmc2_cmd.mcspi3_simo", OMAP_PIN_OUTPUT);
	omap_mux_init_signal("sdmmc2_dat0.mcspi3_somi", OMAP_PIN_INPUT_PULLUP);
}

static void __init omap3_beagle_config_mcspi4_mux(void)
{
        // NOTE: Clock pins need to be in input mode
	omap_mux_init_signal("mcbsp1_clkr.mcspi4_clk", OMAP_PIN_INPUT);
	omap_mux_init_signal("mcbsp1_fsx.mcspi4_cs0", OMAP_PIN_OUTPUT);
	omap_mux_init_signal("mcbsp1_dx.mcspi4_simo", OMAP_PIN_OUTPUT);
	omap_mux_init_signal("mcbsp1_dr.mcspi4_somi", OMAP_PIN_INPUT_PULLUP);
}

static struct spi_board_info beagle_mcspi_board_info[] = {
	// spi 3.0
	{
		.modalias	= "spidev",
		.max_speed_hz	= 48000000, //48 Mbps
		.bus_num	= 3,
		.chip_select	= 0,	
		.mode = SPI_MODE_1,
	},

	// spi 3.1
	{
		.modalias	= "spidev",
		.max_speed_hz	= 48000000, //48 Mbps
		.bus_num	= 3,
		.chip_select	= 1,	
		.mode = SPI_MODE_1,
	},

	// spi 4.0
	{
		.modalias	= "spidev",
		.max_speed_hz	= 48000000, //48 Mbps
		.bus_num	= 4,
		.chip_select	= 0,	
		.mode = SPI_MODE_1,
	},
};

int init_module()
{
	omap3_beagle_config_mcspi3_mux();
	omap3_beagle_config_mcspi4_mux();
	spi_register_board_info(beagle_mcspi_board_info,
			ARRAY_SIZE(beagle_mcspi_board_info));
}

void cleanup_module()
{
	printk(KERN_INFO "spidev kernel module removed\n");
}
