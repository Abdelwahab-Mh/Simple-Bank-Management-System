#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
using namespace std;
const string ClientFileName = "Clients.txt";
struct sClient
{
    string AccountNum;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkToDelete = false;
};
string ReadString()
{
    string S1;
    cout << "Please Enter Your Account Number? " << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, S1);
    return S1;
}
char ReadChar()
{
    char C1;
    cout << "Please Enter a Character? " << endl;
    cin >> C1;
    return C1;
}
sClient ReadClientData()
{
    sClient Client;
    cout << "Enter Account Number ? " << endl;
    // usage std::ws will extract all the whitespace character
    getline(cin >> ws, Client.AccountNum);
    cout << "Enter PinCode ? " << endl;
    getline(cin, Client.PinCode);
    cout << "Enter Name ? " << endl;
    getline(cin, Client.Name);
    cout << "Enter Phone ? " << endl;
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance ? " << endl;
    cin >> Client.AccountBalance;
    cout << endl;
    return Client;
}
void PrintClientRecord(sClient client)
{
    cout << "| " << setw(15) << left << client.AccountNum;
    cout << "| " << setw(10) << left << client.PinCode;
    cout << "| " << setw(40) << left << client.Name;
    cout << "| " << setw(12) << left << client.Phone;
    cout << "| " << setw(12) << left << client.AccountBalance;
}
void PrintAllClientRecord(vector<sClient> client)
{
    cout << "\n\t\t\t\t\tClient List (" << client.size() << ")" << "Client(s)." << endl;
    cout << "------------------------------------------------------------------";
    cout << "------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n------------------------------------------------------------------";
    cout << "------------------------------------------------------------------" << endl;
    for (sClient c : client)
    {
        PrintClientRecord(c);
        cout << endl;
    }
    cout << "------------------------------------------------------------------";
    cout << "------------------------------------------------------------------" << endl;
}
vector<string> SplitToVector(string S1, string token)
{
    vector<string> v1;
    string sWord;
    short pos = 0;
    while ((pos = S1.find(token)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            v1.push_back(sWord);
        }
        S1.erase(0, pos + token.length());
    }
    if (S1 != "")
    {
        v1.push_back(S1);
    }
    return v1;
}
sClient ClientDataToRecord(string line, string Seperator = "#//#")
{
    sClient client;
    vector<string> vClient;
    vClient = SplitToVector(line, Seperator);
    client.AccountNum = vClient[0];
    client.PinCode = vClient[1];
    client.Name = vClient[2];
    client.Phone = vClient[3];
    client.AccountBalance = stod(vClient[4]);
    return client;

}
vector<sClient> getClientFromFile(string FileName)
{
    string s1;
    sClient client;
    fstream MyFile;
    vector<sClient> vClient;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        while (getline(MyFile, s1))
        {
            vClient.push_back(ClientDataToRecord(s1, "#//#"));
        }
        MyFile.close();
    }
    return vClient;
}
string ClientDataToOneLine(sClient Client, string Seperator = "#//#")
{
    return  Client.AccountNum + Seperator + Client.PinCode + Seperator + Client.Name + Seperator + Client.Phone + Seperator + to_string(Client.AccountBalance);
}
void AddingClientToFile(sClient client)
{
    string Line = ClientDataToOneLine(client);
    fstream MyFile;
    MyFile.open(ClientFileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << Line << endl;
        MyFile.close();
    }
}
void AddNewClient()
{
    char c = 'y';
    while (c == 'y' || c == 'Y')
    {
        AddingClientToFile(ReadClientData());
        cout << "Do you need to add new clients ? Y/N  ";
        cin >> c;
    }

}
void PrintData(sClient Client)
{
    cout << "Account Number: " << Client.AccountNum << endl;
    cout << "PinCode: " << Client.PinCode << endl;
    cout << "Name: " << Client.Name << endl;
    cout << "Phone: " << Client.Phone << endl;
    cout << "AccountBalance: " << Client.AccountBalance << endl;
}
bool FindClient(string AccountNum, vector<sClient> vClient, sClient& client)
{
    vClient = getClientFromFile(ClientFileName);
    for (sClient c : vClient)
    {
        if (AccountNum == c.AccountNum)
        {
            client = c;
            return true;
        }
    }
    return false;
}
bool MarkClientForDelete(string AccountNum, vector<sClient>& vClient)
{
    for (sClient& c : vClient)
    {
        if (c.AccountNum == AccountNum)
        {
            c.MarkToDelete = true;
            return true;
        }
    }
    return false;
}
vector<sClient> SaveClientToFile(string FileName, vector<sClient> vClient)
{
    string s1;
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (sClient c : vClient)
        {
            if (!c.MarkToDelete)
            {
                s1 = ClientDataToOneLine(c);
                MyFile << s1 << endl;
            }
        }
        MyFile.close();
    }
    return vClient;
}
bool DeleteClient(string AccountNum, vector<sClient>& vClient)
{
    char Answer = 'n';
    string s1;
    sClient client;

    if (FindClient(AccountNum, vClient, client))
    {
        cout << "The following are the client details: \n\n";
        PrintData(client);

        cout << "Are you sure you want to delete this client? y/n ?:  ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDelete(AccountNum, vClient);
            SaveClientToFile(ClientFileName, vClient);
            vClient = getClientFromFile(ClientFileName);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }


    }
    else
    {
        cout << "Client with Account Number (" << AccountNum << ") Not Found !\n";
        return false;
    }
}
sClient ReadClientDataUpdate(string AccountNum)
{
    sClient Client;
    //cout << "Enter Account Number ? " << endl;
    // usage std::ws will extract all the whitespace character
    //getline(cin >> ws, Client.AccountNum);
    Client.AccountNum = AccountNum;
    cout << "Enter PinCode ? " << endl;
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name ? " << endl;
    getline(cin, Client.Name);
    cout << "Enter Phone ? " << endl;
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance ? " << endl;
    cin >> Client.AccountBalance;
    cout << endl;
    return Client;
}
vector<sClient> SaveClientToFileUpdate(string FileName, vector<sClient> vClient)
{
    string s1;
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (sClient c : vClient)
        {
            if (!c.MarkToDelete)
            {
                s1 = ClientDataToOneLine(c);
                MyFile << s1 << endl;
            }
            else
            {
                s1 = ClientDataToOneLine(ReadClientDataUpdate(c.AccountNum));
                MyFile << s1 << endl;
            }
        }
        MyFile.close();
    }
    return vClient;
}
bool UpdateClient(string AccountNum, vector<sClient>& vClient)
{
    char Answer = 'n';
    string s1;
    sClient client;

    if (FindClient(AccountNum, vClient, client))
    {
        cout << "The following are the client details: \n\n";
        PrintData(client);

        cout << "Are you sure you want to update this client? y/n ?:  ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDelete(AccountNum, vClient);
            SaveClientToFileUpdate(ClientFileName, vClient);
            vClient = getClientFromFile(ClientFileName);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "Client with Account Number (" << AccountNum << ") Not Found !\n";
        return false;
    }
}
void MainMenue()
{
    int select;
    while (true)
    {
        system("cls");
        cout << "=======================================" << endl;
        cout << "\t\tMain Menue Screen\t\t" << endl;
        cout << "=======================================" << endl;
        cout << "\t[1] Show Client List.\t\t" << endl;
        cout << "\t[2] Add New Client.\t\t" << endl;
        cout << "\t[3] Delete Client.\t\t" << endl;
        cout << "\t[4] Update Client.\t\t" << endl;
        cout << "\t[5] Find Client.\t\t" << endl;
        cout << "\t[6] Exit.\t\t" << endl;
        cout << "=======================================" << endl;
        cout << "Choose what do you want to do? [1 to 6]?" << endl;
        cin >> select;
        if (select > 0 && select <= 6)
        {
            if (select == 1)
            {
                system("cls");
                cout << "=======================================" << endl;
                cout << "\t\tShow Client Screen\t\t" << endl;
                cout << "=======================================" << endl;
                vector<sClient> client;
                client = getClientFromFile(ClientFileName);
                PrintAllClientRecord(client);
                system("pause");
            }
            else if (select == 2)
            {
                system("cls");
                cout << "=======================================" << endl;
                cout << "\t\tAdd New Client Screen\t\t" << endl;
                cout << "=======================================" << endl;
                AddNewClient();
                system("pause");
            }
            else if (select == 3)
            {
                system("cls");
                cout << "=======================================" << endl;
                cout << "\t\tDelete Client Screen\t\t" << endl;
                cout << "=======================================" << endl;
                vector<sClient> client;
                client = getClientFromFile(ClientFileName);
                string S1 = ReadString();
                DeleteClient(S1, client);
                system("pause");
            }
            else if (select == 4)
            {
                system("cls");
                cout << "=======================================" << endl;
                cout << "\t\tUpdate Client Screen\t\t" << endl;
                cout << "=======================================" << endl;
                vector<sClient> client;
                client = getClientFromFile(ClientFileName);
                string s1 = ReadString();
                UpdateClient(s1, client);
                system("pause");
            }
            else if (select == 5)
            {
                system("cls");
                cout << "=======================================" << endl;
                cout << "\t\tFind Client Screen\t\t" << endl;
                cout << "=======================================" << endl;
                vector<sClient> client;
                client = getClientFromFile(ClientFileName);
                string S1 = ReadString();
                sClient c;
                 if (FindClient(S1,client,c))
                 {
                     cout << "The following are the client details: \n\n";
                     PrintData(c);
                 }
                 else
                 {
                     cout << "Client with Account Number (" << S1 << ") Not Found !\n";
                 }
                 system("pause");
            }
            else if (select == 6)
            {
                system("cls");
                exit(0);
            }
        }
        else
        {
            cout << "Please Select correct Value";
        }
    }
}
int main()
{
    MainMenue();
    return 0;
}