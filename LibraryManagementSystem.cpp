#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Date
{
public:
	int year, month, day;

	Date(int d, int m, int y) : year(y), month(m), day(d) {}

	bool operator>(Date &other)
	{
		if (year != other.year)
			return year > other.year;
		if (month != other.month)
			return month > other.month;
		return day > other.day;
	}
};

int daysInMonth(int month)
{
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
	{
		return 31;
	}
	else if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		return 30;
	}
	else if (month == 2)
	{
		return 28;
	}
	return -1;
}

int totalDays(int day, int month, int year)
{
	int total = 0;
	total += year * 365;

	for (int m = 1; m < month; m++)
	{
		total += daysInMonth(m);
	}

	total += day;
	return total;
}

class BookRequest
{
public:
	int UserId;
	Date reqDate;

	BookRequest(int Id, Date date) : UserId(Id), reqDate(date) {}

	bool operator>(BookRequest &other)
	{
		return reqDate > other.reqDate;
	}
};

struct CompareBookRequest
{
	bool operator()(BookRequest &lhs, BookRequest &rhs)
	{
		return lhs.reqDate > rhs.reqDate;
	}
};

class Book
{

public:
	int ISSN;
	vector<string> authors;
	string title;
	int reference_copies;
	int issuable_copies;
	priority_queue<BookRequest, vector<BookRequest>, CompareBookRequest> Demand;

	Book *left;
	Book *right;

	Book(int ISSN_, string title_, int refernce, int issuable, vector<string> authors_)
	{
		ISSN = ISSN_;
		title = title_;
		reference_copies = refernce;
		issuable_copies = issuable;
		authors = authors_;
		left = nullptr;
		right = nullptr;
	}

	void Display()
	{
		cout << "Title of the book: " << title << endl;
		cout << "ISSN Number: " << ISSN << endl;
		cout << "Reference Copies: " << reference_copies << endl;
		cout << "Issuable Copies: " << issuable_copies << endl;
		cout << "Authors: ";
		for (int i = 0; i < authors.size(); i++)
		{
			cout << authors[i] << " ";
		}
	}
};

class User
{

public:
	int userID;
	string name;
	vector<int> ISSNs;
	int fine;
	User *left;
	User *right;
	int height;
	vector<Date> Borrowdates;

	User(int Id, string name_)
	{
		userID = Id;
		name = name_;
		left = nullptr;
		right = nullptr;
		fine = 0;
		height = 1;
	}
	void Display()
	{
		cout << "User Id: " << userID << endl;
		cout << "User Name: " << name << endl;
		cout << "Outstandig Fine: " << fine << endl;
		cout << "ISSNs of the books borrowed: ";
		for (int i = 0; i < ISSNs.size(); i++)
		{
			cout << ISSNs[i] << " ";
		}
	}
};

class BST
{

private:
	Book *root;

public:
	BST()
	{
		root = nullptr;
	}

	Book *Insert(Book *root, int ISSN, string title, int ref, int issuable, vector<string> authors)
	{

		if (root == nullptr)
		{
			root = new Book(ISSN, title, ref, issuable, authors);
			return root;
		}

		if (ISSN < root->ISSN)
		{
			root->left = Insert(root->left, ISSN, title, ref, issuable, authors);
		}
		else if (ISSN > root->ISSN)
		{
			root->right = Insert(root->right, ISSN, title, ref, issuable, authors);
		}
		return root;
	}

	void Insert(int ISSN, string title, int ref, int issuable, vector<string> authors)
	{
		root = Insert(root, ISSN, title, ref, issuable, authors);
	}

	void DisplayBooks(Book *root)
	{

		if (root == nullptr)
		{
			return;
		}

		DisplayBooks(root->left);
		root->Display();
		DisplayBooks(root->right);
	}

	void DisplayBooks()
	{
		DisplayBooks(root);
	}

	bool Search(int ISSN, Book *root)
	{

		if (root == nullptr)
		{
			return false;
		}

		if (root->ISSN == ISSN)
		{
			return true;
		}
		else if (root->ISSN < ISSN)
		{
			return Search(ISSN, root->right);
		}
		else if (root->ISSN > ISSN)
		{
			return Search(ISSN, root->left);
		}
		return false;
	}

