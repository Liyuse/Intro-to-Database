//
//  main.cpp
//  DB_project_C++
//
//  Created by 李雨泽 on 2017/11/5.
//  Copyright © 2017年 Albert. All rights reserved.
//
#include <mysql.h>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <ctime>
#include <chrono>
using namespace std;

MYSQL *conn;

void login();
void transcript(std::string);
void personalinfo(std::string);
void enroll(std::string);
void withdraw(std::string);
void Course_details(std::string);
void Course_helper(std::string, std::string);

bool login_helper(string a, string b, vector<string> &aa , vector<string> &bb)
{
    
    for(int i=0; i<aa.size(); i++){
        if(aa[i] == a){
            if(bb[i] == b){
                return false;
            }
        }
    }
    
    return true;
    
}

void Course_helper(string current, string student_id){
    if( current == "1" )
    {
        Course_details(student_id);
    }
    else if ( current == "2" )
    {
        transcript(student_id);
    }
    else
    {
        cout << "*** Invalid option! ***" << endl;
        Course_details(student_id);
    }
}

void menu(string student_id)
{
    string opt;
    time_t rawtime;
    time(&rawtime);
    
    //get year and month
    struct tm* timeinfo;
    timeinfo = localtime(&rawtime);
    
    int mon = timeinfo->tm_mon + 1;
    int year = timeinfo->tm_year + 1900;
    string semester = "";
    
    
    mysql_close(conn);
    conn = mysql_init ( NULL );
    mysql_real_connect (conn,"localhost","root","Albert@1357","project3-nudb",3306,NULL,0);
    
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    cout << "<<<                         CURRENT COURSES :                       >>>"<< endl;
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    if (mon>=9 && mon<=11){semester = "Q1";};
    if (mon==12 || mon == 1 || mon ==2 ){semester = "Q2";};
    if (mon>=3 && mon<=5){semester = "Q3";};
    if (mon>=6 && mon<=8){semester = "Q4";};
    string year_s = std::to_string(year);
    
    string sql = "select * from transcript where StudId = " + student_id + " and Semester = '" + semester + "' and Year = "+year_s+"  and grade is null;";
    
    
    mysql_query(conn,sql.c_str());
    res_set = mysql_store_result(conn);
    int numrows =  (int)mysql_num_rows(res_set);
    
    for (int i =0; i< numrows; i++)
    {
        row = mysql_fetch_row(res_set);
        if (row != NULL)
        {
            cout<<"|course : "<<row[1]<<endl;
            cout<<"|Year:"<<row[3]<<endl;
        }
    }
    
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    cout << "Choose one of the following option :" << endl;
    cout << "1: Transcript" << endl;
    cout << "2: Enroll" << endl;
    cout << "3: Withdraw" << endl;
    cout << "4: Personal Details" << endl;
    cout << "5: Logout" << endl;
    cout << "Your option is :" ;
    cin >> opt;
    
    int opt_ = stoi(opt);
    
    
    switch(opt_){
        case 1: {
            transcript(student_id);
        }
        case 2: {
            enroll(student_id);
        }
        case 3:{
            withdraw(student_id);
        }
        case 4:{
            personalinfo(student_id);
        }
        case 5:{
            login();
        }
        default: {
            cout << "*** You need to enter a valid option! ***" << endl << endl;
            menu(student_id);
        }
    }
    
}

void transcript(string student_id){
    string opt;
    string sql = "SELECT UoSCode, Grade FROM transcript where StudId = " + student_id+";";
    
    
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    mysql_query(conn, sql.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);
    
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    cout << "<<<                         Student Transcript :                    >>>"<< endl;
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    
    for (int i = 0; i < numrows; i++)
    {
        row = mysql_fetch_row(res_set);
        if(row[1]!= NULL)
        {
            cout << "| Course  : " << row[0] << endl;
            cout << "| Grade   : " << row[1] << endl;
        }
        else
        {
            cout << "| Course  : " << row[0] << endl;
            cout << "| Grade   : null" << endl;
        }
    }
    cout << "---------------------------------"<<endl;
    mysql_free_result( res_set );
    
    cout << "Please selectollowing option:" << endl << "1: Details" << endl << "2: Menu"<<endl;
    cout << "Your option is : " ;
    cin>>opt;
    
    if ( opt == "1" )
    {
        Course_details(student_id);
    }
    else if ( opt == "2")
    {
        menu(student_id);
    }
    else
    {
        cout << "<<<<<< Input not valid! <<<<<<" << endl;
        transcript(student_id);
    }
    
}


