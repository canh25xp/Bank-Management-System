#include<iostream>
#include<fstream>
#include<string.h>
#include<string>
#include<conio.h>
#include<unistd.h>
#include<iomanip>

using namespace std;

#define adminPassword "11111" // Change password here.`

struct Account{
	int accNum=1000;
	char accName[50];
	int idNum;
	unsigned long int accBalance;
}customer;

void checkPassword(string password);
void adminPanel();
void customerPanel();
void createAcc(Account c);
void showAcc(Account c);

int main(){
    // system("color %");
    // Color table
	system("color 3f");
    char choice;
    while(1){
        system("cls");
        cout<<"Login"			<<endl;
        cout<<"1.Admin"			<<endl;
        cout<<"2.Customer"		<<endl;
        cout<<"3.Exit"			<<endl;
        cout<<"Enter your choice : ";
        cin>>choice;
        system("cls");
        switch(choice){
            case '1':{
				adminPanel();
                break;
            }
            case '2':{
				customerPanel();
                break;
            }
            case '3':{
				// cout<<"Exiting, Thank you!"<<endl;
				// system("pause");
				exit(0);
			}
        }
    }
    return 0;
}

void createAcc(Account c){
	cout<<"Enter Account Number : ";
	cin>>c.accNum;
	cout<<"Enter Account Holder Name : ";
	cin.ignore();
	cin.getline(c.accName,50);
	cout<<"Enter identity number : ";
	cin>>c.idNum;
	cout<<"Enter Initial Balance : ";
	cin>>c.accBalance;
}
void showAccData(Account c){
	cout<<"Account Number : "<<c.accNum<<endl;
	cout<<"Account Holder : "<<c.accName<<endl;
	cout<<"Identity Card Number : "<<c.idNum<<endl;
	cout<<"Account Balance : "<<c.accBalance<<endl;
}
void checkPassword(string password){
	unsigned int trail=5;
	begin :
	// cout<<"Require admin permission."<<endl;
	cout<<"Enter password to continue : ";
	char ch;
	string input="";
	ch = _getch();
	while(ch != 13){
		if(ch==127||ch==8){ // backspace, delete
			system("cls");
			goto begin;
		}
		input.push_back(ch);
		cout<<"*";
		ch = _getch();
	}
	system("cls");
	if(input==password){
		cout<<"Password correct."<<endl;
        return;
	}
	else{
		trail--;
		if(trail==0){
			cout<<"0 trail left.Try again after 30 seconds...";
			sleep(30);
            system("cls");
            system("pause");
			exit(0);
		}
		cout<<"Password incorrect. ( "<<trail<<" trail left )";
		getch();
		system("cls");
		goto begin;
	}
}
void adminPanel(){
	checkPassword(adminPassword);
	char choice;
	int flag;
	int num;
	while(1){
		system("cls");
		cout<<"=========Admin Panel========"<<endl;
		cout<<"1.Create Account."<<endl;
		cout<<"2.Display All Account Info."<<endl;
		cout<<"3.Search for Account Number."<<endl;
		cout<<"4.Modify Account Infomation."<<endl;
		cout<<"5.Delete Account."<<endl;
		cout<<"6.Recycle Bin"<<endl;
		cout<<"7.Reset All Data"<<endl;
		cout<<"8.Back"<<endl;
		cout<<"Enter your choice : ";
		cin>>choice;
		system("cls");
		switch (choice){
			case '1':{
				ofstream accountFile;
				accountFile.open("Account List.dat",ios::binary|ios::app);
				createAcc(customer);
				accountFile.write(reinterpret_cast<char*>(&customer),sizeof(Account));
				accountFile.close();
				system("pause");
				break;
			}
			case '2':{
				ifstream accountFile;
				accountFile.open("Account List.dat",ios::binary|ios::app);
				if(!accountFile){
					cout<<"File could not be open."<<endl;
					system("pause");
					return ;					
				}
				flag=0;
				while(accountFile.read(reinterpret_cast<char*>(&customer),sizeof(Account))){
					showAccData(customer);
					cout<<"\n=================================================\n";
					flag=1;
				}
				if(flag==0){
					cout<<"Record file is Empty"<<endl;
				}
				accountFile.close();
				system("pause");
				break;
			}
			case '3':{
				cout<<"Enter ID to found : "; 
				cin>>num;
				ifstream accountFile;
				accountFile.open("Account List.dat",ios::binary);
				if(!accountFile){
					cout<<"File could not be open."<<endl;
					system("pause");
					break;
				}
				flag=0;
				while(accountFile.read(reinterpret_cast<char*>(&customer),sizeof(Account))){
					if(customer.accNum==num){
						cout<<"=================================================\n";
						showAccData(customer);
						cout<<endl;
						flag=1;
					}
				}
				accountFile.close();
				if(flag==0){
					cout<<"Account Number "<<num<<" does not exists\n";
				}
				system("pause");
				break;
			}
			case '4':{
				cout<<"Enter Account Number to edit : "; 
				cin>>num;
				flag=0;
				fstream accountFile;
				accountFile.open("Account List.dat",ios::binary|ios::in|ios::out);
				if(!accountFile){
					cout<<"File could not be opened."<<endl;
					system("pause");
					break;
				}
				while(accountFile.eof()==0 && flag==0){
					accountFile.read(reinterpret_cast<char*>(&customer),sizeof(Account));
					if(customer.accNum==num){
						cout<<"=================================================\n";
						cout<<"Enter Account "<<num<<" new details:\n";
						createAcc(customer);
						int pos=(-1)*static_cast<int>(sizeof(customer));
					    accountFile.seekp(pos,ios::cur);
					    accountFile.write(reinterpret_cast<char*>(&customer),sizeof(Account));
					    cout<<"Account Updated\n";
					    flag=1;
					}
				}
				accountFile.close();
				if(flag==0)
					cout<<"Record Not Found"<<endl;
				system("pause");
				break;
			}
			case '5':{
				cout<<"Enter Account Number to delete : "; 
				cin>>num;
				ifstream accountFile;
				accountFile.open("Account List.dat",ios::binary);
				if(!accountFile){
					cout<<"File could not be open."<<endl;
					system("pause");
					break;
				}
				ofstream tempFile;
				ofstream recycleBin;
				recycleBin.open("Recycle Bin.dat",ios::binary|ios::app);
				tempFile.open("Temp.dat",ios::out);
				accountFile.seekg(0,ios::beg);
				flag=0;
				while(accountFile.read(reinterpret_cast<char *> (&customer), sizeof(Account))){
					if(customer.accNum!=num){
						tempFile.write(reinterpret_cast<char *> (&customer), sizeof(Account));
					}
					else{
						flag=1;
						recycleBin.write(reinterpret_cast<char *> (&customer), sizeof(Account));
					}
				}
				tempFile.close();
				accountFile.close();
				recycleBin.close();
				if(flag==0){
					cout<<"ID "<<num<<" does not exists"<<endl;
					remove("Temp.dat");
					system("pause");
				}
				else{
					remove("Account List.dat");
					rename("Temp.dat","Account List.dat");
					cout<<"Record Deleted.\n";
					system("pause");
				}
				break;
			}
			case '6':{
				ifstream accountFile;
				accountFile.open("Recycle Bin.dat",ios::binary);
				cout<<"\tRecycle Bin";
				cout<<"\n=================================================\n";
				if(!accountFile){
					cout<<"Recycle Bin is empty"<<endl;
					system("pause");
				}
				else{
					while(accountFile.read(reinterpret_cast<char*>(&customer),sizeof(Account))){
						showAccData(customer);
						cout<<"\n=================================================\n";
					}
					accountFile.close();
					system("pause");
				}
				break;
			}
			case '7':{
				char confirm='1';
				cout<<"All recorded data will be delete\nConfirm to reset ? (y/n) ";
				while(confirm!='n'){
					cin>>confirm;
					if(confirm=='y'){
						remove("Account List.dat");
						remove("Recycle Bin.dat");
						remove("Temp.dat");
						cout<<"Reset Successfully.";
						return;					
					}
					else{
						system("cls");
						cout<<"All recorded data will be delete\nConfirm to reset ? (y/n) ";
					}
				}
				break;
			}
			case '8':{
				return;
			}
		}
	}
}