	bool Search(int ISSN)
	{
		return Search(ISSN, root);
	}

	bool BorrowBook(Book *&root, int ISSN, int id, Date date)
	{

		if (root == nullptr)
		{
			return false;
		}

		else if (ISSN == root->ISSN)
		{
			BookRequest B(id, date);
			root->Demand.push(B);
			if (root->issuable_copies != 0)
			{
				int id_ = root->Demand.top().UserId;
				root->Demand.pop();
				cout << "Book Borrowed: ISSN: " << ISSN << ", Title: " << root->title << endl;
				root->issuable_copies--;
				if (id_ != id)
				{
					cout << "Book demand updated. User " << id << " added to the queue for ISSN: " << ISSN << endl;
					return false;
				}
				return true;
			}
			else
			{

				cout << "Book demand updated. User " << id << " added to the queue for ISSN: " << ISSN << endl;
			}
		}

		else if (ISSN < root->ISSN)
		{
			return BorrowBook(root->left, ISSN, id, date);
		}
		else if (ISSN > root->ISSN)
		{
			return BorrowBook(root->right, ISSN, id, date);
		}
		return false;
	}

	bool BorrowBook(int ISSN, int id, Date date)
	{
		return BorrowBook(root, ISSN, id, date);
	}

	void UpdateBookinfo(Book *&root, int ISSN, string field)
	{
		if (root == nullptr)
		{
			cout << "Invalid ISSN" << endl;
			return;
		}
		else if (root->ISSN == ISSN)
		{
			if (field == "Title")
			{
				string newtitle;
				cout << "Enter the new title: ";
				getline(cin, newtitle);
				root->title = newtitle;
				cout << "Book updated: ISSN: " << ISSN << ", New Title: " << newtitle << endl;
			}
			else if (field == "Authors")
			{
				vector<string> newaut;
				string aut;
				cout << "Enter new Authors -1 to stop taking input: " << endl;
				while (aut != "-1")
				{
					getline(cin, aut);
					newaut.push_back(aut);
				}
				root->authors = newaut;
				cout << "Book updated: ISSN: " << ISSN << ", New Authors: ";
				for (int i = 0; i < newaut.size(); i++)
				{
					cout << newaut[i] << " ";
				}
				cout << endl;
			}
			else if (field == "Reference Copies")
			{
				int ref;
				cout << "Enter new value: ";
				cin >> ref;

				while (ref <= 0)
				{
					cout << "Invalid Value" << endl;
					cout << "Please Enter Again: ";
					cin >> ref;
				}
				cin.ignore();
				root->reference_copies = ref;
				cout << "Book updated: ISSN: " << ISSN << ", New Reference Copies: " << ref;
			}
			else
			{
				int iss;
				cout << "Enter new value: ";
				cin >> iss;
				while (iss <= 0)
				{
					cout << "Invalid Value" << endl;
					cout << "Please Enter Again: ";
					cin >> iss;
				}
				cin.ignore();

				root->issuable_copies = iss;
				cout << "Book updated: ISSN: " << ISSN << ", New Reference Copies: " << iss;
			}
		}
		else if (root->ISSN > ISSN)
		{
			UpdateBookinfo(root->left, ISSN, field);
		}
		else if (root->ISSN < ISSN)
		{
			UpdateBookinfo(root->right, ISSN, field);
		}
	}

	void UpdateBookinfo(int ISSN, string field)
	{
		UpdateBookinfo(root, ISSN, field);
	}

	Book *getSuccessor(Book *curr)
	{
		curr = curr->right;
		while (curr != NULL && curr->left != NULL)
			curr = curr->left;
		return curr;
	}

	Book *DeleteBook(Book *root, int ISSN)
	{
		if (root == NULL)
			return root;

		if (root->ISSN > ISSN)
			root->left = DeleteBook(root->left, ISSN);
		else if (root->ISSN < ISSN)
			root->right = DeleteBook(root->right, ISSN);

		else
		{
			while (!root->Demand.empty())
			{
				root->Demand.pop();
			}
			cout << "Book Deleted: ISSN: " << ISSN << ", " << "Title: " << root->title << endl;
			if (root->left == NULL)
			{
				Book *temp = root->right;
				delete root;
				return temp;
			}

			if (root->right == NULL)
			{
				Book *temp = root->left;
				delete root;
				return temp;
			}

			Book *succ = getSuccessor(root);
			root->ISSN = succ->ISSN;
			root->authors = succ->authors;
			root->Demand = succ->Demand;
			root->issuable_copies = succ->issuable_copies;
			root->reference_copies = succ->reference_copies;
			root->title = succ->title;
			root->right = DeleteBook(root->right, succ->ISSN);
		}
		return root;
	}

