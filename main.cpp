// A basic deque command interpreter.
// Supports operations similar to common OJ deque problems:
// push_front x, push_back x, pop_front, pop_back, front, back, size, empty, clear.
// Input format:
// - Optional first integer T or Q indicating number of operations (auto-detected).
// - Otherwise, read commands until EOF.
// Output:
// - For pop_* on empty: print "error".
// - For front/back on empty: print "error".
// - For empty: print 1 if empty else 0.
// - For size: print integer size.
// - For clear: print nothing.

#include <iostream>
#include <string>
#include <deque>
#include <cctype>

using namespace std;

static inline string trim(const string &s) {
    size_t i = 0, j = s.size();
    while (i < j && isspace(static_cast<unsigned char>(s[i]))) ++i;
    while (j > i && isspace(static_cast<unsigned char>(s[j-1]))) --j;
    return s.substr(i, j - i);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    deque<long long> dq;
    string cmd;

    // Try to detect if first token is an integer count
    // We'll peek the stream: read first non-space sequence
    cin >> ws;
    streampos start = cin.tellg();
    string first;
    if (!(cin >> first)) return 0;

    auto is_integer = [](const string &t)->bool {
        if (t.empty()) return false;
        size_t i = 0;
        if (t[0] == '+' || t[0] == '-') i = 1;
        if (i == t.size()) return false;
        for (; i < t.size(); ++i) if (!isdigit(static_cast<unsigned char>(t[i]))) return false;
        return true;
    };

    auto process = [&](const string &op)->void {
        if (op == "pop_front") {
            if (dq.empty()) cout << "error\n";
            else { dq.pop_front(); }
        } else if (op == "pop_back") {
            if (dq.empty()) cout << "error\n";
            else { dq.pop_back(); }
        } else if (op == "front") {
            if (dq.empty()) cout << "error\n";
            else cout << dq.front() << "\n";
        } else if (op == "back") {
            if (dq.empty()) cout << "error\n";
            else cout << dq.back() << "\n";
        } else if (op == "size") {
            cout << dq.size() << "\n";
        } else if (op == "empty") {
            cout << (dq.empty() ? 1 : 0) << "\n";
        } else if (op == "clear") {
            dq.clear();
        } else if (op.rfind("push_front", 0) == 0) {
            // Format could be: push_front x  OR push_front(x) OR push_front    x
            long long x;
            if (op.find('(') != string::npos) {
                // e.g., push_front(123)
                auto l = op.find('(');
                auto r = op.find(')', l+1);
                string num = (r!=string::npos)? op.substr(l+1, r-l-1) : op.substr(l+1);
                x = stoll(trim(num));
            } else if (op == "push_front") {
                if (!(cin >> x)) x = 0; // fallback
            } else {
                // e.g., "push_front123" (unlikely). Try to parse trailing number.
                string num;
                for (size_t i = 10; i < op.size(); ++i) num.push_back(op[i]);
                x = stoll(trim(num));
            }
            dq.push_front(x);
        } else if (op.rfind("push_back", 0) == 0) {
            long long x;
            if (op.find('(') != string::npos) {
                auto l = op.find('(');
                auto r = op.find(')', l+1);
                string num = (r!=string::npos)? op.substr(l+1, r-l-1) : op.substr(l+1);
                x = stoll(trim(num));
            } else if (op == "push_back") {
                if (!(cin >> x)) x = 0;
            } else {
                string num;
                for (size_t i = 9; i < op.size(); ++i) num.push_back(op[i]);
                x = stoll(trim(num));
            }
            dq.push_back(x);
        } else {
            // Unknown command; ignore line or attempt to parse with parentheses
            // Attempt: if it contains '(' and ')' and starts with push_*
            if (op.find("push_") != string::npos && op.find('(') != string::npos) {
                bool front = op.find("push_front") != string::npos;
                auto l = op.find('(');
                auto r = op.find(')', l+1);
                if (l != string::npos) {
                    string num = (r!=string::npos)? op.substr(l+1, r-l-1) : op.substr(l+1);
                    long long x = 0;
                    try { x = stoll(trim(num)); } catch (...) {}
                    if (front) dq.push_front(x); else dq.push_back(x);
                }
            }
        }
    };

    if (is_integer(first)) {
        long long q = 0;
        try { q = stoll(first); } catch (...) { q = 0; }
        for (long long i = 0; i < q; ++i) {
            string op;
            if (!(cin >> op)) break;
            process(op);
        }
    } else {
        // No count given: process the first token then rest until EOF
        process(first);
        string op;
        while (cin >> op) process(op);
    }

    return 0;
}
