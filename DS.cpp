#include <iostream>
#include <list>
using namespace std;

template <class T>
class StackList{                //STL list-based stack
    private:
        list<T> l;
    public:
        void push(T);
        T pop();
        T top() const;
        bool isEmpty() const;
        void clear();
};

template<class T>
void StackList<T>::clear() {
    l.clear();
}

template<class T>
bool StackList<T>::isEmpty() const {
    return l.empty();
}

template<class T>
T StackList<T>::top() const {
    return l.back();
}

template<class T>
T StackList<T>::pop() {
    T tmp = l.back();
    l.pop_back();
    return tmp;
}

template<class T>
void StackList<T>::push(T val) {
    l.push_back(val);
}

template <class T>
class QueueList{                 //STL list-based queue
    private:
        list<T> l;
    public:
        void enqueue(T);
        T dequeue();
        T front() const;
        bool isEmpty() const;
        void clear();
        int size();
};

template<class T>
void QueueList<T>::clear() {
    l.clear();
}

template<class T>
bool QueueList<T>::isEmpty() const {
    return l.empty();
}

template<class T>
T QueueList<T>::front() const {
    return l.front();
}

template<class T>
T QueueList<T>::dequeue() {
    T tmp = l.back();
    l.pop_back();
    return tmp;
}

template<class T>
void QueueList<T>::enqueue(T val) {
    l.push_front(val);
}

template<class T>
int QueueList<T>::size() {
    return l.size();
}

template <class T>
void copier(StackList<T>& s1, StackList<T>& s2) {
    if (!s1.isEmpty()) {
        T temp = s1.pop();
        copier(s1, s2);
        s2.push(temp);  // Push elements in reverse order
    }
}


template<class T>
void copyStack(StackList<T>& s1, StackList<T>& s2) {
    // Clear s2 to ensure a fresh copy
    while (!s2.isEmpty()) {
        s2.pop();
    }
    copier(s1, s2);
}
string convertToNS(int num, int base){
    string x;
    StackList<char> s;
    do {
        s.push((num%base)<10?num%base+48:num%base+55);  // the ternary condition is for handling converting integer numbers to characters, and to handle if number was bigger than 9 to convert it to Alphabetics.
    }while (num/=base); // iterate until element becomes 0 after division
    while (!s.isEmpty()) x+=s.pop();    // add characters to returned string from top to bottom.
    return x;
}

string addLargeIntegers(StackList<int>& s1, StackList<int>& s2) {
    int sum = 0, carry = 0;
    string result = "";

    // Iterate until both stacks are empty
    while (!s1.isEmpty() || !s2.isEmpty()) {
        // Get digits from stacks, handling cases where one stack might be empty
        int digit1 = s1.isEmpty() ? 0 : s1.top();
        int digit2 = s2.isEmpty() ? 0 : s2.top();

        // Calculate sum and carry
        sum = digit1 + digit2 + carry;
        carry = sum / 10;
        sum %= 10;

        // Push the sum digit onto the result string
        result += to_string(sum);

        // Pop digits from stacks
        if (!s1.isEmpty()) {
            s1.pop();
        }
        if (!s2.isEmpty()) {
            s2.pop();
        }
    }

    // If there's a final carry, append it to the result
    if (carry > 0) {
        result += to_string(carry);
    }


    return result;  // Return the final, added string
}

string subtractLargeIntegers(string x, string y) {                      //N.B. Algorithm for subtracting reference: geeksforgeeks.org (not a copy of the code)
    // Create stacks to store digits of numbers in reverse order
    StackList<int> first, second;
    bool negative = false;
    string result;

    // Determine signs and adjust inputs accordingly
    bool xNegative = x[0] == '-';
    bool yNegative = y[0] == '-';

    if (xNegative && yNegative)
        if ((x.length() > y.length()) || (x.length() == y.length() && x>y)) negative = true;
        else if ((x.length()<y.length()) || (x.length() == y.length() && x<y)) {
            negative = false;
            string tmp = x;
            x = y;
            y = tmp;
        }
    else if (!xNegative && !yNegative) {
            if (x.length() < y.length()) {
                string tmp = x;
                x = y;
                y = tmp;
                negative = true;
            } else if (x.length() == y.length() && x < y) {
                string tmp = x;
                x = y;
                y = tmp;
                negative = true;
            }
        }
    else if (xNegative && !yNegative) negative = true;
    else if (!xNegative && yNegative) negative = false;

    // Push digits of x onto first stack (converting chars to ints)
    for (char c : x) {
        if (c == '-') continue;
        first.push(c - 48);  // Subtract 48 to convert ASCII char to int
    }

    // Push digits of y onto second stack (converting chars to ints)
    for (char c : y) {
        if (c == '-') continue;
        second.push(c - 48);
    }

    if (xNegative && !yNegative || !xNegative && yNegative) result = addLargeIntegers(first, second);
    else {
        // Initialize variables for result and borrow
        int borrow = 0;
        // Iterate until both stacks are empty
        while (!first.isEmpty() || !second.isEmpty()) {
            // Get digits from stacks, handling cases where one stack might be empty
            int digit1 = first.isEmpty() ? 0 : first.top();
            int digit2 = second.isEmpty() ? 0 : second.top();

            // Calculate difference and handle borrow
            int difference = digit1 - digit2 - borrow;
            if (difference < 0) {
                difference += 10;  // Borrow from the next digit
                borrow = 1;
            } else {
                borrow = 0;
            }

            // Append difference to result string
            result += to_string(difference);

            // Pop digits from stacks
            if (!first.isEmpty()) {
                first.pop();
            }
            if (!second.isEmpty()) {
                second.pop();
            }
        }

        // Append any remaining borrow to the result string
        if (borrow > 0) {
            result += to_string(borrow);
        }
    }

    // Reverse the result string to get the correct order
    string reversedRes;
    int l = result.length();
    for (int i = l - 1; i >= 0; i--) {
        reversedRes += result[i];
    }

    // Remove leading zeros from the result
    while (reversedRes.size() > 1 && reversedRes[0] == '0') {
        reversedRes.erase(0, 1);
    }
    if (negative) reversedRes = '-' + reversedRes;
    return reversedRes;  // Return the final, subtracted string
}

