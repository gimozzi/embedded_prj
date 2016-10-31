//#include "jni.h"
#include "com_example_androidex_JNI_test.h"
#include "jni.h"
#include "android/log.h"
//#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define LOG_TAG "JNI_TAG"

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

#define MAJOR_NUM 245
#define DEVICE_FILE_NAME "/dev/stopwatch"

#define MAX_BUTTON 9
#define DEVICE_FILE_NAME2 "/dev/fpga_push_switch"


JNIEXPORT void JNICALL Java_com_example_androidex_JNI_1test_sendToBoard
  (JNIEnv *enf, jobject obj, jint flag, jint temp, jint mode, jint data){
	LOGV("jni start...");
	int file_desc;
	int n = 0;
	LOGV("value %d %d %d %d", flag, temp, mode, data);
	LOGV("jni start2...");
	file_desc = open(DEVICE_FILE_NAME, O_RDWR);
	if(file_desc < 0) {
		printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
		exit(-1);
	}
	LOGV("jni file open");
	n = flag & 0xFF;
	n = (n<<8) | (temp & 0xFF);
	n = (n<<8) | (mode & 0xFF);
	n = (n<<8) | (data & 0xFF);
	LOGV("jni calculate %d", n);
	write(file_desc, &n, sizeof(n));
	LOGV("jni write");
	close(file_desc);
	LOGV("jni close");
}