	void DeleteBook(int ISSN)
	{
		root = DeleteBook(root, ISSN);
	}

	void MostDemandingBook(Book *root, int &demand, Book *&temp)
	{

		if (root == nullptr)
		{
			return;
		}

		else if (root->Demand.size() > demand)
		{
			demand = root->Demand.size();
			temp = root;
		}

		MostDemandingBook(root->right, demand, temp);
		MostDemandingBook(root->left, demand, temp);
	}

	void MostDemandingBook()
	{	
		if(root==nullptr){
			cout<<"No Books in Record\n";
			return;
		}
		
		int demand = 0;
		Book *temp = nullptr;
		MostDemandingBook(root, demand, temp);
		cout << "Most Demanded Book: " << temp->title << ", ISSN: " << temp->ISSN << ", Pending requests: " << demand << endl;
	}

	void ReturnBook(Book *&root, int ISSN, int &id)
	{
		if (root == nullptr)
		{
			return;
		}
		else if (root->ISSN == ISSN)
		{
			root->issuable_copies++;
			cout << "Book Returned: ISSN: " << ISSN << ", Title: " << root->title << endl;
			if (root->issuable_copies > 0 && !root->Demand.empty())
			{
				int id_ = root->Demand.top().UserId;
				root->Demand.pop();
				id = id_;
				cout << "Book Borrowed: ISSN: " << ISSN << ", Title: " << root->title << " by User: " << id_;
				root->issuable_copies--;
			}
		}
		else if (root->ISSN < ISSN)
		{
			ReturnBook(root->right, ISSN, id);
		}
		else if (root->ISSN > ISSN)
		{
			ReturnBook(root->left, ISSN, id);
		}
	}

	int ReturnBook(int ISSN)
	{
		int id = -1;
		ReturnBook(root, ISSN, id);
		return id;
	}
};

int returnheightofNode(User *root)
{
	if (root == nullptr)
		return 0;
	return root->height;
}

class Avl
{

private:
	User *root;
	int borrow_period;

public:
	Avl()
	{
		root = nullptr;
		borrow_period = 14;
	}
	bool isEmpty()
	{
		return root == nullptr;
	}
	User *insert(User *root, int id, string name)
	{

		if (root == nullptr)
		{
			User *newNode = new User(id, name);
			cout << endl;
			cout << "User Registered: User ID: " << id << ", Name: " << name << endl;
			return newNode;
		}
		if (id < root->userID)
		{
			root->left = insert(root->left, id, name);
		}
		else if (id > root->userID)
		{
			root->right = insert(root->right, id, name);
		}
		else
		{
			return root;
		}
		root->height = 1 + max(height(root->left), height(root->right));

		int balance = getBalancedFactor(root);

		if (balance > 1 && id < root->left->userID)
			return rightRotate(root);

		if (balance < -1 && id > root->right->userID)
			return leftRotate(root);

		if (balance > 1 && id > root->left->userID)
		{
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}

		if (balance < -1 && id < root->right->userID)
		{
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}

		return root;
	}

	void insert(int id, string name)
	{

		root = insert(root, id, name);
	}

	int height(User *root)
	{

		if (root == nullptr)
		{
			return 0;
		}

		return 1 + max(height(root->left), height(root->right));
	}

	int height()
	{
		int h = height(root);
		return h;
	}

	int getBalancedFactor(User *root)
	{
		if (root == nullptr)
		{
			return 0;
		}

		return (returnheightofNode(root->left) - returnheightofNode(root->right));
	}

	User *rightRotate(User *y)
	{
		User *x = y->left;
		User *T2 = x->right;

		x->right = y;
		y->left = T2;

		y->height = 1 + max(returnheightofNode(y->left), returnheightofNode(y->right));
		x->height = 1 + max(returnheightofNode(x->left), returnheightofNode(x->right));

		return x;
	}