template<class T>
bool moveNthElem(QueueList<T>& q, int n){
    if (n<1 || n>q.size()) return false;
    QueueList<T> q2;
    T tmp;
    int counter = 1;    // counter for counting current element position
    while(!q.isEmpty()){
        if (counter == n) tmp = q.dequeue();    // if current position is in, store its value in tmp variable
        else q2.enqueue(q.dequeue());           // if not, enqueue value in second queue
        counter++;
    }
    q.enqueue(tmp);                             // put nth element in the first posiiton of queue
    while (!q2.isEmpty()) q.enqueue(q2.dequeue());  //enqueue remaining elements from the second queue
    return true;
}

template<class T>
void reverseQueue(QueueList<T>& x){
    StackList<T> s;
    while (!x.isEmpty()) s.push(x.dequeue());   // add elements to stack in reverse order.
    while (!s.isEmpty()) x.enqueue(s.pop());    // add elemetns to queue from reverse-ordered stack
}


int main() {
    // Assuming Class T fpr stacks was chosen to be "int".
    StackList<int> S1;
    StackList<int> S2;
    QueueList<double> Qd;
    QueueList<string> Qs;

    int num, base;
    int nth;
    double in;

    string l, m, input;
    bool exc = false;
    int x;
    do {
        cout << "1- Copy a stack.\n"
                "2- Convert decimal to another base\n"
                "3- Subtract 2 big integers.\n"
                "4- Move Nth element to front in a queue of strings.\n"
                "5- Reverse elements of a queue of doubles.\n"
                "6- Exit.\n\n";

        cin >> x;
        switch (x) {
            case 1:
                do {
                    cout << "Enter Number of elements you want to insert in 1st Stack.\n";
                    cin >> input;
                    try {
                        x = stoi(input);
                    }
                    catch (exception &err) {
                        exc = true;
                        cout << "Enter a Valid Number\n";
                    }
                } while (exc);
                while (x--) {
                    do {
                        cout << "Enter element: \n";
                        cin >> input;
                        try {
                            num = stoi(input);
                        }
                        catch (exception &err) {
                            exc = true;
                            cout << "Enter a Valid Number\n";
                        }
                    } while (exc);

                    S1.push(num);
                }
                copyStack(S1, S2);
                cout << "Stack 2 is: " << endl;
                while (!S2.isEmpty()) cout << S2.pop() << endl;
                S1.clear();
                S2.clear();
                break;
            case 2:
                do {
                    cout << "Enter number to convert: \n";
                    cin >> input;
                    try {
                        num = stoi(input);
                    }
                    catch (exception &err) {
                        exc = true;
                        cout << "Enter a Valid Number\n";
                    }
                } while (exc);
                do {
                    cout << "Enter base (radix) of the system: \n";
                    cin >> input;
                    try {
                        base = stoi(input);
                    }
                    catch (exception &err) {
                        exc = true;
                        cout << "Enter a Valid Number\n";
                    }
                } while (exc);
                cout << "The Converted Number is: " << convertToNS(num, base) << endl;
                break;
            case 3:
                do {
                    exc = false;
                    cout << "Enter the first large integer: \n";
                    cin >> l;
                    for (char i: l)
                        if ( (i < '0' && i!='-') || i > '9') {
                            cout << "You entered an invalid Character\n";
                            exc = true;
                        }
                } while (exc);
                do {
                    exc = false;
                    cout << "Enter the second large integer: \n";
                    cin >> m;
                    for (char i: m)
                        if ( (i < '0' && i!='-') || i > '9') {
                            cout << "You entered an invalid Character\n";
                            exc = true;
                        }
                } while (exc);
                cout << "Result is: " << subtractLargeIntegers(l, m) << endl;
                break;
            case 4:
                do {
                    cout << "Enter Number of elements you want to insert in Queue.\n";
                    cin >> input;
                    try {
                        x = stoi(input);
                    }
                    catch (exception &err) {
                        exc = true;
                        cout << "Enter a Valid Number\n";
                    }
                } while (exc);
                while (x--) {
                    cout << "Enter element: \n";
                    cin >> input;
                    Qs.enqueue(input);
                }
                do {
                    cout << "Enter number of element N to be moved: \n";
                    cin >> input;
                    try {
                        nth = stoi(input);
                    }
                    catch (exception &err) {
                        exc = true;
                        cout << "Enter a Valid Number\n";
                    }
                } while (exc);
                moveNthElem(Qs, nth);
                cout << "Edited Queue is: \n";
                while (!Qs.isEmpty()) cout << Qs.dequeue() << " ";
                cout << endl;
                break;
            case 5:
                do {
                    cout << "Enter Number of elements you want to insert in Queue.\n";
                    cin >> input;
                    try {
                        x = stoi(input);
                    }
                    catch (exception &err) {
                        exc = true;
                        cout << "Enter a Valid Number\n";
                    }
                } while (exc);
                while (x--) {
                    cout << "Enter element: \n";
                    cin >> in;
                    Qd.enqueue(in);
                }
                reverseQueue(Qd);
                cout << "Reversed Queue is: \n";
                while (!Qd.isEmpty()) cout << Qd.dequeue() << " ";
                cout << endl;
                break;
            case 6:
                exit(0);
            default:
                cout << "Enter a valid option.\n\n";
        }
    } while (true);
}