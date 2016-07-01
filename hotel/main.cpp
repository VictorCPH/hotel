#include <QApplication>
#include "mainwindow.h"
#include "dialogloading.h"

#include "hotel.h"
#include <QDebug>
#include <QString>
#include <dialoginquiry.h>

Roomlink hotel[LINKNUM];//记录房间链表的头节点，0~4号链为空闲链，5~9号链为占据链
Clientlink clients;//客户信息链表
Hotel myhotel;//记录房间总体信息

Reserve_clientlink head_Reserve_clientlink;//预约客户链表头结点
Reserve_client  reserve_tab[7][65];//未来7天房间的预约表
int can_occupy_room_num[30][5];//未来30天5种类型房间可用房数量记录

int roomnumberforshow = 101;//房间信息展示中标明房间号
int general_income=0;//总收入
float occupancy_rate=0;//入住率
int amount = 1;//额外消费中记录物品数量
int flagforbuygoods = 1;//额外消费中作为标记使用
int ratio = 2;//时间推进倍率，1秒推进ratio分钟，初始设为2

struct tm *timeNow = NULL; //当前时间，现实中的时间，年月日时分秒
time_t end = 0;//系统内部的时间，单位为秒，从一个时间点（一般是1970年1月1日0时0分0秒）到那时的秒数，用于计算下一时间

int PRICESINGLE = 0;//单人间房价
int PRICESTANDAR = 0;//标准间房价
int PRICEBUSINESS = 0;//商务间房价
int PRICEDELUXE = 0;//豪华套房房价
int PRICEPRESIDENT = 0;//总统套房房价
int loadsuccess = 0;//登陆成功标记

Clientlink show_history;   //查询客户历史信息
int roomnumber_in[66]; //记录某客户居住各房间的次数，智能识别模块使用
int roomtype_in[6];   //记录某客户居住各个房间类型的频率，智能识别模块使用
int general_staytime2=0; //记录某用户总共住房天数，智能识别模块使用

Income income[66];//记录65个房间的收入
Dayincome  dayincome[13][32];//记录一年12个月每天收入情况

void Initialization();
Roomlink Createroomlist(int floor);
void Loadclient();
void Loadroom();
void Loadsoftwaretime();
void Loadroomprize();
void Load_general_income_occupancy_rate();

void Load_income();

void Loadreserve_client();
void Load_reserve_tab();
void Can_use_room_num_tab();
int time_difference(int  timein,int nowtime);
void output_reserve_client();


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    timeNow = new(tm);
    Loadsoftwaretime();//载入时间end 和 timeNow获得初值
    Loadroomprize();//获取房间定价
    Initialization();//初始化旅馆信息
    Loadclient();//载入客户数据
    Loadroom();//载入房间信息
    Load_income();

    Loadreserve_client();  //载入预约信息
    Load_reserve_tab();   //载入预分配表
    Can_use_room_num_tab();  //计算30天每天可用房间

    Clientlink q;  //创建历史节点
    q=new Clientnode;
    q->next=NULL;
    show_history=q;

    Dialogloading p;
    p.show();
    p.exec();

    if (loadsuccess == 1)
    {
        MainWindow w;
        w.show();

        a.exec();
    }
    return 0;
}

void Initialization()
{
    myhotel.total = 65;
    int i;
    for (i=0; i<FLOOR; i++)
        hotel[i] = Createroomlist(i+1);

    for (i=FLOOR; i<FLOOR*2; i++)
    {
        hotel[i] = new(Roomnode);
        hotel[i]->room.roomnumber = 0;//占据链的头结点的roomnumber为占据链长度=占据房+维修房
        hotel[i]->room.roomprice = 0;//占据链的头结点的roomprice为维修房个数， 故占据房 = roomnumber-roomprice
        hotel[i]->next = NULL;
    }
}

void Loadroomprize()
{
    ifstream in("roomprize.txt");
    if (! in.is_open())
    {
        qDebug()<< "Error opening roomprize.txt";
        exit (1);
    }
    string str;
    in>>str>>PRICESINGLE;
    in>>str>>PRICESTANDAR;
    in>>str>>PRICEBUSINESS;
    in>>str>>PRICEDELUXE;
    in>>str>>PRICEPRESIDENT;

    in.close();
}

void Loadsoftwaretime()
{
    FILE *in;

    if ((in = fopen("softwaretime.txt","r")) == NULL)
    {
        qDebug()<<"Error opening softwaretime.txt";
        exit(1);
    }

    fscanf(in, "%d %d %d %d %d %d", &(timeNow->tm_year),&(timeNow->tm_mon),
           &(timeNow->tm_mday),&(timeNow->tm_hour),&(timeNow->tm_min),
           &(timeNow->tm_sec));

    timeNow->tm_year -= 1900;
    timeNow->tm_mon --;
    timeNow->tm_isdst=-1;
    end = mktime(timeNow);
    timeNow = localtime(&end);
    timeNow->tm_year = timeNow->tm_year+1900;
    timeNow->tm_mon = timeNow->tm_mon+1;
}

