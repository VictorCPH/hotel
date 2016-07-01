#ifndef HOTEL_H
#define HOTEL_H

#include <QtDebug>
#include <string>
#include <iostream>
#include <fstream>
#include <QTextStream>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <iomanip>

using namespace std;

#define FLOOR 5 //楼层总数
#define HOUSE 13 //每层房间总数
#define LINKNUM 10 //

enum state{Empty, Reserve, Occupy, Repair};//房间状态：空闲，预定，占据，维修
enum type{single, standard, business, deluxe, president}; //房间类型：单人间，标准间，商务间，豪华套房，总统套房
enum Event{check_in,Check_out,repair,repair_to_empty,reserve,reserve_to_in,change_price};//日志记录事件：入住，离店，维修，维修转空闲，预约，预约转入住，修改房价

typedef struct hotel{
    int total;//房间总数
    int Empty;//空闲房间数
    int Reserve;//预订房间数
    int Occupy;//入住房间数
    int Repair;	//维修房间数
}Hotel,*Hotelptr;//旅店总体信息

typedef struct client{
      string name,//名字
             sex,//性别
             identy,//身份证
             telephone;//电话
      int roomnumber,//入住房号
          staytime,//住房时间
          timein,//入住时间
          leavetime;//计划离店时间
      enum type roomtype;//房间类型
}Client,*Clientptr;//入住客户节点

typedef struct node{
    string name,//名字
           telephone;//电话
    int reserve_time,//预约时间
        timein,//预约入住时间
        room_num,//预约房间数目
        reserve_stay;//预约天数
    enum type roomtype;//房间类型
}Reserve_client,*Reserve_clientptr;//预约客户节点

typedef struct room{
    int roomnumber;//房间号
    int roomprice;//房间价格

    string clientname;//客户名
    double day;//住房时间
    int stayin;//入住时间
    int leavetime;//计划离店时间
    enum type roomtype;//房间类型

    int consumption;//额外消费，餐饮，损坏赔偿等
    enum state sign;//房间状态
}Room,*Roomptr;//房间节点

typedef struct List{
    Client person;
    struct List *next;
}Clientnode,*Clientlink;//入住客户链表

typedef struct roomlist{
    Room room;
    struct roomlist *next;
}Roomnode,*Roomlink;//房间链表

typedef struct link{
    Reserve_client reserve_client;
    struct link *next;
}Reserve_clientnode,*Reserve_clientlink;//预约客户链表


typedef struct income{
    int roomnumber;//房间号
    int roomincome;//房间收入
    int consumption;//房间消费
}Income,*Incomeptr;//单日收入来源节点

typedef struct dayincome{
    int roomincome;//房费收入
    int consumption;//额外消费
    int generalincome;//总消费
}Dayincome,*Dayincomelink;//单月收入来源节点

#endif
