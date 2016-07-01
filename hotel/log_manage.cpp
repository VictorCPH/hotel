#include"log_manage.h"
#include <iomanip>

extern struct tm *timeNow;
long int posi = 0;
extern int manageflag;
extern string receiveman;


void backup_client_leave(Clientlink client_leave){
    ofstream fout("history_client.txt",ios_base::out | ios_base::in);

    fout.seekp(0,ios::end);


   // else{//客户名字	性别	身份证	电话	入住房号	入住时间	住房时间	离店时间	房间类型
   // fout<<setiosflags(ios::left)
  //   <<setw(10)
    fout.width(16);
    fout<<setiosflags(ios::left)<<client_leave->person.name;
    fout.width(16);
    fout<<setiosflags(ios::left)<<client_leave->person.sex;
    fout.width(16);
    fout<<setiosflags(ios::left)<<client_leave->person.identy;
    fout.width(16);
    fout<<setiosflags(ios::left)<<client_leave->person.telephone;
    fout.width(16);
    fout<<setiosflags(ios::left)<<client_leave->person.roomnumber;
    fout.width(16);
    fout<<setiosflags(ios::left)<<client_leave->person.timein;
    fout.width(16);
    fout<<setiosflags(ios::left)<<client_leave->person.staytime;
    fout.width(16);
    fout<<setiosflags(ios::left)<<client_leave->person.leavetime;
    fout.width(16);
    if (client_leave->person.roomtype == single)
       fout<<setiosflags(ios::left)<<"单人间"<<endl;
    else
        if (client_leave->person.roomtype == standard)
            fout<<setiosflags(ios::left)<<"标准间"<<endl;
    else
        if (client_leave->person.roomtype == business)
            fout<<setiosflags(ios::left)<<"商务间"<<endl;
    else
        if (client_leave->person.roomtype == deluxe)
            fout<<setiosflags(ios::left)<<"豪华套房"<<endl;
    else
        if (client_leave->person.roomtype == president)
            fout<<setiosflags(ios::left)<<"总统套房"<<endl;
}


void log_manage(Roomlink room,int i)                     //i??????????????????
{
    int year,moon,day,hour,min;
    year = timeNow->tm_year;
    moon = timeNow->tm_mon;
    day = timeNow->tm_mday;
    hour = timeNow->tm_hour;
    min = timeNow->tm_min;
    int first = 0;
    ifstream in("log_manage.txt");
    string str;
    in >> str;
    if(str.length() == 0)
        first = 1;
    else
        {
            if(manageflag == 0)
            {
                in.seekg(0,ios::end);
                posi = in.tellg();
            }
            else if(manageflag == 1)
            {
                in.seekg(0,ios::end);
                posi = in.tellg();
                manageflag = 2;
            }

        }
    in.close();
    ofstream fout("log_manage.txt",ios::app);
    if (!fout.is_open())
    {
        cout << "Error opening log_manage.txt" << endl;
        exit(1);
    }
    else
    {
        if(first == 1)
        {
            fout << "事件\t\t" << "房间\t\t" << "时间\t\t\t" << "客户名\t\t" << "房价\t\t" << "修改类型\t\t" << "备注\n";
            first = 0;
            if(manageflag == 1)
            {
                fout.seekp(0,ios::end);
                posi = fout.tellp();
            }
        }
        switch (i)
        {
        case check_in:                                       //???????????
            fout << "入住\t\t";
            fout << room->room.roomnumber << "\t\t";
            fout << year << "/" << moon << "/" << day << " " << hour << ":" << min << "\t\t";
            fout << room->room.clientname << "\t\t";
            fout << room->room.roomprice << "\t\t";
            fout << "-\t\t";
            fout << "-\n";
            break;

        case Check_out:                                       //???????????
            fout << "离店\t\t";
            fout << room->room.roomnumber << "\t\t";
            fout << year << "/" << moon << "/" << day << " " << hour << ":" << min << "\t\t";
            fout << room->room.clientname << "\t\t";
            fout << room->room.roomprice << "\t\t";
            fout << "-\t\t";
            fout << "-\n";
            break;

        case repair:                                        //???????????
            fout << "维修\t\t";
            fout << room->room.roomnumber << "\t\t";
            fout << year << "/" << moon << "/" << day << " " << hour << ":" << min << "\t\t";
            fout << "-\t\t";
            fout << "-\t\t";
            fout << "-\t\t";
            fout << "-\n";
            break;

        case repair_to_empty:                                        //???????????
            fout << "维修成功\t\t";
            fout << room->room.roomnumber << "\t\t";
            fout << year << "/" << moon << "/" << day << " " << hour << ":" << min << "\t\t";
            fout << "-\t\t";
            fout << "-\t\t";
            fout << "-\t\t";
            fout << "-\n";
            break;

        case reserve:                                       //???????????
            fout << "预约\t\t";
            fout << "-\t\t";
            fout << year << "/" << moon << "/" << day << " " << hour << ":" << min << "\t\t";
            fout << receiveman << "\t\t";
            fout << "-\t\t";
           // qDebug() << QString::fromStdString(receiveman);
            fout << "-\t\t";
            fout << "-\n";
            break;

        case reserve_to_in:                                       //???????????
            fout << "预约转入住\t\t";
            fout << room->room.roomnumber << "\t\t";
            fout << year << "/" << moon << "/" << day << " " << hour << ":" << min << "\t\t";
            fout << room->room.clientname << "\t\t";
           // qDebug() << QString::fromStdString(room->room.clientname);
            fout << room->room.roomprice << "\t\t";
            fout << "-\t\t";
            fout << "-\n";
            break;

        case change_price:                                       //???????????
            fout << "修改房价\t\t";
            fout << "-\t\t";
            fout << year << "/" << moon << "/" << day << " " << hour << ":" << min << "\t\t";
            fout << "-\t\t";
            fout << room->room.roomprice << "\t\t";
            switch (room->room.roomtype) {
            case 0:
                fout << "单人间\t\t";
                break;
            case 1:
                fout << "标准间\t\t";
                break;
            case 2:
                fout << "商务间\t\t";
                break;
            case 3:
                fout << "豪华间\t\t";
                break;
            case 4:
                fout << "总统套房\t\t";
                break;
            default:
                break;
            }
            fout << "-\n";
            break;

        }

        fout.close();
    }
}

