/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
    perror(s);
    abort();  // abort()是使异常程序终止，同时发送SIGABRT信号给调用进程
}

static const char *device = "/dev/spidev4.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 48000000;
static uint16_t delay;

static void transfer(int fd)
{
    int ret;
    uint8_t tx[] = {0x31, 0x32};
    uint8_t rx[ARRAY_SIZE(tx)] = {0, };	//the comma here doesn't matter, tested by Tom Xue
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = ARRAY_SIZE(tx),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,	//important, bits = 8 means byte transfer is possible
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message\n");

    printf("the received data is below:\n");
    for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {	//print the received data, by Tom Xue
        if (!(ret % 6))
            puts("");
        printf("%.2X ", rx[ret]);
    }
    puts("");
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int fd;

    fd = open(device, O_RDWR);
    if (fd < 0)
        pabort("can't open device\n");

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
        pabort("can't set spi mode\n");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
        pabort("can't get spi mode\n");

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't set bits per word\n");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't get bits per word\n");

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't set max speed hz\n");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't get max speed hz\n");

    printf("open device: %s\n", device);
    printf("set spi mode: %d\n", mode);
    printf("set bits per word: %d\n", bits);
    printf("set max speed: %d Hz (%d MHz)\n", speed, speed/1000000);

    transfer(fd);

    close(fd);

    return ret;
}