void Course_details(string student_id){
    
    string course_number;
    cout << "Please enter a course number :" << endl;
    cin >> course_number;
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    string courseinformation="SELECT Trans.UoSCode,Trans.Year,Trans.Semester, UoSName, Enrollment,MaxEnrollment,Name,Grade,StudId FROM transcript Trans, faculty Fac,  unitofstudy Unito, uosoffering Uoso where StudId = " + student_id+" and Trans.UoSCode= '"+course_number+"'and Trans.UoSCode=Unito.UoSCode and Trans.UoSCode=Uoso.UoSCode and Uoso.InstructorId=Fac.Id and Trans.Year=Uoso.Year and Trans.Semester=Uoso.Semester ;";
    mysql_query(conn, courseinformation.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);
    
    
    if(numrows==1)
    {
        row = mysql_fetch_row(res_set);
        if(row[7]!= NULL)
        {
            cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            cout << "<<<               COURSE DETAILS :                  >>>" << endl;
            cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            cout << "** Quarter     :" << row[3] << endl;
            cout << "** Emrollnumber:" << row[4] << endl;
            cout << "** MaxEmroll   :" << row[5] << endl;
            cout << "** Lecturer    :" << row[6] << endl;
            cout << "** Grade       :" << row[7] << endl;
            cout << "** Coursenumber:" << row[0] << endl;
            cout << "** Title       :" << row[1] << endl;
            cout << "** Year        :" << row[2] << endl;
            cout << "----------------------------------------------------" << endl;
            cout << "Enter 1 for another course detail" << endl << "Enter 2 to Transcript" << endl;
            cout << "Your option is : ";
            string current;
            cin >> current;
            Course_helper(current,student_id);
        }
        else
        {
            cout << "-----------------------------------------------------" << endl;
            cout << "<<<               COURSE DETAILS :                >>>" << endl;
            cout << "-----------------------------------------------------" << endl;
            cout << "| Quarter     :" << row[3] << endl;
            cout << "| Emrollnumber:" << row[4] << endl;
            cout << "| MaxEmroll   :" << row[5] << endl;
            cout << "| Lecturer    :" << row[6] << endl;
            cout << "| Coursenumber:" << row[0] << endl;
            cout << "| Title       :" << row[1] << endl;
            cout << "| Year        :" << row[2] << endl;
            cout << "| Grade       : null" << endl;
            cout << "-----------------------------------------------------" << endl;
            cout << " Enter 1 for another course detail" << endl << " Enter 2 to Transcript" << endl;
            cout << " Your option is : ";
            string current;
            cin >> current;
            Course_helper(current,student_id);
        }
    }
    else {
        cout << "*** Please enter a valid course number! ***" << endl;
        Course_details(student_id);
    }
    mysql_free_result( res_set );
    
    
    
}