void Load_general_income_occupancy_rate(){
    ifstream in("general_income_occupancy_rate.txt");
    if (! in.is_open())
    {
        qDebug()<< "Error opening roomprize.txt";
        exit (1);
    }
    string str;
    in>>str>>general_income;
    in>>str>>occupancy_rate;
    in.close();
}

Roomlink Createroomlist(int floor)
{
    Roomlink q=NULL, front=NULL, rear=NULL;

    front = rear = new(Roomnode);
    front->next = rear->next = NULL;

    for (int i=1; i<=HOUSE; i++)
    {
        q = new(Roomnode);
        if (q)
        {
            q->room.roomnumber = floor*100 +  i;
            q->room.clientname = "empty";
            q->room.day = 0;
            q->room.leavetime = 0;
            q->room.sign = Empty;
            q->room.consumption = 0;
            q->room.stayin = 0;

            switch(floor)
            {
                case 1:
                    q->room.roomprice = PRICESINGLE;
                    q->room.roomtype = single;
                    break;
                case 2:
                    q->room.roomprice = PRICESTANDAR;
                    q->room.roomtype = standard;
                    break;
                case 3:
                    q->room.roomprice = PRICEBUSINESS;
                    q->room.roomtype = business;
                    break;
                case 4:
                    q->room.roomprice = PRICEDELUXE;
                    q->room.roomtype = deluxe;
                    break;
                case 5:
                    q->room.roomprice = PRICEPRESIDENT;
                    q->room.roomtype = president;
                    break;
            }
            q->next = NULL;

            rear->next = q;
            rear = rear->next;
        //	cout<<i<<' '<<q->room.roomnumber<<' '<<q->room.day<<' '<<q->room.sign<<' '<< q->room.roomprice<<' '<<q->room.roomtype<<' '<<q->room.clientname<<'\n';
        }
    }
    front->room.roomnumber = HOUSE;//头结点的room.roomnumber记录链表长度
    front->room.roomprice=0;
    return front;
}

void Loadclient()
{
    string name,//名字
           sex,//性别
           identy,//身份证
           telephone;//电话
    int roomnumber,//入住房号
        timein,//入住时间
        staytime;//住房时间
    int leavetime;//计划离店时间
    string roomtype;//房间类型
    string str;//读取第一行

    Clientlink p, rear;
    clients = rear = new(Clientnode);
    clients->next = rear->next =NULL;
    clients->person.roomnumber = 0;

    ifstream in("client.txt");
    if (! in.is_open())
    {
        qDebug()<< "Error opening client.txt";
        exit (1);
    }

    getline(in,str);

    while (in.peek() != EOF)
    {
        roomnumber = 0;
        in>>name>>sex>>identy>>telephone>>roomnumber>>timein>>staytime>>leavetime>>roomtype;


        if (roomnumber == 0)
            break;
        p = new(Clientnode);

        p->person.name = name;
        p->person.sex = sex;
        p->person.identy = identy;
        p->person.telephone = telephone;
        p->person.roomnumber = roomnumber;
        p->person.timein = timein;
        p->person.staytime = staytime;
        p->person.leavetime = leavetime;

        if (roomtype == "单人间")
            p->person.roomtype = single;
        else
            if (roomtype == "标准间")
                p->person.roomtype = standard;
        else
            if (roomtype == "商务间")
                p->person.roomtype = business;
        else
            if (roomtype == "豪华套房")
                p->person.roomtype = deluxe;
        else
            if (roomtype == "总统套房")
                p->person.roomtype = president;

        p->next = NULL;
        rear->next = p;
        rear = rear->next;
        clients->person.roomnumber++;//客户队列头结点的person.roomnumber记录人数，更新一个人就+1
    }
    in.close();
}