	User *leftRotate(User *x)
	{
		User *y = x->right;
		User *T2 = y->left;

		y->left = x;
		x->right = T2;

		x->height = 1 + max(returnheightofNode(x->left), returnheightofNode(x->right));
		y->height = 1 + max(returnheightofNode(y->left), returnheightofNode(y->right));

		return y;
	}

	bool IsAvl(User *root)
	{

		if (root == nullptr)
		{
			return true;
		}

		int balancedFactor = getBalancedFactor(root);
		if (balancedFactor == 0 || balancedFactor == 1 || balancedFactor == -1)
		{
			return true;
		}
		else
		{
			return false;
		}

		return IsAvl(root->left) && IsAvl(root->right);
	}

	bool IsAvl()
	{
		return IsAvl(root);
	}

	bool Search(int id, User *root)
	{

		if (root == nullptr)
		{
			return false;
		}

		if (root->userID == id)
		{
			return true;
		}
		else if (root->userID < id)
		{
			return Search(id, root->right);
		}
		else if (root->userID > id)
		{
			return Search(id, root->left);
		}
		return false;
	}

	bool Search(int val)
	{
		return Search(val, root);
	}

	void InOrder(User *root)
	{
		if (root == nullptr)
		{
			return;
		}

		InOrder(root->left);
		root->Display();
		cout << endl
			 << endl;
		InOrder(root->right);
	}

	void InOrder()
	{
		InOrder(root);
	}

	bool AddIsn(User *root, int id, int ISSN, Date date)
	{

		if (root == nullptr)
		{
			return false;
		}
		else if (root->userID == id)
		{
			root->ISSNs.push_back(ISSN);
			root->Borrowdates.push_back(date);
		}
		else if (id < root->userID)
		{
			return AddIsn(root->left, id, ISSN, date);
		}
		else if (id > root->userID)
		{
			return AddIsn(root->right, id, ISSN, date);
		}
		return false;
	}

	bool AddIsn(int id, int ISSN, Date date)
	{
		return AddIsn(root, id, ISSN, date);
	}

	void RemovefromISSNs(vector<int> &ISSNs, vector<Date> &borrowdates, int ISSN, Date &date)
	{
		if (ISSNs.empty())
		{
			return;
		}

		int temp = ISSNs.back();
		ISSNs.pop_back();
		Date tempd = borrowdates.back();
		borrowdates.pop_back();

		RemovefromISSNs(ISSNs, borrowdates, ISSN, date);

		if (temp != ISSN)
		{
			ISSNs.push_back(temp);
			borrowdates.push_back(tempd);
		}
		else
		{
			date = tempd;
		}
	}
	void RemoveBookRecord(User *root, int ISSN, Date &d)
	{

		if (root == nullptr)
		{
			return;
		}

		RemovefromISSNs(root->ISSNs, root->Borrowdates, ISSN, d);
		RemoveBookRecord(root->left, ISSN, d);
		RemoveBookRecord(root->right, ISSN, d);
	}

	void RemoveBookRecord(int ISSN, Date &d)
	{
		RemoveBookRecord(root, ISSN, d);
	}

	void DisplayUserInfo(int id, User *root)
	{

		if (root == nullptr)
		{
			return;
		}

		if (root->userID == id)
		{
			root->Display();
		}
		else if (root->userID > id)
		{
			DisplayUserInfo(id, root->left);
		}
		else if (root->userID < id)
		{
			DisplayUserInfo(id, root->right);
		}
	}

	void DisplayUserInfo(int id)
	{
		DisplayUserInfo(id, root);
	}

	void DisplayOverdueFines(User *root)
	{
		if (root == nullptr)
		{
			return;
		}
		DisplayOverdueFines(root->left);
		if (root->fine > 0)
		{
			cout << "User ID: " << root->userID << ", Fine: $" << root->fine << endl;
		}
		DisplayOverdueFines(root->right);
	}

	void DisplayNoFines(User *root)
	{
		if (root == nullptr)
		{
			return;
		}
		DisplayNoFines(root->left);
		if (root->fine == 0)
		{
			cout << "User ID: " << root->userID << " ";
		}
		DisplayNoFines(root->right);
	}