void customerPanel(){
	char choice;
	int flag;
	int num;
	Account temp;
	cout<<"Enter ID to found : "; 
	cin>>num;
	ifstream accountFile;
	accountFile.open("Account List.dat",ios::binary|ios::app);
	if(!accountFile){
		cout<<"File could not be open."<<endl;
		system("pause");
		return ;	
	}
	flag=0;
	while(accountFile.read(reinterpret_cast<char*>(&customer),sizeof(Account))){
		if(customer.accNum==num){
			temp=customer;
			flag=1;
		}
	}
	accountFile.close();
	if(flag==0){
		cout<<"Account Number "<<num<<" does not exists\n";
		return;
	}
	while(1){
		system("cls");
		cout<<"=========Customer Panel========"<<endl;
		cout<<"1.Deposit Cash."<<endl;
		cout<<"2.With Draw cash."<<endl;
		cout<<"3.Check Balance"<<endl;
		cout<<"4.Back"<<endl;
		cout<<"Enter your choice : ";
		cin>>choice;
		system("cls");
		switch (choice){
			case '1':{
				system("pause");
				break;
			}
			case '2':{
				system("pause");
				break;
			}
			case '3':{
				cout<<"Account Balance : "<<temp.accBalance<<endl;
				system("pause");
				break;
			}
			case '4':{
				return;
			}
		}
	}
}