void Loadroom()
{
    int emptylist, occupylist;//链表号
    int roomnumber;//房间号
    int roomprice;//房间价格
    double day;//住房时间
    int stayin;//入住时间
    int leavetime;//计划离店
    int consumption;//额外消费，餐饮，损坏赔偿等
    string clientname;//客户名
    string roomtype;//房间类型
    string sign;//房间状态
    string str;//读取第一行
    Roomlink preptr=NULL,searchptr=NULL,roompreptr=NULL,roomsearchptr=NULL;

    ifstream in("room.txt");

    if (! in.is_open())
    {
        qDebug()<< "Error opening room.txt";
        exit (1);
    }

    getline(in,str);
    //房间号	客户名1	房间类型	房间价格	房间状态	入住时间1	已住天数1	计划离店时间	额外消费
    while (in.peek() != EOF)
    {
        roomnumber = 0;
        in>>roomnumber>>clientname>>roomtype>>roomprice>>sign>>stayin>>day>>leavetime>>consumption;

        if (roomnumber == 0)
            break;
        emptylist = roomnumber/100-1;
        occupylist = emptylist + 5;

        roompreptr=hotel[occupylist];
        roomsearchptr=hotel[occupylist]->next;
        preptr=hotel[emptylist];
        searchptr=hotel[emptylist]->next;

        while(searchptr && searchptr->room.roomnumber != roomnumber)
        {
            preptr = searchptr;
            searchptr = searchptr->next;
        }

        if(searchptr != NULL)
        {
            searchptr->room.clientname = clientname;//客户名
            searchptr->room.stayin = stayin;
            searchptr->room.leavetime = leavetime;
            searchptr->room.day = day;
            searchptr->room.consumption = consumption;
            searchptr->room.roomprice = roomprice;
            if (sign == "空闲")
                searchptr->room.sign = Empty;
            else if (sign == "占据")
                searchptr->room.sign = Occupy;
            else if (sign == "预定"){
                searchptr->room.sign = Reserve;
                hotel[emptylist]->room.roomprice++;

            }
            else if (sign == "维修")
                searchptr->room.sign = Repair;

            if (sign == "占据" || sign == "维修")//占据和维修都为不可用房，都放在占据链，空闲和预约房到当前为止还可以先放在空闲链，后续可能调整，这里标记一下
            {
                preptr->next=searchptr->next;
                hotel[emptylist]->room.roomnumber--;//空闲链的头结点的roomnumber为空闲链长度=空闲房+预约房
                searchptr->next=NULL;

                while(roomsearchptr && roomsearchptr->room.roomnumber < roomnumber)
                {
                    roompreptr = roomsearchptr;
                    roomsearchptr = roomsearchptr->next;
                }
                if(roomsearchptr==NULL)
                    roompreptr->next=searchptr;
                else
                {
                    searchptr->next=roomsearchptr;
                    roompreptr->next=searchptr;
                }
                hotel[occupylist]->room.roomnumber++;//占据链的头结点的roomnumber为占据链长度=占据房+维修房
                if (sign == "维修")
                    hotel[occupylist]->room.roomprice++;//维修房个数
            }
            if(sign == "预定"){
                hotel[emptylist]->room.roomprice++;
            }
        }
    }
    in.close();

}

int time_difference(int  timein,int nowtime){
    int t;
    t=(timein%10000)/100;
    if(nowtime%100<=timein%100)
       return timein%100-nowtime%100;
    else{
        if(t==1||t==3||t==5||t==7||t==8||t==10||t==12)
           return 31-nowtime%100+timein%100;
        else
           return 30-nowtime%100+timein%100;
    }
}

void Loadreserve_client()
{
    string name,
           telephone;
    int reserve_time,timein,room_num,reserve_stay;
    string  roomtype,str;
    Reserve_clientlink newptr=NULL;
    newptr = new Reserve_clientnode;
    newptr->next=NULL;
    newptr->reserve_client.room_num=0;
    head_Reserve_clientlink=newptr;

    ifstream in("reserve.txt");
    if (! in.is_open())
    {
        qDebug()<< "Error opening reserve.txt";
        exit (1);
    }
    getline(in,str);

    while (in.peek() != EOF)
    {        //reserve_time,timein,room_num,reserve_stay;
        reserve_stay=0;
        in>>name>>telephone>>reserve_time>>timein>>reserve_stay>>roomtype>>room_num;
      //  qDebug()<<roomnumber;//<<name<<sex<<identy<<telephone<<roomnumber<<timein<<staytime<<roomtype<<endl;
        if(reserve_stay==0)
           break;
        Reserve_clientlink p=NULL;
        p = new Reserve_clientnode;
        p->next=NULL;

        p->reserve_client.name = name;
        p->reserve_client.reserve_time = reserve_time;
        p->reserve_client.reserve_stay = reserve_stay;
        p->reserve_client.telephone = telephone;
        p->reserve_client.timein = timein;
        p->reserve_client.room_num = room_num;

        if (roomtype == "单人间")
            p->reserve_client.roomtype = single;
        else
            if (roomtype == "标准间")
                p->reserve_client.roomtype = standard;
        else
            if (roomtype == "商务间")
                p->reserve_client.roomtype = business;
        else
            if (roomtype == "豪华套房")
                p->reserve_client.roomtype = deluxe;
        else
            if (roomtype == "总统套房")
                p->reserve_client.roomtype = president;        //p->reserve_client.name = name;

        Reserve_clientlink lastptr=NULL;

        lastptr=head_Reserve_clientlink;

        while(lastptr->next)
        {
            lastptr = lastptr->next;
        }
        if(lastptr==head_Reserve_clientlink)
            head_Reserve_clientlink->next=p;
        else
            lastptr->next=p;
        head_Reserve_clientlink->reserve_client.room_num++;

    }
    in.close();
}

