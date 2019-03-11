package com.peng.serialportlibrary

import android.util.Log
import java.io.FileDescriptor
import java.io.FileInputStream
import java.io.FileOutputStream
import java.io.IOException

/**
 * 串口类
 *
 * @author peng
 */
class SerialPort constructor(val path: String, val baudrate: Int) {

    private val tag = "SerialPort"
    private var mFd: FileDescriptor? = null
    private var mFileInputStream: FileInputStream? = null
    private var mFileOutputStream: FileOutputStream? = null


    private external fun close(fd: Int)

    fun close() {
        mFd?.let {
            try {
                val descriptor = FileDescriptor::class.java.getDeclaredField("descriptor")
                descriptor.isAccessible = true
                val anInt = descriptor.getInt(mFd)
                Log.d(tag, "close fd :$anInt")
                close(anInt)
            } catch (e: NoSuchFieldException) {
                e.printStackTrace()
            } catch (e: IllegalAccessException) {
                e.printStackTrace()
            }
        }
    }

    external fun setOpt(fd: Int, bits: Int, event: Byte, stop: Int): Int

    private external fun open(path: String, baudrate: Int): FileDescriptor?

    fun open() {
        mFd = open(path, baudrate)
        if (mFd == null) {
            Log.e(tag, "native open returns null")
            throw IOException()
        }
        mFileInputStream = FileInputStream(mFd)
        mFileOutputStream = FileOutputStream(mFd)
    }

    companion object {

        init {
            System.loadLibrary("serialport")
        }
    }


}
