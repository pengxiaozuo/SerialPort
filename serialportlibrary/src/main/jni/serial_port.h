//
// Created by peng on 2018/6/30.
//

#ifndef TESTAPPLICATION_SERIAL_PORT_H
#define TESTAPPLICATION_SERIAL_PORT_H


/*
 * 打开串口
 * @param path          路径
 * @param baudrate      波特率
 * @return 成功返回文件描述符，失败返回错误码,错误码可以查询 {@link error_code.h}
 */
int serial_port_open(const char *path, int baudrate);


/**
 * 设置串口属性 成功返回0
 * @param fd            文件描述符
 * @param bits          数据位 7|8
 * @param event         奇偶校验 'O':偶数 'E':奇数 'N':无奇偶校验
 * @param stop          停止位
 * @return
 */
int serial_port_set_opt(int fd, int bits, char event, int stop);

/**
 * 关闭串口设备
 * @param fd            文件描述符
 * @return
 */
int serial_port_close(int fd);

#endif //TESTAPPLICATION_SERIAL_PORT_H