void Load_reserve_tab(){
    int j=0,i=0;
    for(i=0;i<7;i++)
        for(j=0;j<65;j++){
            reserve_tab[i][j].room_num=0;
            reserve_tab[i][j].name="";
            reserve_tab[i][j].reserve_stay=0;
            reserve_tab[i][j].reserve_time=0;
            reserve_tab[i][j].telephone="";
            reserve_tab[i][j].timein=0;
          //  reserve_tab[i][j].roomtype=Empty;
        }
    Reserve_clientlink ptr=head_Reserve_clientlink->next;
    j=0;
    int time_d;

    int t=timeNow->tm_year*10000+timeNow->tm_mon*100+timeNow->tm_mday;
    while(ptr!=NULL&&time_difference(ptr->reserve_client.timein,t)<7) {
        time_d=time_difference(ptr->reserve_client.timein,t);
        reserve_tab[time_d][j]=ptr->reserve_client;
        j++;
        ptr=ptr->next;
    }
}
void Can_use_room_num_tab(){

    int i=0,j=0;

    int date_time=timeNow->tm_year*10000+timeNow->tm_mon*100+timeNow->tm_mday;
    enum type type_;
    while(i<30){
        j=0;
        while(j<5){
            switch(j){
                case 0:
                    type_=single;  //standard, business, deluxe, president
                    break;
                case 1:
                    type_=standard;
                    break;
                case 2:
                    type_=business;
                    break;
                case 3:
                    type_=deluxe;
                    break;
                case 4:
                    type_=president;
                    break;
                default:
                    break;
            }
            can_occupy_room_num[i][j]=hotel[j]->room.roomnumber;
            Reserve_clientlink searchptr;
            searchptr=head_Reserve_clientlink->next;
            while(searchptr!=NULL&&type_==searchptr->reserve_client.roomtype&&searchptr->reserve_client.timein<=date_time){
                    if(time_difference(date_time,searchptr->reserve_client.timein)<searchptr->reserve_client.reserve_stay)  //
                        can_occupy_room_num[i][j]=can_occupy_room_num[i][j]-searchptr->reserve_client.room_num;
                    searchptr=searchptr->next;
            }
            Roomlink ptr=hotel[j+5]->next;
            while(ptr){
                int nowtime=timeNow->tm_year*10000+timeNow->tm_mon*100+timeNow->tm_mday;
                 int q=time_difference( ptr->room.leavetime,nowtime);//..................................
//
                    if(i>=q)
                        can_occupy_room_num[i][j]=can_occupy_room_num[i][j]+1;
                    ptr=ptr->next;
            }
            j++;
        }
        i++;
        date_time++;
        int t=(date_time%10000)/100;
        if(t==1||t==3||t==5||t==7||t==8||t==10||t==12)
           if(date_time%100<31)
               date_time++;
           else
               date_time=date_time%10000+(t+1)*100+1;
        else
            if(date_time%100<30)
                date_time++;
            else
                date_time=date_time%10000+(t+1)*100+1;
    }
}


void Load_income()
{
    ifstream in("yearincome.txt");
    string str;
    if (! in.is_open())
    {
        cout << "Error opening roomprize.txt";
        exit (1);
    }
    getline(in,str);

    in>>str>>dayincome[0][0].roomincome>>dayincome[0][0].consumption>>dayincome[0][0].generalincome;


    in>>str;
    int i=1,j=1;
    for(i=1;i<13;i++){
        dayincome[i][0].roomincome=0;
        dayincome[i][0].consumption=0;
        dayincome[i][0].generalincome=0;
    }

    for(i=1;i<13;i++){
        for(j=1;j<32;j++){

           in>>dayincome[i][j].roomincome;
           in>>dayincome[i][j].consumption;
           in>>dayincome[i][j].generalincome;
           dayincome[i][0].roomincome+=dayincome[i][j].roomincome;
           dayincome[i][0].consumption+=dayincome[i][j].consumption;
           dayincome[i][0].generalincome+=dayincome[i][j].generalincome;


        }
    }
    ifstream in1("dayincome.txt");
    string str1;
    int t=0;
    if (! in1.is_open())
    {
        cout << "Error opening roomprize.txt";
        exit (1);
    }
    getline(in1,str1);
    income[0].roomnumber=0;
    in1>>str1>>income[0].roomincome>>income[0].consumption>>t;


    int q=1;
    for(q=1;q<66;q++){


           in1>>income[q].roomnumber;
           in1>>income[q].roomincome;
           in1>>income[q].consumption>>t;

    }


}