void personalinfo(string student_id){
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    string sql="SELECT * FROM student where Id="+student_id+";";
    mysql_query(conn,sql.c_str());
    res_set = mysql_store_result(conn);
    row = mysql_fetch_row( res_set );
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    cout << "<<<                         Student Info :                          >>>"<< endl;
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    cout << "Id: "<<row[0]<<endl;
    cout << "Name :"<<row[1]<<endl;
    cout << "Password :"<<row[2]<<endl;
    cout << "Address :"<<row[3]<<endl;
    cout << "<<<                You have the following options                  >>>"<<endl;
    cout << "1:change the address" <<endl<< "2:change the password" <<endl<<"3:back to menu"<<endl;
    cout << "Your option : ";
    string opt;
    cin >> opt;
    
    int opt_ = stoi(opt);
    
    switch(opt_)
    {
        case 1: {
            cout << "Enter a new address" << endl;
            string newaddress;
            cin >> newaddress;
            string revise2 = "UPDATE student SET student.Address = '"+newaddress+"' WHERE student.Id = " + student_id+";";
            mysql_query(conn,"START TRANSACTION;");
            mysql_query(conn, revise2.c_str());
            mysql_query(conn,"COMMIT;");
            res_set = mysql_store_result(conn);
            cout << "*** The address is changed successfully! ***" << endl;
            personalinfo(student_id);
            break;}
        case 2: {
            cout << "Enter a new password : "<<endl;
            string newpassword;
            cin >> newpassword;
            string sql = "UPDATE student SET student.Password = '"+newpassword+"' WHERE student.Id = " + student_id+";";
            mysql_query(conn,"START TRANSACTION;");
            mysql_query(conn, sql.c_str());
            mysql_query(conn,"COMMIT;");
            res_set = mysql_store_result(conn);
            cout << "*** The password is changed successfully! ***" << endl;
            personalinfo( student_id );
            break; }
        case 3: {
            menu(student_id);
        }
            
        default: {
            cout << "*** You need to enter a valid option! ***" << endl;
            personalinfo(student_id);
        }
            
            
    }
    
}

void withdraw(string student_id){
    string opt;
    time_t rawtime;
    time(&rawtime);
    string semester1;
    string semester2;
    //get year and month
    struct tm* timeinfo;
    timeinfo = localtime(&rawtime);
    
    int mon = timeinfo->tm_mon + 1;
    int year = timeinfo->tm_year + 1900;
    
    //get the current enrolled classes' quarter information
    if (mon>=9 && mon<=11){semester1 = "Q1";};
    if (mon==12 || mon == 1 || mon ==2 ){semester1 = "Q2";};
    if (mon>=3 && mon<=5){semester1 = "Q3";};
    if (mon>=6 && mon<=8){semester1 = "Q4";};
    
    if (semester1 == "Q1"){semester2 = "Q2";};
    if (semester1 == "Q2"){semester2 = "Q3";};
    if (semester1 == "Q3"){semester2 = "Q4";};
    if (semester1 == "Q4"){semester2 = "Q1";};
    
    //initial database result
    mysql_close(conn);
    conn = mysql_init ( NULL );
    mysql_real_connect(conn,"localhost","root","Albert@1357","project3-nudb",0,0,0);
    
    cout << "You can only withdraw the class you currently enrolled within two recent semesters." << endl;
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    //string withdraw1 = "CALL withdraw ("+student_id+","+ to_string(year) +",'"+semester1+"');" ;
    
    string withdraw1 = "CALL withdraw ("+student_id+","+ to_string(year) +",'"+semester1+"','"+semester2+"');";
    
    mysql_query(conn, withdraw1.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);
    
    if (numrows == 0){
        cout<<"no course can be withdrawn";
        cout << "*** Go back to student menu! ***" << endl << endl;
        menu(student_id);
    }
    else {
        
        cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
        cout << "<<<                         withdraw :                          >>>"<< endl;
        cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
        cout << " Course can withdraw : " << endl;
        
        
        string U;
        string Y;
        string S;
        vector<string> AAA;
        
        for (int i = 0; i < numrows; i++)
        {
            row = mysql_fetch_row(res_set);
            if (row!= NULL)
            {
                cout << "option: " << i << " UoSCode: " << row[0] <<"  Year: "<< row[1] << " Semester: " <<row[2]<<endl;
                AAA.push_back(row[0]);
                AAA.push_back(row[1]);
                AAA.push_back(row[2]);
            }
        }
        
        cout << "make your selection now: " << endl;
        cin >> opt;
        int opt_ = stoi(opt);
        
        //determine if input is valid or not
        if(opt_>=numrows || opt_<0){
            cout << "please make a valid input!" << endl;
            cout << "go back to withdraw menue!" << endl;
            withdraw(student_id);
        }
        else
        {
            //get the user selection
            U = AAA[3*opt_];
            Y = AAA[3*opt_+1];
            S = AAA[3*opt_+2];
            cout<<"what i get U is: "<< U <<endl;
            cout<<"Y: " << Y <<endl;
            cout<<"S：" << S <<endl;
            mysql_free_result(res_set);
            mysql_close(conn);
            conn = mysql_init ( NULL );
            mysql_real_connect(conn,"localhost","root","Albert@1357","project3-nudb",0,0,0);
            
            string withdraw11 = "CALL declass ("+student_id+", '"+ U +"' ,"+Y+",'"+S+"');";
            MYSQL_RES *res_set2;
            mysql_query(conn, withdraw11.c_str());
            res_set2 = mysql_store_result(conn);
            mysql_free_result( res_set2 );
            
            cout << "*** Successfully withdraw! ***" << endl << endl;
            
            MYSQL_RES *res_set00;
            string WARNING = "SELECT * FROM Warning ;";
            mysql_query(conn, WARNING.c_str());
            res_set00 = mysql_store_result(conn);
            int numrows10 = (int)mysql_num_rows(res_set00);
            
            if(numrows10!=0)
            {cout << "*** Warning: Enrollment number goes below 50% of the MaxEnrollment! ***" << endl<< endl;
                mysql_free_result(res_set00);}
            
            cout << "Enter 1 to withdraw another course " << endl << "Enter 2 to go back to memu" << endl;
            cout << "Your option is : ";
            string wp;
            cin >> wp;
            int ww = stoi(wp);
            switch(ww){
                case 1:{
                    withdraw(student_id);
                    break;
                }
                case 2:{
                    menu(student_id);
                    break;
                }
                default:{
                    cout<<"<<<<<invalid command<<<<"<<endl;
                    cout<<"*****back to menu*******"<<endl;
                    menu(student_id);
                }
            }
            
        }
        
        
        
        
        
        
    }
    
}

