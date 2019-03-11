//
// Created by peng on 2018/6/30.
//

#include <jni.h>
#include "serial_port.h"


extern "C"
JNIEXPORT void JNICALL
Java_com_peng_serialportlibrary_SerialPort_close(JNIEnv *env, jobject instance, jint fd) {
    serial_port_close(fd);
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_peng_serialportlibrary_SerialPort_setOpt(JNIEnv *env, jobject instance, jint fd, jint bits,
                                             jbyte event, jint stop) {

    return serial_port_set_opt(fd, bits, event, stop);
}extern "C"
JNIEXPORT jobject JNICALL
Java_com_peng_serialportlibrary_SerialPort_open(JNIEnv *env, jobject instance, jstring path_,
                                           jint baudrate) {
    jobject mFileDescriptor;
    int fd;
    const char *path = env->GetStringUTFChars(path_, 0);

    fd = serial_port_open(path, baudrate);

    if (fd < 0) {
        return NULL;
    }

    jclass cFileDescriptor = env->FindClass("java/io/FileDescriptor");
    jmethodID iFileDescriptor = env->GetMethodID(cFileDescriptor, "<init>", "()V");
    jfieldID descriptorID = env->GetFieldID(cFileDescriptor, "descriptor", "I");

    mFileDescriptor = env->NewObject(cFileDescriptor, iFileDescriptor);
    env->SetIntField(mFileDescriptor, descriptorID, fd);

    env->ReleaseStringUTFChars(path_, path);
    return mFileDescriptor;
}

