//
// Created by peng on 2018/6/30.
//
#include <android/log.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "serial_port.h"
#include "error_code.h"

static const char *TAG = "serial_port";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

static speed_t getBaudrate(int baudrate) {
    switch (baudrate) {
        case 0:
            return B0;
        case 50:
            return B50;
        case 75:
            return B75;
        case 110:
            return B110;
        case 134:
            return B134;
        case 150:
            return B150;
        case 200:
            return B200;
        case 300:
            return B300;
        case 600:
            return B600;
        case 1200:
            return B1200;
        case 1800:
            return B1800;
        case 2400:
            return B2400;
        case 4800:
            return B4800;
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        case 230400:
            return B230400;
        case 460800:
            return B460800;
        case 500000:
            return B500000;
        case 576000:
            return B576000;
        case 921600:
            return B921600;
        case 1000000:
            return B1000000;
        case 1152000:
            return B1152000;
        case 1500000:
            return B1500000;
        case 2000000:
            return B2000000;
        case 2500000:
            return B2500000;
        case 3000000:
            return B3000000;
        case 3500000:
            return B3500000;
        case 4000000:
            return B4000000;
        default:
            return -1;
    }
}

int serial_port_open(const char *path, int baudrate) {
    int fd;
    speed_t speed;

    /* 检查波特率 */
    {
        speed = getBaudrate(baudrate);
        if (speed == -1) {
            LOGE("Invalid baudrate");
            return ERROR_INVALID_BAUDRATE;
        }
    }

    /* 打开串口 */
    {

        fd = open(path, O_RDWR | O_NOCTTY);
        LOGD("open() fd = %d", fd);
        if (fd == -1) {
            /* Throw an exception */
            LOGE("Cannot open port");
            return ERROR_SPI_OPEN;
        }
    }


    /* 配置设备属性 */
    {
        struct termios cfg;
        LOGD("Configuring serial port");
        if (tcgetattr(fd, &cfg)) {
            LOGE("tcgetattr() failed");
            close(fd);
            return ERROR_SPI_GET_ATTR;
        }

        /* 设置终端属性为原始属性 */
        cfmakeraw(&cfg);

        /* 设置波特率 */
        cfsetispeed(&cfg, speed);
        cfsetospeed(&cfg, speed);




        /* 激活配置 */
        if (tcsetattr(fd, TCSANOW, &cfg)) {
            LOGE("tcsetattr() failed");
            close(fd);
            return ERROR_SPI_SET_ATTR;
        }
    }
    LOGD("open serial port success");

    return fd;
}

int serial_port_set_opt(int fd, int bits, char event, int stop) {
    struct termios cfg;
    LOGD("Configuring serial port");
    if (tcgetattr(fd, &cfg)) {
        LOGE("tcgetattr() failed");
        close(fd);
        return ERROR_SPI_GET_ATTR;
    }

    /* 设置数据位 */
    switch (bits) {
        case 7:
            cfg.c_cflag |= CS7;
            break;
        case 8:
            cfg.c_cflag |= CS8;
            break;
        default:
            break;
    }


    /* 设置奇偶校验位 */
    switch (event) {
        case 'o':
        case 'O': //奇数
            cfg.c_cflag |= PARENB;
            cfg.c_cflag |= PARODD;
            cfg.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'e':
        case 'E': //偶数
            cfg.c_iflag |= (INPCK | ISTRIP);
            cfg.c_cflag |= PARENB;
            cfg.c_cflag &= ~PARODD;
            break;
        case 'n':
        case 'N':  //无奇偶校验位
            cfg.c_cflag &= ~PARENB;
            break;
        default:
            break;
    }

    /* 设置停止位 */
    if (stop == 1)
        cfg.c_cflag &= ~CSTOPB;
    else if (stop == 2)
        cfg.c_cflag |= CSTOPB;
    /* 激活配置 */
    if (tcsetattr(fd, TCSANOW, &cfg)) {
        LOGE("tcsetattr() failed");
        close(fd);
        return ERROR_SPI_SET_ATTR;
    }
    LOGD("Configuring serial port success");

    return 0;
}

int serial_port_close(int fd) {
    return close(fd);
}