void enroll(string student_id){
    //inital time for enroll;
    string opt;
    time_t rawtime;
    time(&rawtime);
    string semester1;
    string semester2;
    //get year and month
    struct tm* timeinfo;
    timeinfo = localtime(&rawtime);
    
    int mon = timeinfo->tm_mon + 1;
    int year = timeinfo->tm_year + 1900;
    
    //get the current enrolled classes' quarter information
    if (mon>=9 && mon<=11){semester1 = "Q1";};
    if (mon==12 || mon == 1 || mon ==2 ){semester1 = "Q2";};
    if (mon>=3 && mon<=5){semester1 = "Q3";};
    if (mon>=6 && mon<=8){semester1 = "Q4";};
    
    if (semester1 == "Q1"){semester2 = "Q2";};
    if (semester1 == "Q2"){semester2 = "Q3";};
    if (semester1 == "Q3"){semester2 = "Q4";};
    if (semester1 == "Q4"){semester2 = "Q1";};
    
    //initialize database result
    mysql_close(conn);
    conn = mysql_init ( NULL );
    mysql_real_connect(conn,"localhost","root","Albert@1357","project3-nudb",0,0,0);
    
    cout << "You can only enroll the class with the following choice: " << endl;
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    string enroll_list = "CALL enrollment_list ("+student_id+","+ to_string(year) +",'"+semester1+"','"+semester2+"');";
    
    mysql_query(conn, enroll_list.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);
    
    if (numrows == 0){
        cout<<"no course can be enrroled now";
        cout << "*** Go back to student menu! ***" << endl << endl;
        menu(student_id);
    }
    else {
        
        cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
        cout << "<<<                         Enroll List :                          >>>"<< endl;
        cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
        cout << " Course can be enrolled : " << endl;
        
        
        string U;
        string Y;
        string S;
        vector<string> AAA;
        
        for (int i = 0; i < numrows; i++)
        {
            row = mysql_fetch_row(res_set);
            if (row!= NULL)
            {
                cout << "option: " << i << " UoSCode: " << row[0] <<"  Year: "<< row[1] << " Semester: " <<row[2]<<endl;
                AAA.push_back(row[0]);
                AAA.push_back(row[1]);
                AAA.push_back(row[2]);
            }
        }
        
        cout << "make your selection now: " << endl;
        cin >> opt;
        int opt_ = stoi(opt);
        
        //determine if input is valid or not
        if(opt_>=numrows || opt_<0){
            cout << "please make a valid input!" << endl;
            cout << "go back to enroll menue!" << endl;
            enroll(student_id);
        }
        else{
            //get the user selection
            U = AAA[3*opt_];
            Y = AAA[3*opt_+1];
            S = AAA[3*opt_+2];
            cout<<"what i get U is: "<< U <<endl;
            cout<<"Y: " << Y <<endl;
            cout<<"S：" << S <<endl;
            
            mysql_free_result(res_set);
            mysql_close(conn);
            
            MYSQL_RES *res_set4;
            MYSQL_ROW row;
            
            conn = mysql_init ( NULL );
            mysql_real_connect(conn,"localhost","root","Albert@1357","project3-nudb",0,0,0);
            
            string state_enroll = "CALL coursecheck("+student_id+",'"+ U +"',"+Y+",'"+S+"',@output);";
            
            
            
            mysql_query(conn, state_enroll.c_str());
            mysql_query(conn, "select @output;");
            res_set4 = mysql_store_result(conn);
            
            int numrows = (int)mysql_num_rows(res_set4);
            cout << "coursecheck length: " << numrows << endl;
            
            row = mysql_fetch_row(res_set4);
            string status_;
            status_ = row[0];
            cout << "here is the status_" << status_ << endl;
            
            mysql_free_result(res_set4);
            
            mysql_close(conn);
            
            MYSQL_RES *res_set5;
            MYSQL_ROW row1;
            
            conn = mysql_init ( NULL );
            mysql_real_connect(conn,"localhost","root","Albert@1357","project3-nudb",0,0,0);
            
            string course_enroll = "CALL coursecheck_state("+student_id+",'"+ U +"',"+Y+",'"+S+"');";
            
            mysql_query(conn, course_enroll.c_str());
            res_set5 = mysql_store_result(conn);
            int numrows1 = (int)mysql_num_rows(res_set5);
            
            
            if(status_ == "")
            {
                cout<<"no condition mathes here!! wired back to enroll"<<endl;
                enroll(student_id);
            }
            
            int status = stoi(status_);
            int choice;
            
            switch(status){
                case 0:{
                    cout << "successfully enroll..." << endl;
                    cout << "option 1: enroll another course;" << endl;
                    cout << "option 2: back to menu;" << endl;
                    cin >> choice;
                    if(choice == 1){enroll(student_id);}
                    if(choice == 2)
                    {
                        menu(student_id);
                        
                    }
                    else{
                        cout << "invalid input back to enroll.." << endl;
                        enroll(student_id);
                    }
                    
                    break;
                }
                    
                case 1:{
                    cout << "all prerequeste successfell but exceeds maximum capacity for this course" <<endl;
                    
                    for (int i = 0; i < numrows1; i++)
                    {
                        row1 = mysql_fetch_row(res_set5);
                        if (row1!= NULL)
                            cout << "invalid courses:" << row1[0] <<endl;
                        
                    }
                    cout << "option 1: enroll another course;" << endl;
                    cout << "option 2: back to menu;" << endl;
                    cin >> choice;
                    if(choice == 1){enroll(student_id);}
                    if(choice == 2)
                    {
                        menu(student_id);
                        
                    }
                    else{
                        cout << "invalid input back to enroll.." << endl;
                        enroll(student_id);
                    }
                    
                    break;
                }
                    
                case 2:{
                    cout << "some of this course'prerequeste classes are status NULL or F" << endl;
                    
                    for (int i = 0; i < numrows1; i++)
                    {
                        row1 = mysql_fetch_row(res_set5);
                        if (row1!= NULL)
                            cout << "invalid courses:" << row1[0] <<endl;
                        
                    }
                    cout << "option 1: enroll another course;" << endl;
                    cout << "option 2: back to menu;" << endl;
                    cin >> choice;
                    if(choice == 1){enroll(student_id);}
                    if(choice == 2)
                    {
                        menu(student_id);
                        
                    }
                    else{
                        cout << "invalid input back to enroll.." << endl;
                        enroll(student_id);
                    }
                    
                    break;
                }
                    
                case 3:{
                    cout << "this course needs prerequisite but prerequisite not in your transcript:" << endl;
                    
                    for (int i = 0; i < numrows1; i++)
                    {
                        row1 = mysql_fetch_row(res_set5);
                        if (row1!= NULL)
                            cout << "invalid courses:" << row1[0] <<endl;
                        
                    }
                    
                    cout << "option 1: enroll another course;" << endl;
                    cout << "option 2: back to menu;" << endl;
                    cin >> choice;
                    if(choice == 1){enroll(student_id);}
                    if(choice == 2)
                    {
                        menu(student_id);
                        
                    }
                    else{
                        cout << "invalid input back to enroll.." << endl;
                        enroll(student_id);
                    }
                    
                    break;
                }
                    
                case 4:{
                    cout << "this course does not have prerequiste, but exceeds the maximum capacity " << endl;
                    
                    for (int i = 0; i < numrows1; i++)
                    {
                        row1 = mysql_fetch_row(res_set5);
                        if (row1!= NULL)
                            cout << "invalid courses:" << row1[0] <<endl;
                        
                    }
                    cout << "option 1: enroll another course;" << endl;
                    cout << "option 2: back to menu;" << endl;
                    cin >> choice;
                    if(choice == 1){enroll(student_id);}
                    if(choice == 2)
                    {
                        menu(student_id);
                        
                    }
                    else{
                        cout << "invalid input back to enroll.." << endl;
                        enroll(student_id);
                    }
                    
                    break;
                }
                    
                default:{
                    cout << "no valid status for this course, error , back to menue auto" << endl;
                    menu(student_id);
                }
                    
                    
            }
            mysql_free_result(res_set5);
            
            
        }
        
        
    }
    
    
    
}


