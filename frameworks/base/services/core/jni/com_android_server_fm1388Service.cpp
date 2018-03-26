/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "Fm1388Service"

#include "jni.h"  
#include "JNIHelp.h"  
#include "android_runtime/AndroidRuntime.h"  
#include <utils/misc.h>  
#include <cutils/log.h>  
#include <hardware/hardware.h>  
//#include <hardware/im401.h>  
#include <hardware/fm1388.h>  
#include <stdio.h>

namespace android
{
	/*在硬件抽象层中定义的硬件访问结构体，参考<hardware/im401.h>*/
	struct fm1388_device_t* fm1388_device = NULL;

//#if 1	
	/*通过硬件抽象层定义的硬件访问接口设置硬件寄存器val的值*/
	static void fm1388_setVal(JNIEnv* env, jobject clazz, jint value) {
		int val = value;

		ALOGE("fm1388 JNI: fm1388_setVal , %d", val);
		//ALOGI("fm1388 JNI: set value %d to device.", val);
		if(!fm1388_device) {
			ALOGI("fm1388 JNI: device is not open.");
			return;
		}
		fm1388_device->set_val(fm1388_device, val);
	}

    /*通过硬件抽象层定义的硬件访问接口读取硬件寄存器val的值*/
	static jint fm1388_getVal(JNIEnv* env, jobject clazz, jint value) {
		int val = value;
		int ret;
		
		ALOGE("fm1388 JNI: fm1388_getVal");
		if(!fm1388_device) {
			ALOGI("fm1388 JNI: device is not open.");
			return -1;
		}

		ret = fm1388_device->get_val(fm1388_device, &val);
		if (ret != 0) {
			ALOGE("fm1388 JNI: get value failed.");
			return -1;
		}
		//ALOGI("fm1388 JNI: get value %d from device.", val);

		return val;
	}

	static jint fm1388_getData(JNIEnv* env, jobject clazz, jint action, jlong regaddr) {
		long val = regaddr;		
		int ret;
		
		ALOGE("fm1388 JNI: fm1388_getData s");
		if(!fm1388_device) {
			ALOGI("fm1388 JNI: device is not open.");
			return -1;
		}

		ret = fm1388_device->get_data(fm1388_device, action, &val);
		if (ret < 0) {
			ALOGE("fm1388 JNI: get data failed.");
			return -1;
		}
		
		return ret;				
	}


	static void fm1388_setData(JNIEnv* env, jobject clazz, jint action, jlong regaddr, jint value) {

		int ret;
		ALOGE("fm1388 JNI: fm1388_setData");
		if(!fm1388_device) {
			ALOGI("fm1388 JNI: device is not open.");
		}

		ret = fm1388_device->set_data(fm1388_device, action, regaddr, value);
		if (ret < 0) {
			ALOGE("fm1388 JNI: set data failed.");
		}
		
		ALOGE("fm1388 JNI: fm1388_setData");
	}



    /*通过硬件抽象层定义的硬件模块打开接口打开硬件设备*/
	static inline int fm1388_device_open(const hw_module_t* module, struct fm1388_device_t** device) {
		return module->methods->open(module, FM1388_HARDWARE_MODULE_ID, (struct hw_device_t**)device);
	}

    /*通过硬件模块ID来加载指定的硬件抽象层模块并打开硬件*/
	static jint fm1388_init(JNIEnv* env, jclass clazz) {
		fm1388_module_t* module;
		
		ALOGI("fm1388 JNI: initializing......");

		if(hw_get_module(FM1388_HARDWARE_MODULE_ID, (const struct hw_module_t**)&module) == 0) {
			//ALOGI("fm1388 JNI: fm1388 Stub found.");
			if(fm1388_device_open(&(module->common), &fm1388_device) == 0) {
				//ALOGI("fm1388 JNI: fm1388 device is open.");
				return 0;
			}
			ALOGE("fm1388 JNI: failed to open fm1388 device.");
			return -1;
		}
		ALOGE("fm1388 JNI: failed to get fm1388 stub module.");
		return -1;				
//		return 30;		
	}

    /*JNI方法表*/
	static const JNINativeMethod method_table[] = {
		{"init_native", "()I", (void*)fm1388_init},
		{"setVal_native", "(I)V", (void*)fm1388_setVal},
		{"getVal_native", "(I)I", (void*)fm1388_getVal},
		{"setData_native", "(IJI)V", (void*)fm1388_setData},
		{"getData_native", "(IJ)I", (void*)fm1388_getData},		
	};
    
    /*注册JNI方法*/
	int register_android_server_fm1388Service(JNIEnv *env) {
//    		return jniRegisterNativeMethods(env, "com/fortemedia/jfm1388/Jfm1388", method_table, NELEM(method_table));
    		return jniRegisterNativeMethods(env, "com/android/server/fm1388Service", method_table, NELEM(method_table));			
            //package com.android.server;
            //package com.fortemedia.jfm1388;			
	}
//#endif    	
};