	void DisplayFines()
	{

		DisplayOverdueFines(root);
		cout << "No Fines for ";
		DisplayNoFines(root);
	}

	void AddFine(int id, User *&root, int fine)
	{
		if (root == nullptr)
		{
			return;
		}

		else if (root->userID == id)
		{

			root->fine += fine;
			if (fine != 0)
			{
				cout << endl;
				cout << "Fine Added: User ID: " << root->userID << ", Fine: $" << fine << endl;
			}
			return;
		}

		else if (root->userID > id)
		{
			AddFine(id, root->left, fine);
		}

		else if (root->userID < id)
		{
			AddFine(id, root->right, fine);
		}
	}

	void ReturnBook(int ISSN, Date returndate, int id)
	{
		Date borrow_date(0, 0, 0);
		RemoveBookRecord(ISSN, borrow_date);

		int finee = 0;

		int b_day = borrow_date.day;
		int b_month = borrow_date.month;
		int b_year = borrow_date.year;

		int r_day = returndate.day;
		int r_month = returndate.month;
		int r_year = returndate.year;

		int borrow_days = totalDays(b_day, b_month, b_year);
		int return_days = totalDays(r_day, r_month, r_year);

		int days_borrowed = return_days - borrow_days;

		if (days_borrowed > borrow_period)
		{
			finee = days_borrowed - borrow_period;
		}

		AddFine(id, root, finee);
	}
};