void login()
{
    MYSQL_RES *res_set = NULL;
    MYSQL_ROW row;
    // MYSQL  mysql;
    
    string id = "33";
    string password = "l";
    vector<string> ID;
    vector<string> PASS;
    
    string sql = "select * from Student;";
    mysql_query(conn,sql.c_str());
    res_set = mysql_store_result(conn);
    
    int numrows =  (int)mysql_num_rows(res_set);
    
    for (int i =0; i< numrows; i++)
    {
        row = mysql_fetch_row(res_set);
        if (row != NULL)
        {
            ID.push_back(row[0]);
            std::cout << "id: " << row[0] << endl;
            PASS.push_back(row[2]);
            std::cout << "course " << row[2] << endl;
        }
    }
    do{
        cout << "Now we are in user login page" << endl;
        cout << "Please give us your Id"<< endl;
        getline(cin,id);
        //studid=std::stoi(id);
        cout << "Please give us your Password"<< endl;
        getline(cin,password);
        
    }while(login_helper(id, password, ID, PASS));
    
    menu(id);
    
}


int main(int argc, const char * argv[]) {
    
    //connect to database first
    //mysql_free_result(res_set);
    
    conn = mysql_init(NULL);
    mysql_real_connect(conn,"localhost","root","Albert@1357","project3-nudb",0,0,0);
    
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    cout << "You are now in Northwestern registration system!" << endl;
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    
    login();
    
    
    
}













