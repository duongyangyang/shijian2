#include <iostream>
#include <string.h>
#include <string>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <vector>
using namespace std;

int main(){
    char start_stop = 'Y'; 

    while (start_stop == 'Y'){
        //_____Open file congig.txt
        ifstream fileInput("config.txt");
        if (fileInput.fail()){ //There is no needed file, 配置文件格式不
            cout << "配置文件格式不对" << endl;
        }
        //_____Read file and copy file's data to an array (line[]).
        string line[9999];
        int lines_of_doc = 0; //文件的行的数量。
        while (!fileInput.eof()){
            //注：每次遇到换行getline会结束，所以每次这个循环运行就是在处理文件的某一行。
            char temp[9999]; //A character array in which to store the data.
            fileInput.getline(temp, 9999); //9999 is the maximum number of characters to extract, 在这我任意取9999。
            line[lines_of_doc] = temp; 
            lines_of_doc++; //计算文件有多少行为了以后要使用到。
        } 
        fileInput.close(); //Close the file 




        //_____Take N and R
        int filesize_num = 0; //"FILEZIZE = N" 中的N。
        int randnum_num = 0; //"RANDNUM = R" 中的R。
        int blocks; //The number of blocks that will be filled.
        for (int i = 0; i < lines_of_doc + 1; i++){
            string temp_line = line[i]; 
                /*A temporary string variable that used for processing data without changing the original data. 
                To be honest, in this program this thing does not have any function; however, I like it and
                maybe in the future when we need to do more complex projects it will be necessary.*/
            if (temp_line.find("#") == 0){ //Do not process data if a line starts with # symbol.
                continue;
            }
            else{//Process data of those lines which do not start with # symbol.
                string FILESIZE = "FILESIZE = "; //Just a string variable.
                string RANDNUM = "RANDNUM = "; //Another string variable.
                if(temp_line.substr(0,11) == FILESIZE || temp_line.substr(0,10) == RANDNUM){//Find lines that begin with FILESIZE or RANDNUM
                    //Find positions of spaces (空格)
                    vector <int> pos_space; 
                    string equal = " ";
                    for (int j = 0; j < temp_line.size(); j++){ 
                        string tempp = temp_line.substr(j,1);
                        if (tempp == equal) {
                            pos_space.push_back(j);
                        }
                    }    

                    //___Take N/R as a string and convert to double and int.
                    string N_or_R;
                    if (pos_space.size() == 2){ //There are only two spaces in this line.
                        N_or_R = temp_line.substr(pos_space.at(1)+1);
                    }
                    else{
                        N_or_R = temp_line.substr(pos_space.at(1)+1,pos_space.at(2)-pos_space.at(1)-1); //More than two spaces.
                    }
                    /*
                    Explain: We use this if-else block to avoid 'std::out_of_range' bug, which will appear if there are 
                    only two spaces but we use "pos_space.at(2)"(not exist) in else block.
                    */
                
                    int NR_i = stoi(N_or_R); double NR_d = stod(N_or_R); //String to int and sting to double.

                    //___Process whether N/R is legal or not.
                    if (temp_line.substr(0,11) == FILESIZE){
                        if (NR_i > 0 && (NR_d - NR_i) == 0){ 
                            filesize_num = NR_i;                      
                        }
                        else{ //解决不存在"FILESIZE = N"的行的问题。
                            cout << "配置文件格式不对" << endl; 
                            filesize_num = 4;
                        }
                    }
                    else{
                        if (NR_i > 0 && (NR_d - NR_i) == 0){
                            randnum_num = NR_i;
                        }
                        else{
                            cout << "配置文件格式不对" << endl; 
                            blocks = 1;           
                        }
                    }
                    pos_space.clear(); //Clear the vector after processing each line.
                }          
            }    
        }

        //______Creat matrix
        char matrix[filesize_num][filesize_num]; //Because matrix including 2, 4 and * so I have to use char type.
        char two_or_four_char;
        int two_or_four;
        srand(time(NULL));
        while (two_or_four != 2 && two_or_four != 4){ //Random 2 or 4. The probability here is not 50-50, 我在下面解决这个问题。
            two_or_four = rand()%5;
        }
        if (two_or_four == 2) { two_or_four_char = '2';} //Convert int to char (because type of matrix is char, cannot use int)
        if (two_or_four == 4) { two_or_four_char = '4';}

        if (blocks != 1) {blocks = (filesize_num*filesize_num*randnum_num)/100;} 
        //blocks = 1是有错误, 没有错误我正常进行计算会被填的空位的数量。
        int count = 0;
        while (count < blocks){ //按照题目要求填空位。
            int row = rand()%filesize_num; //随机选行
            int col = rand()%filesize_num; //随机选列
            if (matrix[row][col] != '2' && matrix[row][col] != '4'){ //只填以前没被填过的空位。
                if (two_or_four_char == '2'){
                    matrix[row][col] = two_or_four_char;
                    two_or_four_char = '4'; //如果上次填的是2，下次会填4.满足比例50/50的要求。
                }
                else if (two_or_four_char == '4'){
                    matrix[row][col] = two_or_four_char;
                    two_or_four_char = '2'; //如果上次填的是4，下次会填2.
                }
                count++; //每次有空位被填会加上1，直到count=blocks-1会停下（因为count从0开始），所以会有blocks个空位被填。
            }
        }

        for (int i = 0; i < filesize_num; i++){ //不被填的空位，输出“*”。
            for (int j = 0; j < filesize_num; j++){
                if (matrix[i][j] != '2' && matrix[i][j] != '4'){
                    matrix[i][j] = '*' ;
                }
            }
        }

        //_____Print matrix and some related information in console window.
        for (int i = 0; i < filesize_num; i++){ 
            for (int j = 0; j < filesize_num; j++){
                cout << setw(3) << matrix[i][j];
            }
            cout << endl;
        }
        cout << "There are " << blocks << " blocks was (were) filled " << endl;
        cout << "Matrix size is " << filesize_num << "*" << filesize_num << endl;

        //_____Write (and create) output file (cmd.txt)
        ofstream fileOutput("cmd.txt", ios::trunc);
        if (fileOutput.fail()){ //There is no needed file, 配置文件格式不对
            cout << "配置文件格式不" << endl;
            ofstream {"cmd.txt"}; //Create new cmd.txt file
        }
        //_____在cmd.txt里输出矩阵和一些有关信息
        for (int i = 0; i < filesize_num;  i++){ 
            for (int j = 0; j < filesize_num; j++){
                fileOutput << " " << matrix[i][j] << " ";
            }
            fileOutput << endl;
        }
        fileOutput << "There are " << blocks << " block(s) was(were) filled" << endl;
        fileOutput << "The size of matrix is " << filesize_num << "x" << filesize_num << endl;
        fileOutput.close();
        
        //_____Press Q to end
        cout << "Press Y to continue or Press Q to end this program: "; cin >> start_stop;  
        if (start_stop != 'Y' && start_stop != 'Q'){
            cout << "Undefined character!!" << endl;
            cout << "Press Y to continue or Press Q to end this program: "; cin >> start_stop;
        }
    }
    return 0;
}