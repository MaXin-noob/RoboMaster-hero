/*
 * @Description:
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-08-01 16:41:42
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-01 16:53:13
 */
#ifndef __NIMINGTASK_H__
#define __NIMINGTASK_H__
#include "main.h"

#define NIMINGLength 70
#define BYTE0(dwTemp) (*((char *)(&dwTemp)))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))
#endif