int main()
{
	BST *Books = new BST;
	Avl *Users = new Avl;

	int in = 0;
	cout << endl
		 << endl;
	cout << "\t \t \t \t Library management system  " << endl
		 << endl;
	do
	{
		cout << endl
			 << endl;
		cout << "Enter 1 to Add a Book" << endl;
		cout << "Enter 2 to Register a new User" << endl;
		cout << "Enter 3 to borrow a book" << endl;
		cout << "Enter 4 to return a book" << endl;
		cout << "Enter 5 to update book" << endl;
		cout << "Enter 6 to delete a book" << endl;
		cout << "Enter 7 to find Most demanding book" << endl;
		cout << "Enter 8 to Display User Record" << endl;
		cout << "Enter 9 to to Display Fines of all Users" << endl;
		cout << "Enter -1 to exit" << endl;
		cout << "Your input: ";
		cin >> in;
		cout<<endl<<endl;

		if (in == 1)
		{

			int ISSN, ref, iss;
			string aut, title;
			vector<string> authors;
			cout << "Enter ISSN: ";
			cin >> ISSN;
			cin.ignore();
			cout << "Enter Title: ";
			getline(cin, title);
			cout << "Enter Authors(comma separared) -1 to stop taking input: " << endl;
			while (aut != "-1")
			{
				getline(cin, aut);
				authors.push_back(aut);
			}
			cout << "Enter Reference Copies: ";
			cin >> ref;
			while (ref < 0)
			{
				cout << "Invalid Value" << endl;
				cout << "Please Enter Again: ";
				cin >> ref;
			}
			cout << "Enter Issuable Copies: ";
			cin >> iss;
			while (iss < 0)
			{
				cout << "Invalid Value" << endl;
				cout << "Please Enter Again: ";
				cin >> iss;
			}
			cin.ignore();
			Books->Insert(ISSN, title, ref, iss, authors);
		}

		else if (in == 2)
		{
			int id = 0;
			string name;
			cout << "Enter User ID: ";
			cin >> id;
			while (id <= 0)
			{
				cout << "Invalid Value" << endl;
				cout << "Please Enter Again: ";
				cin >> id;
			}
			if (Users->Search(id))
			{
				cout << endl;
				cout << "User already exists!" << endl;
				continue;
			}
			cin.ignore();
			cout << "Enter Name: ";
			getline(cin, name);
			Users->insert(id, name);
		}

		else if (in == 3)
		{
			int id;
			int ISSN;
			int day, month, year;
			cout << "Enter User ID: ";
			cin >> id;
			while (id <= 0)
			{
				cout << "Invalid Value" << endl;
				cout << "Please Enter Again: ";
				cin >> id;
			}
			if (!Users->Search(id))
			{
				cout << "The user is not registered" << endl;
				continue;
			}
			cin.ignore();
			cout << "Enter ISSN: ";
			cin >> ISSN;
			cin.ignore();
			if (!Books->Search(ISSN))
			{
				cout << "Invalid Book ISSN" << endl;
				continue;
			}
			cout << "Enter Request Date (dd-mm-yyyy): ";
			cin >> day >> month >> year;
			cin.ignore();
			month = -month;
			year = -year;
			while (day <= 0 || day > 31 || month <= 0 || month > 12 || year < 2000)
			{
				cout << "Invalid Date format, Please Enter again(dd-mm-yyyy): ";
				cin >> day >> month >> year;
				month = -month;
				year = -year;
			}
			cin.ignore();
			Date requestDate(day, month, year);

			if (Books->BorrowBook(ISSN, id, requestDate))
			{
				Users->AddIsn(id, ISSN, requestDate);
			}
		}
		else if (in == 4)
		{
			int id, ISSN = 0;
			int day, month, year;
			cout << "Enter User ID: ";
			cin >> id;
			while (id <= 0)
			{
				cout << "Invalid Value" << endl;
				cout << "Please Enter Again: ";
				cin >> id;
			}
			if (!Users->Search(id))
			{
				cout << "The user is not registered" << endl;
				continue;
			}
			cout << "Enter ISSN: ";
			cin >> ISSN;
			if (!Books->Search(ISSN))
			{
				cout << "Invalid Book ISSN" << endl;
				continue;
			}
			cin.ignore();
			cout << "Enter Return Date (dd-mm-yyyy): ";
			cin >> day >> month >> year;
			cin.ignore();
			month = -month;
			year = -year;
			while (day <= 0 || day > 31 || month <= 0 || month > 12 || year < 2000)
			{
				cout << "Invalid Date format, Please Enter again(dd-mm-yyyy): ";
				cin >> day >> month >> year;
				month = -month;
				year = -year;
			}
			cin.ignore();
			Date returnDate(day, month, year);

			if (Books->Search(ISSN))
			{
				Users->ReturnBook(ISSN, returnDate, id);
				int id1 = Books->ReturnBook(ISSN);
				if (id != -1)
				{
					Users->AddIsn(id, ISSN, returnDate);
				}
			}
		}
		else if (in == 5)
		{
			int ISSN;
			cout << "Enter ISSN: ";
			cin >> ISSN;
			cin.ignore();
			string field;
			if (Books->Search(ISSN))
			{
				cout << "Enter field to update (Title/Authors/Reference Copies/Issuable Copies): ";
				getline(cin, field);
				while (field != "Title" && field != "Authors" && field != "Reference Copies" && field != "Issuable Copies")
				{
					cout << "Invalid input!" << endl;
					cout << "Enter again: ";
					getline(cin, field);
				}
				Books->UpdateBookinfo(ISSN, field);
			}
			else
			{
				cout << "Invalid Book ISSN" << endl;
			}
		}
		else if (in == 6)
		{
			int ISSN;
			cout << "Enter ISSN: ";
			cin >> ISSN;
			cin.ignore();

			if (Books->Search(ISSN))
			{
				Books->DeleteBook(ISSN);
				Date d(0, 0, 0);
				Users->RemoveBookRecord(ISSN, d);
			}
			else
			{
				cout << "Invalid Book ISSN" << endl;
			}
		}
		else if (in == 7)
		{
			Books->MostDemandingBook();
		}
		else if (in == 8)
		{
			int id;
			cout << "Enter User ID: ";
			cin >> id;
			cin.ignore();
			while (id <= 0)
			{
				cout << "Invalid Value" << endl;
				cout << "Please Enter Again: ";
				cin >> id;
			}
			if (!Users->Search(id))
			{
				cout << "The user is not registered" << endl;
				continue;
			}
			cin.ignore();
			cout << endl;
			Users->DisplayUserInfo(id);
		}
		else if (in == 9)
		{
			if (!Users->isEmpty())
			{
				Users->DisplayFines();
			}
			else
			{
				cout << "No user is registered currently" << endl;
			}
		}

	} while (in != -1);
	return 0;
}